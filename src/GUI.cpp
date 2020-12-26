#include "GUI.hpp"
#include <mutex>
#include <iostream>

#include <fstream>
#include <iostream>
using namespace std;


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


GUI::GUI(/* args */): clearColor(0.45f, 0.55f, 0.60f, 1.00f), loadedDataSet(imebra::CodecFactory::load("data/DICOM_Image_for_Lab_2.dcm"))
{

}


GUI::~GUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
}

void    GUI::initLines()
{
    		// The fullscreen quad's FBO
	// GLuint quad_VertexArrayID;
	// glGenVertexArrays(1, &quad_VertexArrayID);
	// glBindVertexArray(quad_VertexArrayID);

	// static const GLfloat g_quad_vertex_buffer_data[] = {
	// 	-1.0f, -1.0f, 0.0f,
	// 	1.0f, -1.0f, 0.0f,
	// 	-1.0f,  1.0f, 0.0f,
	// 	-1.0f,  1.0f, 0.0f,
	// 	1.0f, -1.0f, 0.0f,
	// 	1.0f,  1.0f, 0.0f,
	// };

	// GLuint quad_vertexbuffer;
	// glGenBuffers(1, &quad_vertexbuffer);
	// glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	// // Create and compile our GLSL program from the shaders
	// GLuint quad_programID = create_program( "shaders/passthrough.vert.shader", "shaders/texture.frag.shader" );
	// GLuint texID = glGetUniformLocation(quad_programID, "renderedTexture");
	// GLuint timeID = glGetUniformLocation(quad_programID, "time");
}



bool GUI::initGL()
{
        // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        std::cerr << "ERROR: Failed initialization of GLFW" << std::endl;
        return false;
    }
    // Decide GL+GLSL versions
#ifdef __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 330 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    this->window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MedicalVisuals", NULL, NULL);
    if (this->window == NULL)
    {
        std::cerr << "ERROR: Failed creation of window" << std::endl;
        return false;
    }
    glfwMakeContextCurrent(this->window);
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
        return false;
    }

// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);



    return true;
}







void    GUI::events(std::atomic<bool> &isRunning)
{
    glfwPollEvents();
    if (glfwWindowShouldClose(this->window))
        isRunning = false;

}

bool LoadTextureFromArray(char *pixels, GLuint* out_texture, int image_width, int image_height)
{
    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    *out_texture = FramebufferName;

    // The texture we're going to render to
    GLuint image_texture;
    glGenTextures(1, &image_texture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 1024, 768, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, image_texture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;
    return true;
}


void	GUI::update()
{
    // memset(&(this->flags), 0, sizeof(flags));
    static bool show_demo_window = true;
    static imebra::Image image(loadedDataSet.getImageApplyModalityTransform(0));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    // if (show_demo_window)
    //     ImGui::ShowDemoWindow(&show_demo_window);


    {
        static int x1 = 50, x2 = 500, y1 = 50, y2 = 500;
        ImGui::Begin("Plotter");
        ImGui::Text("Window Width = %d", SCREEN_WIDTH);
        ImGui::Text("Window Height = %d", SCREEN_HEIGHT);
        ImGui::InputInt("x1", &x1);
        ImGui::InputInt("y1", &y1);
        ImGui::InputInt("x2", &x2);
        ImGui::InputInt("y2", &y2);
        ImVec2 pos = ImGui::GetMousePos();
        ImGui::Text("x = %f", pos.x);
        ImGui::Text("y = %f", pos.y);
        // ImGui::Image((void*)(intptr_t)this->my_image_texture, ImVec2(500, 500));
        ImGui::End();
    }


    static bool isProcessed = false;
    static char *image_data = NULL;

    // if (!isProcessed)
    // {

    //     std::uint32_t width = image.getWidth();
    //     std::uint32_t height = image.getHeight();
    //     image_data = new char[width * height];
    //     image.getReadingDataHandler().data(image_data, width * height);
    //     if (isProcessed = LoadTextureFromArray(image_data, &this->my_image_texture, width, height))
    //         std::cout << "Result: " << isProcessed << std::endl;
    //     delete []image_data;
    // }
    

}



void	GUI::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->my_image_texture);
    glViewport(0,0,1024,768); // Render on the whole framebuffer, complete from the lower left corner to the upper right  
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    // glBindFramebuffer(GL_FRAMEBUFFER, this->my_image_texture);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    // glMatrixMode(GL_PROJECTION)
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}


GLFWwindow *GUI::getWindow() { return (this->window); }

