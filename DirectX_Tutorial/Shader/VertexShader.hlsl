struct VSOut
{
    float3 color : COLOR;
    float4 pos : SV_POSITION;
};

cbuffer Cbuff
{
    // in gpu matrix is col_major 
    matrix transform;
};

VSOut main(float2 pos : POSITION, float3 color : COLOR)
{ 
    VSOut vso;

    // V1 = V1 * R ---- R: Transform Matrix
    vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform); // Check it out this order in the future!!!
    vso.color = color;
    
    return vso;
}