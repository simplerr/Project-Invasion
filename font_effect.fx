uniform extern texture  gTex;
uniform extern float3	gColor;

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	MaxAnisotropy = 8;
};

float4 ColorPS(float4 c : COLOR0, float2 tex0 : TEXCOORD0) : COLOR
{
	float4 texColor;

	texColor = tex2D(TexS, tex0).rgba;
	texColor.rgb = gColor.rgb/255.0f;	// To get 0-1 range!
	
	return texColor;
}

technique FontTech
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        pixelShader  = compile ps_2_0 ColorPS();

		SrcBlend = Zero;
		DestBlend = InvSrcAlpha;
		AlphaBlendEnable = True;	
    }
}
