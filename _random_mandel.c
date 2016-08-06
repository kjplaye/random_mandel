// gcc _random_mandel.c -o _random_mandel.so -fPIC -shared -lm
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 800
#define MAX_ITER 256
#define ZOOM_RATIO 1.7
#define COLOR_STEP 8

#define TWO_PI 6.2831853

#define FP_SIZE 28
#define FPM(x,y) ((int)(((long long)(x)*(long long)(y)) >> FP_SIZE))

#define point(x,y) pnt[(int)(x)+(int)(y)*SCREEN_WIDTH]
unsigned * pnt;

double X1 = -4;
double X2 = 4;
double Y1 = -2;
double Y2 = 2;

unsigned mandel_max_iters = 256;
unsigned color_table[MAX_ITER];

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

int mandel_iters(double x,double y)
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
  return iters;
}

unsigned color(double x,double y)
{
  int c;
  int iters = 1;

  iters = mandel_iters(x,y);
  if (iters == mandel_max_iters) return 0x000000;

  c = color_table[iters % 256];
  return c;  
}

void draw_fractal(void)
{
  int x,y;
  int i,j;

  for (x=0;x<SCREEN_WIDTH;x++)
    for (y=0;y<SCREEN_HEIGHT;y++)
      point(x,y) = color( ((double)x)*(X2-X1)/SCREEN_WIDTH+X1,
                          ((double)y)*(Y2-Y1)/SCREEN_HEIGHT+Y1 );
}

void pick_random_xy2(int * xx, int * yy)
{
  int x,y,i;
  int a;
  int i_array[SCREEN_WIDTH][SCREEN_HEIGHT];

  int max, count, magic;

  max = 0;
  for(i=0;i<1000;i++)
    {
      x = random() % SCREEN_WIDTH;
      y = random() % SCREEN_HEIGHT;

      a = mandel_iters( ((double)x)*(X2-X1)/SCREEN_WIDTH+X1,
			((double)y)*(Y2-Y1)/SCREEN_HEIGHT+Y1) % MAX_ITER;
      if (a > max) max = a;
    }

  for(i=0;i<1000;i++)
    {
      x = random() % SCREEN_WIDTH;
      y = random() % SCREEN_HEIGHT;

      a = mandel_iters( ((double)x)*(X2-X1)/SCREEN_WIDTH+X1,
			((double)y)*(Y2-Y1)/SCREEN_HEIGHT+Y1) % MAX_ITER;
      if (a > max/2) break;
    }

  *xx = x;
  *yy = y;
  return;
}

int main_draw(unsigned * screen)
{
  double alpha,v1,v2;

  int i,j,x,y;
  double xx,yy;
  int flag;

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
      v1 = (double)x*(X2-X1)+SCREEN_WIDTH*X1;
      v2 = (X2-X1)*alpha;
      X2 = -v1 + ((double)x-SCREEN_WIDTH)*v2;
      X1 = -v1 + x*v2;
      X1/=-SCREEN_WIDTH;
      X2/=-SCREEN_WIDTH;
      
      
      v1 = (double)y*(Y2-Y1)+SCREEN_HEIGHT*Y1;
      v2 = (Y2-Y1)*alpha;
      Y2 = -v1 + ((double)y-SCREEN_HEIGHT)*v2;
      Y1 = -v1 + y*v2;	  
      Y1/=-SCREEN_HEIGHT;
      Y2/=-SCREEN_HEIGHT;	      
    }		  

  draw_fractal();
}
