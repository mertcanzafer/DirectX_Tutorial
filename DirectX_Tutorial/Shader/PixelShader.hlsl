static const unsigned int NUM_OF_FACES = 6;

cbuffer Cbuff
{
    float4 face_colors[NUM_OF_FACES];
};

float4 main(unsigned int tid:SV_PrimitiveID) : SV_TARGET
{
    return face_colors[tid / 2];
}