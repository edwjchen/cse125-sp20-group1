//
//  Terrain.cpp
//  Gaia
//
//  Created by Wenlin Mao on 4/25/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#include "Terrain.hpp"

Terrain::Terrain(){
}

Terrain::Terrain(int width, int depth, float step) : width(width), depth(depth), step(step){
    
    height.resize(width * depth, 0);
    
    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);
}

Terrain::~Terrain(){
    delete mesh;
}

float Terrain::getHeight(unsigned int w, unsigned int d)
{
    return height[(w) * depth + (d)];
}

void Terrain::setHeight(unsigned int w, unsigned int d, float h)
{
    height[(w) * depth + (d)] = h;
}

float Terrain::getHeightAt(float x, float z)
{
   /* Terrain's Z extends towards -Z, but our vertices need positive numbers */
   z = -z;

   /* Find offsets of the coords into a terrain quad */
   float offx = fmodf(x, step);
   float offz = fmodf(z, step);

   /* Compute the plane equation for the triangle we are in */
   glm::vec3 p1, p2, p3;
   float A, B, C, D;
   if (offx + offz <= step) {
      /* First triangle in the quad */
      p1.x = trunc(x / step);
      p1.z = trunc(z / step);
      p1.y = getHeight((int)p1.x, (int)p1.z);

      p2.x = trunc(x / step) + 1;
      p2.z = trunc(z / step);
      p2.y = getHeight((int)p2.x, (int)p2.z);

      p3.x = trunc(x / step);
      p3.z = trunc(z / step) + 1;
      p3.y = getHeight((int)p3.x, (int)p3.z);
   } else {
      /* Second triangle in the quad */
      p1.x = trunc(x / step) + 1;
      p1.z = trunc(z / step);
      p1.y = getHeight((int)p1.x, (int)p1.z);

      p2.x = trunc(x / step);
      p2.z = trunc(z / step) + 1;
      p2.y = getHeight((int)p2.x, (int)p2.z);

      p3.x = trunc(x / step) + 1;
      p3.z = trunc(z / step) + 1;
      p3.y = getHeight((int)p3.x, (int)p3.z);
   }

   /* Above we compute X,Z coords as vertex indices so we could use TERRAIN()
    * to compute heights at specific vertices, but to apply the plane equation
    * we need to turn the coordinates into world units
    */
   p1.x *= step;
   p1.z *= step;
   p2.x *= step;
   p2.z *= step;
   p3.x *= step;
   p3.z *= step;

   /* FIXME: we probably want to pre-compute plane equations for each
    * triangle in the terrain rather than recomputing them all the time
    */
   A = (p2.y - p1.y) * (p3.z - p1.z) - (p3.y - p1.y) * (p2.z - p1.z);
   B = (p2.z - p1.z) * (p3.x - p1.x) - (p3.z - p1.z) * (p2.x - p1.x);
   C = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);
   D = -(A * p1.x + B * p1.y + C * p1.z);

   /* Use the plane equation to find Y given (X,Z) */
   return (-D - C * z - A * x) / B;
}


glm::vec3 Terrain::calculateNormal(unsigned x, unsigned z)
{
   if (x == 0)
      x = 1;
   if (z == 0)
      z = 1;
   float hl = getHeight(x - 1, z);
   float hr = getHeight(x + 1, z);
   float hd = getHeight(x, z + 1); /* Terrain expands towards -Z */
   float hu = getHeight(x, z - 1);
   glm::vec3 n = glm::vec3(hl - hr, 2.0f, hd - hu);
   n = glm::normalize(n);
   return n;
}

void Terrain::terrainBuildMesh()
{
    /* GL's +Z axis goes towards the camera, so make the terrain's Z coordinates
    * negative so that larger (negative) Z coordinates are more distant.
    */
    mesh = new TerrainMesh();

    int vertices_w = width;
    int vertices_d = depth;

    /* Add each vertex in the grid. We are going to render using an index buffer
    * so we don't need to duplicate vertices, which reduces massively the
    * storage requirements for the vertex data, since terrains have high
    * vertex counts.
    */
    for (int vx = 0; vx < vertices_w; vx++) {
        for (int vz = 0; vz < vertices_d; vz++) {
             float vy = getHeight(vx, vz);
             glm::vec3 v0 = glm::vec3(vx * step, vy, -vz * step);
             glm::vec3 n0 = calculateNormal(vx, vz);
             mesh->addVertex(v0.x, v0.y, v0.z, n0.x, n0.y, n0.z);
        }
    }

    /* Build the indices to render the terrain using a single triangle strip
    * (using degenerate triangles) since that yields much better performance
    * than rendering triangles.
    */
    unsigned numIdx =
      (vertices_w - 1) * (vertices_d * 2) + (vertices_w - 2) + (vertices_d - 2);
    indices.resize(numIdx, 0);

    /* Initialize the number of rendering indices so it covers the entire
    * terrain.
    */
    ClipVolume clip;
    clip.x0 = 0.0f;
    clip.x1 = (width - 1) * step;
    clip.z1 = 0.0f;
    clip.z0 = -(depth - 1) * step;
    computeIndicesForClipVolume(&clip);
    
    assert(numIdx = num_indices);
}

void Terrain::computeIndicesForClipVolume(ClipVolume *clip)
{
    int min_col = std::max(std::min((clip->x0 / step), width - 2.0f), 0.0f);
    int max_col = std::max(std::min((clip->x1 + step) / step, width - 2.0f), 0.0f);

   /* Z1 is always largest, because our terrain grows towrds -Z, it means that
    * it defines the std::min_row
    */
    int min_row = std::max(std::min(-clip->z1 / step, depth - 1.0f), 0.0f);
    int max_row = std::max(std::min(- (clip->z0 - step) / step, depth - 1.0f), 0.0f);

   /* If this happens then the terrain is not visible */
   if (min_col == max_col || min_row == max_row) {
      num_indices = 0;
      return;
   }

   unsigned index = 0;
   for (int c = min_col; c <= max_col; c++) {
      for (int r = min_row; r <= max_row; r++) {
         /* If this is not the first strip then we need to produce a degenerate
          * link with the previous strip using the first vertex from this strip
          * and the last vertex from the last before we start recording the
          * new strip. Here is the first vertex of this strip.
          */
         if (c > min_col && r == min_row)
            indices[index++] = c * depth + r;

         indices[index++] = c * depth + r;
         indices[index++] = (c + 1) * depth + r;

         /* Link the next triangle strip using degenerate triangles. For that
          * we need to duplicate the last vertex of this strip and the first
          * vertex of the next.
          */
         if (r == max_row && c < max_col)
            indices[index++] = (c + 1) * depth + r;
      }
   }

   num_indices = index;
}

void Terrain::prepareDraw(){
    
    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->vertices.size(), mesh->vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* mesh->normals.size(), mesh->normals.data(), GL_STATIC_DRAW);
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

static void terrainUnbind()
{
   // Unbind the VAO and shader program
   glBindVertexArray(0);
   glUseProgram(0);
}

void Terrain::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader){
    
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
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLE_STRIP, num_indices, GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    terrainUnbind();
}

void Terrain::setHeightsFromTexture(const char *file, float offset, float scale)
{
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        std::cout << "Could not initialize STL Image" << SDL_GetError() <<std::endl;
        return;
    }

    SDL_Surface *image = IMG_Load(file);
    
    if (!image) {
      std::cout << "Texture Manager: Failed to load file, " << file << std::endl;
      return;
    }
    
    uint8_t *pixels = (uint8_t *) image->pixels;
    float scale_x = ((float) image->w) / (width - 1);
    float scale_z = ((float) image->h) / (depth - 1);

    for (int x = 0; x < width; x++) {
        for (int z = 0; z < depth; z++) {
            int img_x = (int) truncf(x * scale_x);
            int img_y = (int) truncf(z * scale_z);
            float h = pixels[img_y * image->pitch + img_x * 4];

            /* Normalize height to [-1, 1] */
            h = h / 127.5 - 1.0f;

            /* Apply scale */
            h *= scale;

            /* Apply height offset */
            h += offset;

            setHeight(x, z, h);
        }
    }
}
