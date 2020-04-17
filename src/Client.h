//
//  Client.h
//  Gaia
//
//  Created by Cain on 4/16/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#ifndef Client_h
#define Client_h

#include <stdio.h>
#include "Cube.h"
#include "shader.h"

class Client {
public:
  Client(int width, int height);
  ~Client();
  bool initialize();
  
  // Draw and update
  void idleCallback();
  void displayCallback();
  
private:
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
  
  // objects
  Cube* cube;
};

#endif /* Client_h */
