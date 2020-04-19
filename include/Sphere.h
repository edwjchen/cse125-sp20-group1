//
//  Sphere.h
//  Gaia
//
//  Created by Wenlin Mao on 4/18/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#ifndef Sphere_h
#define Sphere_h


#include "Primitive.h"

using namespace std;

class Sphere : public Primitive {
public:
    Sphere();
    Sphere(float m, float r);
    Sphere(float mass, const glm::vec3& pos, const glm::vec3& vel, float r);
    ~Sphere();
    
    glm::vec3 computeNormal();
    glm::vec3 computeSurfaceV();
    void computeAreoForce(Wind* wind);
    
    glm::vec3 getCenter() { return position; }
    
    void setRadius(float r);
    
    void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
    void reset();
    
    void prepareDraw();
    void createVerts();
    void createIndices();
    
    //glm::vec3 getNormal() { return normal; }
    
private:
    float radius;
    int numLon;
    int numLat;
    
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;
    
    vector<glm::vec3> positions;
    vector<glm::vec3> normals;
    vector<unsigned int> indices;
    
    const float PI = glm::pi<float>();
};

#endif /* Sphere_h */
