
Texture2D diffuse : register(t0);
SamplerState samplerLinear : register(s0);

cbuffer CBMatrix : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
};

cbuffer CBLight : register(b1)
{
    vector lightDirection[2];
    vector lightColor[2];
    vector outputColor;
};

struct VERTEX {
    float4 pos : POSITION;
    float4 norm : NORMAL;
    float2 tex : TEXCOORD;
};

struct PIXEL
{
    float4 pos : SV_POSITION;
    float4 norm : TEXCOORD1;
    float2 tex : TEXCOORD;
};

// Vertex shader

PIXEL VertexMain(VERTEX input)
{
    PIXEL output = (VERTEX) 0;
    output.pos = mul(input.pos, model);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    output.norm = mul(input.norm, model);
    output.tex = input.tex;
    return output;
}

// Pixel shader : cube

vector PixelMainCube(PIXEL input) : SV_Target
{ 
    vector finalColor = 0;
    
    for (int i = 0; i < 2; i++)
    {
        finalColor += saturate(dot((float3) lightDirection[i], (float3)input.norm) * lightColor[i]);
    }
    
    finalColor += diffuse.Sample(samplerLinear, input.tex);
    finalColor.a = 1;
    return finalColor;
}

// Pixel shader : lights

vector PixelMainLights(PIXEL input) : SV_Target
{
    return outputColor;
}