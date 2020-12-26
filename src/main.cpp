#include "GUI.hpp"

// #include <GL/glew.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <ft2build.h>
//#include <ctime>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// Called for keyboard events
void keyboard(int key, int action);

// Render scene
void display(GLuint &vao, GLFWwindow *window);

// Initialize the data to be rendered
void initialize(GLuint &vao);

// Load an image from the disk with FreeImage
void load_image(const char *fname);

int main () {

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        std::cerr << "ERROR: Failed initialization of GLFW" << std::endl;
        exit(-1);
    }

	// Use OpenGL 3.2 core profile
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac


    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MedicalVisuals", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "ERROR: Failed creation of window" << std::endl;
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        std::cerr << "Failed to initialize OpenGL loader" << std::endl;
        exit(-1);
    }



	// Create a vertex array object
	GLuint vao;

	// Initialize the data to be rendered
	initialize(vao);

	// Create a rendering loop
	bool running = true;

	while(running) {
		// Display scene
		display(vao, window);

		glfwPollEvents();
		if (glfwWindowShouldClose(window))
			running = false;
		// Check if the window was closed

	}

	// Terminate GLFW
	glfwTerminate();

	return 0;
}

// Render scene
void display(GLuint &vao, GLFWwindow *window) {
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vao);
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Swap front and back buffers
	glfwSwapBuffers(window);
}

void initialize(GLuint &vao) {
	// Use a Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 1 square (made by 2 triangles) to be rendered
	GLfloat vertices_position[8] = {
		-1.0, 0.0,
		0.0, 0.0,
		0.0, 1.0,
		-1.0, 1.0,
				
	};

 	GLfloat texture_coord[8] = {
 		
 		0.0, 1.0,
 		1.0, 1.0,
 		1.0, 0.0,
		 0.0, 0.0,
	};


	GLuint indices[6] = {
		0, 1, 2,
		2, 3, 0
	};

	// Create a Vector Buffer Object that will store the vertices on video memory
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Allocate space for vertex positions and texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position) + sizeof(texture_coord), NULL, GL_STATIC_DRAW);

	// Transfer the vertex positions:
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_position), vertices_position);

	// Transfer the texture coordinates:
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices_position), sizeof(texture_coord), texture_coord);

	// Create an Element Array Buffer that will store the indices array:
	GLuint eab;
	glGenBuffers(1, &eab);

	// Transfer the data from indices to eab
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Create a texture
	GLuint texture;
	glGenTextures(1, &texture);

	// Specify that we work with a 2D texture
	glBindTexture(GL_TEXTURE_2D, texture);

	load_image("squirrel.jpg");

	GLuint shaderProgram = create_program("shaders/vert.shader", "shaders/frag.shader");

	// Get the location of the attributes that enters in the vertex shader
	GLint position_attribute = glGetAttribLocation(shaderProgram, "position");

	// Specify how the data for position can be accessed
	glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable the attribute
	glEnableVertexAttribArray(position_attribute);

	// Texture coord attribute
	GLint texture_coord_attribute = glGetAttribLocation(shaderProgram, "texture_coord");
	glVertexAttribPointer(texture_coord_attribute, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vertices_position));
	glEnableVertexAttribArray(texture_coord_attribute);

}

void load_image(const char *fname)
{
	char 				*image_data_raw = NULL;
	unsigned char		*image_data = NULL;
	imebra::DataSet 	loadedDataSet(imebra::CodecFactory::load("data/DICOM_Image_for_Lab_2.dcm"));
	imebra::Image		image(loadedDataSet.getImage(0));
	std::uint32_t 		width = image.getWidth();
	std::uint32_t 		height = image.getHeight();

	std::cout << "width: " << width << " height: " << height << std::endl;
	image_data_raw = new char[width * height];
	image.getReadingDataHandler().data(image_data_raw, width * height);

	image_data = new unsigned char[width * height * 3];
	for (size_t i = 0; i < width * height; i++)
	{
		image_data[i * 3] = image_data_raw[i];
		image_data[i * 3 + 1] = image_data_raw[i];
		image_data[i * 3 + 2] = image_data_raw[i];
	}
	

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	delete []image_data_raw;
	delete []image_data;
}


// Called for keyboard events
void keyboard(int key, int action) {
	if(key == 'Q' && action == GLFW_PRESS) {
		glfwTerminate();
		exit(0);
	}
}

// Read a shader source from a file
// store the shader source in a std::vector<char>
// void read_shader_src(const char *fname, std::vector<char> &buffer) {
// 	std::ifstream in;
// 	in.open(fname, std::ios::binary);

// 	if(in.is_open()) {
// 		// Get the number of bytes stored in this file
// 		in.seekg(0, std::ios::end);
// 		size_t length = (size_t)in.tellg();

// 		// Go to start of the file
// 		in.seekg(0, std::ios::beg);

// 		// Read the content of the file in a buffer
// 		buffer.resize(length + 1);
// 		in.read(&buffer[0], length);
// 		in.close();
// 		// Add a valid C - string end
// 		buffer[length] = '\0';
// 	}
// 	else {
// 		std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
// 		exit(-1);
// 	}
// }

// // Compile a shader
// GLuint load_and_compile_shader(const char *fname, GLenum shaderType) {
// 	// Load a shader from an external file
// 	std::vector<char> buffer;
// 	read_shader_src(fname, buffer);
// 	const char *src = &buffer[0];

// 	// Compile the shader
// 	GLuint shader = glCreateShader(shaderType);
// 	glShaderSource(shader, 1, &src, NULL);
// 	glCompileShader(shader);
// 	// Check the result of the compilation
// 	GLint test;
// 	glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
// 	if(!test) {
// 		std::cerr << "Shader compilation failed with this message:" << std::endl;
// 		std::vector<char> compilation_log(512);
// 		glGetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
// 		std::cerr << &compilation_log[0] << std::endl;
// 		glfwTerminate();
// 		exit(-1);
// 	}
// 	return shader;
// }

// // Create a program from two shaders
// GLuint create_program(const char *path_vert_shader, const char *path_frag_shader) {
// 	// Load and compile the vertex and fragment shaders
// 	GLuint vertexShader = load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
// 	GLuint fragmentShader = load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);

// 	// Attach the above shader to a program
// 	GLuint shaderProgram = glCreateProgram();
// 	glAttachShader(shaderProgram, vertexShader);
// 	glAttachShader(shaderProgram, fragmentShader);

// 	// Flag the shaders for deletion
// 	glDeleteShader(vertexShader);
// 	glDeleteShader(fragmentShader);

// 	// Link and use the program
// 	glLinkProgram(shaderProgram);
// 	glUseProgram(shaderProgram);

// 	return shaderProgram;
// }


// int		main(int argc, char **argv)
// {
// 	std::atomic<bool> isRunning;
// 	GUI gui;

// 	isRunning = true;
// 	if (gui.initGL() == false)
// 		isRunning = false;
// 	gui.initLines();
// 	while (isRunning)
// 	{
// 		gui.events(isRunning);
// 		gui.update();
// 		gui.render();
// 	}

//     return 0;
// }