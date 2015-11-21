// +----------------------------------------------------------------------
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
#ifndef _H_GUI_IMAGE_LOADER_H_
#define _H_GUI_IMAGE_LOADER_H_

#include <ray/rtti.h>

_NAME_BEGIN

namespace Gui
{
	enum PixelFormat
	{
		Unknow,
		L8, // 1 byte pixel format, 1 byte luminance
		L8A8, // 2 byte pixel format, 1 byte luminance, 1 byte alpha
		R8G8B8, // 24-bit pixel format, 8 bits for red, green and blue.
		R8G8B8A8 // 32-bit pixel format, 8 bits for red, green, blue and alpha.
	};

	class EXPORT GuiImageLoader : public rtti::Interface
	{
		__DeclareSubClass(GuiImageLoader, rtti::Interface)
	public:
		GuiImageLoader() noexcept;
		virtual ~GuiImageLoader() noexcept;

		virtual void* loadImage(int& _width, int& _height, Gui::PixelFormat& _format, const std::string& _filename);
		virtual void saveImage(int _width, int _height, Gui::PixelFormat _format, void* _texture, const std::string& _filename);
	};
}

_NAME_END

#endif