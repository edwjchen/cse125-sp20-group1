//
//  Camera.cpp
//  Gaia
//
//  Created by Cain on 4/25/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#include "Camera.hpp"
#define SENS 0.1f

Camera::Camera(glm::vec3 eyePos, glm::vec3 lookAtPos) {
    this->eyePos = eyePos;
    sensitivity = SENS;
    
    glm::vec3 front = glm::normalize(lookAtPos - eyePos);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::normalize(glm::cross(right, front));
    
    this->frontVector = front;
    this->upVector = up;
    this->view = glm::lookAt(eyePos, eyePos + frontVector, upVector);}

Camera::~Camera() {
    
}

void Camera::setLookAt(glm::vec3 pos) {
    glm::vec3 front = glm::normalize(pos - eyePos);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::normalize(glm::cross(right, front));
    
    this->frontVector = front;
    this->upVector = up;
    this->view = glm::lookAt(eyePos, eyePos + frontVector, upVector);
}

glm::mat4 Camera::getView() {
    return view;
}

glm::vec3 Camera::getPos() {
    return eyePos;
}

void Camera::setPos(glm::vec3 pos) {
    this->eyePos = pos;
    this->view = glm::lookAt(eyePos, eyePos + frontVector, upVector);
}
