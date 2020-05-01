//
//  Camera.hpp
//  Gaia
//
//  Created by Cain on 4/25/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "core.h"

class Camera {
public:
    Camera(glm::vec3 eyePos, glm::vec3 lookAtPos);
    ~Camera();
    void setPos(glm::vec3 pos);
    void setLookAt(glm::vec3 pos);
    void rotateAround(float xAngle, float yAngle);
    glm::vec3 getPos();
    glm::mat4 getView();
    glm::vec3 getLookAtPos(); 
private:
    // Camera Matrices
    glm::mat4 view;
    glm::vec3 eyePos, frontVector, upVector;
    float sensitivity;
    glm::vec3 lookAtPos;
    
};

#endif /* Camera_hpp */
