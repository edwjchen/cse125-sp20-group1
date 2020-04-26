//
//  Camera.cpp
//  Gaia
//
//  Created by Cain on 4/25/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#include "Camera.hpp"
#define SENS 0.1f

Camera::Camera(glm::vec3 pos, float yaw, float pitch, glm::vec3 up) {
    eyePos = pos;
    upVector = up;
    this->yaw = yaw;
    this->pitch = pitch;
    sensitivity = SENS;
    updateVectors();
}

Camera::~Camera() {
    
}

void Camera::move(glm::vec3 dis) {
    eyePos = glm::translate(glm::mat4(1), dis) * glm::vec4(eyePos, 1.0f);
}

void Camera::updateLookAt(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw   += xoffset;
    pitch += yoffset;
    
    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    
    // Update Front, Right and Up Vectors using the updated Euler angles
    updateVectors();
}

void Camera::updateVectors() {
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, front));
    
    this->frontVector = front;
    this->upVector = up;
    
    this->view = glm::lookAt(eyePos, eyePos + front, upVector);
}

glm::mat4 Camera::getView() {
    return view;
}
