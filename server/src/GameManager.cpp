#include "GameManager.hpp"

using namespace std;

GameManager::GameManager(){
    time = "";
    score = -1;
    terrain = new Terrain(251, 251, 0.5f);
    std::vector<glm::vec2> tmp = {
        glm::vec2(1.0f, 1.0f),
        glm::vec2(125.0f, 125.0f),
        glm::vec2(135.0f, 125.0f),
        glm::vec2(250.0f, 250.0f)
    };
    terrain->edit(tmp, 10);
    terrain->terrainBuildMesh();
    terrain->computeBoundingBoxes();
    
    sphere1 = new Sphere(5.0f, 2.0f);
    sphere1->move(glm::vec3(64,-10,-65));
    sphere2 = new Sphere(5.0f, 2.0f);
    sphere1->move(glm::vec3(58,0,-54));
}

void GameManager::UpdateScore(){
    obj.score++;
}

void GameManager::UpdateTime(){
    obj.time = "5:00";
}

void GameManager::KeyUpdate1(char op){
    glm::vec3 newPos;
    switch (op) {
        case 'w':
            newPos = sphere1->getCenter();
            newPos.z -= 1;
            sphere1->move(newPos);
            break;
        case 'a':
            newPos = sphere1->getCenter();
            newPos.x -= 1;
            sphere1->move(newPos);
            break;
        case 's':
            newPos = sphere1->getCenter();
            newPos.z += 1;
            sphere1->move(newPos);
            break;
        case 'd':
            newPos = sphere1->getCenter();
            newPos.x += 1;
            sphere1->move(newPos);
            break;
    }
}

void GameManager::KeyUpdate2(char op){
    glm::vec3 newPos;
    switch (op) {
        case 'w':
            newPos = sphere2->getCenter();
            newPos.z -= 1;
            sphere2->move(newPos);
            break;
        case 'a':
            newPos = sphere2->getCenter();
            newPos.x -= 1;
            sphere2->move(newPos);
            break;
        case 's':
            newPos = sphere2->getCenter();
            newPos.z += 1;
            sphere2->move(newPos);
            break;
        case 'd':
            newPos = sphere2->getCenter();
            newPos.x += 1;
            sphere2->move(newPos);
            break;
    }
}

void GameManager::editTerrain(std::vector<glm::vec2> editPoints, float height){
    // terrain->edit(editPoints, height);
}

string GameManager::encode(){
    return obj.encode()+'\n';
}

void GameManager::handle_input(string data, int id){
    std::string key_op = "";
    std::string mouse_op = "";
    
    std::vector<glm::vec2> editPoints;
    float height = 10;
    obj.decode(data, key_op, mouse_op, editPoints);
    
    if(key_op != ""){
        cout << "id: " << id << ", operation: "<< key_op << endl;
        if(id == 1){
            KeyUpdate1(key_op.at(0));
        }else if(id == 2){
            KeyUpdate2(key_op.at(0));
        }
    }
    editTerrain(editPoints, height);
    checkTerrainCollisions(sphere1);
    checkTerrainCollisions(sphere2);
}

void GameManager::checkTerrainCollisions(Sphere* sphere) {
    
    std::vector<unsigned int>* indices = terrain->getIndices();
    std::vector<glm::vec3>* vertices = terrain->getVertices();
    std::vector<TerrainBoundingBox>* boxes = terrain->getBoundingBoxes();
    
    // resolve force
    // sphere->move(sphere->getCenter() + sphere->force);
    // sphere->force = glm::vec3(0);
    
    for (int k = 0; k < 20; k++) {
        for (int j = 0; j < boxes->size(); j++) {
            TerrainBoundingBox& box = (*boxes)[j];
            glm::vec2& tminPoint = box.minPoint;
            glm::vec2& tmaxPoint = box.maxPoint;
            glm::vec2 sminPoint(sphere->getCenter().x, sphere->getCenter().z);
            sminPoint += glm::vec2(-sphere->getRadius(), -sphere->getRadius());
            glm::vec2 smaxPoint(sphere->getCenter().x, sphere->getCenter().z);
            smaxPoint += glm::vec2(sphere->getRadius(), sphere->getRadius());
            
            if (sminPoint.x > tmaxPoint.x || tminPoint.x > smaxPoint.x || sminPoint.y > tmaxPoint.y || tminPoint.y > smaxPoint.y) { // not in box
                continue;
            }
            
            for (int i = 0; i < box.indices2triangles.size(); i++) {
                int curInd = box.indices2triangles[i];
                glm::vec3& a = (*vertices)[(*indices)[curInd-2]];
                glm::vec3& b = (*vertices)[(*indices)[curInd-1]];
                glm::vec3& c = (*vertices)[(*indices)[curInd]];
                glm::vec3 n = -glm::normalize(glm::cross(c-a, b-a));
                if (glm::dot(n, glm::vec3(0, 1, 0)) < 0) { // little hack to make sure normals are upwards
                    n = -n;
                }
                
                glm::vec3 offset = sphere->checkCollision(a, b, c, n);
                if (glm::length(offset) < 0.0001f) { // clamp to avoid bouncing too many times
                    offset = glm::vec3(0);
                    continue;
                }
                sphere->move(sphere->getCenter() + offset); // move to right position
            }
        }
    }
    
    // if sphere has fallen off, freaking lift it up
    float height = terrain->getHeightAt(sphere->getCenter().x, sphere->getCenter().z);
    if (height > sphere->getCenter().y + sphere->getRadius()) {
        glm::vec3 offset(0);
        offset.y = height - (sphere->getCenter().y - sphere->getRadius());
        sphere->move(sphere->getCenter() + offset);
    }
}
