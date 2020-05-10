//
//  Terrain.hpp
//  Gaia
//
//  Created by Wenlin Mao on 4/25/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#ifndef Terrain_hpp
#define Terrain_hpp

#include "core.h"
#include "TerrainMesh.hpp"
#include "TerrainBoundingBox.hpp"

typedef struct {
   float x0, x1, y0, y1, z0, z1;
} ClipVolume;

class Terrain{
public:
    Terrain();
    Terrain(int width, int depth, float step);
    ~Terrain();
    
    void update (float deltaTime);
    void draw (const glm::mat4& view, const glm::mat4& projection, GLuint shader);
    void setHeightsFromTexture(const char *file, float offset, float scale);
    void terrainBuildMesh();
    void computeBoundingBoxes(); // called once after building mesh for the first time
    float getHeightAt(float x, float z);
    
    std::vector<unsigned int>* getIndices();
    std::vector<glm::vec3>* getVertices();
    std::vector<glm::vec3>* getNormals();
    std::vector<TerrainBoundingBox>* getBoundingBoxes(); 
    
    
private:
    unsigned num_indices;

    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;
    
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    std::vector<unsigned int> indices;
    std::vector<TerrainBoundingBox> boundingBoxes; 
    
    std::vector<float> height;
    int width;
    int depth;
    float step;
    
    TerrainMesh * mesh;
    
    void prepareDraw();
    float getHeight(unsigned int w, unsigned int d);
    void setHeight(unsigned int w, unsigned int d, float h);
    glm::vec3 calculateNormal(unsigned x, unsigned z);
    void computeIndicesForClipVolume(ClipVolume *clip);
};

#endif /* Terrain_hpp */
