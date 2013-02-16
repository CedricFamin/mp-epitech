kernel void square(global float* input, global float* outpust)
{
    size_t = get_global_id(0);
    ouput[i] = input[i] * input[i];
}