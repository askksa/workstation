#ifndef __YUV2PPM_H_
#define __YUV2PPM_H_

int yuv2ppm_i420(char *infile, char *outfile, int width, int height);
int yuv2ppm_nv12(char *infile, char *outfile, int width, int height);
int yuv2ppm_yuyv(char *infile, char *outfile, int width, int height);

#endif

