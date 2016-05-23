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
#include <ray/graphics_framebuffer.h>

_NAME_BEGIN

__ImplementSubInterface(GraphicsFramebuffer, GraphicsChild, "GraphicsFramebuffer")
__ImplementSubInterface(GraphicsFramebufferLayout, GraphicsChild, "GraphicsFramebufferLayout")

GraphicsAttachment::GraphicsAttachment() noexcept
	: _type(GraphicsImageLayout::GraphicsImageLayoutUndefined)
	, _slot(0)
	, _format(GraphicsFormat::GraphicsFormatUndefined)
{
}

GraphicsAttachment::GraphicsAttachment(std::uint32_t slot, GraphicsImageLayout type, GraphicsFormat format) noexcept
	: _type(type)
	, _slot(slot)
	, _format(format)
{
}

GraphicsAttachment::~GraphicsAttachment() noexcept
{
}

void
GraphicsAttachment::setAttachType(GraphicsImageLayout type) noexcept
{
	_type = type;
}

GraphicsImageLayout
GraphicsAttachment::getAttachType() const noexcept
{
	return _type;
}

void
GraphicsAttachment::setAttachFormat(GraphicsFormat format) noexcept
{
	_format = format;
}

GraphicsFormat
GraphicsAttachment::getAttachFormat() const noexcept
{
	return _format;
}

void
GraphicsAttachment::setAttachSlot(std::uint32_t slot) noexcept
{
	_slot = slot;
}

std::uint32_t
GraphicsAttachment::getAttachSlot() const noexcept
{
	return _slot;
}

GraphicsFramebufferLayoutDesc::GraphicsFramebufferLayoutDesc() noexcept
{
}

GraphicsFramebufferLayoutDesc::~GraphicsFramebufferLayoutDesc() noexcept
{
}

void
GraphicsFramebufferLayoutDesc::addComponent(const GraphicsAttachment& component) noexcept
{
	assert(component.getAttachFormat() != GraphicsFormat::GraphicsFormatUndefined);
	assert(std::find_if(_components.begin(), _components.end(), [&](const GraphicsAttachment& it) { return  it.getAttachSlot() == component.getAttachSlot(); }) == _components.end());
	_components.push_back(component);
}

void 
GraphicsFramebufferLayoutDesc::setComponents(const GraphicsAttachments& components) noexcept
{
	_components = components;
}

const GraphicsAttachments&
GraphicsFramebufferLayoutDesc::getComponents() const noexcept
{
	return _components;
}

GraphicsFramebufferDesc::GraphicsFramebufferDesc() noexcept
	: _width(0)
	, _height(0)
{
}

GraphicsFramebufferDesc::~GraphicsFramebufferDesc() noexcept
{
}

void
GraphicsFramebufferDesc::setWidth(std::uint32_t w) noexcept
{
	_width = w;
}

void
GraphicsFramebufferDesc::setHeight(std::uint32_t h) noexcept
{
	_height = h;
}

std::uint32_t
GraphicsFramebufferDesc::getWidth() const noexcept
{
	return _width;
}

std::uint32_t
GraphicsFramebufferDesc::getHeight() const noexcept
{
	return _height;
}

void
GraphicsFramebufferDesc::attach(GraphicsTexturePtr texture) noexcept
{
	assert(texture);
	assert(std::find(_textures.begin(), _textures.end(), texture) == _textures.end());

	_textures.push_back(texture);
}

void
GraphicsFramebufferDesc::detach(GraphicsTexturePtr texture) noexcept
{
	auto it = std::find(_textures.begin(), _textures.end(), texture);
	if (it != _textures.end())
		_textures.erase(it);
}

GraphicsTextures&
GraphicsFramebufferDesc::getTextures() noexcept
{
	return _textures;
}

const GraphicsTextures&
GraphicsFramebufferDesc::getTextures() const noexcept
{
	return _textures;
}

void
GraphicsFramebufferDesc::setSharedDepthStencilTexture(GraphicsTexturePtr target) noexcept
{
	_sharedDepthStencilTexture = target;
}

GraphicsTexturePtr
GraphicsFramebufferDesc::getSharedDepthStencilTexture() const noexcept
{
	return _sharedDepthStencilTexture;
}

void
GraphicsFramebufferDesc::setGraphicsFramebufferLayout(GraphicsFramebufferLayoutPtr layout) noexcept
{
	_framebufferLayout = layout;
}

GraphicsFramebufferLayoutPtr
GraphicsFramebufferDesc::getGraphicsFramebufferLayout() const noexcept
{
	return _framebufferLayout;
}

GraphicsFramebufferLayout::GraphicsFramebufferLayout() noexcept
{
}

GraphicsFramebufferLayout::~GraphicsFramebufferLayout() noexcept
{
}

GraphicsFramebuffer::GraphicsFramebuffer() noexcept
{
}

GraphicsFramebuffer::~GraphicsFramebuffer() noexcept
{
}

_NAME_END