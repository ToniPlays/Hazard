#pragma once

#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_METAL
#include "MetalContext.h"
#include "MetalRenderCommandBuffer.h"
#include "MetalFrameBuffer.h"
#include "Pipeline/MetalPipeline.h"
#include "Pipeline/MetalShader.h"

#include "Pipeline/MetalRenderPass.h"
#include "Pipeline/MetalVertexBuffer.h"
#include "Pipeline/MetalIndexBuffer.h"
#include "Pipeline/MetalUniformBuffer.h"
#include "Pipeline/MetalStorageBuffer.h"

#include "Texture/MetalCubemapTexture.h"

#endif
