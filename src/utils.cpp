#include "utils.h"

void display(GLuint vao, int size)
{
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, size);
}

void read_shader_src(const char *fname, std::vector<char> &buffer) {
	std::ifstream in;
	in.open(fname, std::ios::binary);

	if(in.is_open()) {
		// Get the number of bytes stored in this file
		in.seekg(0, std::ios::end);
		size_t length = (size_t)in.tellg();

		// Go to start of the file
		in.seekg(0, std::ios::beg);

		// Read the content of the file in a buffer
		buffer.resize(length + 1);
		in.read(&buffer[0], length);
		in.close();
		// Add a valid C - string end
		buffer[length] = '\0';
	}
	else {
		std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
		exit(-1);
	}
}

GLuint load_and_compile_shader(const char *fname, GLenum shaderType) {
	// Load a shader from an external file
	std::vector<char> buffer;
	read_shader_src(fname, buffer);
	const char *src = &buffer[0];

	// Compile the shader
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	// Check the result of the compilation
	GLint test;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
	if(!test) {
		std::cerr << "Shader compilation failed with this message:" << std::endl;
		std::vector<char> compilation_log(512);
		glGetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
		std::cerr << &compilation_log[0] << std::endl;
		glfwTerminate();
		exit(-1);
	}
	return shader;
}

GLuint create_program(const char *path_vert_shader, const char *path_frag_shader) {
	// Load and compile the vertex and fragment shaders
	GLuint vertexShader = load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
	GLuint fragmentShader = load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);

	// Attach the above shader to a program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Flag the shaders for deletion
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Link and use the program
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	return shaderProgram;
}

GLfloat *normalize_coordinates(const std::vector<float> &buf)
{
    GLfloat *vertices_position = new GLfloat[buf.size()];

    for (int i = 0; i < buf.size(); i += 2)
    {
        // vertices_position[i] = buf[i];
        // vertices_position[i + 1] = buf[i + 1];
        vertices_position[i] = buf[i]/ SCREEN_WIDTH;
        vertices_position[i + 1] = buf[i + 1] / SCREEN_HEIGHT;
        // vertices_position[i] = (buf[i] - SCREEN_WIDTH / 2) / SCREEN_WIDTH;
        // vertices_position[i + 1] = (buf[i + 1] - SCREEN_HEIGHT / 2) / SCREEN_HEIGHT;
        // std::cout << "(" << vertices_position[i] << "; " << vertices_position[i + 1] << ")" << std::endl;
    }
    // std::cout << "Buf size: " << buf.size() << std::endl;
    return vertices_position;
}

void initialize(GLuint &vao, const std::vector<float> &buf)
{
	// Use a Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 4 triangles to be rendered
    GLfloat *vertices_position = normalize_coordinates(buf);

	// Create a Vector Buffer Object that will store the vertices on video memory
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Allocate space and upload the data from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, buf.size() * sizeof(float), vertices_position, GL_STATIC_DRAW);	

	GLuint shaderProgram = create_program("shaders/vert.shader", "shaders/frag.shader");

	// Get the location of the attributes that enters in the vertex shader
	GLint position_attribute = glGetAttribLocation(shaderProgram, "position");

	// Specify how the data for position can be accessed
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	// Enable the attribute
	glEnableVertexAttribArray(position_attribute);
    delete []vertices_position;
}
