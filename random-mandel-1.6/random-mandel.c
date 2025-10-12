#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <jpeglib.h>

#define MAX_ITER 256
#define ZOOM_RATIO 1.7
#define COLOR_STEP 32
#define MAX_INTERIOR_COLOR 50

#define TWO_PI 6.2831853

#define FP_SIZE 28
#define FPM(x,y) ((int)(((long long)(x)*(long long)(y)) >> FP_SIZE))

#define point(x,y) pnt[(int)(x)+(int)(y)*screen_width]
uint32_t * pnt;

double X1 = -4;
double X2 = 4;
double Y1 = -2;
double Y2 = 2;

uint32_t mandel_max_iters = 256;
uint32_t color_table[MAX_ITER];

int screen_width; 
int screen_height;

void writeJPEG(const char *filename, uint32_t *image_flat, int width, int height) {
  uint32_t (*image)[width] = (uint32_t (*)[width]) image_flat;

  // Open the JPEG file for writing
  FILE *outfile = fopen(filename, "wb");
  if (!outfile) {
    fprintf(stderr, "Error opening %s for writing\n", filename);
    exit(1);
  }

  // Initialize JPEG structures
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, outfile);

  // Set image parameters
  cinfo.image_width = width;
  cinfo.image_height = height;
  cinfo.input_components = 3; // RGB image
  cinfo.in_color_space = JCS_RGB;
  
  // Set compression options
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, 90, TRUE);
  
  // Start compression
  jpeg_start_compress(&cinfo, TRUE);
  
  // Write image data
  JSAMPLE row[3 * width]; // Assuming RGB image with given width  
  int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      // Extract RGB values from uint32_t
      uint32_t pixelValue = image[i][j];
      row[3 * j]     = (pixelValue >> 16) & 0xFF; // Red
      row[3 * j + 1] = (pixelValue >> 8)  & 0xFF; // Green
      row[3 * j + 2] = pixelValue & 0xFF;          // Blue
    }
    
    // Write the row
    JSAMPROW row_pointer[1]; // JSAMPROW is the correct type for a row
    row_pointer[0] = row;
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }
  
  // Finish compression
  jpeg_finish_compress(&cinfo);
  
  // Clean up
  fclose(outfile);
  jpeg_destroy_compress(&cinfo);
}

void pal_init(void)
{
  int x,y;

  double r[256],g[256],b[256];
  
  for(x=0;x<256;x+=COLOR_STEP)
    {
      r[x] = random() % 256;
      g[x] = random() % 256;
      b[x] = random() % 256;
    }

  for(y=0;y<256;y+=COLOR_STEP)
    {
      for(x=0;x<COLOR_STEP;x++)
	{
	  r[x+y] = (r[y] * (COLOR_STEP-x) + r[(y+COLOR_STEP) % 256] * x) / COLOR_STEP;
	  g[x+y] = (g[y] * (COLOR_STEP-x) + g[(y+COLOR_STEP) % 256] * x) / COLOR_STEP;
	  b[x+y] = (b[y] * (COLOR_STEP-x) + b[(y+COLOR_STEP) % 256] * x) / COLOR_STEP;      
	}
    }
  for (x=0;x<256;x++) color_table[x] = (((int) r[x]) << 16) + (((int) g[x]) << 8) + (((int) b[x]));
}

int mandel_iters(double x,double y, double * out_x, double * out_y)
{
  int c;
  int iters = 1;
  double renorm_iters;
  int next_record = 1;
  int fp_x,fp_y;
  int fp_x0,fp_y0;
  int fp_x2,fp_y2;
  int fp_next_x,fp_next_y;

  fp_x0 = fp_x = x*(1<<FP_SIZE);
  fp_y0 = fp_y = y*(1<<FP_SIZE);

  for(;iters<mandel_max_iters;iters++)
    {
      //Check for size
      if (fp_x < -(1<<FP_SIZE)*2  || fp_x > (1<<FP_SIZE)*2 ||
          fp_y < -(1<<FP_SIZE)*2  || fp_y > (1<<FP_SIZE)*2) break;
      //Next iteration
      fp_next_x = FPM(fp_x,fp_x) - FPM(fp_y,fp_y) + fp_x0;
      fp_next_y = (FPM(fp_x,fp_y)<<1) + fp_y0;

      fp_x = fp_next_x;
      fp_y = fp_next_y;
    }
  if (out_x) *out_x = (double)fp_x / (1<<FP_SIZE);
  if (out_y) *out_y = (double)fp_y / (1<<FP_SIZE);
  return iters;
}

double int_r;
double int_g;
double int_b;
uint32_t color(double x,double y)
{
  int c;
  int iters = 1;


  double out_x, out_y;
  iters = mandel_iters(x,y,&out_x,&out_y);
  if (iters == mandel_max_iters)
    {
      double r = sqrt(out_x*out_x + out_y*out_y);
      uint32_t int_scale = ((uint32_t)(r * 1000)) % (2*MAX_INTERIOR_COLOR);
      uint32_t int_scale2 = (int_scale>MAX_INTERIOR_COLOR) ? (2*MAX_INTERIOR_COLOR - int_scale) : int_scale;

      return (((int)(int_scale2 * int_b)) << 16) + (((int)(int_scale2 * int_g)) << 8) + (int)(int_scale2 * int_r);
    }
  
  c = color_table[iters % 256];
  return c;  
}

void draw_fractal(void)
{
  int x,y;
  int i,j;

  int_r = (128 + (random() % 128)) / 128.0;
  int_g = (128 + (random() % 128)) / 128.0;
  int_b = (128 + (random() % 128)) / 128.0;
  for (x=0;x<screen_width;x++)
    for (y=0;y<screen_height;y++)
      point(x,y) = color( ((double)x)*(X2-X1)/screen_width+X1,
                          ((double)y)*(Y2-Y1)/screen_height+Y1 );
}

void pick_random_xy2(int * xx, int * yy)
{
  int x,y,i;
  int a;
  int max, count, magic;

  max = 0;
  for(i=0;i<1000;i++)
    {
      x = random() % screen_width;
      y = random() % screen_height;

      a = mandel_iters( ((double)x)*(X2-X1)/screen_width+X1,
			((double)y)*(Y2-Y1)/screen_height+Y1, NULL, NULL) % MAX_ITER;
      if (a > max) max = a;
    }

  for(i=0;i<1000;i++)
    {
      x = random() % screen_width;
      y = random() % screen_height;

      a = mandel_iters( ((double)x)*(X2-X1)/screen_width+X1,
			((double)y)*(Y2-Y1)/screen_height+Y1, NULL, NULL) % MAX_ITER;
      if (a > max/2) break;
    }

  *xx = x;
  *yy = y;
  return;
}

int main_draw(uint32_t * screen, int width, int height)
{
  double alpha,v1,v2;

  int i,j,x,y;
  double xx,yy;
  int flag;

  screen_height = height;
  screen_width = width;
  
  srand(time(NULL));

  pnt = screen;
  pal_init();


  X1 = -4;
  X2 = 4;
  Y1 = -2;
  Y2 = 2;
  j = random() % 10 + 10;
  for(i=0;i<j;i++)
    {
      alpha = 1/ZOOM_RATIO;
      
      pick_random_xy2(&x,&y);
      
      v1 = (double)x*(X2-X1)+screen_width*X1;
      v2 = (X2-X1)*alpha;
      X2 = -v1 + ((double)x-screen_width)*v2;
      X1 = -v1 + x*v2;
      X1/=-screen_width;
      X2/=-screen_width;
            
      v1 = (double)y*(Y2-Y1)+screen_height*Y1;
      v2 = (Y2-Y1)*alpha;
      Y2 = -v1 + ((double)y-screen_height)*v2;
      Y1 = -v1 + y*v2;	  
      Y1/=-screen_height;
      Y2/=-screen_height;	      
    }		  

  draw_fractal();
}

int main(int argc, char **argv)
{
  if (argc < 4)
    {
      fprintf(stderr, "Usage: random_mandel outfile.jpg width height\n");
      exit(1);
    }
  int width = atoi(argv[2]);
  int height = atoi(argv[3]);

  uint32_t * screen;
  if ((screen = malloc(sizeof(uint32_t) * width * height)) == NULL)
    {
      fprintf(stderr, "Out of memory\n");
      exit(1);
    }
  
  main_draw(screen, width, height);
  writeJPEG(argv[1], screen, width, height);   
}
