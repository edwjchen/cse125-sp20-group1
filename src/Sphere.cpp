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
    numLon = 10;
    numLat = 10;
    
    createVerts();
    createIndices();
    
    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);
    glGenBuffers(1, &EBO);
}

Sphere::Sphere(float mass, const glm::vec3& pos, const glm::vec3& vel, float r): Primitive(mass, pos, vel), radius(r){
    
    numLon = 10;
    numLat = 10;
    
    createVerts();
    createIndices();
    
    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);
    glGenBuffers(1, &EBO);
}

Sphere::~Sphere(){
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Sphere::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader){
    
    prepareDraw();
    
    // actiavte the shader program
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

    // Bind the VAO
    glBindVertexArray(VAO);
    //    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}


void Sphere::prepareDraw(){
    
    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Generate EBO, bind the EBO to the bound VAO and send the data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
    
    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Sphere::createVerts(){
    for(int i=0; i<=numLat; i++){
        
        float theta = (i * PI) / numLat;
        
        for(int j=0; j<=numLon; j++){
            float phi = (j*2*PI)/numLon;
            float x = radius * glm::sin(theta) * glm::cos(phi);
            float y = radius * glm::cos(theta);
            float z = radius * glm::sin(theta) * glm::sin(phi);
            
            positions.push_back(glm::vec3(x,y,z));
            normals.push_back(glm::normalize(glm::vec3(x,y,z)));
        }
    }
}

void Sphere::createIndices(){
    for(int i=0; i<numLat; i++){
        for(int j=0; j<numLon; j++){
            
            int first = (i*(numLon+1))+j;
            int second = first + (numLon + 1);
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first+1);
            
            indices.push_back(second);
            indices.push_back(second+1);
            indices.push_back(first+1);
        }
    }
}

void Sphere::computeAreoForce(Wind* wind){
    
    glm::vec3 v = velocity - wind->windV;
    
    float a = PI * pow(radius, 2);
    
    glm::vec3 f_areo =  wind->dragCoeff * 0.5f * wind->airDensity * (float) pow(glm::length(v), 2) * a * -glm::normalize(v);
    
    applyForce(f_areo);
}

void Sphere::reset(){
    Primitive::reset();
    
    positions.clear();
    normals.clear();
    indices.clear();
}

void Sphere::setRadius(float r){
    radius = r;
    
    positions.clear();
    normals.clear();
    indices.clear();
    
    createVerts();
    createIndices();
}
