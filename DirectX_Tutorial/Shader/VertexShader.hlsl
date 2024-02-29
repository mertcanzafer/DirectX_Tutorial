cbuffer Cbuff
{
    // in gpu matrix is col_major 
    matrix transform;
};

float4 main(float3 pos : POSITION): SV_Position
{ 
    // V1 = V1 * R ---- R: Transform Matrix
    return mul(float4(pos, 1.0f), transform); // Check it out this order in the future!!!
}