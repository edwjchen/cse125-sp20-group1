#ifndef GameManager_hpp
#define GameManager_hpp

#include "core.h"
#include "chat_message.hpp"
#include "Terrain.hpp"
#include <ctime>

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

    string time;
    int score;

    clock_t startTime;
    clock_t endTime;
    float totalGameTime;
};



#endif 