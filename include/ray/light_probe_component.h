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
#ifndef _H_LIGHT_PROBE_COMPONENT_H_
#define _H_LIGHT_PROBE_COMPONENT_H_

#include <ray/render_component.h>
#include <ray/light_probe.h>

_NAME_BEGIN

class EXPORT LightProbeComponent final : public RenderComponent
{
	__DeclareSubClass(LightProbeComponent, RenderComponent)
public:
	LightProbeComponent() noexcept;
	LightProbeComponent(const archivebuf& reader) noexcept;
	~LightProbeComponent() noexcept;

	void setVisible(bool enable) noexcept;
	bool getVisible() const noexcept;

	void setSH9(const SH9& sh) noexcept;
	const SH9& getSH9() const noexcept;

	const CameraPtr& getCamera() const noexcept;

	void load(const archivebuf& reader) noexcept;
	void save(archivebuf& write) noexcept;

	GameComponentPtr clone() const noexcept;

private:
	virtual void onActivate() noexcept;
	virtual void onDeactivate() noexcept;

	virtual void onMoveAfter() noexcept;

private:
	LightProbeComponent(const LightProbeComponent&) = delete;
	LightProbeComponent& operator=(const LightProbeComponent&) = delete;

private:
	LightProbePtr _lightProbe;
};

_NAME_END

#endif