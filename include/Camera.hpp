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
    Camera(glm::vec3 pos, float yaw = -90.0f, float pitch = 0.0f, glm::vec3 up = glm::vec3(0, 1, 0));
    ~Camera();
    void move(glm::vec3 dis);
    void updateLookAt(float xoffset, float yoffset);
    glm::mat4 getView(); 
private:
    // Camera Matrices
    glm::mat4 view;
    glm::vec3 eyePos, frontVector, upVector;
    float yaw, pitch;
    void updateVectors();
    float sensitivity; 
    
};

#endif /* Camera_hpp */
