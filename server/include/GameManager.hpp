#ifndef GameManager_hpp
#define GameManager_hpp

#include <boost/enable_shared_from_this.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <algorithm>
#include <iomanip>
#include <array>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/foreach.hpp>
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

#include "core.h"
#include "Terrain.hpp"
#include "Sphere.h"

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <string>
#include <vector>
#include <boost/archive/text_oarchive.hpp>

using namespace std;

class GameManager{
public:
    GameManager();

    int UpdateTime();
    void UpdateScore();  
    void updateStatus(string s); 

    void update1(char op, glm::vec3 lookat);
    void update2(char op, glm::vec3 lookat);

    void editTerrain(std::vector<glm::vec2> & editPoints, float height);
    void handle_input(string data, int id);
    void checkTerrainCollisions(Sphere* sphere);
    void checkSphereCollisions();

    string encode();
    void decode(string data, string & key_op, string & mouse_op, glm::vec3 & camLookatFront, vector<glm::vec2> & editPoints);

    Terrain * terrain;
    
    string currTime;
    int timeSignal = 0;
    int scoreT1 = -1;
    int scoreT2 = -2;

    time_t startTime;
    time_t endTime;
    float totalGameTime;

    Sphere* sphere1;
    Sphere* sphere2;
    
    glm::mat4 transM1, transM2; 

    bool updateTerrain;

    string status;
    int player;

    template <class Archive>
    void serialize(
            Archive& ar,
            unsigned int version
            )
    {
        ar & status;
        ar & player;
        ar & scoreT1;
        ar & scoreT2;
        ar & currTime;
     
        int matrix1[16];
        int matrix2[16];
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                matrix1[4*i+j] = transM1[i][j];
                matrix2[4*i+j] = transM2[i][j];
            }
        }
        ar & matrix1;
        ar & matrix2;
        ar & updateTerrain;

        vector<float> height_map;
        vector<float> height = terrain->getHeightMap();
        for(int i=0;i<height.size();i++){
            height_map.push_back(height[i]);
        }
        ar & height_map;
        // if(updateTerrain){
        //     cout << "222" << endl;
        //     updateTerrain = false;
        // }
        
    }
};

#endif 
