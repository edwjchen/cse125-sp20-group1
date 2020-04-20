//
//  Client.cpp
//  Gaia
//
//  Created by Cain on 4/16/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#include "Client.h"

Cube* Client::cube;
Sphere* Client::sphere_player1;
Sphere* Client::sphere_player2;

IO_handler* Client::io_handler;

Client::Client(int width, int height) {
  window = new Window(width, height, "Window");
  std::pair<int, int> windowSize = window->getFrameBufferSize();
  this->width = windowSize.first;
  this->height = windowSize.second;
  eyePos = glm::vec3(0, 0, 20);      // Camera position.
  lookAtPoint = glm::vec3(0, 0, 0);    // The point we are looking at.
  upVector = glm::vec3(0, 1, 0);    // The up direction of the camera.
  view = glm::lookAt(eyePos, lookAtPoint, upVector);
  projection = glm::perspective(glm::radians(60.0), double(width) / (double)height, 1.0, 1000.0);
  
  // Print OpenGL and GLSL versions.
  printVersions();
  // Setup OpenGL settings.
  setupOpenglSettings();
  
  setupCallbacks();
}

Client::~Client() {
  // Deallcoate the objects.
  delete cube;
  delete sphere_player1;
  delete sphere_player2;

  // Delete the shader program.
  glDeleteProgram(shaderProgram);
  
  delete window;
}

bool Client::initializeProgram() {
  // Create a shader program with a vertex shader and a fragment shader.
  shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
  
  // Check the shader program.
  if (!shaderProgram)
  {
    std::cerr << "Failed to initialize shader program" << std::endl;
    return false;
  }
  
  // Create io_handler (0 for balls)
  io_handler = new IO_handler(0);

  return true;
}

bool Client::initializeObjects()
{
    // Create a cube of size 5.
    cube = new Cube(5.0f);
    sphere_player1 = new Sphere(5.0f, 2.0f);
    sphere_player2 = new Sphere(5.0f, 2.0f);
    return true;
}

void Client::idleCallback() {
  //cube->update();
}

void Client::displayCallback() {
  // Clear the color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Render the objects
  //cube->draw(view, projection, shaderProgram);
    sphere_player1->draw(view, projection, shaderProgram);
    sphere_player2->draw(view, projection, shaderProgram);
}

bool Client::initialize() {
  return initializeProgram() && initializeObjects(); 
}

void Client::setupOpenglSettings()
{
  // Set the viewport size.
  glViewport(0, 0, width, height);
  
  // Enable depth buffering.
  glEnable(GL_DEPTH_TEST);
  
  // Related to shaders and z value comparisons for the depth buffer.
  glDepthFunc(GL_LEQUAL);
  
  // Set polygon drawing mode to fill front and back of each polygon.
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
  // Set clear color to black.
  glClearColor(0.0, 0.0, 0.0, 0.0);
  

}

void Client::printVersions()
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

void Client::run() {
    if (!initialize())
    throw "Unable to initialize client";

    // Client Try
    try
    {
        boost::asio::io_service io_service;
        tcp::endpoint endpoint(ip::address::from_string("127.0.0.1"),8888);

        chat_client c(io_service, endpoint);

        boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

        std::string msg;

        // Loop while GLFW window should stay open.
        while (!glfwWindowShouldClose(window->getWindow()))
        {
            // Main render display callback. Rendering of objects is done here. (Draw)
            displayCallback();
            window->displayCallback();

            // Idle callback. Updating objects, etc. can be done here. (Update)
            idleCallback();
            io_handler -> SendPackage(&c);
            updateFromServer(c.getMsg());
        } 

        c.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}

void Client::errorCallback(int error, const char* description)
{
    // Print error.
    std::cerr << description << std::endl;
}

void Client::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  
    /*
      * TODO: Modify below to add your key callbacks.
      */
	
	  // Check for a key press.
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE:{
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            }
            // take user's io
            case GLFW_KEY_W:{
                io_handler->SendInput(0);
                break;
            }
            case GLFW_KEY_A:{
                io_handler->SendInput(1);
                break;
            }
            case GLFW_KEY_S:{
                io_handler->SendInput(2);
                break;
            }
            case GLFW_KEY_D:{
                io_handler->SendInput(3);
                break;
            }
            default:
                break;
        }
    }
    
    // Check for holding key
	else if (action == GLFW_REPEAT)
	{
		switch (key)
		{
            // Contineous movement
            case GLFW_KEY_W:{
                io_handler->SendInput(0);
                break;
            }
            case GLFW_KEY_A:{
                io_handler->SendInput(1);
                break;
            }
            case GLFW_KEY_S:{
                io_handler->SendInput(2);
                break;
            }
            case GLFW_KEY_D:{
                io_handler->SendInput(3);
                break;
            }
            default:
                break;
        }
    }
}

void Client::setupCallbacks()
{
    // Set the error callback.
    glfwSetErrorCallback(errorCallback);
    
    // Set the key callback.
    glfwSetKeyCallback(window->getWindow(), keyCallback);
}
void Client::updateFromServer(string msg)
{
    if(msg != ""){
        
        // Hardcode string decoding for now
        vector<string> result;
        stringstream s_stream(msg);
        while(s_stream.good()) {
           string substr;
           getline(s_stream, substr, ','); //get first string delimited by comma
           result.push_back(substr);
        }
        float x1 = stof(result.at(0));
        float y1 = stof(result.at(1));
        float x2 = stof(result.at(2));
        float y2 = stof(result.at(3));
        glm::vec3 pos1 = glm::vec3(x1, y1, 0);
        glm::vec3 pos2 = glm::vec3(x2, y2, 0);
        sphere_player1->move(pos1);
        sphere_player2->move(pos2);
    }
}