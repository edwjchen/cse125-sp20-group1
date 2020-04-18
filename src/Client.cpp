//
//  Client.cpp
//  Gaia
//
//  Created by Cain on 4/16/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#include "Client.h"

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
  


}

Client::~Client() {
  // Deallcoate the objects.
  delete cube;
  
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
  
  return true;
}

bool Client::initializeObjects()
{
  // Create a cube of size 5.
  cube = new Cube(5.0f);
  
  return true;
}

void Client::idleCallback() {
  cube->update();
}

void Client::displayCallback() {
  // Clear the color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Render the objects
  cube->draw(view, projection, shaderProgram);
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
  
  // Loop while GLFW window should stay open.
  while (!glfwWindowShouldClose(window->getWindow()))
  {
    // Main render display callback. Rendering of objects is done here. (Draw)
    displayCallback();
    window->displayCallback();
    
    // Idle callback. Updating objects, etc. can be done here. (Update)
    idleCallback();
  }
}

void Client::errorCallback(int error, const char* description)
{
  // Print error.
  std::cerr << description << std::endl;
}

void Client::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  
  // Check for a key press.
  if (action == GLFW_PRESS)
  {
    switch (key)
    {
      case GLFW_KEY_ESCAPE:
        // Close the window. This causes the program to also terminate.
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;
        
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
