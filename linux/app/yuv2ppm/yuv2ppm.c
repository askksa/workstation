#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "yuv2ppm.h"

/*
    example.ppm

    P3
    # feep.ppm
    4 4
    15
    0  0  0    0  0  0    0  0  0   15  0 15
    0  0  0    0 15  7    0  0  0    0  0  0
    0  0  0    0  0  0    0 15  7    0  0  0
    15  0 15    0  0  0    0  0  0    0  0  0
    */
    /*
    Use "P6" for binary data, or "P3" for ascii data.
*/

static int make_outfile(char *outfile, char *infile)
{
    int i;

    for(i=0; infile[i]!='.'; i++)
        outfile[i] = infile[i];

    outfile[i++] = '.';
    outfile[i++] = 'p';
    outfile[i++] = 'p';
    outfile[i++] = 'm';
    outfile[i++] = 0;

    return 0;
}


static int yuv2rgb(int y, int u, int v)
{
    unsigned int pixel32;
    unsigned char *pixel = (unsigned char *)&pixel32;//pixel32=pixel[3]pixel[2]pixel[1]pixel[0]
    int r, g, b;

    r = y + (1.370705 * (v-128));
    g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
    b = y + (1.732446 * (u-128));

    // Even with proper conversion, some values still need clipping.
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    // Values only go from 0-220..  Why?
    pixel[0] = r;
    pixel[1] = g;
    pixel[2] = b;
    pixel[3] = 0;

    /*
    printf("yuv2rgb(%i, %i, %i) -> %i, %i, %i  (0x%x)\n",
            y, u, v,
            pixel[0], pixel[1], pixel[2],
            pixel32);
    */

    return pixel32;
}

int yuv2ppm_i420(char *infile, char *outfile, int width, int height)
{
    FILE *in, *out;
    int i,j;
    unsigned char pixel_24[3];
    unsigned int pixel32;
    int y, u, v;

    char *ppmheader = "P6\n%d %d\n255\n";

    in = fopen(infile, "rb");
    out = fopen(outfile, "wb");

    if (!in  ||  !out)  return 0;

    fprintf(out, ppmheader, width, height);

    unsigned char * bufY = (unsigned char *)malloc(width*height);
    memset(bufY,128,width*height);
    unsigned char * bufU = (unsigned char *)malloc(width*height/4);
    memset(bufU,128,width*height/4);
    unsigned char * bufV = (unsigned char *)malloc(width*height/4);
    memset(bufV,128,width*height/4);

    fread(bufY, 1, width*height, in);
    fread(bufU, 1, width*height/4, in);
    fread(bufV, 1, width*height/4, in);

    for(i=0; i<height; i++)
        for(j=0; j<width; j++)
        {
            y=bufY[i*width+j];
            u=bufU[((int)i/2)*(width/2)+(int)j/2];
            v=bufV[((int)i/2)*(width/2)+(int)j/2];

            pixel_24[0] = pixel_24[1] = pixel_24[2] = 0;
            pixel32 = yuv2rgb(y, u, v);
            pixel_24[0] = (pixel32 & 0x000000ff);
            pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
            pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;

            // For binary PPM
            fwrite(pixel_24, 3, 1, out);
            // For ascii PPM
            //fprintf(out, "%i %i %i\n",
            //     pixel_24[0],
            //     pixel_24[1],
            //     pixel_24[2]);
        }

    free(bufY);
    free(bufU);
    free(bufV);
    fclose(in);
    fclose(out);

    return 1;
}


int yuv2ppm_nv12(char *infile, char *outfile ,int width, int height)
{
    FILE *in, *out;
    int i,j;
    unsigned char pixel_24[3];
    unsigned int pixel32;
    int y, u, v;

    char *ppmheader = "P6\n%d %d\n255\n";

    in = fopen(infile, "rb");
    out = fopen(outfile, "wb");

    if (!in  ||  !out)  return 0;

    fprintf(out, ppmheader, width, height);

    unsigned char * bufY = (unsigned char *)malloc(width*height);
    memset(bufY,128,width*height);
    unsigned char * bufUV = (unsigned char *)malloc(width*height/2);
    memset(bufUV,128,width*height/2);

    fread(bufY, 1, width*height, in);
    fread(bufUV, 1, width*height/2, in);

    for(i=0; i<height; i++)
        for(j=0; j<width; j++)
        {
            y=bufY[i*width+j];
            u=bufUV[((int)i/2)*width+(int)j/2*2];
            v=bufUV[((int)i/2)*width+(int)j/2*2+1];

            pixel_24[0] = pixel_24[1] = pixel_24[2] = 0;
            pixel32 = yuv2rgb(y, u, v);
            pixel_24[0] = (pixel32 & 0x000000ff);
            pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
            pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;

            fwrite(pixel_24, 3, 1, out);
        }

    free(bufY);
    free(bufUV);
    fclose(in);
    fclose(out);

    return 1;
}

int yuv2ppm_yuyv(char *infile, char *outfile, int width, int height)
{
    FILE *in, *out;
    int i,j;
    unsigned char pixel_24[3];
    unsigned int pixel32;
    int y, u, v;

    char *ppmheader = "P6\n%d %d\n255\n";

    in = fopen(infile, "rb");
    out = fopen(outfile, "wb");

    if (!in  ||  !out)  return 0;

    fprintf(out, ppmheader, width, height);

    unsigned char * bufYUV = (unsigned char *)malloc(width*height*2);
    memset(bufYUV,128,width*height*2);

    fread(bufYUV, 1, width*height*2, in);

    for(i=0; i<height; i++)
        for(j=0; j<width; j++)
        {
            y=bufYUV[i*width*2+j*2];
            u=bufYUV[i*width*2+(int)j/2*4+1];
            v=bufYUV[i*width*2+(int)j/2*4+3];

            pixel_24[0] = pixel_24[1] = pixel_24[2] = 0;
            pixel32 = yuv2rgb(y, u, v);
            pixel_24[0] = (pixel32 & 0x000000ff);
            pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
            pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;

            fwrite(pixel_24, 3, 1, out);
        }

    free(bufYUV);
    fclose(in);
    fclose(out);

    return 1;
}

int main(int argc, char **argv)
{
    char *infile, outfile[256];
    int width, height;
    int success = 0;

    infile = argv[1];
    width = atoi(argv[2]);
    height = atoi(argv[3]);

    make_outfile(outfile, infile);
    printf("%s -> %s...  ", infile, outfile);

    success = yuv2ppm_yuyv(infile, outfile, width, height);
    //success = yuv2ppm_nv12(infile, outfile, width, height);
    //success = yuv2ppm_i420(infile, outfile, width, height);

    if(success)
    {
        printf("Done.\n");
    }
    else
    {
        printf("Failed.  Aborting.\n");
        return 1;
    }

    return 0;
}
