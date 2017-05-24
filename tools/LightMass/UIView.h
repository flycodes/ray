// +----------------------------------------------------------------------
// | Project : ray.
// | All rights reserved.
// +----------------------------------------------------------------------
// | Copyright (c) 2013-2017.
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
#ifndef _H_UI_CONTROLLER_H_
#define _H_UI_CONTROLLER_H_

#include <ray/game_component.h>
#include "UIParams.h"

class GuiViewComponent final : public ray::GameComponent
{
	__DeclareSubClass(GuiViewComponent, ray::GameComponent)
public:
	GuiViewComponent() noexcept;
	~GuiViewComponent() noexcept;

	ray::GameComponentPtr clone() const noexcept;

	void setProjectImportListener(std::function<bool(ray::util::string::const_pointer, ray::util::string&)> delegate);
	void setProjectSaveListener(std::function<bool(ray::util::string::const_pointer, ray::util::string&)> delegate);
	void setProjectSaveAsListener(std::function<bool(ray::util::string::const_pointer, ray::util::string&)> delegate);

	void setModelImportListener(std::function<bool(ray::util::string::const_pointer, ray::util::string&)> delegate);
	void setModelSaveAsListener(std::function<bool(ray::util::string::const_pointer, ray::util::string&)> delegate);

private:
	virtual void onMessage(const ray::MessagePtr& message) noexcept;

private:
	void showMainMenu() noexcept;
	void showStyleEditor() noexcept;
	void showLightMass() noexcept;
	void showAboutWindow() noexcept;

	bool showFileOpenBrowse(ray::util::string::pointer path, ray::util::string::size_type max_length, ray::util::string::const_pointer ext_name) noexcept;
	bool showFileSaveBrowse(ray::util::string::pointer path, ray::util::string::size_type max_length, ray::util::string::const_pointer ext_name) noexcept;

	void showModelImportBrowse() noexcept;
	void showModelExportBrowse() noexcept;

	void showProjectOpenBrowse() noexcept;
	void showProjectSaveBrowse() noexcept;
	void showProjectSaveAsBrowse() noexcept;

private:
	GuiViewComponent(const GuiViewComponent&) = delete;
	GuiViewComponent& operator=(const GuiViewComponent&) = delete;

private:
	float _fps;

	bool _showMainMenu;
	bool _showLightMassWindow;
	bool _showStyleEditor;
	bool _showAboutWindow;

	std::string _pathProject;

	ray::float4 _clearColor;

	GuiParams _default;
	GuiParams _setting;

	std::function<bool(ray::util::string::const_pointer, ray::util::string&)> _onProjectOpen;
	std::function<bool(ray::util::string::const_pointer, ray::util::string&)> _onProjectSave;
	std::function<bool(ray::util::string::const_pointer, ray::util::string&)> _onProjectSaveAs;

	std::function<bool(ray::util::string::const_pointer, ray::util::string&)> _onModelImport;
	std::function<bool(ray::util::string::const_pointer, ray::util::string&)> _onModelSaveAs;
};

#endif