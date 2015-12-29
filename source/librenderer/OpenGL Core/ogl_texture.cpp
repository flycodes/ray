// +----------------------------------------------------------------------
// | Project : ray.
// | All rights reserved.
// +----------------------------------------------------------------------
// | Copyright (c) 2013-2014.
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
#include "ogl_texture.h"

_NAME_BEGIN

__ImplementSubClass(OGLTexture, GraphicsTexture, "OGLTexture")

OGLTexture::OGLTexture() noexcept
	: _texture(0)
	, _target(GL_NONE)
{
}

OGLTexture::~OGLTexture() noexcept
{
	this->close();
}

bool
OGLTexture::setup(const GraphicsTextureDesc& textureDesc) noexcept
{
	assert(_texture == GL_NONE);

	GLenum target = OGLTypes::asTextureTarget(textureDesc.getTexDim(), textureDesc.isMultiSample());
	if (target == GL_INVALID_ENUM)
		return false;

	glCreateTextures(target, 1, &_texture);
	if (_texture == GL_NONE)
	{
		GL_PLATFORM_LOG("glCreateTextures fail");
		return false;
	}

	GLsizei w = (GLsizei)textureDesc.getWidth();
	GLsizei h = (GLsizei)textureDesc.getHeight();
	GLsizei depth = (GLsizei)textureDesc.getDepth();

	if (!applySamplerWrap(textureDesc.getSamplerWrap()))
		return false;

	if (!applySamplerFilter(textureDesc.getSamplerFilter()))
		return false;

	if (!applySamplerAnis(textureDesc.getSamplerAnis()))
		return false;

	GLenum internalFormat = OGLTypes::asTextureInternalFormat(textureDesc.getTexFormat());
	if (internalFormat == GL_INVALID_ENUM)
		return false;

	GLsizei level = target == GL_TEXTURE_CUBE_MAP ? 6 : 1;
	level = std::max(level, textureDesc.getMipLevel());

	if (target == GL_TEXTURE_2D)
		glTextureStorage2D(_texture, level, internalFormat, w, h);
	else if (target == GL_TEXTURE_2D_MULTISAMPLE)
		glTextureStorage2DMultisample(_texture, level, internalFormat, w, h, GL_FALSE);
	else if (target == GL_TEXTURE_2D_ARRAY)
		glTextureStorage3D(_texture, level, internalFormat, w, h, depth);
	else if (target == GL_TEXTURE_2D_MULTISAMPLE_ARRAY)
		glTextureStorage3DMultisample(_texture, level, internalFormat, w, h, depth, GL_FALSE);
	else if (target == GL_TEXTURE_3D)
		glTextureStorage3D(_texture, level, internalFormat, w, h, depth);
	else if (target == GL_TEXTURE_CUBE_MAP)
		glTextureStorage2D(_texture, level, internalFormat, w, h);
	else if (target == GL_TEXTURE_CUBE_MAP_ARRAY)
		glTextureStorage3D(_texture, level, internalFormat, w, h, depth);

	auto stream = textureDesc.getStream();
	if (stream)
	{
		if (internalFormat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT ||
			internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ||
			internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT ||
			internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT ||
			internalFormat == GL_COMPRESSED_RG_RGTC2 ||
			internalFormat == GL_COMPRESSED_SIGNED_RG_RGTC2)
		{
			GLsizei offset = 0;
			GLsizei blockSize = internalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16;

			for (GLint mip = 0; mip < level; mip++)
			{
				GLsizei mipSize = ((w + 3) / 4) * ((h + 3) / 4) * blockSize;

				glCompressedTextureSubImage2D(_texture, mip, 0, 0, w, h, internalFormat, mipSize, (char*)stream + offset);

				w = std::max(w >> 1, 1);
				h = std::max(h >> 1, 1);

				offset += mipSize;
			}
		}
		else
		{
			GLenum format = OGLTypes::asTextureFormat(textureDesc.getTexFormat());
			GLenum type = OGLTypes::asTextureType(textureDesc.getTexFormat());

			switch (target)
			{
			case GL_TEXTURE_2D:
				glTextureSubImage2D(_texture, 0, 0, 0, w, h, format, type, stream);
				break;
			case GL_TEXTURE_2D_ARRAY:
				glTextureSubImage3D(_texture, 0, 0, 0, 0, w, h, depth, format, type, stream);
				break;
			case GL_TEXTURE_3D:
				glTextureSubImage3D(_texture, 0, 0, 0, 0, w, h, depth, format, type, stream);
				break;
			case GL_TEXTURE_CUBE_MAP:
				glTextureSubImage3D(_texture, 0, 0, 0, 0, w, h, 6, format, type, stream);
				break;
			break;
			default:
				assert(false);
				break;
			}
		}
	}

	if (textureDesc.isMipmap())
	{
		glGenerateTextureMipmap(_texture);
	}


	_target = target;
	_textureDesc = textureDesc;

	return true;
}

void
OGLTexture::close() noexcept
{
	if (_texture)
	{
		glDeleteTextures(1, &_texture);
		_texture = GL_NONE;
	}
}

bool
OGLTexture::isMultiSample() const noexcept
{
	return _textureDesc.isMultiSample();
}

GLenum
OGLTexture::getTarget() const noexcept
{
	return _target;
}

GLuint
OGLTexture::getInstanceID() noexcept
{
	return _texture;
}

const GraphicsTextureDesc&
OGLTexture::getGraphicsTextureDesc() const noexcept
{
	return _textureDesc;
}

bool
OGLTexture::applySamplerWrap(SamplerWrap wrap) noexcept
{
	GLenum glwrap = OGLTypes::asSamplerWrap(wrap);
	if (glwrap != GL_INVALID_ENUM)
	{
		glTextureParameteri(_texture, GL_TEXTURE_WRAP_S, glwrap);
		glTextureParameteri(_texture, GL_TEXTURE_WRAP_T, glwrap);
		glTextureParameteri(_texture, GL_TEXTURE_WRAP_R, glwrap);

		return true;
	}

	return false;
}

bool
OGLTexture::applySamplerFilter(SamplerFilter filter) noexcept
{
	GLenum glfilter = OGLTypes::asSamplerFilter(filter);
	if (glfilter != GL_INVALID_ENUM)
	{
		glTextureParameteri(_texture, GL_TEXTURE_MAG_FILTER, glfilter);
		glTextureParameteri(_texture, GL_TEXTURE_MIN_FILTER, glfilter);
		return true;
	}

	return false;
}

bool
OGLTexture::applySamplerAnis(SamplerAnis anis) noexcept
{
	if (anis == SamplerAnis::Anis1)
		glTextureParameteri(_texture, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
	else if (anis == SamplerAnis::Anis2)
		glTextureParameteri(_texture, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
	else if (anis == SamplerAnis::Anis4)
		glTextureParameteri(_texture, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
	else if (anis == SamplerAnis::Anis8)
		glTextureParameteri(_texture, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
	else if (anis == SamplerAnis::Anis16)
		glTextureParameteri(_texture, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	else
	{
		if (anis != SamplerAnis::Anis0)
		{
			GL_PLATFORM_LOG("Invalid SamplerAnis");
			return false;
		}
	}

	return true;
}

void
OGLTexture::setDevice(GraphicsDevicePtr device) noexcept
{
	_device = device;
}

GraphicsDevicePtr
OGLTexture::getDevice() noexcept
{
	return _device.lock();
}

_NAME_END