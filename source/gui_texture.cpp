/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#include <ray/gui_texture.h>
#include <ray/gui_renderer.h>
#include <ray/gui_assert.h>
#include <ray/gui_system.h>

#include <GL/glew.h>

_NAME_BEGIN

using namespace MyGUI;

OpenGL3Texture::OpenGL3Texture(const std::string& _name, GuiImageLoader* _loader) :
    mName(_name),
	mWidth(0),
    mHeight(0),
	mPixelFormat(0),
    mInternalPixelFormat(0),
    mUsage(0),
    mAccess(0),
    mNumElemBytes(0),
    mDataSize(0),
    mTextureID(0),
    mPboID(0),
    mLock(false),
    mBuffer(0),
	mImageLoader(_loader),
	mRenderTarget(nullptr)
{
}

OpenGL3Texture::~OpenGL3Texture()
{
	destroy();
}

const std::string& OpenGL3Texture::getName() const
{
	return mName;
}

void OpenGL3Texture::setUsage(TextureUsage _usage)
{
	mAccess = 0;
	mUsage = 0;

	if (_usage == TextureUsage::Default)
	{
		mUsage = GL_STATIC_READ;
		mAccess = GL_READ_ONLY;
	}
	else if (_usage.isValue(TextureUsage::Static))
	{
		if (_usage.isValue(TextureUsage::Read))
		{
			if (_usage.isValue(TextureUsage::Write))
			{
				mUsage = GL_STATIC_COPY;
				mAccess = GL_READ_WRITE;
			}
			else
			{
				mUsage = GL_STATIC_READ;
				mAccess = GL_READ_ONLY;
			}
		}
		else if (_usage.isValue(TextureUsage::Write))
		{
			mUsage = GL_STATIC_DRAW;
			mAccess = GL_WRITE_ONLY;
		}
	}
	else if (_usage.isValue(TextureUsage::Dynamic))
	{
		if (_usage.isValue(TextureUsage::Read))
		{
			if (_usage.isValue(TextureUsage::Write))
			{
				mUsage = GL_DYNAMIC_COPY;
				mAccess = GL_READ_WRITE;
			}
			else
			{
				mUsage = GL_DYNAMIC_READ;
				mAccess = GL_READ_ONLY;
			}
		}
		else if (_usage.isValue(TextureUsage::Write))
		{
			mUsage = GL_DYNAMIC_DRAW;
			mAccess = GL_WRITE_ONLY;
		}
	}
	else if (_usage.isValue(TextureUsage::Stream))
	{
		if (_usage.isValue(TextureUsage::Read))
		{
			if (_usage.isValue(TextureUsage::Write))
			{
				mUsage = GL_STREAM_COPY;
				mAccess = GL_READ_WRITE;
			}
			else
			{
				mUsage = GL_STREAM_READ;
				mAccess = GL_READ_ONLY;
			}
		}
		else if (_usage.isValue(TextureUsage::Write))
		{
			mUsage = GL_STREAM_DRAW;
			mAccess = GL_WRITE_ONLY;
		}
	}
else if (_usage.isValue(TextureUsage::RenderTarget))
{
    mUsage = GL_DYNAMIC_READ;
    mAccess = GL_READ_ONLY;
}
}

void OpenGL3Texture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
{
	createManual(_width, _height, _usage, _format, 0);
}

void OpenGL3Texture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data)
{
	MYGUI_PLATFORM_ASSERT(!mTextureID, "Texture already exist");

	//FIXME перенести в метод
	mInternalPixelFormat = 0;
	mPixelFormat = 0;
	mNumElemBytes = 0;
	if (_format == PixelFormat::R8G8B8)
	{
		mInternalPixelFormat = GL_RGB8;
		mPixelFormat = GL_BGR;
		mNumElemBytes = 3;
	}
	else if (_format == PixelFormat::R8G8B8A8)
	{
		mInternalPixelFormat = GL_RGBA8;
		mPixelFormat = GL_BGRA;
		mNumElemBytes = 4;
	}
	else
	{
		MYGUI_PLATFORM_EXCEPT("format not support");
	}

	mWidth = _width;
	mHeight = _height;
	mDataSize = _width * _height * mNumElemBytes;
	setUsage(_usage);
	//MYGUI_PLATFORM_ASSERT(mUsage, "usage format not support");

	mOriginalFormat = _format;
	mOriginalUsage = _usage;

	// Set unpack alignment to one byte
	int alignment = 0;
	glGetIntegerv( GL_UNPACK_ALIGNMENT, &alignment );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	// создаем тукстуру
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, mInternalPixelFormat, mWidth, mHeight, 0, mPixelFormat, GL_UNSIGNED_BYTE, (GLvoid*)_data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Restore old unpack alignment
	glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );

	if (!_data && GuiRenderer::getInstance().isPixelBufferObjectSupported())
	{
		//создаем текстурнный буфер
		glGenBuffers(1, &mPboID);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPboID);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, mDataSize, 0, mUsage);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	}
}

void OpenGL3Texture::destroy()
{
	if (mRenderTarget != nullptr)
	{
		delete mRenderTarget;
		mRenderTarget = nullptr;
	}

	if (mTextureID != 0)
	{
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}
	if (mPboID != 0)
	{
		glDeleteBuffers(1, &mPboID);
		mPboID = 0;
	}

	mWidth = 0;
	mHeight = 0;
	mLock = false;
	mPixelFormat = 0;
	mDataSize = 0;
	mUsage = 0;
	mBuffer = 0;
	mInternalPixelFormat = 0;
	mAccess = 0;
	mNumElemBytes = 0;
	mOriginalFormat = PixelFormat::Unknow;
	mOriginalUsage = TextureUsage::Default;
}

void* OpenGL3Texture::lock(TextureUsage _access)
{
	MYGUI_PLATFORM_ASSERT(mTextureID, "Texture is not created");

	if (_access == TextureUsage::Read)
	{
		glBindTexture(GL_TEXTURE_2D, mTextureID);

		mBuffer = new unsigned char[mDataSize];
		glGetTexImage(GL_TEXTURE_2D, 0, mPixelFormat, GL_UNSIGNED_BYTE, mBuffer);

		mLock = false;

		return mBuffer;
	}

	// bind the texture
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	if (!GuiRenderer::getInstance().isPixelBufferObjectSupported())
	{
		//Fallback if PBO's are not supported
		mBuffer = new unsigned char[mDataSize];
	}
	else
	{
		// bind the PBO
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPboID);
			
		// Note that glMapBuffer() causes sync issue.
		// If GPU is working with this buffer, glMapBuffer() will wait(stall)
		// until GPU to finish its job. To avoid waiting (idle), you can call
		// first glBufferData() with NULL pointer before glMapBuffer().
		// If you do that, the previous data in PBO will be discarded and
		// glMapBuffer() returns a new allocated pointer immediately
		// even if GPU is still working with the previous data.
		glBufferData(GL_PIXEL_UNPACK_BUFFER, mDataSize, 0, mUsage);

		// map the buffer object into client's memory
		mBuffer = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, mAccess);
		if (!mBuffer)
		{
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			MYGUI_PLATFORM_EXCEPT("Error texture lock");
		}
	}

	mLock = true;

	return mBuffer;
}

void OpenGL3Texture::unlock()
{
	if (!mLock && mBuffer)
	{
        delete[] (char*)mBuffer;
		mBuffer = 0;

		glBindTexture(GL_TEXTURE_2D, 0);

		return;
	}

	MYGUI_PLATFORM_ASSERT(mLock, "Texture is not locked");

	if (!GuiRenderer::getInstance().isPixelBufferObjectSupported())
	{
		//Fallback if PBO's are not supported
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, mBuffer);
        delete[] (char*)mBuffer;
	}
	else
	{
		// release the mapped buffer
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

		// copy pixels from PBO to texture object
		// Use offset instead of ponter.
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, 0);

		// it is good idea to release PBOs with ID 0 after use.
		// Once bound with 0, all pixel operations are back to normal ways.
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	}
		
	glBindTexture(GL_TEXTURE_2D, 0);
	mBuffer = 0;
	mLock = false;
}

void OpenGL3Texture::loadFromFile(const std::string& _filename)
{
	destroy();

	if (mImageLoader)
	{
		int width = 0;
		int height = 0;
		PixelFormat format = PixelFormat::Unknow;

		void* data = mImageLoader->loadImage(width, height, format, _filename);
		if (data)
		{
			createManual(width, height, TextureUsage::Static | TextureUsage::Write, format, data);
            delete[] (unsigned char*)data;
		}
	}
}

void OpenGL3Texture::saveToFile(const std::string& _filename)
{
	if (mImageLoader)
	{
		void* data = lock(TextureUsage::Read);
		mImageLoader->saveImage(mWidth, mHeight, mOriginalFormat, data, _filename);
		unlock();
	}
}

IRenderTarget* OpenGL3Texture::getRenderTarget()
{
	if (mRenderTarget == nullptr)
		mRenderTarget = new OpenGL3RTTexture(mTextureID);

	return mRenderTarget;
}

unsigned int OpenGL3Texture::getTextureID() const
{
	return mTextureID;
}

int OpenGL3Texture::getWidth()
{
	return mWidth;
}

int OpenGL3Texture::getHeight()
{
	return mHeight;
}

bool OpenGL3Texture::isLocked()
{
	return mLock;
}

PixelFormat OpenGL3Texture::getFormat()
{
	return mOriginalFormat;
}

TextureUsage OpenGL3Texture::getUsage()
{
	return mOriginalUsage;
}

size_t OpenGL3Texture::getNumElemBytes()
{
	return mNumElemBytes;
}

OpenGL3RTTexture::OpenGL3RTTexture(unsigned int _texture) :
	mTextureID(_texture),
	mWidth(0),
	mHeight(0),
	mFBOID(0),
	mRBOID(0)
{
	int miplevel = 0;
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &mWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &mHeight);
	glBindTexture(GL_TEXTURE_2D, 0);

	mRenderTargetInfo.maximumDepth = 1.0f;
	mRenderTargetInfo.hOffset = 0;
	mRenderTargetInfo.vOffset = 0;
	mRenderTargetInfo.aspectCoef = float(mHeight) / float(mWidth);
	mRenderTargetInfo.pixScaleX = 1.0f / float(mWidth);
	mRenderTargetInfo.pixScaleY = 1.0f / float(mHeight);

	// create a framebuffer object, you need to delete them when program exits.
	glGenFramebuffersEXT(1, &mFBOID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBOID);

	// create a renderbuffer object to store depth info
	// NOTE: A depth renderable image should be attached the FBO for depth test.
	// If we don't attach a depth renderable image to the FBO, then
	// the rendering output will be corrupted because of missing depth test.
	// If you also need stencil test for your rendering, then you must
	// attach additional image to the stencil attachement point, too.
	glGenRenderbuffersEXT(1, &mRBOID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mRBOID);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, mWidth, mHeight);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	// attach a texture to FBO color attachement point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, mTextureID, 0);

	// attach a renderbuffer to depth attachment point
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mRBOID);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

OpenGL3RTTexture::~OpenGL3RTTexture()
{
	if (mFBOID != 0)
	{
		glDeleteFramebuffersEXT(1, &mFBOID);
		mFBOID = 0;
	}
	if (mRBOID != 0)
	{
		glDeleteRenderbuffersEXT(1, &mRBOID);
		mRBOID = 0;
	}
}

void OpenGL3RTTexture::begin()
{
	glGetIntegerv(GL_VIEWPORT, mSavedViewport); // save current viewport

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBOID);

	glViewport(0, 0, mWidth, mHeight);

	GuiRenderer::getInstance().begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGL3RTTexture::end()
{
	GuiRenderer::getInstance().end();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind

	glViewport(mSavedViewport[0], mSavedViewport[1], mSavedViewport[2], mSavedViewport[3]); // restore old viewport
}

void OpenGL3RTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
{
	GuiRenderer::getInstance().doRenderRTT(_buffer, _texture, _count);
}

_NAME_END
