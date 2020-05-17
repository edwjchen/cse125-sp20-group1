#ifndef GameManager_hpp
#define GameManager_hpp

#include "core.h"
#include "chat_message.hpp"
#include "Terrain.hpp"

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

    string encode();
    void decode(string data, string & key_op, string & mouse_op, vector<glm::vec2> & editPoints);

    Terrain * terrain;
    string time;
    int score;
    int x1, y1, x2, y2;

    glm::mat4 transM1, transM2;
};



#endif 