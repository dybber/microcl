__kernel void vecAdd14(__global float* a)
{
    int gid = get_global_id(0);
    a[gid] += a[gid] + 14;
}

__kernel void vecAdd10(__global float* a)
{
    int gid = get_global_id(0);
    a[gid] += a[gid] + 10;
}
