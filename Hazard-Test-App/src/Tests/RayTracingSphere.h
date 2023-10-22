
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include "Backend/Core/RTCore/AccelerationStructure.h"

#include "Hazard/Math/Time.h"

#include "vendor/stb_image.h"
#include <glm/glm/ext/matrix_clip_space.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <Hazard/Rendering/Mesh/Mesh.h>
#include <Hazard/Rendering/Mesh/MeshFactory.h>
#include <Hazard.h>
#include <EditorCamera.h>
#include "Hazard/RenderContext/TextureFactory.h"
#include "Hazard/RenderContext/ShaderLibrary.h"

#include <GLFW/glfw3.h>

using namespace HazardRenderer;
using namespace Hazard;

namespace RayTracingSphere
{
	//OpenGL : Test
	//Vulkan : Test
	//Metal	 : Test
	//DX12	 : Test
	//DX11	 : Test

	static void Run(RenderAPI api)
	{
		static bool running = true;

		Editor::EditorCamera camera(45.0f, 1.0f, 0.03f, 1000.0f);
		camera.SetSpeedMultiplier(0.25f);
#pragma region Init
		Logging::Logger logger = Logging::Logger();

		Window* window = CreateTestWindow("Raytraced sphere test", api, &running);
		window->Show();

		auto device = window->GetContext()->GetDevice();

		std::cout << "Selected device: " << device->GetDeviceName() << std::endl;
		HZR_ASSERT(device->SupportsRaytracing(), "Device does not support Raytracing");

#pragma region Mesh
		Ref<Mesh> corvette = LoadMesh("assets/models/c8_corvette_colored.fbx");
		Ref<Mesh> cube = LoadMesh("assets/models/cube.fbx");
#pragma endregion
#pragma region OutputImage
		Image2DCreateInfo outputImageSpec = {};
		outputImageSpec.DebugName = "OutputImage";
		outputImageSpec.Extent.Width = window->GetWidth();
		outputImageSpec.Extent.Height = window->GetHeight();
		outputImageSpec.Format = ImageFormat::RGBA;
		outputImageSpec.Usage = ImageUsage::Storage;

		Ref<Image2D> outputImage = Image2D::Create(&outputImageSpec);
#pragma endregion
#pragma region RayTracing
#pragma region BLAS

		AccelerationStructureGeometry carGeometry = {};
		carGeometry.VertexBuffer = corvette->GetVertexBuffer();
		carGeometry.IndexBuffer = corvette->GetIndexBuffer();
		carGeometry.BoundingBox = corvette->GetBoundingBox();
		carGeometry.Transform = glm::mat4(1.0f);

		AccelerationStructureGeometry cubeGeometry = {};
		cubeGeometry.VertexBuffer = cube->GetVertexBuffer();
		cubeGeometry.IndexBuffer = cube->GetIndexBuffer();
		cubeGeometry.BoundingBox = cube->GetBoundingBox();
		cubeGeometry.Transform = glm::translate(glm::mat4(1.0f), { 0.0f, 1.0f, 6.0f });

		std::vector<AccelerationStructureGeometry> geometries
		{
			carGeometry, cubeGeometry
		};

		AccelerationStructureCreateInfo bottomAccelInfo = {};
		bottomAccelInfo.DebugName = "BottomLevelAccelerationStructure";
		bottomAccelInfo.Level = AccelerationStructureLevel::Bottom;
		bottomAccelInfo.GeometryCount = geometries.size();
		bottomAccelInfo.pGeometries = geometries.data();

		Ref<BottomLevelAS> bottomLevelAccelerationStructure = BottomLevelAS::Create(&bottomAccelInfo);

		AccelerationStructureInstance instance1 = {};
		instance1.CustomIndex = 0;
		instance1.pBottomLevelAS = bottomLevelAccelerationStructure;
		instance1.Transform = glm::translate(glm::mat4(1.0f), { -4.0f, 0.0f, 0.0f });

		AccelerationStructureInstance instance2 = {};
		instance2.CustomIndex = 1;
		instance2.pBottomLevelAS = bottomLevelAccelerationStructure;
		instance2.Transform = glm::translate(glm::mat4(1.0f), { 4.0f, 0.0f, 0.0f });

		AccelerationStructureInstance instance3 = {};
		instance3.CustomIndex = 2;
		instance3.pBottomLevelAS = bottomLevelAccelerationStructure;

		std::vector<AccelerationStructureInstance> instances = {
			instance1, instance2, instance3
		};
#pragma endregion
#pragma region TLAS
		AccelerationStructureCreateInfo topAccelInfo = {};
		topAccelInfo.DebugName = "TopLevelAccelerationStructure";
		topAccelInfo.Level = AccelerationStructureLevel::Top;
		topAccelInfo.InstanceCount = instances.size();
		topAccelInfo.pInstances = instances.data();

		Ref<TopLevelAS> topLevelAccelerationStructure = TopLevelAS::Create(&topAccelInfo);
#pragma endregion
#pragma region Raygen
		std::vector<ShaderStageCode> shaderCode = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/raygen.glsl", api);
		std::vector<ShaderStageCode> screenPassCode = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/composite.glsl", api);

		PipelineSpecification pipelineSpec = {};
		pipelineSpec.DebugName = "RaygenPipeline";
		pipelineSpec.Usage = PipelineUsage::Raygen;
		pipelineSpec.ShaderCodeCount = shaderCode.size();
		pipelineSpec.pShaderCode = shaderCode.data();
		pipelineSpec.MaxRayDepth = 2;

		Ref<Pipeline> raygenPipeline = Pipeline::Create(&pipelineSpec);

		ShaderBindingTableCreateInfo bindingSpec = {};
		bindingSpec.pPipeline = raygenPipeline;

		Ref<ShaderBindingTable> bindingTable = ShaderBindingTable::Create(&bindingSpec);
#pragma endregion
#pragma endregion
#pragma region ScreenPass
		PipelineSpecification screenPassSpec = {};
		screenPassSpec.DebugName = "ScreenPass";
		screenPassSpec.Usage = PipelineUsage::GraphicsBit;
		screenPassSpec.CullMode = CullMode::None;
		screenPassSpec.DepthTest = false;
		screenPassSpec.ShaderCodeCount = screenPassCode.size();
		screenPassSpec.pShaderCode = screenPassCode.data();
		screenPassSpec.pTargetRenderPass = window->GetSwapchain()->GetRenderPass();

		Ref<Pipeline> screenPass = Pipeline::Create(&screenPassSpec);
#pragma endregion
#pragma region EnvironmentMap

		TextureHeader header = TextureFactory::LoadTextureFromSourceFile("assets/Textures/pink_sunrise_4k.hdr", true);

		Image2DCreateInfo envInfo = {};
		envInfo.DebugName = "EnvMap";
		envInfo.Format = ImageFormat::RGBA;
		envInfo.Usage = ImageUsage::Texture;
		envInfo.Extent.Width = header.Width;
		envInfo.Extent.Height = header.Height;
		envInfo.Data = header.ImageData;

		Ref<Image2D> environmentMapImage = Image2D::Create(&envInfo);

		Ref<RenderCommandBuffer> computeBuffer = RenderCommandBuffer::Create("RadianceMap compute", DeviceQueue::ComputeBit);
		Ref<RenderCommandBuffer> graphicsBuffer = RenderCommandBuffer::Create("Transition", DeviceQueue::GraphicsBit);

		CubemapTextureCreateInfo radianceInfo = {};
		radianceInfo.DebugName = "RadianceMap " + environmentMapImage->GetDebugName();
		radianceInfo.Usage = ImageUsage::Texture;
		radianceInfo.Format = ImageFormat::RGBA;
		radianceInfo.Width = 4096;
		radianceInfo.Height = 4096;

		Ref<CubemapTexture> radianceMap = CubemapTexture::Create(&radianceInfo);

		ImageTransitionInfo generalTransition = {};
		generalTransition.Cubemap = radianceMap;
		generalTransition.SourceLayout = ImageLayout_ShaderReadOnly;
		generalTransition.DestLayout = ImageLayout_General;

		std::vector<ShaderStageCode> computeCode = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/EquirectangularToCubeMap.glsl", api);

		PipelineSpecification computeSpec = {};
		computeSpec.DebugName = "EquirectangularToCubemap";
		computeSpec.Usage = PipelineUsage::ComputeBit;
		computeSpec.ShaderCodeCount = computeCode.size();
		computeSpec.pShaderCode = computeCode.data();

		Ref<Pipeline> computePipeline = Pipeline::Create(&computeSpec);

		auto shader = computePipeline->GetShader();
		//shader->Set("o_CubeMap", 0, radianceMap);
		//shader->Set("u_EquirectangularTexture", 0, environmentMapImage);

		GroupSize size = { radianceInfo.Width / 32, radianceInfo.Height / 32, 6 };

		computeBuffer->Begin();
        computeBuffer->SetPipeline(computePipeline);
		computeBuffer->DispatchCompute(size);
		computeBuffer->End();
		computeBuffer->Submit();

#pragma endregion

		struct CameraData
		{
			glm::mat4 InvProjection;
			glm::mat4 InvView;
		};

		BufferCreateInfo uboInfo = {};
		uboInfo.Name = "Camera";
		uboInfo.Size = sizeof(CameraData);

		Ref<GPUBuffer> cameraUBO = GPUBuffer::Create(&uboInfo);

		while (running)
		{
			Time::Update(glfwGetTime());
			Input::Update();

			camera.OnUpdate();

			window->BeginFrame();

			auto commandBuffer = window->GetSwapchain()->GetSwapchainBuffer();

			CameraData cameraData = {};
			cameraData.InvProjection = glm::inverse(camera.GetProjection());
			cameraData.InvView = glm::inverse(camera.GetView());

			BufferCopyRegion copyRegion = {};
			copyRegion.Data = &cameraData;
			copyRegion.Size = sizeof(CameraData);

			//cameraUBO->SetData(copyRegion);
			//commandBuffer->SetUniformBuffers(&cameraUBO, 1);

			//raygenPipeline->GetShader()->Set("TLAS", 0, topLevelAccelerationStructure.As<AccelerationStructure>());
			//raygenPipeline->GetShader()->Set("outputImage", 0, outputImage);
			//raygenPipeline->GetShader()->Set("Vertices", 0, corvette->GetVertexBuffer().As<GPUBuffer>());
			//raygenPipeline->GetShader()->Set("Indices", 0, corvette->GetIndexBuffer().As<GPUBuffer>());
			//raygenPipeline->GetShader()->Set("u_EnvironmentMap", 0, radianceMap);

			TraceRaysInfo info = {};
			info.Extent = outputImageSpec.Extent;
			info.pBindingTable = bindingTable;

			commandBuffer->SetPipeline(raygenPipeline);
			commandBuffer->TraceRays(info);
			{
				ImageTransitionInfo imageInfo = {};
				imageInfo.Image = outputImage;
				imageInfo.SourceLayout = ImageLayout_General;
				imageInfo.DestLayout = ImageLayout_ShaderReadOnly;

				//commandBuffer->TransitionImageLayout(imageInfo);
			}
			commandBuffer->BeginRenderPass(window->GetSwapchain()->GetRenderPass());

			//screenPass->GetShader()->Set("u_Image", 0, outputImage);
			commandBuffer->SetPipeline(screenPass);
			commandBuffer->Draw(6);

			commandBuffer->EndRenderPass();
			{
				ImageTransitionInfo imageInfo = {};
				imageInfo.Image = outputImage;
				imageInfo.SourceLayout = ImageLayout_ShaderReadOnly;
				imageInfo.DestLayout = ImageLayout_General;

				//commandBuffer->TransitionImageLayout(imageInfo);
			}

			Renderer::WaitAndRender();
			window->Present();
		}
	}
}
