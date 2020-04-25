//
//  Client.h
//  Gaia
//
//  Created by Cain on 4/16/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#ifndef Client_h
#define Client_h

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include "engine.h"
#include "Window.h"
#include "shader.h"
#include "IO_handler.hpp"

class Client {
public:
  Client(int width, int height);
  ~Client();
  bool initialize();

  // Draw and update
  void idleCallback();
  void displayCallback();

  void run();

private:
  Window* window;
  int width;
  int height;
  // Camera Matrices
  glm::mat4 projection;
  glm::mat4 view;
  glm::vec3 eyePos, lookAtPoint, upVector;

  // Shader Program ID
  GLuint shaderProgram;

  // Constructors and Destructors
  bool initializeProgram();
  bool initializeObjects();
  void setupOpenglSettings();
  void printVersions();
  void setupCallbacks();

  static void errorCallback(int error, const char* description);
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

  // objects
  static Cube* cube;
  static Sphere* sphere_player1;
  static Sphere* sphere_player2;

  // IO Handler
  static IO_handler* io_handler;

  // Decode message from server and update client side graphic
  static void updateFromServer(std::string msg);
};

#endif /* Client_h */
