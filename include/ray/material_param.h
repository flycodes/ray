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
#ifndef _H_MATERIAL_PARAM_H_
#define _H_MATERIAL_PARAM_H_

#include <ray/material_fwd.h>

_NAME_BEGIN

class EXPORT MaterialVariant
{
public:
	MaterialVariant() noexcept;
	MaterialVariant(const std::string& name, ShaderVariantType type) noexcept;
	virtual ~MaterialVariant() noexcept;

	void setName(const std::string& name) noexcept;
	const std::string& getName() const noexcept;

	void setType(ShaderVariantType type) noexcept;
	ShaderVariantType getType() const noexcept;

	std::size_t getSize() const noexcept;

	void assign(bool value) noexcept;
	void assign(int value) noexcept;
	void assign(const int2& value) noexcept;
	void assign(const int3& value) noexcept;
	void assign(const int4& value) noexcept;
	void assign(float value) noexcept;
	void assign(const float2& value) noexcept;
	void assign(const float3& value) noexcept;
	void assign(const float4& value) noexcept;
	void assign(const float3x3& value) noexcept;
	void assign(const float4x4& value) noexcept;
	void assign(const std::vector<float>& value) noexcept;
	void assign(const std::vector<float2>& value) noexcept;
	void assign(const std::vector<float3>& value) noexcept;
	void assign(const std::vector<float4>& value) noexcept;
	void assign(GraphicsTexturePtr texture, GraphicsSamplerPtr sampler = nullptr) noexcept;

	bool getBool() const noexcept;
	int getInt() const noexcept;
	const int2& getInt2() const noexcept;
	const int3& getInt3() const noexcept;
	const int4& getInt4() const noexcept;
	float getFloat() const noexcept;
	const float2& getFloat2() const noexcept;
	const float3& getFloat3() const noexcept;
	const float4& getFloat4() const noexcept;
	const float3x3& getFloat3x3() const noexcept;
	const float4x4& getFloat4x4() const noexcept;
	const std::vector<float>& getFloatArray() const noexcept;
	const std::vector<float2>& getFloat2Array() const noexcept;
	const std::vector<float3>& getFloat3Array() const noexcept;
	const std::vector<float4>& getFloat4Array() const noexcept;
	GraphicsTexturePtr getTexture() const noexcept;
	GraphicsSamplerPtr getTextureSampler() const noexcept;

private:

	std::string _name;

	GraphicsTexturePtr _texture;
	GraphicsSamplerPtr _textureSampler;

	union
	{
		bool b;
		int i[4];
		float f[4];
		float3x3* m3;
		float4x4* m4;
		std::vector<float>* farray;
		std::vector<float2>* farray2;
		std::vector<float3>* farray3;
		std::vector<float4>* farray4;
	} _value;

	ShaderVariantType _type;
	ShaderVariants _params;
};

class EXPORT MaterialSemantic final : public MaterialVariant
{
public:
	MaterialSemantic() noexcept;
	MaterialSemantic(const std::string& name, ShaderVariantType type) noexcept;
	virtual ~MaterialSemantic() noexcept;
};

class EXPORT MaterialParam final
{
public:
	MaterialParam() noexcept;
	MaterialParam(const std::string& name, ShaderVariantType type) noexcept;
	virtual ~MaterialParam() noexcept;

	void setName(const std::string& name) noexcept;
	const std::string& getName() const noexcept;

	void setType(ShaderVariantType type) noexcept;
	ShaderVariantType getType() const noexcept;

	void setSemantic(MaterialSemanticPtr semantic) noexcept;
	MaterialSemanticPtr getSemantic() const noexcept;

	void assign(bool value) noexcept;
	void assign(int value) noexcept;
	void assign(const int2& value) noexcept;
	void assign(float value) noexcept;
	void assign(const float2& value) noexcept;
	void assign(const float3& value) noexcept;
	void assign(const float4& value) noexcept;
	void assign(const float3x3& value) noexcept;
	void assign(const float4x4& value) noexcept;
	void assign(const std::vector<float>& value) noexcept;
	void assign(const std::vector<float2>& value) noexcept;
	void assign(const std::vector<float3>& value) noexcept;
	void assign(const std::vector<float4>& value) noexcept;
	void assign(GraphicsTexturePtr texture) noexcept;
	void assign(GraphicsSamplerPtr sampler) noexcept;

	GraphicsTexturePtr getTexture() const noexcept;
	GraphicsSamplerPtr getSampler() const noexcept;

	void addShaderUniform(ShaderUniformPtr& uniform) noexcept;
	void removeShaderUniform(ShaderUniformPtr& uniform) noexcept;
	ShaderUniforms& getShaderUniform() noexcept;;

private:
	MaterialParam(const MaterialParam&) = delete;
	MaterialParam& operator=(const MaterialParam&) = delete;

private:

	std::string _name;
	MaterialSemanticPtr _semantic;
	GraphicsTexturePtr _texture;
	GraphicsSamplerPtr _sampler;
	ShaderVariantType _type;
	ShaderUniforms _uniforms;
};

_NAME_END

#endif