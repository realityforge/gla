#ifdef HEADER_ONLY
#define GLA_IMPLEMENTATION
#endif
#include "GLA/gla.h"
#include <stdio.h>

int main()
{
    // This next line will crash if you run it because we have not set up a window context...
    const int init_result = glaInit();
    if (GLA_OK == init_result) {
        const char* renderer = (const char*)glGetString(GL_RENDERER);
        printf("glaInit() success with renderer %s\n", renderer);
    } else {
        const char* error_message = glaError();
        printf("glaInit() failed with error %d: %s\n", init_result, NULL == error_message ? "unknown reason" : error_message);
        return -1;
    }
    if (GLA_VERSION_3_1) {
        printf("GLA_VERSION_3_1 profile present");
    }
    if (GLA_VERSION_3_2) {
        printf("GLA_VERSION_3_2 profile present");
    }
    if (GLA_ARB_framebuffer_object) {
        printf("GL_ARB_framebuffer_object present");
    }
    if (GLA_ARB_depth_clamp) {
        printf("GL_ARB_depth_clamp present");
    }
    if (GLA_ARB_seamless_cube_map) {
        printf("GL_ARB_seamless_cube_map present");
    }
    if (GLA_NVX_gpu_memory_info) {
        printf("GL_NVX_gpu_memory_info present");
    }
    if (GLA_ATI_meminfo) {
        printf("GL_ATI_meminfo present");
    }
    if (GLA_ARB_texture_compression_rgtc) {
        printf("GLA_ARB_texture_compression_rgtc present");
    }
    if (GLA_ARB_texture_compression_bptc) {
        printf("GLA_ARB_texture_compression_bptc present");
    }
    if (GLA_EXT_texture_compression_s3tc) {
        printf("GL_EXT_texture_compression_s3tc present");
    }
    if (GLA_EXT_texture_filter_anisotropic) {
        printf("GL_EXT_texture_filter_anisotropic present");
    }
    if (GLA_EXT_texture_sRGB) {
        printf("GL_EXT_texture_sRGB present");
    }
    if (GLA_S3_s3tc) {
        printf("GL_EXT_texture_sRGB present");
    }

    const int dispose_result = glaDispose();
    if (GLA_OK == dispose_result) {
        printf("glaDispose() success\n");
    } else {
        const char* error_message = glaError();
        printf("glaDispose() failed with error %d: %s\n", dispose_result, NULL == error_message ? "unknown reason" : error_message);
        return -2;
    }

    return 0;
}
