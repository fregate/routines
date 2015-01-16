
#ifndef ___AA_FILLED_CIRCLE___H____
#define ___AA_FILLED_CIRCLE___H____

#pragma  once

/*

Algorithm of drawing anti-aliased circles and ellipses
Dan Field 1984
https://cs.uwaterloo.ca/research/tr/1984/CS-84-38.pdf

*/

#include <memory.h>

#define WIDTH 10
#define HEIGHT 10

struct CCC
{
	int i, q; // current pixel address
	int R; // circle radius
	int x_center, y_center; // circle center coordinates
	int I; // intensity of circle
	int Z; // sub-pixel resolution
	int y, y_hat; // exact and predicted values if f(I)
	int delta_y, delta2_y; // 1st and 2nd order differences of y
	int f2; // value of f(I)*f(I)
	int delta_f2, delta2_f2; // 1st and 2nd order differences of f(I)*f(I)
	int v, v_old; // v/Z - percentage of pixel intersected by circle

	int buffer[WIDTH][HEIGHT];
	CCC()
	{
		memset(&buffer, 0, sizeof(int) * WIDTH * HEIGHT);
	}

	void circle(int x_center_, int y_center_, int R_)
	{
		x_center = x_center_;
		y_center = y_center_;
		R = R_;
		I = 1;

		int c;

		init();

		q = R;
		i = 0;

		while(i < q)
		{
			predict();
			minimize();
			correct();
			v_old = v;
			v += delta_y;
			if(v >= 0) // single pixel on perimeter
			{
				eight_pixel(i, q, (v + v_old) >> 1);
				fill(i, q - 1, -q, I);
				fill(-i - 1, q - 1, -q, I);
			}
			else // two pixels on perimeter
			{
				v += Z;
				eight_pixel(i, q, v_old >> 1);
				q--;
				fill(i, q - 1, -q, I);
				fill(-i - 1, q - 1, -q, I);
				if(i < q) // Haven't gone below the diagonal
				{
					eight_pixel(i, q, (v + Z) >> 1);
					fill(q, i - 1, -i, I);
					fill(-q - 1, i- 1, -i, I);
				}
				else // went below the diagonal, fix v for final pixels
					v += Z;
			}

			i++;
		}
	
		c = v >> 1;
		pixel(q, q, c);
		pixel(-q - 1, q, c);
		pixel(-q - 1, -q - 1, c);
		pixel(q, -q - 1, c);
	}

	void init()
	{
		v = 0;
		Z = I;
		delta2_y = 0;
		delta_y = 0;
		y = Z * R;

		delta_f2 = Z * Z;
		delta2_f2 = -delta_f2 - delta_f2;
		f2 = y * y;
	}

	void predict()
	{
		delta_y += delta2_y;
		y_hat = y + delta_y; // y_hat is predicted value of f(i)
	}

	void minimize()
	{
		int e, e_old, d;
		// initialize the minimization
		delta_f2 += delta2_f2;
		f2 += delta_f2;
		e = y_hat * y_hat - f2;
		d = 1;
		y = y_hat;

		// force a negative
		if(e > 0)
		{
			e = -e;
			d = -1;
		}

		while(e < 0)
		{
			y += d;
			e_old = e;
			e = e + y + y - d;
		}

		// e or e_old is minimum squared error
		if(e + e_old > 0)
			y -= d;
	}

	void correct()
	{
		int error;
		error = y - y_hat;
		delta2_y += error;
		delta_y += error;
	}

	void eight_pixel(int x, int y, int c)
	{
		pixel(x, y, c);
		pixel(x, -y - 1, c);
		pixel(-x - 1, -y - 1, c);
		pixel(-x - 1, y, c);
		pixel(y, x, c);
		pixel(y, -x - 1, c);
		pixel(-y - 1, -x - 1, c);
		pixel(-y - 1, x, c);
	}

	void fill(int x, int ymax, int ymin, int c)
	{
		while(ymin <= ymax)
		{
			pixel(x, ymin, c);
			ymin++;
		}
	}

	void pixel(int x, int y, int c)
	{
		buffer[x + x_center][y + x_center] += c;
	}
};

void put_aa_pixel(byte* bbb, double x, double y)
{
	int x0 = int(x);
	int x1 = x0 + 1;
	int y0 = int(y);
	int y1 = y0 + 1;

	double weight_x1 = x - x0;
	double weight_x0 = 1 - weight_x1;
	double weight_y1 = y - y0;
	double weight_y0 = 1 - weight_y1;

	put_pixel(bbb, x0, y0, int((weight_x0 * weight_y0) * 255)); // change only opacity channel?
	put_pixel(bbb, x1, y0, int((weight_x1 * weight_y0) * 255));
	put_pixel(bbb, x0, y1, int((weight_x0 * weight_y1) * 255));
	put_pixel(bbb, x1, y1, int((weight_x1 * weight_y1) * 255));
}

template<class T>
void filled_rect(T* bbb, int x1, int y1, int x2, int y2, T val) // error = memset sets only (val & 0xff)
{
	// bbb - surface allocated buffer
	// val - value to set
	int const SW = 10; // dimensions of surface
	int const SH = 10;
	
	if(x1 > x2)
	{
		int t(x1);
		x1 = x2;
		x2 = t;
	}

	if(y1 > y2)
	{
		int t(y1);
		y1 = y2;
		y2 = t;
	}

	if(x1 < 0)
		x1 = 0;
	if(y1 < 0)
		y1 = 0;
	if(x2 >= SW)
		x2 = 9;
	if(y2 >= SH)
		y2 = 9;

	int h = y2 - y1;
	if((x2 - x1 + 1) == SW)
	{
	/*
	// possible optimization:
	if (x2-x1 + 1) == width do memset whole block
	*/
		memset(bbb + y1 * SW, val, SW * h * sizeof(T));
		h = -1;
	}
	while(h >= 0) 
	{
		unsigned short* l0 = bbb + x1 + (y1 + h) * SW;
		memset(l0, val, (x2 - x1 + 1) * sizeof(T));
		h--;
	}
}

void fill_ellipse_2(byte* bbb, int xc, int yc, int a, int b)
{			/* e(x,y) = b^2*x^2 + a^2*y^2 - a^2*b^2 */
	int x = 0, y = b;
	int rx = x, ry = y;
	unsigned int width = 1;
	unsigned int height = 1;
	long a2 = (long)a*a, b2 = (long)b*b;
	long crit1 = -(a2/4 + a%2 + b2);
	long crit2 = -(b2/4 + b%2 + a2);
	long crit3 = -(b2/4 + b%2);
	long t = -a2*y; /* e(x+1/2,y-1/2) - (a^2+b^2)/4 */
	long dxt = 2*b2*x, dyt = -2*a2*y;
	long d2xt = 2*b2, d2yt = 2*a2;

	if (b == 0) {
		draw_filled_rect(bbb, xc-a, yc, xc-a + 2*a+1, yc + 1);
		return;
	}

	while (y>=0 && x<=a) {
		if (t + b2*x <= crit1 ||     /* e(x+1,y-1/2) <= 0 */
			t + a2*y <= crit3) {     /* e(x+1/2,y) <= 0 */
				if (height == 1)
					; /* draw nothing */
				else if (ry*2+1 > (height-1)*2) {
					draw_filled_rect(bbb, xc-rx, yc-ry, xc-rx + width, yc-ry + height-1);
					draw_filled_rect(bbb, xc-rx, yc+ry+1, xc-rx + width, yc+ry+1 + 1-height);
					ry -= height-1;
					height = 1;
				}
				else {
					draw_filled_rect(bbb, xc-rx, yc-ry, xc-rx + width, yc-ry + ry*2+1);
					ry -= ry;
					height = 1;
				}
				x++, dxt += d2xt, t += dxt;
				rx++;
				width += 2;
		}
		else if (t - a2*y > crit2) { /* e(x+1/2,y-1) > 0 */
			 y--, dyt += d2yt, t += dyt;
			height++;
		}
		else {
			if (ry*2+1 > height*2) {
				draw_filled_rect(bbb, xc-rx, yc-ry, xc-rx + width, yc-ry + height);
				draw_filled_rect(bbb, xc-rx, yc+ry+1, xc-rx + width, yc+ry+1-height);
			}
			else {
				draw_filled_rect(bbb, xc-rx, yc-ry, xc-rx + width, yc-ry + ry*2+1);
			}
			x++, dxt += d2xt, t += dxt;
			y--, dyt += d2yt, t += dyt;
			rx++;
			width += 2;
			ry -= height;
			height = 1;
		}
	}

	if (ry > height) {
		draw_filled_rect(bbb, xc-rx, yc-ry, xc-rx + width, yc-ry + height);
		draw_filled_rect(bbb, xc-rx, yc+ry+1, xc-rx + width, yc+ry+1-height);
	}
	else {
		draw_filled_rect(bbb,xc-rx, yc-ry, xc-rx + width, yc-ry + ry*2+1);
	}
}

// check http://www.codeproject.com/Articles/16600/CTGraphics-Anti-Alias-C-Drawing
// some filled curves.

// change all to lines - no pixels?

#endif