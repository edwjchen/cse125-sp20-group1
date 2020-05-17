//
//  Sphere.cpp
//  Gaia
//
//  Created by Wenlin Mao on 4/18/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#include "Sphere.h"

Sphere::Sphere(){
}

Sphere::Sphere(float m, float r): Primitive(m), radius(r){
    force = glm::vec3(0);
}

Sphere::Sphere(float mass, const glm::vec3& pos, const glm::vec3& vel, float r): Primitive(mass, pos, vel), radius(r){
    force = glm::vec3(0);
}

Sphere::~Sphere(){
}

void Sphere::computeAreoForce(Wind* wind){
    
    glm::vec3 v = velocity - wind->windV;
    
    float a = PI * pow(radius, 2);
    
    glm::vec3 f_areo =  wind->dragCoeff * 0.5f * wind->airDensity * (float) pow(glm::length(v), 2) * a * -glm::normalize(v);
    
    applyForce(f_areo);
}

void Sphere::setRadius(float r){
    radius = r;
}

/*
 * a, b, c forms a triangle and n is normal. Return a vector to translate the sphere to just
 * resolve intersection. Return vec3(0) if no collision.
 */
glm::vec3 Sphere::checkCollision(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 n) {
    position = (glm::vec3)model[3]; 
    if (a == b || a == c || b == c) {
        return glm::vec3(0);
    }
    
    float t = -(glm::dot(a, n) - glm::dot(position, n));
    if (t < -(radius + 0.0001f) || t > (radius + 0.0001f)) {
        return glm::vec3(0);
    }
    
    glm::vec3 P = position + t * (-n);
    
    glm::vec3 e1 = b - a;
    glm::vec3 e2 = c - b;
    glm::vec3 e3 = a - c;
    glm::vec3 A = P - a;
    glm::vec3 B = P - b;
    glm::vec3 C = P - c;
    
    //if P is inside triangle
    if (glm::dot(n, glm::cross(e1, A)) >= 0 &&
        glm::dot(n, glm::cross(e2, B)) >= 0 &&
        glm::dot(n, glm::cross(e3, C)) >= 0) {
        if (t > 0) {
            return P - (position + radius * glm::normalize(P - position));
        } else {
            return P - (position + radius * glm::normalize(position - P));
        }
    } else {
        return glm::vec3(0);
    }
}

// translate to pos
void Sphere::move(const glm::vec3& pos){
    float len = glm::length(pos - position);
    if (len > 0.0001f) {
        glm::vec3 dir = glm::normalize(pos - position);
        if (glm::length(glm::vec3(dir.x, 0, dir.z)) > 0.0001f) {
            glm::vec3 up = glm::vec3(0, 1, 0);
            glm::vec3 right = glm::cross(dir, up);
            float angle = - len / radius;
            glm::mat4 coef = glm::rotate(glm::mat4(1), angle, right);
            model = coef * model;
        }
    }
    model[3] = glm::vec4(pos, 1);
    position = pos;
}
