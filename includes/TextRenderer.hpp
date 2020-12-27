#pragma once

#include <string>

#include <iostream>
#include <vector>
#include <map>

#include <GL/gl3w.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "utils.h"


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};




class TextRenderer
{
private:
	GLuint		vao;
	GLuint		vbo;
	GLuint		programID;
	int			windowWidth;
	int			windowHeight;
	std::map<GLchar, Character> Characters;

public:
	TextRenderer(/* args */);
	TextRenderer(int winWidth, int winHeight);
	~TextRenderer();

	void		createShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
	void		loadFont(const std::string &fontPath);
	void 		renderText(std::string text, float x, float y, float scale, glm::vec3 color);

};

