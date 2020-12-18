#include <vector>
#include <cmath>
#include <iostream>

void        ft_buf_putpixel(std::vector<float> &buf, int x, int y)
{
    // std::cout << "(" << x << "; " << y << ")" << std::endl;
    buf.push_back(x);
    buf.push_back(y);
}

void ft_draw_line(std::vector<float> &buf, int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sgnX = x0 < x1 ? 1 : -1;
    int sgnY = y0 < y1 ? 1 : -1;
    int e = 0;
    for (int i=0; i < dx+dy; i++) {
        ft_buf_putpixel(buf, x0, y0);
        int e1 = e + dy;
        int e2 = e - dx;
        if (abs(e1) < abs(e2)) {
            x0 += sgnX;
            e = e1;
        } else {
            y0 += sgnY;
            e = e2;
        }
    }
}

