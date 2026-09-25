cbuffer SpriteConstants : register(b0)
{
    row_major float4x4 transform;
    float opacity;
    float3 padding;
};

struct VSInput
{
    float2 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    output.position =
        mul(float4(input.position, 0.0f, 1.0f), transform);

    output.uv = input.uv;

    return output;
}
