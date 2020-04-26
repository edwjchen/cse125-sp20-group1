#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "core.h"

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
