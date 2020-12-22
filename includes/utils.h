#pragma once

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

# define SCREEN_WIDTH 500
# define SCREEN_HEIGHT 500


void    LoadTextureFromArray(char *pixels, GLuint* out_texture, int image_width, int image_height);

void    min_max_pixel(char *data, size_t size, int *min, int *max);

void    transform_image(char *image_data, const size_t size, float slope, float intercept, int max, int min);
