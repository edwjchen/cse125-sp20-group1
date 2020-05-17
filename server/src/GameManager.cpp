#include "GameManager.hpp"

using namespace std;
namespace pt = boost::property_tree;

GameManager::GameManager(): x1(5), x2(5), y1(5), y2(5){
    time = "";
    score = -1;
    terrain = new Terrain(251, 251, 0.5f);
    string time = "";
    int score = -1;

    transM1 = glm::mat4(1.0f);
    transM1[3] = glm::vec4(64,-10,-65,1);
    transM2 = glm::mat4(1.0f);
    transM2[3] = glm::vec4(58,0,-54,1);
}

void GameManager::UpdateScore(){
    score++;
}

void GameManager::UpdateTime(){
    time = "5:00";
}

void GameManager::update1(char op){
    switch (op) {
        case 'w':
            y1++;
            transM1[3][2]--;
            break;
        case 'a':
            x1--;
            transM1[3][0]--;
            break;
        case 's':
            y1--;
            transM1[3][2]++;
            break;
        case 'd':
            x1++;
            transM1[3][0]++;
            break;
    }
}

void GameManager::update2(char op){
    switch (op) {
        case 'w':
            y2++;
            transM2[3][2]--;
            break;
        case 'a':
            x2--;
            transM2[3][0]--;
            break;
        case 's':
            y2--;
            transM2[3][2]++;
            break;
        case 'd':
            x2++;
            transM2[3][0]++;
            break;
    }
}

void GameManager::editTerrain(std::vector<glm::vec2> & editPoints, float height){
    //cout << "editing terrain..." << endl;
    //std::cout << editPoints[0][0] << ", " << editPoints[0][1] << " & " << editPoints[1][0] << ", " << editPoints[1][1] << std::endl;
    glm::vec2 sT = glm::vec2(editPoints[0][0] * 2, editPoints[0][1] * -2);
    glm::vec2 eT = glm::vec2(editPoints[1][0] * 2, editPoints[1][1] * -2);
    std::vector<glm::vec2> temp = {sT, eT};
    terrain->edit(temp, height);
}

void GameManager::handle_input(string data, int id){
    
    std::string key_op = "";
    std::string mouse_op = "";
    
    std::vector<glm::vec2> editPoints;
    float height = 10;
    decode(data, key_op, mouse_op, editPoints);
    
    if(key_op != ""){
        cout << "id: " << id << ", operation: "<< key_op << endl;
        if(id == 1){
            update1(key_op.at(0));
        }else if(id == 2){
            update2(key_op.at(0));
        }
    }
    if(!editPoints.empty()){
        editTerrain(editPoints, height);
    }
}


string GameManager::encode()
{
    pt::ptree root;
    pt::ptree obj;

    pt::ptree obj1;
    pt::ptree obj2;

    pt::ptree m1;
    pt::ptree m2;

    pt::ptree matrix1[16];
    pt::ptree matrix2[16];

    pt::ptree height_root;

    pt::ptree timeNode;
    pt::ptree scoreNode;

    obj1.put("id", 1);
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
        matrix1[4*i+j].put("", transM1[i][j]);
        }
    }
    for(int i=0;i<16;i++){
        m1.push_back(std::make_pair("", matrix1[i]));
    }
    obj1.add_child("transformation", m1);

    obj2.put("id", 2);
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
        matrix2[4*i+j].put("", transM2[i][j]);
        }
    }
    for(int i=0;i<16;i++){
        m2.push_back(std::make_pair("", matrix2[i]));
    }
    obj2.add_child("transformation", m2);


    obj.push_back(std::make_pair("", obj1));
    obj.push_back(std::make_pair("", obj2));

    vector <float> height_map = terrain->getHeightMap();
    // build and add current height map node to root
    for(int i = 0; i < height_map.size(); i++){
        pt::ptree node;
        node.put("", height_map[i]);
        height_root.push_back(std::make_pair("", node));
    }

    scoreNode.put("", score);
    timeNode.put("", time);

    root.add_child("Obj", obj);

    root.add_child("height_map" ,height_root);

    root.add_child("Score", scoreNode);

    root.add_child("Time", timeNode);

    stringstream ss;
    write_json(ss, root, false);
    return ss.str() + '\n';
}

void GameManager::decode(string data, string & key_op, string & mouse_op, vector<glm::vec2> & editPoints)
{
    //cout << data << endl;
    float temp[4];
    //std::cout << data << std::endl;
    // Read JSON from client
    try{
        if(data != ""){
            stringstream ss;
            ss << data;

            pt::ptree tar;
            pt::read_json(ss, tar);

            int i = 0;
            BOOST_FOREACH(const pt::ptree::value_type& child, tar.get_child("cmd")) {
                if(i == 0){
                    key_op = child.second.get<std::string>("key");
                }
                else{
                    // Mouse
                    mouse_op = child.second.get<std::string>("mouse");

                    if(mouse_op.compare("l") == 0){
                        int index = 0;
                        BOOST_FOREACH(const pt::ptree::value_type& t, child.second.get_child("mouse_l")){
                            temp[index] = stof(t.second.data());
                            index++;
//                                    if(temp[index]!= 0){
//                                        cout << temp[index] << " ";
//                                    }
                        }
                    } else if(mouse_op.compare("r") == 0){
                        int index = 0;
                        BOOST_FOREACH(const pt::ptree::value_type& t, child.second.get_child("mouse_r")){
                            temp[index] = stof(t.second.data());
                            index++;
                        }
                    }
                }
                i++;
            }
        }
    }catch(...){
        std::cout << "decode exception" << std::endl;
    }
    if(mouse_op != ""){
        editPoints.push_back(glm::vec2(temp[0],temp[1]));
        editPoints.push_back(glm::vec2(temp[2],temp[3]));
    }
}

