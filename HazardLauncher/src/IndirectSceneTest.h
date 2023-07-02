
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include "TestUtils.h"
#include "Hazard/Math/Time.h"

#include <Hazard.h>
#include <EditorCamera.h>

#include <GLFW/glfw3.h>
#include <Hazard/RenderContext/TextureFactory.h>

using namespace HazardRenderer;
using namespace Hazard;

namespace IndirectSceneTest
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

		Logging::Logger logger = Logging::Logger();

		Window* window = CreateTestWindow("Raytraced sphere test", api, &running);
		window->Show();

		auto device = window->GetContext()->GetDevice();

		std::cout << "Selected device: " << device->GetDeviceName() << std::endl;

		TextureHeader header = TextureFactory::LoadTextureFromSourceFile("assets/Textures/pink_sunrise_4k.hdr", true);

		Ref<Mesh> corvette = LoadMesh("assets/models/c8_corvette_colored.fbx");
		Ref<Mesh> cube = LoadMesh("assets/models/cube.fbx");

		Image2DCreateInfo envInfo = {};
		envInfo.DebugName = "EnvMap";
		envInfo.Format = ImageFormat::RGBA;
		envInfo.Usage = ImageUsage::Texture;
		envInfo.Extent.Width = header.Width;
		envInfo.Extent.Height = header.Height;
		envInfo.Data = header.ImageData;

		SamplerCreateInfo samplerInfo = {};
		samplerInfo.DebugName = "Sampler";
		samplerInfo.MinFilter = FilterMode::Linear;
		samplerInfo.MagFilter = FilterMode::Linear;
		samplerInfo.Wrapping = ImageWrap::ClampEdge;

		Ref<Image2D> environmentMapImage = Image2D::Create(&envInfo);
		Ref<Sampler> sampler = Sampler::Create(&samplerInfo);
		Ref<RenderCommandBuffer> computeBuffer = RenderCommandBuffer::Create("RadianceMap compute", DeviceQueue::ComputeBit, 1);

		CubemapTextureCreateInfo radianceInfo = {};
		radianceInfo.DebugName = "RadianceMap " + environmentMapImage->GetDebugName();
		radianceInfo.Usage = ImageUsage::Texture;
		radianceInfo.Format = ImageFormat::RGBA;
		radianceInfo.Width = 4096;
		radianceInfo.Height = 4096;

		Ref<CubemapTexture> radianceMap = CubemapTexture::Create(&radianceInfo);

		//Compute environment map
		{
			std::vector<ShaderStageCode> computeCode = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/EquirectangularToCubeMap.glsl", api);

			PipelineSpecification computeSpec = {};
			computeSpec.DebugName = "EquirectangularToCubemap";
			computeSpec.Usage = PipelineUsage::ComputeBit;
			computeSpec.ShaderCodeCount = computeCode.size();
			computeSpec.pShaderCode = computeCode.data();

			DescriptorSetLayout layout = {
				{ "o_CubeMap", 0, DESCRIPTOR_TYPE_STORAGE_IMAGE },
				{ "u_EquirectangularTexture", 1, DESCRIPTOR_TYPE_SAMPLER_2D }
			};

			DescriptorSetCreateInfo setInfo = {};
			setInfo.Set = 0;
			setInfo.pLayout = &layout;

			Ref<Pipeline> computePipeline = Pipeline::Create(&computeSpec);
			Ref<DescriptorSet> set = DescriptorSet::Create(&setInfo);

			set->Write(0, radianceMap, nullptr, true);
			set->Write(1, environmentMapImage, sampler, true);

			computeBuffer->Begin();
			computeBuffer->SetPipeline(computePipeline);
			computeBuffer->SetDescriptorSet(set, 0);
			computeBuffer->DispatchCompute({ radianceInfo.Width / 32, radianceInfo.Height / 32, 6 });
			computeBuffer->End();
			computeBuffer->Submit();
		}

		struct CameraData
		{
			glm::mat4 InvProjection;
			glm::mat4 InvView;
		};

		BufferCreateInfo uboInfo = {};
		uboInfo.Name = "Camera";
		uboInfo.Size = sizeof(CameraData);

		DescriptorSetLayout layout = {
				{ "o_CubeMap", 0, DESCRIPTOR_TYPE_STORAGE_IMAGE },
				{ "u_EquirectangularTexture", 1, DESCRIPTOR_TYPE_SAMPLER_2D }
		};

		DescriptorSetCreateInfo setInfo = {};
		setInfo.Set = 0;
		setInfo.pLayout = &layout;

		Ref<GPUBuffer> cameraUBO = GPUBuffer::Create(&uboInfo);
		Ref<DescriptorSet> set = DescriptorSet::Create(&setInfo);

		while (running)
		{
			Time::Update(glfwGetTime());
			Input::Update();
			camera.OnUpdate();

			CameraData cameraData = {};
			cameraData.InvProjection = glm::inverse(camera.GetProjection());
			cameraData.InvView = glm::inverse(camera.GetView());

			BufferCopyRegion region = {};
			region.Data = &cameraData;
			region.Size = sizeof(CameraData);
			region.Offset = 0;

			cameraUBO->SetData(region);

			window->BeginFrame();

			auto commandBuffer = window->GetSwapchain()->GetSwapchainBuffer();

			commandBuffer->BeginRenderPass(window->GetSwapchain()->GetRenderPass());
			//commandBuffer->SetPipeline(somePipeline);
			commandBuffer->EndRenderPass();

			Renderer::WaitAndRender();
			window->Present();
		}
	}
}
