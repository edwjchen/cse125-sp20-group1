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
    void setId(int player_id);
    void setTime(std::string t);
    void setScore(int s);
  
private:
    GLFWwindow* window;
    int width;
    int height;
    std::string title;
    bool closed;
    
    int user_id;
    std::string time;
    int score;

    GLFWwindow* createWindow(int width, int height, std::string title);
    void setupGui();
    void cleanupGui();
};

#endif
