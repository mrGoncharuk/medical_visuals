#include <vector>
#include <cmath>
#include <iostream>

void        ft_buf_putpixel(std::vector<float> &buf, int x, int y)
{
    // std::cout << "(" << x << "; " << y << ")" << std::endl;
    buf.push_back(x);
    buf.push_back(y);
}

void		ft_draw_low_line(std::vector<float> &buf, int x0, int y0, int x1, int y1)
{
	int     dx = x1 - x0;
    int     dy = y1 - y0;
    int     y;
    int     x;
	int	    d;
	int		yi;


    // Selecting the way move on OY axis
	yi = 1;
	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}
	d = 2 * dy - dx;
	y = y0;
	x = x0;
	while (x <= x1)
	{
        ft_buf_putpixel(buf, x, y);
		if (d > 0)
		{
			y += yi;
			d -= 2 * dx;
		}
		d += 2 * dy;
		x++;
	}
}



void		ft_draw_high_line(std::vector<float> &buf, int x0, int y0, int x1, int y1)
{
	int     dx = x1 - x0;
    int     dy = y1 - y0;
    int     y;
    int     x;
	int	    d;
	int		xi;

    // Selecting the way move on OX axis
	xi = 1;
	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	d = 2 * dx - dy;
	y = y0;
	x = x0;
	while (y <= y1)
	{
        ft_buf_putpixel(buf, x, y);
		if (d > 0)
		{
			x += xi;
			d -= 2 * dy;
		}
		d += 2 * dx;
		y++;
	}
}


void		ft_draw_line(std::vector<float> &buf, int x0, int y0, int x1, int y1)
{
    // Selecting way of moving
	if (abs(y1 - y0) < abs(x1 - x0))
	{
        // Swaping pointd coordinats to iterate from the beggining of coordinates
		if (x0 > x1)
			ft_draw_low_line(buf, x1, y1, x0, y0);
		else
			ft_draw_low_line(buf, x0, y0, x1, y1);
	}
	else
	{
        // Swaping pointd coordinats to iterate from the beggining of coordinates
		if (y0 > y1)
			ft_draw_high_line(buf, x1, y1, x0, y0);
		else
			ft_draw_high_line(buf, x0, y0, x1, y1);
	}
}

// void ft_draw_line(std::vector<float> &buf, int x0, int y0, int x1, int y1)
// {
//     int dx = abs(x1 - x0);
//     int dy = abs(y1 - y0);
//     int sgnX = x0 < x1 ? 1 : -1;
//     int sgnY = y0 < y1 ? 1 : -1;
//     int e = 0;
//     for (int i=0; i < dx+dy; i++) {
//         ft_buf_putpixel(buf, x0, y0);
//         int e1 = e + dy;
//         int e2 = e - dx;
//         if (abs(e1) < abs(e2)) {
//             x0 += sgnX;
//             e = e1;
//         } else {
//             y0 += sgnY;
//             e = e2;
//         }
//     }
// }

