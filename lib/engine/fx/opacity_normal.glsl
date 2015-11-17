<?xml version='1.0'?>
<effect language="glsl">
    <include name="sys:fx/common.glsl"/>
    <parameter name="matModel" semantic="matModel" />
    <parameter name="matModelInverseTranspose" semantic="matModelInverseTranspose" />
    <parameter name="matView" semantic="matView"/>
    <parameter name="matViewProject" semantic="matViewProject"/>
    <parameter name="diffuse" type="float4" />
    <parameter name="specular" type="float4" />
    <parameter name="shininess" type="float" />
    <parameter name="texDiffuse" type="sampler2D"/>
    <parameter name="texSpecular" type="sampler2D" />
    <parameter name="texNormal" type="sampler2D" />
    <shader name="vertex">
        <![CDATA[
            uniform mat4 matModel;
            uniform mat4 matModelInverseTranspose;
            uniform mat4 matView;
            uniform mat4 matViewProject;

            struct Vertex
            {
                vec2 coord;
                vec3 normal;
                vec3 tangent;
            };

            out Vertex output;

            void OpaqueVS()
            {
                output.coord = vec2(glsl_Texcoord.x, glsl_Texcoord.y);
                output.normal = mul(mat3(matView), mul(mat3(matModelInverseTranspose), glsl_Normal.xyz));
                output.tangent = mul(mat3(matModelInverseTranspose), glsl_Tangent.xyz);
                gl_Position = mul(matViewProject, mul(matModel, glsl_Position));
            }
        ]]>
    </shader>
    <shader name="fragment">
        <![CDATA[
            uniform float shininess;
            uniform vec4 diffuse;
            uniform vec4 specular;

            uniform sampler2D texNormal;
            uniform sampler2D texDiffuse;
            uniform sampler2D texSpecular;

            struct Vertex
            {
                vec2 coord;
                vec3 normal;
                vec3 tangent;
            };

            in Vertex output;

            float3 BumpMap(sampler2D normalTex, float2 coord)
            {
                float3 bump;
                bump.xy = texture(normalTex, coord).gr * 2.0 - 1.0;
                bump.z = sqrt(1.0 - bump.x * bump.x - bump.y * bump.y);
                return normalize(bump);
            }

            void OpaquePS()
            {
                float4 albedo;

                float3 normal = normalize(output.normal);
                float3 tangent = normalize(output.tangent);
                float3 bitangent = cross(normal, tangent);

                float3x3 tbn = float3x3(tangent, bitangent, normal);
                float3 tangentNormal = BumpMap(texNormal, output.coord);
                float3 highNormal = mul(tbn, tangentNormal);

                if (diffuse.w > 0.0)
                    albedo = diffuse;
                else
                    albedo = texture(texDiffuse, output.coord);

                float roughness = shininess;
                float intensity = luminance(specular.rgb);
                if (specular.w == 0.0)
                {
                    roughness = texture(texSpecular, output.coord).g;
                    intensity = texture(texSpecular, output.coord).r;
                }

                glsl_FragColor0 = StoreGBufferRT0(RGBA2sRGB(albedo), intensity);
                glsl_FragColor1 = StoreGBufferRT1(highNormal, roughness);
            }
        ]]>
    </shader>
    <technique name="opaque">
        <pass name="opaque">
            <state name="vertex" value="OpaqueVS"/>
            <state name="fragment" value="OpaquePS"/>

            <state name="primitive" value="triangle_or_line" />

            <state name="cullmode" value="back"/>
        </pass>
    </technique>
</effect>