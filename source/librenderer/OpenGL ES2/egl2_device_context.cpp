// +---------------------------------------------------------------------
// | Project : ray.
// | All rights reserved.
// +----------------------------------------------------------------------
// | Copyright (c) 2013-2015.
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
#include "egl2_device_context.h"
#include "egl2_state.h"
#include "egl2_shader.h"
#include "egl2_texture.h"
#include "egl2_layout.h"
#include "egl2_sampler.h"
#include "egl2_framebuffer.h"
#include "egl2_vbo.h"
#include "egl2_ibo.h"

_NAME_BEGIN

__ImplementSubClass(EGL2DeviceContext, GraphicsContext, "EGL2DeviceContext")

EGL2DeviceContext::EGL2DeviceContext() noexcept
	: _initOpenGL(false)
	, _maxTextureUnits(32)
	, _maxViewports(4)
	, _clearColor(0.0, 0.0, 0.0)
	, _clearDepth(0.0)
	, _clearStencil(0xFFFFFFFF)
	, _viewport(0, 0, 0, 0)
	, _state(GL_NONE)
	, _renderTexture(GL_NONE)
	, _enableWireframe(false)
{
}

EGL2DeviceContext::~EGL2DeviceContext() noexcept
{
	this->close();
}

bool
EGL2DeviceContext::open(WindHandle window) noexcept
{
	if (!_initOpenGL)
	{
		_glcontext = std::make_shared<EGL2Canvas>();
		_glcontext->open(window);
		_glcontext->setActive(true);

		this->initDebugControl();
		this->initStateSystem();

		_initOpenGL = true;
	}

	return _initOpenGL;
}

void
EGL2DeviceContext::close() noexcept
{
	if (!_initOpenGL)
		return;

	if (_vbo)
	{
		_vbo.reset();
		_vbo = nullptr;
	}

	if (_ibo)
	{
		_ibo.reset();
		_ibo = nullptr;
	}

	if (_shaderObject)
	{
		_shaderObject.reset();
		_shaderObject = nullptr;
	}

	if (_renderTexture)
	{
		_renderTexture.reset();
		_renderTexture = nullptr;
	}

	if (_state)
	{
		_state.reset();
		_state = nullptr;
	}

	if (_glcontext)
	{
		_glcontext.reset();
		_glcontext = nullptr;
	}
}

void
EGL2DeviceContext::setActive(bool active) noexcept
{
	assert(_glcontext);
	_glcontext->setActive(active);
}

bool
EGL2DeviceContext::getActive() const noexcept
{
	assert(_glcontext);
	return _glcontext->getActive();
}

void
EGL2DeviceContext::renderBegin() noexcept
{
}

void
EGL2DeviceContext::renderEnd() noexcept
{
	assert(_glcontext);
	_glcontext->present();
}

void
EGL2DeviceContext::setViewport(const Viewport& view, std::size_t i) noexcept
{
	if (_viewport.left != view.left ||
		_viewport.top != view.top ||
		_viewport.width != view.width ||
		_viewport.height != view.height)
	{
		GL_CHECK(glViewport(view.left, view.top, view.width, view.height));
		_viewport = view;
	}
}

const Viewport&
EGL2DeviceContext::getViewport(std::size_t i) const noexcept
{
	return _viewport;
}

void
EGL2DeviceContext::setWireframeMode(bool enable) noexcept
{
	_enableWireframe = enable;
}

bool
EGL2DeviceContext::getWireframeMode() const noexcept
{
	return _enableWireframe;
}

void
EGL2DeviceContext::setSwapInterval(SwapInterval interval) noexcept
{
	assert(_glcontext);
	_glcontext->setSwapInterval(interval);
}

SwapInterval
EGL2DeviceContext::getSwapInterval() const noexcept
{
	assert(_glcontext);
	return _glcontext->getSwapInterval();
}

void
EGL2DeviceContext::setGraphicsState(GraphicsStatePtr state) noexcept
{
	if (_state != state)
	{
		if (state)
		{
			auto glstate = state->downcast<EGL2GraphicsState>();
			if (glstate)
			{
				glstate->apply(_stateCaptured);

				_state = glstate;
				_stateCaptured = glstate->getGraphicsStateDesc();
			}
			else
			{
				_state = nullptr;
				_stateDefault->apply(_stateCaptured);
			}
		}
		else
		{
			_state = nullptr;
			_stateDefault->apply(_stateCaptured);
		}
	}
}

GraphicsStatePtr
EGL2DeviceContext::getGraphicsState() const noexcept
{
	return _state;
}

void
EGL2DeviceContext::setGraphicsLayout(GraphicsLayoutPtr layout) noexcept
{
	if (_inputLayout != layout)
	{
		if (layout)
		{
			if (layout->isInstanceOf<EGL2GraphicsLayout>())
				_inputLayout = layout->downcast<EGL2GraphicsLayout>();
			else
				_inputLayout = nullptr;
		}
		else
		{
			_inputLayout = nullptr;
		}

		_needUpdateLayout = true;
	}
}

GraphicsLayoutPtr
EGL2DeviceContext::getGraphicsLayout() const noexcept
{
	return _inputLayout;
}

bool
EGL2DeviceContext::updateBuffer(GraphicsDataPtr& data, void* str, std::size_t cnt) noexcept
{
	if (data)
	{
		auto max = std::numeric_limits<GLsizeiptr>::max();
		if (cnt < max)
		{
			if (data->isInstanceOf<EGL2VertexBuffer>())
				this->setVertexBufferData(data);
			else if (data->isInstanceOf<EGL2IndexBuffer>())
				this->setIndexBufferData(data);
			else
				return false;

			auto _data = data->cast<EGL2GraphicsData>();
			_data->resize((const char*)str, cnt);
			return true;
		}
	}

	return false;
}

void*
EGL2DeviceContext::mapBuffer(GraphicsDataPtr& data, std::uint32_t access) noexcept
{
	if (data)
	{
		if (data->isInstanceOf<EGL2VertexBuffer>())
			this->setVertexBufferData(data);
		else if (data->isInstanceOf<EGL2IndexBuffer>())
			this->setIndexBufferData(data);
		else
			return nullptr;

		auto _data = data->cast<EGL2GraphicsData>();
		return _data->map(access);
	}

	return nullptr;
}

void
EGL2DeviceContext::unmapBuffer(GraphicsDataPtr& data) noexcept
{
	if (data)
	{
		if (data->isInstanceOf<EGL2VertexBuffer>())
			this->setVertexBufferData(data);
		else if (data->isInstanceOf<EGL2IndexBuffer>())
			this->setIndexBufferData(data);
		else
			return;

		auto _data = data->cast<EGL2GraphicsData>();
		_data->unmap();
	}
}

void
EGL2DeviceContext::setIndexBufferData(GraphicsDataPtr data) noexcept
{
	if (_ibo != data)
	{
		if (data)
		{
			if (data->isInstanceOf<EGL2IndexBuffer>())
				_ibo = data->downcast<EGL2IndexBuffer>();
			else
				_ibo = nullptr;
		}
		else
		{
			_ibo = nullptr;
		}

		_needUpdateIbo = true;
	}
}

GraphicsDataPtr
EGL2DeviceContext::getIndexBufferData() const noexcept
{
	return _ibo;
}

void
EGL2DeviceContext::setVertexBufferData(GraphicsDataPtr data) noexcept
{
	if (_vbo != data)
	{
		if (data)
		{
			if (data->isInstanceOf<EGL2VertexBuffer>())
				_vbo = data->downcast<EGL2VertexBuffer>();
			else
				_vbo = nullptr;
		}
		else
		{
			_vbo = nullptr;
		}

		_needUpdateVbo = true;
	}
}

GraphicsDataPtr
EGL2DeviceContext::getVertexBufferData() const noexcept
{
	return _vbo;
}

void
EGL2DeviceContext::drawRenderBuffer(const RenderIndirect& renderable) noexcept
{
	if (!_inputLayout || !_vbo)
		return;

	if (_vbo->size() < _inputLayout->getVertexSize() * (renderable.startVertice + renderable.numVertices))
		return;

	if (_inputLayout->getIndexType() != GL_NONE)
	{
		if (!_ibo)
			return;

		if (_ibo->size() < _inputLayout->getIndexSize() * (renderable.startIndice + renderable.numIndices))
			return;
	}

	auto primitiveType = _stateCaptured.getRasterState().primitiveType;
	if (_enableWireframe)
	{
		if (primitiveType == VertexType::PointOrLine ||
			primitiveType == VertexType::TriangleOrLine ||
			primitiveType == VertexType::FanOrLine)
		{
			primitiveType = VertexType::Line;
		}
	}

	if (_needUpdateLayout)
	{
		if (_inputLayout)
			_inputLayout->bindLayout(_shaderObject);
		_needUpdateLayout = false;
	}

	if (_needUpdateVbo)
	{
		if (_vbo)
			_vbo->bind(_inputLayout);
		_needUpdateVbo = false;
	}

	if (_needUpdateIbo)
	{
		if (_ibo)
			_ibo->bind();
		_needUpdateIbo = false;
	}

	if (_ibo)
	{
		GLenum drawType = EGL2Types::asVertexType(primitiveType);
		GLenum indexType = _inputLayout->getIndexType();
		GLvoid* offsetIndices = (GLchar*)(nullptr) + (_inputLayout->getIndexSize() * renderable.startIndice);
		GL_CHECK(glDrawElements(drawType, renderable.numIndices, indexType, offsetIndices));
	}
	else
	{
		GLenum drawType = EGL2Types::asVertexType(primitiveType);
		GL_CHECK(glDrawArrays(drawType, renderable.startVertice, renderable.numVertices));
	}
}

void
EGL2DeviceContext::drawRenderBuffer(const RenderIndirect renderable[], std::size_t first, std::size_t count) noexcept
{
	for (std::size_t i = first; i < first + count; i++)
		this->drawRenderBuffer(renderable[i]);
}

void
EGL2DeviceContext::setGraphicsTexture(GraphicsTexturePtr texture, std::uint32_t slot) noexcept
{
	if (texture)
	{
		auto gltexture = texture->downcast<EGL2Texture>();

		GLuint textureID = gltexture->getInstanceID();
		GLenum textureDim = gltexture->getTarget();

		GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
		GL_CHECK(glBindTexture(textureDim, textureID));
	}
	else
	{
		GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
	}
}

void
EGL2DeviceContext::setGraphicsTexture(GraphicsTexturePtr textures[], std::uint32_t first, std::uint32_t count) noexcept
{
	for (std::uint32_t i = first; i < first + count; i++)
		this->setGraphicsTexture(textures[i], i);
}

void
EGL2DeviceContext::setGraphicsSampler(GraphicsSamplerPtr sampler, std::uint32_t slot) noexcept
{
	auto glsampler = sampler->downcast<EGL2Sampler>();
	if (glsampler)
	{
	}
	else
	{
	}
}

void
EGL2DeviceContext::setGraphicsSampler(GraphicsSamplerPtr samplers[], std::uint32_t first, std::uint32_t count) noexcept
{
	for (std::uint32_t i = first; i < first + count; i++)
		this->setGraphicsSampler(samplers[i], i);
}

void
EGL2DeviceContext::setRenderTexture(GraphicsRenderTexturePtr target) noexcept
{
	if (_renderTexture != target)
	{
		if (_renderTexture)
			_renderTexture->setActive(false);

		if (target)
		{
			_renderTexture = target->downcast<EGL2RenderTexture>();
			_renderTexture->setActive(true);

			auto textureDesc = _renderTexture->getResolveTexture()->getGraphicsTextureDesc();
			this->setViewport(Viewport(0, 0, textureDesc.getWidth(), textureDesc.getHeight()), 0);
		}
		else
		{
			_renderTexture = nullptr;
			glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
		}
	}
}

void
EGL2DeviceContext::setMultiRenderTexture(GraphicsMultiRenderTexturePtr target) noexcept
{
	GL_PLATFORM_LOG("Can't support MRT");
}

void
EGL2DeviceContext::setRenderTextureLayer(GraphicsRenderTexturePtr renderTexture, std::int32_t layer) noexcept
{
	assert(renderTexture);

	if (_renderTexture == renderTexture)
	{
		_renderTexture->setLayer(layer);
	}
	else
	{
		this->setRenderTexture(renderTexture);
		_renderTexture->setLayer(layer);
	}
}

void
EGL2DeviceContext::blitRenderTexture(GraphicsRenderTexturePtr src, const Viewport& v1, GraphicsRenderTexturePtr dest, const Viewport& v2) noexcept
{
	GL_PLATFORM_LOG("Can't support blitRenderTexture");
}

GraphicsRenderTexturePtr
EGL2DeviceContext::getRenderTexture() const noexcept
{
	return _renderTexture;
}

GraphicsMultiRenderTexturePtr
EGL2DeviceContext::getMultiRenderTexture() const noexcept
{
	return nullptr;
}

void
EGL2DeviceContext::clearRenderTexture(ClearFlags flags, const Vector4& color, float depth, std::int32_t stencil) noexcept
{
	GLbitfield mode = 0;

	if (flags & ClearFlags::Color)
	{
		mode |= GL_COLOR_BUFFER_BIT;

		if (_clearColor != color)
		{
			GL_CHECK(glClearColor(color.x, color.y, color.z, color.w));
			_clearColor = color;
		}
	}

	if (flags & ClearFlags::Depth)
	{
		mode |= GL_DEPTH_BUFFER_BIT;

		if (_clearDepth != depth)
		{
			GL_CHECK(glClearDepthf(depth));
			_clearDepth = depth;
		}
	}

	if (flags & ClearFlags::Stencil)
	{
		mode |= GL_STENCIL_BUFFER_BIT;

		if (_clearStencil != stencil)
		{
			GL_CHECK(glClearStencil(stencil));
			_clearStencil = stencil;
		}
	}

	if (mode != 0)
	{
		auto depthWriteMask = _stateCaptured.getDepthState().depthWriteMask;
		if (!depthWriteMask && flags & ClearFlags::Depth)
		{
			GL_CHECK(glDepthMask(GL_TRUE));
		}

		GL_CHECK(glClear(mode));

		if (!depthWriteMask && flags & ClearFlags::Depth)
		{
			glDepthMask(GL_FALSE);
		}
	}

	EGL2Check::checkError();
}

void
EGL2DeviceContext::clearRenderTexture(ClearFlags flags, const Vector4& color, float depth, std::int32_t stencil, std::size_t i) noexcept
{
	this->clearRenderTexture(flags, color, depth, stencil);
}

void
EGL2DeviceContext::discardRenderTexture() noexcept
{
	GL_PLATFORM_LOG("Can't support discardRenderTexture");
}

void
EGL2DeviceContext::readRenderTexture(GraphicsRenderTexturePtr target, TextureFormat pfd, std::size_t w, std::size_t h, void* data) noexcept
{
	assert(w && h && data);

	if (target)
	{
		GLenum format = EGL2Types::asFormat(pfd);
		GLenum type = EGL2Types::asType(pfd);

		if (format != GL_INVALID_ENUM && type != GL_INVALID_ENUM)
		{
			this->setRenderTexture(target);
			glReadPixels(0, 0, w, h, format, type, data);
		}
	}
}

void
EGL2DeviceContext::setGraphicsProgram(GraphicsProgramPtr shader) noexcept
{
	if (shader)
	{
		auto glshader = shader->downcast<EGL2ShaderObject>();
		if (_shaderObject != glshader)
		{
			if (_shaderObject)
				_shaderObject->setActive(false);

			_shaderObject = glshader;

			if (_shaderObject)
				_shaderObject->setActive(true);
		}
		else
		{
			_shaderObject->setActive(true);
		}
	}
	else
	{
		if (_shaderObject)
			_shaderObject->setActive(false);

		glUseProgram(GL_NONE);
	}
}

GraphicsProgramPtr
EGL2DeviceContext::getGraphicsProgram() const noexcept
{
	return _shaderObject;
}

void
EGL2DeviceContext::debugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) noexcept
{
	std::cerr << "source : ";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		std::cerr << "GL_DEBUG_SOURCE_API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		std::cerr << "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		std::cerr << "GL_DEBUG_SOURCE_SHADER_COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		std::cerr << "GL_DEBUG_SOURCE_THIRD_PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		std::cerr << "GL_DEBUG_SOURCE_APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		std::cerr << "GL_DEBUG_SOURCE_OTHER";
		break;
	}

	std::cerr << std::endl;

	std::cerr << "type : ";
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		std::cerr << "GL_DEBUG_TYPE_ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cerr << "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cerr << "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cerr << "GL_DEBUG_TYPE_PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cerr << "GL_DEBUG_TYPE_PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cerr << "GL_DEBUG_TYPE_OTHER";
		break;
	case GL_DEBUG_TYPE_MARKER:
		std::cerr << "GL_DEBUG_TYPE_MARKER";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		std::cerr << "GL_DEBUG_TYPE_PUSH_GROUP";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		std::cerr << "GL_DEBUG_TYPE_POP_GROUP";
		break;
	}

	std::cerr << std::endl;

	std::cerr << "id : " << id << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cerr << "notice";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		std::cerr << "low";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cerr << "medium";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		std::cerr << "high";
		break;
	}

	std::cerr << std::endl;

	std::cerr << "message : " << message << std::endl;
}

void
EGL2DeviceContext::initDebugControl() noexcept
{
#if defined(_DEBUG) && !defined(__ANDROID__)
	// 131184 memory info
	// 131185 memory allocation info
	GLuint ids[] =
	{
		131076,
		131169,
		131184,
		131185,
		131218,
		131204
	};

	GL_CHECK(glEnable(GL_DEBUG_OUTPUT));

	GL_CHECK(glDebugMessageCallback(debugCallBack, this));
	// enable all
	GL_CHECK(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE));
	// disable ids
	GL_CHECK(glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 6, ids, GL_FALSE));
#endif
}

void
EGL2DeviceContext::initStateSystem() noexcept
{
	GL_CHECK(glClearDepthf(1.0));
	GL_CHECK(glClearColor(0.0, 0.0, 0.0, 0.0));
	GL_CHECK(glClearStencil(0));

	GL_CHECK(glViewport(0, 0, 0, 0));

	GL_CHECK(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));

	GL_CHECK(glEnable(GL_DEPTH_TEST));
	GL_CHECK(glDepthMask(GL_TRUE));
	GL_CHECK(glDepthFunc(GL_LEQUAL));

	GL_CHECK(glEnable(GL_CULL_FACE));
	GL_CHECK(glCullFace(GL_BACK));
	GL_CHECK(glFrontFace(GL_CW));
	GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	GL_CHECK(glDisable(GL_STENCIL_TEST));
	GL_CHECK(glStencilMask(0xFFFFFFFF));
	GL_CHECK(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
	GL_CHECK(glStencilFunc(GL_ALWAYS, 0, 0xFFFFFFFF));

	GL_CHECK(glDisable(GL_BLEND));
	GL_CHECK(glBlendEquation(GL_FUNC_ADD));
	GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	_maxTextureUnits = MAX_TEXTURE_UNIT;
	_maxViewports = 4;

	_clearColor.set(0.0, 0.0, 0.0, 0.0);
	_clearDepth = 1.0;
	_clearStencil = 0;

	_viewport.left = 0;
	_viewport.top = 0;
	_viewport.width = 0;
	_viewport.height = 0;

	_enableWireframe = false;

	_textureUnits.resize(_maxTextureUnits);

	_stateDefault = std::make_shared<EGL2GraphicsState>();
	_stateCaptured = GraphicsStateDesc();
}

void
EGL2DeviceContext::setDevice(GraphicsDevicePtr device) noexcept
{
	_device = device;
}

GraphicsDevicePtr
EGL2DeviceContext::getDevice() noexcept
{
	return _device.lock();
}

_NAME_END