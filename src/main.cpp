#include "GUI.hpp"

// #include <cstdlib>
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <ft2build.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include FT_FREETYPE_H


// // settings
// const unsigned int SCR_WIDTH = 800;
// const unsigned int SCR_HEIGHT = 600;



// /// Holds all state information relevant to a character as loaded using FreeType
// struct Character {
//     unsigned int TextureID; // ID handle of the glyph texture
//     glm::ivec2   Size;      // Size of glyph
//     glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
//     unsigned int Advance;   // Horizontal offset to advance to next glyph
// };

// std::map<GLchar, Character> Characters;
// unsigned int VAO, VBO;


// static void glfw_error_callback(int error, const char* description)
// {
//     fprintf(stderr, "Glfw Error %d: %s\n", error, description);
// }

// void RenderText(GLuint &shader, std::string text, float x, float y, float scale, glm::vec3 color);

// // Called for keyboard events
// void keyboard(int key, int action);

// // Render scene
// void display(GLuint &vao, GLFWwindow *window);

// // Initialize the data to be rendered
// GLuint initialize(GLuint &vao, GLuint &vbo, GLuint &texture);

// // Load an image from the disk with FreeImage
// void load_image(const char *fname);

// void displayImage(GLuint &img_shader, GLuint &vao, GLuint &vbo, GLuint &texture,  GLFWwindow *window);

// int main () {

//     glfwSetErrorCallback(glfw_error_callback);
//     if (!glfwInit())
//     {
//         std::cerr << "ERROR: Failed initialization of GLFW" << std::endl;
//         exit(-1);
//     }

// 	// Use OpenGL 3.2 core profile
//     const char* glsl_version = "#version 150";
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
//     // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac


//     // Create window with graphics context
//     GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MedicalVisuals", NULL, NULL);
//     if (window == NULL)
//     {
//         std::cerr << "ERROR: Failed creation of window" << std::endl;
//         glfwTerminate();
//         exit(-1);
//     }
//     glfwMakeContextCurrent(window);
//     glfwSwapInterval(1); // Enable vsync


//     bool err = gl3wInit() != 0;
//     if (err)
//     {
//         std::cerr << "Failed to initialize OpenGL loader" << std::endl;
//         exit(-1);
//     }


//     glEnable(GL_CULL_FACE);
//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// 	// Create a vertex array object



    
//     GLuint shaderProgram = create_program("shaders/text.vertex.shader", "shaders/text.fragment.shader");
//     glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
//     glUseProgram(shaderProgram);
//     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//     // FreeType
//     // --------
//     FT_Library ft;
//     // All functions return a value different than 0 whenever an error occurred
//     if (FT_Init_FreeType(&ft))
//     {
//         std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//         return -1;
//     }

// 	// find path to font
//     std::string font_name = "fonts/open-sans/OpenSans-Bold.ttf";
//     if (font_name.empty())
//     {
//         std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
//         return -1;
//     }
	
// 	// load font as face
//     FT_Face face;
//     if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
//         std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
//         return -1;
//     }
//     else {
//         // set size to load glyphs as
//         FT_Set_Pixel_Sizes(face, 0, 48);

//         // disable byte-alignment restriction
//         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//         // load first 128 characters of ASCII set
//         for (unsigned char c = 0; c < 128; c++)
//         {
//             // Load character glyph 
//             if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//             {
//                 std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//                 continue;
//             }
//             // generate texture
//             unsigned int texture;
//             glGenTextures(1, &texture);
//             glBindTexture(GL_TEXTURE_2D, texture);
//             glTexImage2D(
//                 GL_TEXTURE_2D,
//                 0,
//                 GL_RED,
//                 face->glyph->bitmap.width,
//                 face->glyph->bitmap.rows,
//                 0,
//                 GL_RED,
//                 GL_UNSIGNED_BYTE,
//                 face->glyph->bitmap.buffer
//             );
//             // set texture options
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//             // now store character for later use
//             Character character = {
//                 texture,
//                 glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//                 glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//                 static_cast<unsigned int>(face->glyph->advance.x)
//             };
//             Characters.insert(std::pair<char, Character>(c, character));
//         }
//         glBindTexture(GL_TEXTURE_2D, 0);
//     }
//     // destroy FreeType once we're finished
//     FT_Done_Face(face);
//     FT_Done_FreeType(ft);

    
//     // configure VAO/VBO for texture quads
//     // -----------------------------------
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);

	
// 	GLuint vao, vbo, img_texture;

// 	// Initialize the data to be rendered
// 	GLuint img_shader = initialize(vao, vbo, img_texture);

// 	// Create a rendering loop
// 	bool running = true;

// 	while(running) {

// 		// Display scene
//         glClear(GL_COLOR_BUFFER_BIT);
//         displayImage(img_shader, vao, vbo, img_texture, window);
//         RenderText(shaderProgram, "A", 200.0f, 550.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//         RenderText(shaderProgram, "L", 0.0f, 420.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        

// 		glfwSwapBuffers(window);
//         glfwPollEvents();
// 		if (glfwWindowShouldClose(window))
// 			running = false;
// 		// Check if the window was closed

// 	}

// 	// Terminate GLFW
// 	glfwTerminate();

// 	return 0;
// }


// // Render scene
// void displayImage(GLuint &img_shader, GLuint &vao, GLuint &vbo, GLuint &texture, GLFWwindow *window) {

//     glUseProgram(img_shader);
//     // glActiveTexture(GL_TEXTURE0);
//     glBindVertexArray(vao);
//     glBindTexture(GL_TEXTURE_2D, texture);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo);

//     glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

// }


// void RenderText(GLuint &shaderProgram, std::string text, float x, float y, float scale, glm::vec3 color)
// {
//     // activate corresponding render state	
    
//     glUseProgram(shaderProgram);
//     glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), color.x, color.y, color.z);
//     glActiveTexture(GL_TEXTURE0);
//     glBindVertexArray(VAO);

//     // iterate through all characters
//     std::string::const_iterator c;
//     for (c = text.begin(); c != text.end(); c++) 
//     {
//         Character ch = Characters[*c];

//         float xpos = x + ch.Bearing.x * scale;
//         float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

//         float w = ch.Size.x * scale;
//         float h = ch.Size.y * scale;
//         // update VBO for each character
//         float vertices[6][4] = {
//             { xpos,     ypos + h,   0.0f, 0.0f },            
//             { xpos,     ypos,       0.0f, 1.0f },
//             { xpos + w, ypos,       1.0f, 1.0f },

//             { xpos,     ypos + h,   0.0f, 0.0f },
//             { xpos + w, ypos,       1.0f, 1.0f },
//             { xpos + w, ypos + h,   1.0f, 0.0f }           
//         };
//         // render glyph texture over quad
//         glBindTexture(GL_TEXTURE_2D, ch.TextureID);
//         // update content of VBO memory
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
//         glBindBuffer(GL_ARRAY_BUFFER, 0);
//         // render quad
//         glDrawArrays(GL_TRIANGLES, 0, 6);
//         // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
//         x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
//     }
//     glBindVertexArray(0);
//     glBindTexture(GL_TEXTURE_2D, 0);
// }





// GLuint initialize(GLuint &vao, GLuint &vbo, GLuint &texture) {
// 	// Use a Vertex Array Object
// 	glGenVertexArrays(1, &vao);
// 	glBindVertexArray(vao);

// 	// 1 square (made by 2 triangles) to be rendered
// 	GLfloat vertices_position[8] = {
// 		-1.0, 0.0,
// 		0.0, 0.0,
// 		0.0, 1.0,
// 		-1.0, 1.0,
				
// 	};

//  	GLfloat texture_coord[8] = {
 		
//  		0.0, 1.0,
//  		1.0, 1.0,
//  		1.0, 0.0,
// 		 0.0, 0.0,
// 	};


// 	GLuint indices[6] = {
// 		0, 1, 2,
// 		2, 3, 0
// 	};

// 	// Create a Vector Buffer Object that will store the vertices on video memory
// 	glGenBuffers(1, &vbo);

// 	// Allocate space for vertex positions and texture coordinates
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position) + sizeof(texture_coord), NULL, GL_STATIC_DRAW);

// 	// Transfer the vertex positions:
// 	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_position), vertices_position);

// 	// Transfer the texture coordinates:
// 	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices_position), sizeof(texture_coord), texture_coord);

// 	// Create an Element Array Buffer that will store the indices array:
// 	GLuint eab;
// 	glGenBuffers(1, &eab);

// 	// Transfer the data from indices to eab
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

// 	// Create a texture
// 	glGenTextures(1, &texture);

// 	// Specify that we work with a 2D texture
// 	glBindTexture(GL_TEXTURE_2D, texture);

// 	load_image("squirrel.jpg");

// 	GLuint shaderProgram = create_program("shaders/vert.shader", "shaders/frag.shader");

// 	// Get the location of the attributes that enters in the vertex shader
// 	GLint position_attribute = glGetAttribLocation(shaderProgram, "position");

// 	// Specify how the data for position can be accessed
// 	glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

// 	// Enable the attribute
// 	glEnableVertexAttribArray(position_attribute);

// 	// Texture coord attribute
// 	GLint texture_coord_attribute = glGetAttribLocation(shaderProgram, "texture_coord");
// 	glVertexAttribPointer(texture_coord_attribute, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vertices_position));
// 	glEnableVertexAttribArray(texture_coord_attribute);

//     return shaderProgram;
// }

// void load_image(const char *fname)
// {
// 	char 				*image_data_raw = NULL;
// 	unsigned char		*image_data = NULL;
// 	imebra::DataSet 	loadedDataSet(imebra::CodecFactory::load("data/DICOM_Image_for_Lab_2.dcm"));
// 	imebra::Image		image(loadedDataSet.getImage(0));
// 	std::uint32_t 		width = image.getWidth();
// 	std::uint32_t 		height = image.getHeight();

// 	std::cout << "width: " << width << " height: " << height << std::endl;
// 	image_data_raw = new char[width * height];
// 	image.getReadingDataHandler().data(image_data_raw, width * height);

// 	image_data = new unsigned char[width * height * 3];
// 	for (size_t i = 0; i < width * height; i++)
// 	{
// 		image_data[i * 3] = image_data_raw[i];
// 		image_data[i * 3 + 1] = image_data_raw[i];
// 		image_data[i * 3 + 2] = image_data_raw[i];
// 	}
	

// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 	delete []image_data_raw;
// 	delete []image_data;
// }





int		main(int argc, char **argv)
{
	std::atomic<bool> isRunning;
	GUI gui;

	isRunning = true;
	if (gui.initGL() == false)
		isRunning = false;
	while (isRunning)
	{
		gui.events(isRunning);
		gui.update();
		gui.render();
	}

    return 0;
}