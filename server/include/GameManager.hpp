#ifndef GameManager_hpp
#define GameManager_hpp

#include "core.h"
#include "chat_message.hpp"
#include "Terrain.hpp"
#include "Sphere.h"

using namespace std;

class GameManager{
public:
    GameManager();
    chat_message obj;
    Terrain * terrain;

    void UpdateTime();
    void UpdateScore();    

    void KeyUpdate1(char op);
    void KeyUpdate2(char op);

    void editTerrain(std::vector<glm::vec2> editPoints, float height);
    string encode();
    void handle_input(string data, int id);
    void checkTerrainCollisions(Sphere* sphere);
    void checkSphereCollisions();

    string time;
    int score;
    
    Sphere* sphere1;
    Sphere* sphere2;
};



#endif 
