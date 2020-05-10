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
    void edit(std::vector<glm::vec2> editPoints, float height);
    
    SDL_Surface * surface;
    
    TerrainMesh * mesh;
    
private:
    unsigned num_indices;

    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;
    
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    std::vector<unsigned int> indices;
    
    std::vector<float> height;
    int width;
    int depth;
    float step;

    
    void prepareDraw();
    float getHeight(unsigned int w, unsigned int d);
    void setHeight(unsigned int w, unsigned int d, float h);
    float getHeightAt(float x, float z);
    glm::vec3 calculateNormal(unsigned x, unsigned z);
    void computeIndicesForClipVolume(ClipVolume *clip);
    
    void setHeightsFromSurface(float offset, float scale);
    void drawLineOnSurface(glm::vec2 start, glm::vec2 end, int color);
    void putpixel(int x, int y, int color);
    
};

#endif /* Terrain_hpp */
