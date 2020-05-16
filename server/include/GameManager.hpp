#ifndef GameManager_hpp
#define GameManager_hpp

#include "core.h"
#include "chat_message.hpp"
#include "Terrain.hpp"


class GameManager{
public:
    GameManager();
    chat_message obj;

    void UpdateTime();
    void UpdateScore();    

    void KeyUpdate1(char op);
    void KeyUpdate2(char op);

    void editTerrain(std::vector<glm::vec2> editPoints, float height);

    string time;
    int score;

};



#endif 