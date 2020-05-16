#include "GameManager.hpp"


GameManager::GameManager(){
    time = "";
    score = -1;
}

void GameManager::UpdateScore(){
    obj.score++;
}

void GameManager::UpdateTime(){
    obj.time = "5:00";
}

void GameManager::KeyUpdate1(char op){
    obj.update1(op);
}

void GameManager::KeyUpdate2(char op){
    obj.update2(op);
}

void GameManager::editTerrain(std::vector<glm::vec2> editPoints, float height){
    obj.editTerrain(editPoints, height);
}


