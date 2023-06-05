
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"

#include "Backend/Core/RTCore/AccelerationStructure.h"

#include "vendor/stb_image.h"
#include <glm/glm/ext/matrix_clip_space.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace HazardRenderer;

namespace AccelerationStructureTest
{

	//OpenGL : Definitely a big nono
	//Vulkan : Working
	//Metal	 : Test
	//DX12	 : Test
	//DX11	 : Test

	static void Run(RenderAPI api)
	{
		static bool running = true;

		Window* window = CreateTestWindow("Acceleration structure test", api, &running);
		window->Show();

		auto device = window->GetContext()->GetDevice();

		std::cout << "Selected device: " << device->GetDeviceName() << std::endl;
		HZR_ASSERT(device->SupportsRaytracing(), "Device does not support Raytracing");

		//---------------
		float vertices[] =
		{
			-0.5f, -0.5f,-0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,-0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};
		uint32_t indices[] = {
			0, 1, 2, 2, 3, 0
		};

		BufferLayout layout = { { "a_Position",			ShaderDataType::Float3 },
								{ "a_Color",			ShaderDataType::Float4 },
								{ "a_TextureCoords",	ShaderDataType::Float2 }
		};

		VertexBufferCreateInfo vbo = {};
		vbo.Name = "QuadVBO";
		vbo.Layout = &layout;
		vbo.Size = sizeof(vertices);
		vbo.Data = &vertices;
		vbo.UsageFlags = BUFFER_USAGE_BOTTOM_LEVEL_ACCELERATION_STRUCTURE;

		IndexBufferCreateInfo ibo = {};
		ibo.Name = "QuadIBO";
		ibo.Size = sizeof(indices);
		ibo.Data = indices;
		ibo.UsageFlags = BUFFER_USAGE_BOTTOM_LEVEL_ACCELERATION_STRUCTURE;

		BoundingBox boundingBox = {};
		boundingBox.Encapsulate({ -0.5f, -0.5f, -0.5f });
		boundingBox.Encapsulate({  0.5f, -0.5f,  0.5f });
		boundingBox.Encapsulate({  0.5f,  0.5f,  0.5f });
		boundingBox.Encapsulate({ -0.5f,  0.5f, -0.5f });

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vbo);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);

		Image2DCreateInfo outputImage = {};
		outputImage.DebugName = "OutputImage";
		outputImage.Extent = { 1920, 1080 };
		outputImage.Format = ImageFormat::RGBA;
		outputImage.Usage = ImageUsage::Storage;

		Ref<Image2D> image = Image2D::Create(&outputImage);

		AccelerationStructureGeometry geometry = {};
		geometry.VertexBuffer = vertexBuffer;
		geometry.IndexBuffer = indexBuffer;
		geometry.BoundingBox = boundingBox;

		AccelerationStructureCreateInfo bottomAccelInfo = {};
		bottomAccelInfo.DebugName = "BottomLevelAccelerationStructure";
		bottomAccelInfo.Level = AccelerationStructureLevel::Bottom;
		bottomAccelInfo.GeometryCount = 1;
		bottomAccelInfo.pGeometries = &geometry;

		Ref<BottomLevelAS> bottomLevelAccelerationStructure = BottomLevelAS::Create(&bottomAccelInfo);

		AccelerationStructureInstance instance = {};
		instance.CustomIndex = 0;
		instance.pBottomLevelAS = bottomLevelAccelerationStructure;

		AccelerationStructureCreateInfo topAccelInfo = {};
		topAccelInfo.DebugName = "TopLevelAccelerationStructure";
		topAccelInfo.Level = AccelerationStructureLevel::Top;
		topAccelInfo.InstanceCount = 1;
		topAccelInfo.pInstances = &instance;

		Ref<TopLevelAS> topLevelAccelerationStructure = TopLevelAS::Create(&topAccelInfo);

		std::vector<ShaderStageCode> shaderCode = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/raygenBasic.glsl", api);
		std::vector<ShaderStageCode> screenPassCode = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/composite.glsl", api);

		PipelineSpecification pipelineSpec = {};
		pipelineSpec.DebugName = "RaygenPipeline";
		pipelineSpec.Usage = PipelineUsage::Raygen;
		pipelineSpec.ShaderCodeCount = shaderCode.size();
		pipelineSpec.pShaderCode = shaderCode.data();

		Ref<Pipeline> raygenPipeline = Pipeline::Create(&pipelineSpec);

		ShaderBindingTableCreateInfo bindingSpec = {};
		bindingSpec.pPipeline = raygenPipeline;

		Ref<ShaderBindingTable> bindingTable = ShaderBindingTable::Create(&bindingSpec);

		PipelineSpecification screenPassSpec = {};
		screenPassSpec.DebugName = "ScreenPass";
		screenPassSpec.Usage = PipelineUsage::GraphicsBit;
		screenPassSpec.CullMode = CullMode::None;
		screenPassSpec.DepthTest = false;
		screenPassSpec.ShaderCodeCount = screenPassCode.size();
		screenPassSpec.pShaderCode = screenPassCode.data();
		screenPassSpec.pTargetRenderPass = window->GetSwapchain()->GetRenderPass();

		Ref<Pipeline> screenPass = Pipeline::Create(&screenPassSpec);

		struct CameraData
		{
			glm::mat4 InvView;
			glm::mat4 InvProjection;
		};

		UniformBufferCreateInfo uboInfo = {};
		uboInfo.Name = "Camera";
		uboInfo.Set = 0;
		uboInfo.Binding = 2;
		uboInfo.Size = sizeof(CameraData);

		Ref<UniformBuffer> camera = UniformBuffer::Create(&uboInfo);

		while (running)
		{
			Input::Update();
			window->BeginFrame();

			auto commandBuffer = window->GetSwapchain()->GetSwapchainBuffer();

			CameraData cameraData = {};
			float aspectRatio = (float)window->GetWidth() / (float)window->GetHeight();
			cameraData.InvProjection = glm::inverse(glm::perspective(glm::radians(60.0f), aspectRatio, 0.03f, 100.0f));
			cameraData.InvView = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 5.0f });

			BufferCopyRegion region = {};
			region.Data = &cameraData;
			region.Size = sizeof(CameraData);

			camera->SetData(region);
			commandBuffer->SetUniformBuffers(&camera, 1);

			raygenPipeline->GetShader()->Set("topLevelAS", 0, topLevelAccelerationStructure.As<AccelerationStructure>());
			raygenPipeline->GetShader()->Set("image", 0, image);

			TraceRaysInfo rayInfo = {};
			rayInfo.Extent.Width = outputImage.Extent.Width;
			rayInfo.Extent.Height = outputImage.Extent.Height;
			rayInfo.Extent.Depth = 1;
			rayInfo.pBindingTable = bindingTable;

			commandBuffer->SetPipeline(raygenPipeline);
			commandBuffer->TraceRays(rayInfo);

			ImageTransitionInfo imageInfo = {};
			imageInfo.Image = image;
			imageInfo.SourceLayout = ImageLayout_General;
			imageInfo.DestLayout = ImageLayout_ShaderReadOnly;

			commandBuffer->TransitionImageLayout(imageInfo);
			commandBuffer->BeginRenderPass(window->GetSwapchain()->GetRenderPass());

			screenPass->GetShader()->Set("u_Image", 0, image);

			commandBuffer->SetPipeline(screenPass);
			commandBuffer->Draw(6);

			commandBuffer->EndRenderPass();
			{
				ImageTransitionInfo imageInfo = {};
				imageInfo.Image = image;
				imageInfo.SourceLayout = ImageLayout_ShaderReadOnly;
				imageInfo.DestLayout = ImageLayout_General;

				commandBuffer->TransitionImageLayout(imageInfo);
			}

			Renderer::WaitAndRender();
			window->Present();
		}
	}
}
