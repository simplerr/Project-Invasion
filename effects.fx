//=============================================================================
// dirLightTex.fx by Frank Luna (C) 2004 All Rights Reserved.
//
// Uses a directional light plus texturing.
//=============================================================================

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float power;
};

struct Light
{
	Material material;	// Colors
	float3 position;	// Position
	float3 direction;	// Direction
	float spotPower;	// Determines the size of the spotlight cone (0 = point light).
	float length;		// How far until the light starts to fade (does not affect spotlights).
};

uniform extern float4x4 gWVP;
uniform extern float4x4 gWorld;
uniform extern float4x4 gWorldInverseTranspose;
uniform extern texture	gTex;
uniform extern float3	gEyePosW;

uniform extern Material gObjectMaterial;
uniform extern Light	gLights[10];
uniform extern int		gNumLights;

// Textures
uniform extern texture gTex0;
uniform extern texture gTex1;
uniform extern texture gTex2;
uniform extern texture gBlendMap;

// Skinned mesh transforms
uniform extern float4x4 gFinalXForms[50];
uniform extern float4x4 gBoneTransform;

// Object texture
sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};
 
// Terrain textures
sampler Tex0S = sampler_state
{
	Texture = <gTex0>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};
sampler Tex1S = sampler_state
{
	Texture = <gTex1>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};
sampler Tex2S = sampler_state
{
	Texture = <gTex2>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};
sampler BlendMapS = sampler_state
{
	Texture = <gBlendMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

// Outputs
struct OutputVS
{
    float4 posH    : POSITION0;
    float4 diffuse : COLOR0;
    float4 spec    : COLOR1;
    float2 tex0    : TEXCOORD0;
};
struct TerrainOutputVS
{
    float4 posH         : POSITION0;
    float4 diffuse      : COLOR0;
    float4 spec         : COLOR1;
    float2 tiledTexC    : TEXCOORD0;
    float2 nonTiledTexC : TEXCOORD1;
};
struct SkinnedOutputVS
{
    float4 posH    : POSITION0;
    float3 normalW : TEXCOORD0;
    float3 toEyeW  : TEXCOORD1;
    float2 tex0    : TEXCOORD2;
};

OutputVS TexVS(float3 posL : POSITION0, float3 normalL : NORMAL0, float2 tex0: TEXCOORD0)
{
    // Zero out our output.
	OutputVS outVS = (OutputVS)0;
	
	// Transform normal to world space.
	float3 normalW = mul(float4(normalL, 0.0f), gWorldInverseTranspose).xyz;
	normalW = normalize(normalW);
	
	// Transform vertex position to world space.
	float3 posW = mul(float4(posL, 1.0f), gWorld).xyz;

	for(int i = 0; i < gNumLights; i++)	
	{
		// Unit vector from vertex to light source.
		float3 lightVecW = normalize(gLights[i].position - posW);	// float3(50, 50, 50)	// FELFELFELFLEFLELFELE

		//==================================================
		// Compute the color: Equation 10.3.
	
		// Compute the vector from the vertex to the eye position.
		float3 toEye = normalize(gEyePosW - posW);

		// Compute the reflection vector.
		float3 r = reflect(-lightVecW, normalW);

		// Determine how much (if any) specular light makes it intro the eye.
		float t = pow(max(dot(r, toEye), 0.0f), gLights[i].material.power);

		// Determine the diffuse light intensity that strikes the vertex.
		float s = max(dot(normalW, lightVecW), 0.0f);

		// Compute the specular, diffuse and ambient terms seperately.
		float3 specular = t*(gObjectMaterial.specular*gLights[i].material.specular).rgb;
		float3 diffuse = s*(gObjectMaterial.diffuse*gLights[i].material.diffuse).rgb; 
		float3 ambient = gObjectMaterial.ambient.rgb*gLights[i].material.ambient.rgb;

		// Distance.
		float d = distance(gLights[i].position, posW);

		// Spotlight factor.
		float spot = pow(max(dot(-lightVecW, gLights[i].direction), 0.0f), gLights[i].spotPower);
		spot *=  min(gLights[i].spotPower == 0 ? gLights[i].length / d : 1, 1.2);

		// Sum all the terms together and copy over the diffuse alpha.
		outVS.diffuse.rgb += spot*(ambient + diffuse);
		outVS.spec += float4(specular*spot, 0.0f); 
	}

	outVS.diffuse.a  = gObjectMaterial.diffuse.a;

	// Transform to homogeneous clip space.
	outVS.posH = mul(float4(posL, 1.0f), gWVP);
	
	// Pass on texture coordinates to be interpolated in rasterization.
	outVS.tex0 = tex0;
	
	// Done--return the output.
    return outVS;
}

float4 TexPS(float4 c : COLOR0, float4 spec : COLOR1, float2 tex0 : TEXCOORD0) : COLOR
{
	float4 texColor = tex2D(TexS, tex0);
	float3 diffuse = c.rgb * texColor.rgb;
    return float4(diffuse + spec.rgb, c.a); 
}

//========================================================================================
// Terrain shaders
TerrainOutputVS TerrainVS(float3 posL : POSITION0, float3 normalL : NORMAL0, float2 tex0: TEXCOORD0)
{
    // Zero out our output.
	TerrainOutputVS outVS = (TerrainOutputVS)0;
	
	// Transform normal to world space.
	float3 normalW = mul(float4(normalL, 0.0f), gWorldInverseTranspose).xyz;
	normalW = normalize(normalW);
	
	// Transform vertex position to world space.
	float3 posW = mul(float4(posL, 1.0f), gWorld).xyz;

	for(int i = 0; i < gNumLights; i++)	
	{
		// Unit vector from vertex to light source.
		float3 lightVecW = normalize(gLights[i].position - posW);

		//==================================================
		// Compute the color: Equation 10.3.
	
		// Compute the vector from the vertex to the eye position.
		float3 toEye = normalize(gEyePosW - posW);

		// Compute the reflection vector.
		float3 r = reflect(-lightVecW, normalW);

		// Determine how much (if any) specular light makes it intro the eye.
		float t = pow(max(dot(r, toEye), 0.0f), gLights[i].material.power);

		// Determine the diffuse light intensity that strikes the vertex.
		float s = max(dot(normalW, lightVecW), 0.0f);

		// Compute the specular, diffuse and ambient terms seperately.
		float3 specular = t*(gObjectMaterial.specular*gLights[i].material.specular).rgb;
		float3 diffuse = s*(gObjectMaterial.diffuse*gLights[i].material.diffuse).rgb; 
		float3 ambient = gObjectMaterial.ambient.rgb*gLights[i].material.ambient.rgb;

		// Distance.
		float d = distance(gLights[i].position, posW);
		
		// Spotlight factor.
		float spot = pow(max(dot(-lightVecW, gLights[i].direction), 0.0f), gLights[i].spotPower);
		spot *=  min(gLights[i].spotPower == 0 ? gLights[i].length / d : 1, 1.2);

		// Sum all the terms together and copy over the diffuse alpha.
		outVS.diffuse.rgb += spot*(ambient + diffuse);
		outVS.spec += float4(specular*spot, 0.0f); 
	}

	outVS.diffuse.a = gObjectMaterial.diffuse.a;

	// Transform to homogeneous clip space.
	outVS.posH = mul(float4(posL, 1.0f), gWVP);
	
	// Pass on texture coordinates to be interpolated in rasterization.
	outVS.tiledTexC    = tex0 * 16.0f; // Scale tex-coord to tile 16 times.
	outVS.nonTiledTexC = tex0;
	
	// Done--return the output.
    return outVS;
}

float4 TerrainPS(float4 diffuse : COLOR0, float4 spec : COLOR1, float2 tiledTexC : TEXCOORD0, float2 nonTiledTexC : TEXCOORD1) : COLOR
{
	// Layer maps are tiled
    float3 c0 = tex2D(Tex0S, tiledTexC).rgb;
    float3 c1 = tex2D(Tex1S, tiledTexC).rgb;
    float3 c2 = tex2D(Tex2S, tiledTexC).rgb;
    
    // Blendmap is not tiled.
    float3 B = tex2D(BlendMapS, nonTiledTexC).rgb;

	// Find the inverse of all the blend weights so that we can
	// scale the total color to the range [0, 1].
    float totalInverse = 1.0f / (B.r + B.g + B.b);
    
    // Scale the colors by each layer by its corresponding weight
    // stored in the blendmap.  
    c0 *= B.r * totalInverse;
    c1 *= B.g * totalInverse;
    c2 *= B.b * totalInverse;
    
    // Sum the colors and modulate with the lighting color.
    float3 final = (c0 + c1 + c2) * diffuse.rgb;
    
    return float4(final + spec, diffuse.a);
}

SkinnedOutputVS SkinnedVS(float3 posL : POSITION0, float3 normalL : NORMAL0, float2 tex0 : TEXCOORD0, float weight0 : BLENDWEIGHT0, int4 boneIndex : BLENDINDICES0)
{
	// Zero out our output.
	SkinnedOutputVS outVS = (SkinnedOutputVS)0;
	
	// Do the vertex blending calculation for posL and normalL.
	float weight1 = 1.0f - weight0;

	float4 p = weight0 * mul(float4(posL, 1.0f), gFinalXForms[boneIndex[0]]);
    p       += weight1 * mul(float4(posL, 1.0f), gFinalXForms[boneIndex[1]]);
    p.w = 1.0f;
    
    // We can use the same matrix to transform normals since we are assuming
    // no scaling (i.e., rigid-body).
    float4 n = weight0 * mul(float4(normalL, 0.0f), gFinalXForms[boneIndex[0]]);
    n       += weight1 * mul(float4(normalL, 0.0f), gFinalXForms[boneIndex[1]]);
    n.w = 0.0f;

	// Transform normal to world space.
	outVS.normalW = mul(n, gWorldInverseTranspose).xyz;
	
	// Transform vertex position to world space.
	float3 posW = mul(p, gWorld).xyz;

	// Compute the vector from the vertex to the eye.
	outVS.toEyeW = gEyePosW - posW;
	
	// Transform to homogeneous clip space.
	outVS.posH = mul(p, gWVP);
	
	// Pass on texture coordinates to be interpolated in rasterization.
	outVS.tex0 = tex0;

	// Done--return the output.
    return outVS;
}

float4 SkinnedPS(float3 normalW : TEXCOORD0, float3 toEyeW  : TEXCOORD1, float2 tex0 : TEXCOORD2) : COLOR
{
	// Interpolated normals can become unnormal--so normalize.
	normalW = normalize(normalW);
	toEyeW  = normalize(toEyeW);
	
	float3 color = (float3)0;
	for(int i = 0; i < gNumLights; i++)	
	{
		// Light vector is opposite the direction of the light.
		float3 lightVecW = -gLights[i].direction;
	
		// Compute the reflection vector.
		float3 r = reflect(-lightVecW, normalW);
	
		// Determine how much (if any) specular light makes it intro the eye.
		float t = pow(max(dot(r, toEyeW), 0.0f), gLights[i].material.power);
	
		// Determine the diffuse light intensity that strikes the vertex.
		float s = max(dot(lightVecW, normalW), 0.0f);
	
		// Compute the specular, diffuse and ambient terms seperately.
		float3 specular = t*(gObjectMaterial.specular*gLights[i].material.specular).rgb;
		float3 diffuse = s*(gObjectMaterial.diffuse*gLights[i].material.diffuse).rgb; 
		float3 ambient = gObjectMaterial.ambient.rgb*gLights[i].material.ambient.rgb;
	
		// The vertex position
		float3 posW = float3(0, 0, 0);//gEyePosW - toEyeW;

		// Distance.
		float d = distance(gLights[i].position, posW);
		
		// Spotlight factor.
		float spot = pow(max(dot(-lightVecW, gLights[i].direction), 0.0f), gLights[i].spotPower);
		spot *=  min(gLights[i].spotPower == 0 ? gLights[i].length / d : 1, 1.2);

		// Sum all the terms together and copy over the diffuse alpha.
		color.rgb += spot*(ambient + diffuse);
		color.rgb += float4(specular*spot, 0.0f); 	
	}

	// Get the texture color.
	float4 texColor = tex2D(TexS, tex0);

	color *= texColor.rgb;

	return texColor;

	// Sum all the terms together and copy over the diffuse alpha.
    return float4(color, gObjectMaterial.diffuse.a*texColor.a);
}

technique TexTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_3_0 TexVS();
        pixelShader  = compile ps_3_0 TexPS();
    }
}

technique TerrainTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_3_0 TerrainVS();
        pixelShader  = compile ps_3_0 TerrainPS();
    }
}

technique SkinnedMeshTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        vertexShader = compile vs_3_0 SkinnedVS();
        pixelShader  = compile ps_3_0 SkinnedPS();
    }
}