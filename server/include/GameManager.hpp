#ifndef GameManager_hpp
#define GameManager_hpp

#include "core.h"
#include "Terrain.hpp"
#include <ctime>
#include "Sphere.h"

using namespace std;

class GameManager{
public:
    GameManager();

    void UpdateTime();
    void UpdateScore();    

    void update1(char op);
    void update2(char op);

    void editTerrain(std::vector<glm::vec2> & editPoints, float height);
    void handle_input(string data, int id);
    void checkTerrainCollisions(Sphere* sphere);
    void checkSphereCollisions();

    string encode();
    void decode(string data, string & key_op, string & mouse_op, vector<glm::vec2> & editPoints);

    Terrain * terrain;
    string time;
    int score;

    clock_t startTime;
    clock_t endTime;
    float totalGameTime;

    Sphere* sphere1;
    Sphere* sphere2;
    
    glm::mat4 transM1, transM2; 

    bool updateTerrain;
};

#endif
