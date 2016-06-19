// +----------------------------------------------------------------------
// | Project : ray.
// | All rights reserved.
// +----------------------------------------------------------------------
// | Copyright (c) 2013-2016.
// +----------------------------------------------------------------------
// | * Redistribution and use of this software in source and binary forms,
// |   with or without modification, are permitted provided that the following
// |   conditions are met:
// |
// | * Redistributions of source code must retain the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer.
// |
// | * Redistributions in binary form must reproduce the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer in the documentation and/or other
// |   materials provided with the distribution.
// |
// | * Neither the name of the ray team, nor the names of its
// |   contributors may be used to endorse or promote products
// |   derived from this software without specific prior
// |   written permission of the ray team.
// |
// | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// | "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// | LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// | A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// | OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// | SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// | LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// | DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// | THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// | (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// | OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// +----------------------------------------------------------------------
#include <ray/graphics_device_property.h>

_NAME_BEGIN

__ImplementSubInterface(GraphicsDeviceProperty, rtti::Interface, "GraphicsDeviceProperty")

GraphicsDeviceProperties::GraphicsDeviceProperties() noexcept
	: maxImageDimension1D(0)
	, maxImageDimension2D(0)
	, maxImageDimension3D(0)
	, maxImageDimensionCube(0)
	, maxImageArrayLayers(2048)
	, maxTexelBufferElements(65536)
	, maxUniformBufferRange(16384)
	, maxStorageBufferRange(0x1000000)
	, maxPushConstantsSize(0)
	, maxMemoryAllocationCount(1000)
	, maxSamplerAllocationCount(1000)
	, bufferImageGranularity(0)
	, sparseAddressSpaceSize(0)
	, maxBoundDescriptorSets(0)
	, maxPerStageDescriptorSamplers(0)
	, maxPerStageDescriptorUniformBuffers(0)
	, maxPerStageDescriptorStorageBuffers(0)
	, maxPerStageDescriptorSampledImages(0)
	, maxPerStageDescriptorStorageImages(0)
	, maxPerStageDescriptorInputAttachments(0)
	, maxPerStageResources(0)
	, maxDescriptorSetSamplers(0)
	, maxDescriptorSetUniformBuffers(0)
	, maxDescriptorSetUniformBuffersDynamic(0)
	, maxDescriptorSetStorageBuffers(0)
	, maxDescriptorSetStorageBuffersDynamic(0)
	, maxDescriptorSetSampledImages(0)
	, maxDescriptorSetStorageImages(0)
	, maxDescriptorSetInputAttachments(0)
	, maxVertexInputAttributes(16)
	, maxVertexInputBindings(16)
	, maxVertexInputAttributeOffset(2048)
	, maxVertexInputBindingStride(2048)
	, maxVertexOutputComponents(0)
	, maxTessellationGenerationLevel(0)
	, maxTessellationPatchSize(0)
	, maxTessellationControlPerVertexInputComponents(0)
	, maxTessellationControlPerVertexOutputComponents(0)
	, maxTessellationControlPerPatchOutputComponents(0)
	, maxTessellationControlTotalOutputComponents(0)
	, maxTessellationEvaluationInputComponents(0)
	, maxTessellationEvaluationOutputComponents(0)
	, maxGeometryShaderInvocations(0)
	, maxGeometryInputComponents(0)
	, maxGeometryOutputComponents(0)
	, maxGeometryOutputVertices(0)
	, maxGeometryTotalOutputComponents(0)
	, maxFragmentInputComponents(0)
	, maxFragmentOutputAttachments(1)
	, maxFragmentDualSrcAttachments(0)
	, maxFragmentCombinedOutputResources(0)
	, maxComputeSharedMemorySize(0)
	, maxComputeWorkGroupInvocations(0)
	, subPixelPrecisionBits(0)
	, subTexelPrecisionBits(0)
	, mipmapPrecisionBits(0)
	, maxDrawIndexedIndexValue(65535)
	, maxDrawIndirectCount(0)
	, maxSamplerLodBias(2.0)
	, maxSamplerAnisotropy(0)
	, maxViewports(1)
	, maxViewportDimensionsW(0)
	, maxViewportDimensionsH(0)
	, minViewportBoundsRange(-32768)
	, maxViewportBoundsRange(32767)
	, viewportSubPixelBits(0)
	, minMemoryMapAlignment(0)
	, minTexelBufferOffsetAlignment(0)
	, minUniformBufferOffsetAlignment(0)
	, minStorageBufferOffsetAlignment(0)
	, minTexelOffset(-8)
	, maxTexelOffset(7)
	, minTexelGatherOffset(-8)
	, maxTexelGatherOffset(7)
	, minInterpolationOffset(-0.5)
	, maxInterpolationOffset(0.5)
	, subPixelInterpolationOffsetBits(4)
	, maxFramebufferWidth(16384)
	, maxFramebufferHeight(16384)
	, maxFramebufferLayers(2048)
	, maxFramebufferColorAttachments(1)
	, framebufferColorSampleCounts(15)
	, framebufferDepthSampleCounts(15)
	, framebufferStencilSampleCounts(15)
	, framebufferNoAttachmentsSampleCounts(15)
	, sampledImageColorSampleCounts(15)
	, sampledImageIntegerSampleCounts(15)
	, sampledImageDepthSampleCounts(15)
	, sampledImageStencilSampleCounts(15)
	, storageImageSampleCounts(15)
	, maxSampleMaskWords(0)
	, timestampComputeAndGraphics(0)
	, timestampPeriod(1.0)
	, maxClipDistances(0)
	, maxCullDistances(0)
	, maxCombinedClipAndCullDistances(0)
	, discreteQueuePriorities(0)
	, minPointSizeRange(1.0)
	, maxPointSizeRange(1.0)
	, pointSizeGranularity(0)
	, minLineWidthRange(1.0)
	, maxLineWidthRange(1.0)
	, lineWidthGranularity(0)
	, strictLines(0)
	, standardSampleLocations(0)
	, optimalBufferCopyOffsetAlignment(0)
	, optimalBufferCopyRowPitchAlignment(0)
	, nonCoherentAtomSize(0)
{
	maxComputeWorkGroupCount[0] = 0;
	maxComputeWorkGroupCount[1] = 0;
	maxComputeWorkGroupCount[2] = 0;
	maxComputeWorkGroupSize[0] = 0;
	maxComputeWorkGroupSize[1] = 0;
	maxComputeWorkGroupSize[2] = 0;
}

GraphicsDeviceProperties::~GraphicsDeviceProperties() noexcept
{
}

bool
GraphicsDeviceProperties::isTextureSupport(GraphicsFormat format) const noexcept
{
	return std::find(supportTextures.begin(), supportTextures.end(), format) != supportTextures.end();
}

bool
GraphicsDeviceProperties::isTextureDimSupport(GraphicsTextureDim dimension) const noexcept
{
	return std::find(supportTextureDims.begin(), supportTextureDims.end(), dimension) != supportTextureDims.end();
}

bool
GraphicsDeviceProperties::isVertexSupport(GraphicsFormat format) const noexcept
{
	return std::find(supportAttribute.begin(), supportAttribute.end(), format) != supportAttribute.end();
}

bool
GraphicsDeviceProperties::isShaderSupport(GraphicsShaderStageFlagBits stage) const noexcept
{
	return std::find(supportShaders.begin(), supportShaders.end(), stage) != supportShaders.end();
}

GraphicsDeviceProperty::GraphicsDeviceProperty() noexcept
{
}

GraphicsDeviceProperty::~GraphicsDeviceProperty() noexcept
{
}

_NAME_END