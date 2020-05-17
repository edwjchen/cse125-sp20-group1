#include "GameManager.hpp"

using namespace std;

GameManager::GameManager(){
    time = "";
    score = -1;
    startTime = clock();
    totalGameTime = 300.0f;
    //terrain = new Terrain(251, 251, 0.5f);
}

void GameManager::UpdateScore(){
    //obj.score++;
    // Need to determine which team to add score
}

void GameManager::UpdateTime(){
    string finishedTime = "";
    endTime = clock();
    float duration = totalGameTime - (float)(endTime-startTime) / CLOCKS_PER_SEC;
    finishedTime = finishedTime + to_string((int)duration/60) + ":" + to_string((int)duration%60);
    if(duration <= 0){
        // Send a signal to announce game ends
        duration = 0;
        finishedTime = "0:00";
    }
   //obj.time = to_string(duration);
   obj.time = finishedTime;
}

void GameManager::KeyUpdate1(char op){
    obj.update1(op);
}

void GameManager::KeyUpdate2(char op){
    obj.update2(op);
}

void GameManager::editTerrain(std::vector<glm::vec2> editPoints, float height){
    //terrain->edit(editPoints, height);
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
}

