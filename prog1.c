#include <stdio.h>
#include <stdlib.h>

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;

unsigned char get_blue(unsigned char *data2, int width, int x, int y, int pad);
unsigned char get_green(unsigned char *data2, int width, int x, int y, int pad);
unsigned char get_red(unsigned char *data2, int width, int x, int y, int pad);

struct tagBITMAPFILEHEADER
{
WORD bfType; //specifies the file type
DWORD bfSize; //specifies the size in bytes of the bitmap file
WORD bfReserved1; //reserved; must be 0
WORD bfReserved2; //reserved; must be 0
DWORD bfOffBits; //species the offset in bytes from the bitmapfileheader to the bitmap bits
};

struct tagBITMAPINFOHEADER
{
DWORD biSize; //specifies the number of bytes required by the struct
LONG biWidth; //specifies width in pixels
LONG biHeight; //species height in pixels
WORD biPlanes; //specifies the number of color planes, must be 1
WORD biBitCount; //specifies the number of bit per pixel
DWORD biCompression;//spcifies the type of compression
DWORD biSizeImage; //size of image in bytes
LONG biXPelsPerMeter; //number of pixels per meter in x axis
LONG biYPelsPerMeter; //number of pixels per meter in y axis
DWORD biClrUsed; //number of colors used by th ebitmap
DWORD biClrImportant; //number of colors that are important
};

int main(int argc, char* argv[]){

    struct tagBITMAPFILEHEADER fh;
    struct tagBITMAPINFOHEADER fih;

    FILE *file = fopen(argv[1], "rb");

    fread(&fh.bfType, 1, 2, file);
    fread(&fh.bfSize, 1, 4, file);
    fread(&fh.bfReserved1, 1, 2, file);
    fread(&fh.bfReserved2, 1, 2, file);
    fread(&fh.bfOffBits, 1, 4, file);

    fread(&fih, 1, sizeof(struct tagBITMAPINFOHEADER), file);
    unsigned char *data = (unsigned char*)malloc(fih.biSizeImage);
    fread(data, 1, fih.biSizeImage, file);

    //free(data);
    fclose(file);

    struct tagBITMAPFILEHEADER fh2;
    struct tagBITMAPINFOHEADER fih2;
    FILE *file2 = fopen(argv[2], "rb");

    fread(&fh2.bfType, 1, 2, file2);
    fread(&fh2.bfSize, 1, 4, file2);
    fread(&fh2.bfReserved1, 1, 2, file2);
    fread(&fh2.bfReserved2, 1, 2, file2);
    fread(&fh2.bfOffBits, 1, 4, file2);

    fread(&fih2, 1, sizeof(struct tagBITMAPINFOHEADER), file2);
    unsigned char *data2 = (unsigned char*)malloc(fih2.biSizeImage);
    fread(data2, 1, fih2.biSizeImage, file2);

    //free(data2);
    fclose(file2);


    FILE *out = fopen(argv[4], "wb+");
    fwrite(&fh.bfType, 1, 2, out);
    fwrite(&fh.bfSize, 1, 4, out);
    fwrite(&fh.bfReserved1, 1, 2, out);
    fwrite(&fh.bfReserved2, 1, 2, out);
    fwrite(&fh.bfOffBits, 1, 4, out);

    fwrite(&fih, 1, sizeof(struct tagBITMAPINFOHEADER), out);

    printf("one %d %d", fih.biWidth, fih.biHeight);
    printf("two %d %d", fih2.biWidth, fih2.biHeight);

    struct tagBITMAPINFOHEADER bigfih;

    // bigfih = fih2;

    // if (fih.biHeight*fih.biWidth >= fih2.biWidth*fih2.biHeight){
    //     bigfih = fih;
    // }

    float ratio = 0.5;
    
    int pad = 0;
    int pad2 = 0;
    int b = 0; //blue
    int g = 1;
    int r = 2;
    int b2 = 0;
    int g2 = 0;
    int r2 = 0;

    if ((fih.biWidth*3) % 4 != 0){
        pad = 4 - (fih.biWidth*3) %4 ;
    }
    
    if ((fih2.biWidth*3) % 4 != 0){
        pad2 = 4 - (fih2.biWidth*3) %4 ;
    }

    for (int y = 0; y < fih.biHeight; y++){
        for (int x = 0; x < fih.biWidth; x++){
            b2 = get_blue(data2, fih2.biWidth, x, y, pad2);
            data[x*3 + (y*fih.biWidth*3) + pad*y + b] = b2*(1-ratio) + data[x*3 + (y*fih.biWidth*3) + pad*y + b]*ratio;

            g2 = get_green(data2, fih2.biWidth, x, y, pad2);
            data[x*3 + (y*fih.biWidth*3) + pad*y + g] = g2*(1-ratio) + data[x*3 + (y*fih.biWidth*3) + pad*y + g]*ratio;

            r2 = get_red(data2, fih2.biWidth, x, y, pad2);
            data[x*3 + (y*fih.biWidth*3) + pad*y + r] = r2*(1-ratio) + data[x*3 + (y*fih.biWidth*3) + pad*y + r]*ratio;
        }
    }
    fwrite(data, 1, fih.biSizeImage, out);

    fclose(out);
    return 0;
}

//bgr
unsigned char get_blue(unsigned char *data2, int width, int x, int y, int pad){
    return data2[x*3 + (y*width*3) + pad*y];
}
unsigned char get_green(unsigned char *data2, int width, int x, int y, int pad){
    return data2[x*3 + (y*width*3) + pad*y + 1];
}
unsigned char get_red(unsigned char *data2, int width, int x, int y, int pad){
    return data2[x*3 + (y*width*3) + pad*y + 2];
}