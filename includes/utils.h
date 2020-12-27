#pragma once

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

# define SCREEN_WIDTH 512
# define SCREEN_HEIGHT 512

void    read_shader_src(const char *fname, std::vector<char> &buffer);
void		ft_draw_line(std::vector<float> &buf, int x0, int y0, int x1, int y1);
void initialize(GLuint &vao, const std::vector<float> &buf);
void ft_draw_cylindic_line(std::vector<float> &buf);
void display_smth(GLuint vao, int size);
void init_texture();
GLuint create_program(const char *path_vert_shader, const char *path_frag_shader);
GLuint load_and_compile_shader(const char *fname, GLenum shaderType);