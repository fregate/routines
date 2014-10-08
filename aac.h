
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


#endif
