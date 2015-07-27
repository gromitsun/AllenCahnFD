#define __NINETEEN_STENCIL__
#pragma OPENCL EXTENSION cl_khr_3d_image_writes : enable
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_REPEAT | CLK_FILTER_NEAREST;


__kernel void calc_3d(__read_only image3d_t Phi,
					  __write_only image3d_t PhiNext,
					  const float M,
					  const float a_2,
					  const float a_4,
					  const float K,
					  const float dx,
					  const float dt)
{
	int4 coord = {get_global_id(0),get_global_id(1),get_global_id(2),0};
	int4 size = {get_global_size(0),get_global_size(1),get_global_size(2),0};
    float4 normalizedCoord = (float4)((float)coord.x/size.x, (float)coord.y/size.y, (float)coord.z/size.z, 0);
//    float4 normalizedCoord = (float4)((coord.x +0.5f)/size.x, (coord.y+0.5f)/size.y,(coord.z+0.5f)/size.z,0);
    float incrementx = 1.0f/size.x;
    float incrementy = 1.0f/size.y;
    float incrementz = 1.0f/size.z;
    float4 phi = (read_imagef(Phi, sampler, normalizedCoord).x);
    float4 xm= (read_imagef(Phi,sampler,(normalizedCoord+(float4){-incrementx,0,0,0})));
    float4 xp= (read_imagef(Phi,sampler,(normalizedCoord+(float4){incrementx,0,0,0})));
    float4 ym= (read_imagef(Phi,sampler,(normalizedCoord+(float4){0,-incrementy,0,0})));
    float4 yp= (read_imagef(Phi,sampler,(normalizedCoord+(float4){0,incrementy,0,0})));
    float4 zm= (read_imagef(Phi,sampler,(normalizedCoord+(float4){0,0,-incrementz,0})));
    float4 zp= (read_imagef(Phi,sampler,(normalizedCoord+(float4){0,0,incrementz,0})));
#ifndef __NINETEEN_STENCIL__
    float4 laplacian =(xm+xp+ym+yp+zm+zp-6.0f*phi)/(dx*dx);  // 7-point stencil
#else
    float4 xym= (read_imagef(Phi,sampler,(normalizedCoord+(float4){-incrementx,-incrementy,0,0})));
    float4 xyp= (read_imagef(Phi,sampler,(normalizedCoord+(float4){incrementx,incrementy,0,0})));
    float4 xmyp= (read_imagef(Phi,sampler,(normalizedCoord+(float4){-incrementx,incrementy,0,0})));
    float4 xpym= (read_imagef(Phi,sampler,(normalizedCoord+(float4){incrementx,-incrementy,0,0})));
    
    float4 xzm= (read_imagef(Phi,sampler,(normalizedCoord+(float4){-incrementx,0,-incrementz,0})));
    float4 xzp= (read_imagef(Phi,sampler,(normalizedCoord+(float4){incrementx,0,incrementz,0})));
    float4 xmzp= (read_imagef(Phi,sampler,(normalizedCoord+(float4){-incrementx,0,incrementz,0})));
    float4 xpzm= (read_imagef(Phi,sampler,(normalizedCoord+(float4){incrementx,0,-incrementz,0})));
    
    float4 yzm= (read_imagef(Phi,sampler,(normalizedCoord+(float4){0,-incrementy,-incrementz,0})));
    float4 yzp= (read_imagef(Phi,sampler,(normalizedCoord+(float4){0,incrementy,incrementz,0})));
    float4 ymzp= (read_imagef(Phi,sampler,(normalizedCoord+(float4){0,-incrementy,incrementz,0})));
    float4 ypzm= (read_imagef(Phi,sampler,(normalizedCoord+(float4){0,incrementy,-incrementz,0})));
    
    float4 laplacian = ((xm+xp+ym+yp+zm+zp)/3.0f
                        + (xyp+xym+xpym+xmyp+xzm+xzp+xmzp+xpzm+yzm+yzp+ymzp+ypzm)/6.0f
                        - 4.0f*phi)/(dx*dx); // 19-point stencil
#undef __NINETEEN_STENCIL__
#endif
    float4 phi_next = phi + dt * M * ( 2.0f * K * laplacian - a_2 * phi - a_4 * phi * phi * phi);
    write_imagef(PhiNext,coord,phi_next);
}

