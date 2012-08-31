///////////////////////////////////////////////////////////
//
// GLCD Simulator (for GLCD Library)
//
// 20.08.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////

#include "lcd2bmp.h"

typedef struct __attribute((packed))__ {
  char header[2];
  unsigned int filesize;
  unsigned int zero;
  unsigned int pos;
  unsigned int header_len;
  unsigned int width;
  unsigned int height;
  unsigned short layers;
  unsigned short depth;
  unsigned int compression;
  unsigned int data_len;
  unsigned int xpxm;
  unsigned int ypxm;
  unsigned int used_colors;
  unsigned int important_colors;  
} BMPHeader;

typedef struct {
  unsigned int width;
  unsigned int height;
  unsigned int line_width;
  char* data;
} Bitmap;



void setPixel(Bitmap* bmp, int x, int y, unsigned int color) {
  if(x < 0 || y < 0 || x >= bmp->width || y >= bmp->height) return;
  int byte_pos = (bmp->height - y - 1) * bmp->line_width + x * 3;
  if(byte_pos < 0 || byte_pos >= bmp->height * bmp->line_width) return;
  
  bmp->data[byte_pos] = color & 0xff;
  bmp->data[byte_pos + 1] = (color >> 8) & 0xff;
  bmp->data[byte_pos + 2] = (color >> 16) & 0xff;
}

uint8_t getPixelFromData(uint8_t x, uint8_t y, uint8_t* data) {
 return (data[(x & 127) + (((y & 63) >> 3) << 7)]) & (1 << (y & 7));
}

int outputBMP(char* file, int width, int height, uint8_t* data) {
  // line_width must be rounded to next multiple of 4
  int line_width = ((int)((width * 3 + 3) / 4)) * 4;
  
  FILE* output = fopen(file, "wb");
  if(output == NULL) {
    printf("Error: could not write file \"%s\".\n", file);
    return 5;
  }

  BMPHeader* header = (BMPHeader*)calloc(sizeof(BMPHeader), 1);
  Bitmap* bmp = (Bitmap*)malloc(sizeof(Bitmap));
  bmp->data = (char*)calloc(line_width * height, sizeof(char));
  bmp->width = width;
  bmp->height = height;
  bmp->line_width = line_width;
  
  uint8_t x, y;
  for(x = 0; x < width; x++) {
    for(y = 0; y < height; y++) {
		setPixel(bmp, x, y, getPixelFromData(x, y, data) ? 0xffffff : 0x000080);
	}
  }
 
  header->header[0] = 'B';
  header->header[1] = 'M';
  header->pos = 54;
  header->header_len = 40;
  header->width = width;
  header->height = height;
  header->layers = 1;
  header->depth = 24;
  header->data_len = line_width * height;
  header->filesize = header->data_len + header->pos;
  
  fwrite(header, sizeof(BMPHeader), 1, output);
  fwrite(bmp->data, line_width * height, sizeof(char), output);
  
  free(header);
  free(bmp->data);
  free(bmp);
  
  fclose(output);
  
  return 0;
}



void LCD2BMP_Save(char* file, uint8_t data[128*8]) {
  outputBMP(file, 128, 64, &data[0]);
}
