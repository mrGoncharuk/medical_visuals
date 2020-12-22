#include "GUI.hpp"
#include <mutex>
#include <iostream>

#include <fstream>
#include <sstream>

using namespace std;


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


GUI::GUI(/* args */): clearColor(0.45f, 0.55f, 0.60f, 1.00f), loadedDataSet(NULL)
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
    delete loadedDataSet;
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

    loadedDataSet = new imebra::DataSet(imebra::CodecFactory::load("data/DICOM_Image_for_Lab_2.dcm"));
    imebra::Image image(loadedDataSet->getImageApplyModalityTransform(0));
    char *image_data = NULL;
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
    
    min_max_pixel(image_data, width * height, &(this->min_pixel), &(this->max_pixel));

    delete []image_data;

    return true;
}



void    GUI::events(std::atomic<bool> &isRunning)
{
    glfwPollEvents();
    if (glfwWindowShouldClose(this->window))
        isRunning = false;

}

void    GUI::print_tag_info(imebra::TagId tag)
{
    std::cout << "Tag: (" << std::hex << tag.getGroupId() << ", " <<  std::hex << tag.getTagId() << ")" << std::endl;
    std::cout << "TagDescription: " << imebra::DicomDictionary::getTagDescription(tag) << std::endl;
    // std::cout << "TagType(VR): " << std::hex << int(imebra::DicomDictionary::getTagType(tag)) << std::endl;

    imebra::tagVR_t tag_type = imebra::DicomDictionary::getTagType(tag);
    switch (tag_type)
    {
    case imebra::tagVR_t::DS:
        std::cout << "TagType(VR): " << "DS" << std::endl;
        break;
    default:
        std::cout << "TagType(VR): " << int(tag_type) << std::endl;
    }
    std::cout << "Value: " << loadedDataSet->getString(tag, 0) << std::endl << std::endl;
    
    
}

std::string tagVR_to_string(const imebra::TagId tag)
{
    imebra::tagVR_t vr = imebra::DicomDictionary::getTagType(tag);
    switch (vr)
    {
    case imebra::tagVR_t::DS:
        return "DS(Decimal String)";
    case imebra::tagVR_t::OF:
        return "OF(Other Float String)";
    case imebra::tagVR_t::OW:
        return "OW(Other Word String)";
    default:
        std::stringstream stream;
        stream << std::hex << int(vr);
        return stream.str();
    }

}





void	GUI::update()
{
    // memset(&(this->flags), 0, sizeof(flags));
    static bool show_demo_window = false;
    static bool isProcessed = false;
    static char *image_data = NULL;
    
    // static imebra::Image image(loadedDataSet->getImageApplyModalityTransform(0));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    // if (show_demo_window)
    //     ImGui::ShowDemoWindow(&show_demo_window);

    
    {
        static float    slope = 0.01250557;
        static float    intercept = 4.180;
        static int             max = 175;
        static int             min = 10;
        static int width = 256, height = 256;
        ImGui::Begin("Image Data");
        static bool showData = true;

        static char buf[128] = "data/DICOM_Image_for_Lab_2.dcm";
        static char buf2[128] = "data/DICOM_Image_for_Lab_2_new.dcm";
        ImGui::InputText("Source file", buf, 128);
        if (ImGui::Button("Load file"))
        {
            if (loadedDataSet != NULL)
            {
                delete loadedDataSet;
                loadedDataSet = NULL;
                glDeleteTextures(1, &this->my_image_texture);
            }
            loadedDataSet = new imebra::DataSet(imebra::CodecFactory::load(buf));

            imebra::Image image(loadedDataSet->getImage(0));
            std::uint32_t width = image.getWidth();
            std::uint32_t height = image.getHeight();
            char *image_data = NULL;
            image_data = new char[width * height];

            image.getReadingDataHandler().data(image_data, width * height);
            if (loadedDataSet->getString(imebra::TagId(0x0028, 0x1052), 0) != "0"
                 && loadedDataSet->getString(imebra::TagId(0x0028, 0x1053), 0) != "1")
                transform_image(image_data, width * height, 
                                loadedDataSet->getFloat(imebra::TagId(0x0028, 0x1053), 0),
                                loadedDataSet->getFloat(imebra::TagId(0x0028, 0x1052), 0), max, min);
            LoadTextureFromArray(image_data, &this->my_image_texture, width, height);
            min_max_pixel(image_data, width * height, &(this->min_pixel), &(this->max_pixel));
            delete []image_data;

        }

        ImGui::Checkbox("Show data", &showData);

        if (showData)
        {
            if (loadedDataSet == NULL)
                ImGui::Text("File not loaded.");
            else
            {
                ImGui::Text("Minimal Pixel: %d", this->min_pixel);
                ImGui::Text("Maximal Pixel: %d", this->max_pixel);
                ImGui::Text("Rescale Intercept(0x0028, 0x1052): %s", loadedDataSet->getString(imebra::TagId(0x0028, 0x1052), 0).c_str());
                ImGui::Text("Rescale Slope(0x0028, 0x1053): %s", loadedDataSet->getString(imebra::TagId(0x0028, 0x1053), 0).c_str());

                // ImGui::Text("High Bit(0x0028, 0x0102): %s", loadedDataSet->getString(imebra::TagId(0x0028, 0x0102), 0).c_str());
                // ImGui::Text("Bits Alocated(0x0028, 0x0100): %s", loadedDataSet->getString(imebra::TagId(0x0028, 0x0100), 0).c_str());
                ImGui::Text("Pixel Data(0x7fe0, 0x0010) type: %s", tagVR_to_string(imebra::TagId(0x7fe0, 0x0010)).c_str());
            }
        }
        


        ImGui::InputFloat("Slope", &slope, 0.001f, 0.01f, "%.6f");
        ImGui::InputFloat("Intercept", &intercept, 0.1f, 1.0f, "%.3f");
        ImGui::InputInt("Min", &min);
        ImGui::InputInt("Max", &max);

        ImGui::InputText("New file", buf2, 128);
        if (ImGui::Button("Create New File"))
        {

            imebra::Image image_orig(loadedDataSet->getImage(0));

            // Saving original image to new file
            // because library segfaults when i'm trying 
            // to add the image to the dataset

            // int img_width = image_orig.getWidth();
            // int img_height = image_orig.getHeight();
            // char *image_data = NULL;

            // image_data = new char[img_width * img_height];
            // image_orig.getReadingDataHandler().data(image_data, img_width * img_height);

            // imebra::MutableImage image_mutable(img_width, img_height, 
            //                                     image_orig.getDepth(), 
            //                                     image_orig.getColorSpace(),
            //                                     image_orig.getHighBit());

            
            // // // Add the image to the dataset

            // imebra::WritingDataHandlerNumeric writeIntoImage(image_mutable.getWritingDataHandler());

            // transform_image(image_data, img_width * img_height, slope, intercept, max, min);
            // writeIntoImage.assign(image_data, img_width * img_height);
            // delete []image_data;


            // // // Add the image to the dataset
            imebra::charsetsList_t charsets;
            charsets.push_back("ISO 2022 IR 6");
            imebra::MutableDataSet dataSet("1.2.840.10008.1.2", charsets);
            dataSet.setImage(0, image_orig,  imebra::imageQuality_t::veryHigh);

            imebra::tagsIds_t tags = loadedDataSet->getTags();

            for (auto i = tags.begin(); i != tags.end(); i++)
            {

                if (i->getGroupId() == 0x0028 && i->getTagId() == 0x1052) // Rescale slope
                {
                    dataSet.setString(imebra::TagId(0x0028, 0x1052), to_string(slope));
                    continue;
                }
                if (i->getGroupId() == 0x0028 && i->getTagId() == 0x1053) // Rescale intercept
                {
                    dataSet.setString(imebra::TagId(0x0028, 0x1053), to_string(intercept));
                    continue;
                }

                if (i->getGroupId() == 0x0008  && i->getTagId() == 0x103f)  // Series Description Code Sequence
                    continue;
                if (i->getGroupId() == 0x7fe0  && i->getTagId() == 0x0010)  // image data
                    continue;

                // std::cout << std::hex << i->getGroupId() << ", " << std::hex << i->getTagId() << std::endl;
                dataSet.setString(*i, loadedDataSet->getString(*i, 0));

            }

            // Series Instance UID
            {
                imebra::TagId tag(0x0020, 0x000e);
                imebra::RandomUIDGenerator generator("1.2.276.0", 228, 322);
                imebra::WritingDataHandler dh = dataSet.getWritingDataHandler(tag, 0, imebra::DicomDictionary::getTagType(tag));
                dh.setString(0, generator.getUID());
            }

            // Series Number
            {
                
                imebra::TagId tag(0x020, 0x0011);
                // dataSet.getTagCreate(tag);
                // imebra::RandomUIDGenerator generator("1.2.276.0", 228, 322);
                // imebra::WritingDataHandler dh = dataSet.getWritingDataHandler(tag, 0, imebra::DicomDictionary::getTagType(tag));
                // dh.setString(0, generator.getUID());
                imebra::WritingDataHandler dh = dataSet.getWritingDataHandler(tag, 0, imebra::DicomDictionary::getTagType(tag));
                dh.setString(0, "2");
            }

            // Instance Number
            {
                imebra::TagId tag(0x0020, 0x0013);
                imebra::WritingDataHandler dh = dataSet.getWritingDataHandler(tag, 0, imebra::DicomDictionary::getTagType(tag));
                dh.setString(0, "1");
            }

            // Series Description Code
            {
                imebra::TagId tag(0x0020, 0x0016);
                imebra::RandomUIDGenerator generator("1.2.276.0", 228, 322);
                imebra::WritingDataHandler dh = dataSet.getWritingDataHandler(tag, 0, imebra::DicomDictionary::getTagType(tag));
                dh.setString(0, "1231234");
            }

            // Media Storage SOP Instance UID
            {
                imebra::TagId tag(0x0002, 0x0003);
                imebra::RandomUIDGenerator generator("1.2.276.0", 228, 322);
                imebra::WritingDataHandler dh = dataSet.getWritingDataHandler(tag, 0, imebra::DicomDictionary::getTagType(tag));
                dh.setString(0, dataSet.getString(imebra::TagId(0x0020, 0x0016), 0));
            }

            // Image Type
            {
                imebra::TagId tag(0x0008, 0x0008);
                imebra::WritingDataHandler dh = dataSet.getWritingDataHandler(tag, 0, imebra::DicomDictionary::getTagType(tag));
                dh.setString(0, "DERIVED");
                dh.setString(1, "SECONDARY");
            }

            // Series Description
            {
                imebra::TagId tag(0x0008, 0x103e);
                imebra::WritingDataHandler dh = dataSet.getWritingDataHandler(tag, 0, imebra::DicomDictionary::getTagType(tag));
                std::string series_desc = "slope-" + to_string(slope) + "intercept-" + to_string(intercept);
                dh.setString(0, series_desc);
            }

            // Save to a file
            imebra::CodecFactory::save(dataSet, buf2, imebra::codecType_t::dicom);

        }


        ImGui::Image((void*)(intptr_t)this->my_image_texture, ImVec2(width, height));
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

    glClear(GL_COLOR_BUFFER_BIT);



    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}


GLFWwindow *GUI::getWindow() { return (this->window); }

