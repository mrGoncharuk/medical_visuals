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
    GLuint vao;
    std::vector<float> buf;

    // Initialize the data to be rendered
	ft_draw_line(buf, 0, -SCREEN_HEIGHT, 0, SCREEN_HEIGHT);
    // ft_draw_line(buf, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	initialize(vao, buf);
    lines.insert(std::pair<GLuint, size_t>(vao, buf.size() / 2));

	ft_draw_line(buf, -SCREEN_WIDTH, 0, SCREEN_WIDTH, 0);
    initialize(vao, buf);
    lines.insert(std::pair<GLuint, size_t>(vao, buf.size() / 2));
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



void    GUI::addLine(int x0, int y0, int x1, int y1)
{
    if (lines.size() > 2)
        lines.erase(prev(lines.end()));
    GLuint vao;
    std::vector<float> buf;

    // Initialize the data to be rendered
	ft_draw_line(buf, x0, y0, x1, y1);
    // ft_draw_line(buf, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	initialize(vao, buf);
    lines.insert(std::pair<GLuint, size_t>(vao, buf.size() / 2));
}

void	GUI::toCylindric(int x0, int y0, int x1, int y1)
{
    if (lines.size() > 2)
        lines.erase(prev(lines.end()));
    GLuint vao;
    std::vector<float> buf;

    // Initialize the data to be rendered
	ft_draw_line(buf, x0, y0, x1, y1);
    float   r, f, x, y, xc, yc;

    for (auto i = buf.begin(); i != buf.end(); i += 2)
    {
        x = *i;
        y = *(i + 1);
        r = sqrt(x*x + y*y);
        f = atan(y / x);
        // x = r * cos(f);
        // y = r * sin(f);
        *i = r;
        *(i + 1) = f;
        std::cout << "(" << r << "; " << f << ")" << std::endl;
    }
    initialize(vao, buf);
    lines.insert(std::pair<GLuint, size_t>(vao, buf.size() / 2));
}


void    GUI::events(std::atomic<bool> &isRunning)
{
    glfwPollEvents();
    if (glfwWindowShouldClose(this->window))
        isRunning = false;

}

void LoadTextureFromArray(char *pixels, GLuint* out_texture, int image_width, int image_height)
{
    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    // glPixelStorei( GL_UNPACK_ALIGNMENT, 0 );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_RED};
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image_width, image_height, 0, GL_RED , GL_UNSIGNED_BYTE, pixels);
    *out_texture = image_texture;
    
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
        if (ImGui::Button("Draw Line"))
            addLine(x1, y1, x2, y2);
        if (ImGui::Button("To Cylindic"))
            toCylindric(x1, y1, x2, y2);
        // ImGui::Image((void*)(intptr_t)this->my_image_texture, ImVec2(500, 500));
        ImGui::End();
    }


    static bool isProcessed = false;
    static char *image_data = NULL;

    if (!isProcessed)
    {
        // Get the color space
        std::string colorSpace = image.getColorSpace();

        // Get the size in pixels
        std::uint32_t width = image.getWidth();
        std::uint32_t height = image.getHeight();
        std::cout << "Color Space: " << colorSpace << std::endl;
        std::cout << "Image Width: " << width << std::endl;
        std::cout << "Image Height: " << height << std::endl;
        std::cout << "Number of color channels:  " << image.getChannelsNumber() << std::endl;
        std::cout << "Pixel size:  " << (int)image.getDepth() << std::endl;
        
        image_data = new char[width * height];
        std:: cout << "Ret: " << image.getReadingDataHandler().data(image_data, width * height) << std::endl;
        LoadTextureFromArray(image_data, &this->my_image_texture, width, height);
        isProcessed = true;
        delete []image_data;
    }
    
    {
        ImGui::Begin("Image Data");
        static bool showData = true;
        static std::string manufacturer;
        if (manufacturer.empty())
        {
            imebra::tagsIds_t tags = loadedDataSet.getTags();
            for (auto i = tags.begin(); i != tags.end(); i++)
            {
                if (imebra::DicomDictionary::getTagDescription(*i) == "Manufacturer")
                {
                    std::cout << "TagID: " << i->getTagId() << std::endl;
                    std::cout << "TagDescription: " << imebra::DicomDictionary::getTagDescription(*i) << std::endl;
                    std::cout << "Data type: " << loadedDataSet.getString(*i, 0) << std::endl;
                    manufacturer = loadedDataSet.getString(*i, 0);
                }
                // std::cout << "TagID: " << imebra::DicomDictionary::getTagDescription(*i) << std::endl;
            }
        }
        ImGui::Checkbox("Show data", &showData);
        if (showData)
        {
            ImGui::Text("Color space: %s", image.getColorSpace().c_str());
            ImGui::Text("size = %d x %d", image.getWidth(), image.getHeight());
            ImGui::Text("Manufacturer: %s", manufacturer.c_str());
        }
        ImGui::Image((void*)(intptr_t)this->my_image_texture, ImVec2(500, 500));
        ImGui::End();
    }
}



void	GUI::render()
{
                
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    // glMatrixMode(GL_PROJECTION)
    glClear(GL_COLOR_BUFFER_BIT);
    for (auto i = lines.begin(); i != lines.end(); i++)
    {
        display(i->first, i->second);
    }
    // display(lines., vao_vertecies);
    // display(vao2, vao2_vertecies);
    // display(vao_cyl, vao_cyl_vertecies);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}


GLFWwindow *GUI::getWindow() { return (this->window); }

