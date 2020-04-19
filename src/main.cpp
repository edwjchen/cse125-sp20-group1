#include "main.h"

void error_callback(int error, const char* description)
{
	// Print error.
	std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window)
{
	// Set the error callback.
	glfwSetErrorCallback(error_callback);
	
	// Set the window resize callback.
	glfwSetWindowSizeCallback(window, Window::resizeCallback);

	// Set the key callback.
	glfwSetKeyCallback(window, Window::keyCallback);
}

void setup_opengl_settings()
{
	// Enable depth buffering.
	glEnable(GL_DEPTH_TEST);

	// Related to shaders and z value comparisons for the depth buffer.
	glDepthFunc(GL_LEQUAL);

	// Set polygon drawing mode to fill front and back of each polygon.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Set clear color to black.
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions()
{
	// Get info of GPU and supported OpenGL version.
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) 
		<< std::endl;

	//If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::cout << "Supported GLSL version is: " << 
		glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}



int main(void)
{
	// Create the GLFW window.
	GLFWwindow* window = Window::createWindow(640, 480);
	if (!window) 
		exit(EXIT_FAILURE);
    
    
    
	// Print OpenGL and GLSL versions.
	print_versions();

	// Setup callbacks.
	setup_callbacks(window);

	// Setup OpenGL settings.
	setup_opengl_settings();
    
    
    

	// Initialize the shader program; exit if initialization fails.
	if (!Window::initializeProgram()) 
		exit(EXIT_FAILURE);

	// Initialize objects/pointers for rendering; exit if initialization fails.
	if (!Window::initializeObjects()) 
		exit(EXIT_FAILURE);
	
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    
    // Client Try
    try
    {
        boost::asio::io_service io_service;
        tcp::endpoint endpoint(ip::address::from_string("127.0.0.1"),8888);

        chat_client c(io_service, endpoint);

        boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

        
        std::string msg;
        
        /*
        while (getline(std::cin, msg))
        {
            c.write(msg);
        }

            c.close();
            t.join();
        }
         */

    
    
    
	// Loop while GLFW window should stay open.
        while (!glfwWindowShouldClose(window))
        {
            // Main render display callback. Rendering of objects is done here. (Draw)
            Window::displayCallback(window);

            // Idle callback. Updating objects, etc. can be done here. (Update)
            Window::idleCallback();
            
            
            Window::io_handler -> SendPackage(&c);
        }
        
        c.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

	// destroy objects created
	Window::cleanUp();
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	// Destroy the window.
	glfwDestroyWindow(window);

	// Terminate GLFW.
	glfwTerminate();


    
    
    
	exit(EXIT_SUCCESS);
}
