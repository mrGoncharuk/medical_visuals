#pragma once

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

# define SCREEN_WIDTH 500
# define SCREEN_HEIGHT 500

void    read_shader_src(const char *fname, std::vector<char> &buffer);
void		ft_draw_line(std::vector<float> &buf, int x0, int y0, int x1, int y1);
void initialize(GLuint &vao, const std::vector<float> &buf);
void ft_draw_cylindic_line(std::vector<float> &buf);
void display(GLuint vao, int size);