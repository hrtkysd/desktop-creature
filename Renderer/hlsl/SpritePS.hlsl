cbuffer SpriteConstants : register(b0)
{
    row_major float4x4 transform;
    float opacity;
    float3 padding;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Texture2D spriteTexture : register(t0);
SamplerState spriteSampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    float4 color =
        spriteTexture.Sample(spriteSampler, input.uv);

    color.a *= opacity;

    return color;
}
