__constant sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_REPEAT | CLK_FILTER_NEAREST;


__kernel void calc_2d(__read_only image2d_t Phi,
					  __write_only image2d_t PhiNext,
					  const float M,
					  const float a_2,
					  const float a_4,
					  const float K,
					  const float dx,
					  const float dt)
{
	int2 coord = {get_global_id(0),get_global_id(1)};
	int2 size = {get_global_size(0),get_global_size(1)};
    float2 normalizedCoord = (float2)((float)coord.x/size.x, (float)coord.y/size.y);
//    float2 normalizedCoord = (float2)((coord.x+0.5f)/size.x, (coord.y+0.5f)/size.y);
    float incrementx = 1.0f/size.x;
    float incrementy = 1.0f/size.y;
    float4 phi = (read_imagef(Phi, sampler, normalizedCoord).x);
    float4 xm= (read_imagef(Phi,sampler,(normalizedCoord+(float2){-incrementx,0})));
    float4 xp= (read_imagef(Phi,sampler,(normalizedCoord+(float2){incrementx,0})));
    float4 ym= (read_imagef(Phi,sampler,(normalizedCoord+(float2){0,-incrementy})));
    float4 yp= (read_imagef(Phi,sampler,(normalizedCoord+(float2){0,incrementy})));
//    float4 laplacian =(xm+xp+ym+yp-4.0f*phi)/(dx*dx);  // 4-stencil
    float4 xym= (read_imagef(Phi,sampler,(normalizedCoord+(float2){-incrementx,-incrementx})));
    float4 xyp= (read_imagef(Phi,sampler,(normalizedCoord+(float2){incrementx,incrementx})));
    float4 xpym= (read_imagef(Phi,sampler,(normalizedCoord+(float2){incrementx,-incrementy})));
    float4 xmyp= (read_imagef(Phi,sampler,(normalizedCoord+(float2){-incrementx,incrementy})));
    
    float4 laplacian =((xm+xp+ym+yp)/2.0f + (xyp+xym+xmyp+xpym)/4.0f - 3.0f*phi)/(dx*dx); // 8-stencil
    float4 phi_next = phi + dt * M * ( 2.0f * K * laplacian - a_2 * phi - a_4 * phi * phi * phi);
    write_imagef(PhiNext,coord,phi_next);
}