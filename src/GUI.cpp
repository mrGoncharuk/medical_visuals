#include "GUI.hpp"

using namespace std;


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


GUI::GUI(/* args */): clearColor(0.45f, 0.55f, 0.60f, 1.00f)
                    , loadedDataSet(imebra::CodecFactory::load("data/copy_of_DICOM_Image_for_Lab_2.dcm"))
                    , text_renderer(SCREEN_WIDTH, SCREEN_HEIGHT)
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


void	GUI::read_orientation()
{
    float tmp;
    for (size_t i = 0; i < 6; i++)
    {

        std::cout << loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0037), i) << std::endl;
    }

    tmp = loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0037), 0);
    if (tmp == 0)
        this->orient_abbr[0] = ' ';
    else if (tmp > cos(45))
        this->orient_abbr[0] = 'L';
    else
        this->orient_abbr[0] = 'R';
    
    tmp = loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0037), 1);
    if (tmp == 0)
        this->orient_abbr[1] = ' ';
    else if (tmp > cos(45))
        this->orient_abbr[1] = 'A';
    else
        this->orient_abbr[1] = 'P';

    tmp = loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0037), 2);
    if (tmp == 0)
        this->orient_abbr[2] = ' ';
    else if (tmp > cos(45))
        this->orient_abbr[2] = 'H';
    else
        this->orient_abbr[2] = 'F';
    
    tmp = loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0037), 3);
    if (tmp == 0)
        this->orient_abbr[3] = ' ';
    else if (tmp > cos(45))
        this->orient_abbr[3] = 'L';
    else
        this->orient_abbr[3] = 'R';

    tmp = loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0037), 4);    
    if (tmp == 0)
        this->orient_abbr[4] = ' ';
    else if (tmp > cos(45))
        this->orient_abbr[4] = 'A';
    else
        this->orient_abbr[4] = 'P';

    tmp = loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0037), 5);
    if (tmp == 0)
        this->orient_abbr[5] = ' ';
    else if (tmp < cos(45))
        this->orient_abbr[5] = 'H';
    else
        this->orient_abbr[5] = 'F';

    for (int i = 0; i < 6; i++)
        std::cout << this->orient_abbr[i];
    std::cout << std::endl;
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

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	text_renderer.createShaderProgram("shaders/text.vertex.shader", "shaders/text.fragment.shader");
	text_renderer.loadFont("fonts/open-sans/OpenSans-Bold.ttf");
    

    image_renderer.createShaderProgram("shaders/vert.shader", "shaders/frag.shader");
    image_renderer.loadImage("data/copy_of_DICOM_Image_for_Lab_2.dcm");

    this->read_orientation();


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

        ImGui::Begin("Plotter");

        ImVec2 pos = ImGui::GetMousePos();
        ImVec2 pos_centered(pos.x - SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - pos.y);

        ImGui::Text("x = %f", pos_centered.x);
        ImGui::Text("y = %f", pos_centered.y);


        if (pos.x < SCREEN_WIDTH / 2 && pos.x > 0
         && pos.y < SCREEN_WIDTH / 2 && pos.y > 0)
        {
            float x, y, z;
            float spacing = 

            x = loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0032), 0) + 
                loadedDataSet.getFloat(imebra::TagId(0x0028, 0x0030), 0) * loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0037), 0) * (pos.x - SCREEN_WIDTH / 4) +
                loadedDataSet.getFloat(imebra::TagId(0x0028, 0x0030), 1) * loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0037), 3) * (SCREEN_HEIGHT / 4 - pos.y);

            y = loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0032), 1) + 
                loadedDataSet.getFloat(imebra::TagId(0x0028, 0x0030), 0) * loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0037), 4) * (SCREEN_HEIGHT / 4 - pos.y) +
                loadedDataSet.getFloat(imebra::TagId(0x0028, 0x0030), 1) * loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0037), 1) * (pos.x - SCREEN_WIDTH / 4);
                // loadedDataSet.getFloat(imebra::TagId(0x0028, 0x0030), 0) * loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0032), 0);
            
            z = loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0032), 2) + 
                loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0037), 2) * (pos.x - SCREEN_WIDTH / 4) / loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0032), 2) +
                loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0037), 5) * (SCREEN_HEIGHT / 4 - pos.y) / loadedDataSet.getFloat(imebra::TagId(0x0020, 0x0032), 2);
            
            ImGui::Text("Image.x = %f", x);
            ImGui::Text("Image.y = %f", y );
            ImGui::Text("Image.z = %f", z );
        }

        // ImGui::Image((void*)(intptr_t)this->my_image_texture, ImVec2(500, 500));
        ImGui::End();
    }


    static bool isProcessed = false;
    static char *image_data = NULL;

}



void	GUI::render()
{
    static char buf[3];
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    // glBindFramebuffer(GL_FRAMEBUFFER, this->my_image_texture);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    // glMatrixMode(GL_PROJECTION)
    glClear(GL_COLOR_BUFFER_BIT);
    image_renderer.renderImage();
    

    strncpy(buf, this->orient_abbr + 4, 2);
    buf[2] = '\0';
    text_renderer.renderText(buf, 128.0f, 480.0f, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
    strncpy(buf, this->orient_abbr, 2);
    buf[2] = '\0';
    text_renderer.renderText(buf, 0.0f, 384.0f, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}


GLFWwindow *GUI::getWindow() { return (this->window); }

