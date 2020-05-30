//
//  Skybox.cpp
//  Gaia
//
//  Created by Wenlin Mao on 5/3/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#include "Skybox.hpp"

// The 8 vertices of a cube.
std::vector<glm::vec3> vertices
{
    glm::vec3(-1, 1, 1),
    glm::vec3(-1, -1, 1),
    glm::vec3(1, -1, 1),
    glm::vec3(1, 1, 1),
    glm::vec3(-1, 1, -1),
    glm::vec3(-1, -1, -1),
    glm::vec3(1, -1, -1),
    glm::vec3(1, 1, -1)
};

// Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2
// and v3 in counter-clockwise order.
std::vector<glm::ivec3> indices
{
    // Front face.
    glm::ivec3(0, 1, 2),
    glm::ivec3(2, 3, 0),
    // Back face.
    glm::ivec3(7, 6, 5),
    glm::ivec3(5, 4, 7),
    // Right face.
    glm::ivec3(3, 2, 6),
    glm::ivec3(6, 7, 3),
    // Left face.
    glm::ivec3(4, 5, 1),
    glm::ivec3(1, 0, 4),
    // Top face.
    glm::ivec3(4, 0, 3),
    glm::ivec3(3, 7, 4),
    // Bottom face.
    glm::ivec3(1, 5, 6),
    glm::ivec3(6, 2, 1),
};


static unsigned int loadCubemap(const std::vector<std::string>& faces)
{
    
    SDL_Surface *surface = nullptr;
    GLenum texture_format;
    GLint  nOfColors;
    
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        std::cout << "Could not initialize STL Image" << SDL_GetError() <<std::endl;
        return textureID;
    }
    
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        surface = IMG_Load(faces[i].c_str());
        if (surface) {

//            // Check that the image's width is a power of 2
//            if ( (surface->w & (surface->w - 1)) != 0 ) {
//                printf("warning: bag.png's width is not a power of 2\n");
//            }
//
//            // Also check if the height is a power of 2
//            if ( (surface->h & (surface->h - 1)) != 0 ) {
//                printf("warning: bag.png's height is not a power of 2\n");
//            }
//
//            // get the number of channels in the SDL surface
//            nOfColors = surface->format->BytesPerPixel;
//            if (nOfColors == 4)     // contains an alpha channel
//            {
//                if (surface->format->Rmask == 0x000000ff)
//                    texture_format = GL_RGBA;
//                else
//                    texture_format = GL_BGRA;
//            } else if (nOfColors == 3)     // no alpha channel
//            {
//                if (surface->format->Rmask == 0x000000ff)
//                    texture_format = GL_RGB;
//                else
//                    texture_format = GL_BGR;
//            } else {
//                printf("warning: the image is not truecolor..  this will probably break\n");
//                // this error should not go unhandled
//            }
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
            
            
        } else {
            printf("SDL could not load bag.png: %s\n", SDL_GetError());
            SDL_Quit();
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    if ( surface ) {
        SDL_FreeSurface( surface );
    }

    return textureID;
}

Skybox::Skybox(const std::vector<std::string>& faces){
    
    cubemapTexture = loadCubemap(faces);
    
    // Generate a vertex array (VAO) and vertex buffer object (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind VBO to the bound VAO, and store the vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    // Enable Vertex Attribute 0 to pass the vertex data through to the shader
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Generate EBO, bind the EBO to the bound VAO, and send the index data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBO/VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

Skybox::~Skybox()
{
    // Delete the VBO/EBO and the VAO.
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Skybox::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
    glm::mat4 model = glm::scale(glm::vec3(500 / 2.f));
    
    // Actiavte the shader program
    glUseProgram(shader);
    
    glDepthMask(GL_FALSE);
    
    glm::mat4 view_tmp = glm::mat4(glm::mat3(view));
    
    // Get the shader variable locations and send the uniform data to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view_tmp));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
    
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
//    glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

    // Bind the VAO
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    
    // Draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
   
    glDepthMask(GL_TRUE);
    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

