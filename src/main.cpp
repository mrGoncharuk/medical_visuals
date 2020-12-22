// dear imgui: standalone example application for GLUT/FreeGLUT + OpenGL2, using legacy fixed pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

// !!! GLUT/FreeGLUT IS OBSOLETE SOFTWARE. Using GLUT is not recommended unless you really miss the 90's. !!!
// !!! If someone or something is teaching you GLUT in 2020, you are being abused. Please show some resistance. !!!
// !!! Nowadays, prefer using GLFW or SDL instead!

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/freeglut.h>
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4505) // unreferenced local function has been removed
#endif

#include <imebra/imebra.h>
#include <iostream>
using namespace std;
using namespace imebra;

GLuint texID = 0;
int     g_image_width, g_image_height;

struct s_dicom_data
{
    int     img_width;
    int     img_height;
    double  spacing;
    int     position;
    int     orientation;
    char    *img = NULL;
};



// Our state
static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void my_display_code()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
}

void glut_display_func()
{
    
    static bool isloaded = false;

    if (!isloaded)
    {
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, type, windowWidth, windowHeight, 0, type, getBitsType(), buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    my_display_code();

    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();



    
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();
}


void readImage(s_dicom_data *dicom_data)
{
    const int frameNumber = 0;
    
    
    DataSet loadedDataSet(CodecFactory::load("data/DICOM_Image_Lab_2.dcm"));
    Image image(loadedDataSet.getImage(0));

    dicom_data->img_width = loadedDataSet.getInt32(TagId(0x0028, 0x010), 0);
    dicom_data->img_height = loadedDataSet.getInt32(TagId(0x0028, 0x011), 0);
    dicom_data->img = new char[dicom_data->img_width * dicom_data->img_height];

    cout << "Ret: " << image.getReadingDataHandler().data(dicom_data->img, g_image_width * g_image_height) << endl;
    
}

void initGL()
{
#ifdef __FREEGLUT_EXT_H__
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Dear ImGui GLUT+OpenGL2 Example");
}

int main(int argc, char** argv)
{
    // Create GLUT window
    glutInit(&argc, argv);
    initGL();
    s_dicom_data    dicom_data;
    readImage(&dicom_data);
    // Setup GLUT display function
    // We will also call ImGui_ImplGLUT_InstallFuncs() to get all the other functions installed for us,
    // otherwise it is possible to install our own functions and call the imgui_impl_glut.h functions ourselves.
    glutDisplayFunc(glut_display_func);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();


    glutMainLoop();

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
