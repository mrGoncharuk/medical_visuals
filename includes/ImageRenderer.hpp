#pragma once

#include <string>

#include <iostream>
#include <vector>
#include <map>

#include <GL/gl3w.h> 

#include <imebra/imebra.h>

#include "utils.hpp"




class ImageRenderer
{
private:
	GLuint		vao;
	GLuint		vbo;
	GLuint		programID;
    GLuint      textureID;


private:
    void        ReadDicomImage(const std::string &fname);

public:
	ImageRenderer(/* args */);
	~ImageRenderer();

	void		createShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
	void		loadImage(const std::string &imagePath);
	void 		renderImage();

};

