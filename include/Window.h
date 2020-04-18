#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


class Window {
public:
  Window(int width, int height, std::string title);
  ~Window();
  GLFWwindow* getWindow();
  void displayCallback();
  std::pair<int, int> getFrameBufferSize();
  
private:
  GLFWwindow* window;
  int width;
  int height; 
  std::string title;
  bool closed; 
  
  GLFWwindow* createWindow(int width, int height, std::string title);
  void setupGui();
  void cleanupGui();
};
#endif
