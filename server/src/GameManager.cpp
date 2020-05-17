#include "GameManager.hpp"

using namespace std;
namespace pt = boost::property_tree;

GameManager::GameManager(): updateTerrain(false){
    time = "";
    score = -1;
    startTime = clock();
    totalGameTime = 300.0f;
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
   time = finishedTime;
}

void GameManager::update1(char op){
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

void GameManager::update2(char op){
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
        updateTerrain = true;
    }
    // hardcode to add gravity for now
    sphere1->move(sphere1->getCenter() + glm::vec3(0.0f, -0.1f, 0.0f));
    sphere2->move(sphere2->getCenter() + glm::vec3(0.0f, -0.1f, 0.0f));
    checkTerrainCollisions(sphere1);
    checkTerrainCollisions(sphere2);
    checkSphereCollisions();
}


string GameManager::encode()
{
    transM1 = sphere1->getModel();
    transM2 = sphere2->getModel();
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

    if(updateTerrain){
        vector <float> height_map = terrain->getHeightMap();
        // build and add current height map node to root
        for(int i = 0; i < height_map.size(); i++){
            pt::ptree node;
            node.put("", height_map[i]);
            height_root.push_back(std::make_pair("", node));
        }
        updateTerrain = false;
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

void GameManager::checkSphereCollisions() {
    glm::vec3 sphere1Pos = sphere1->getCenter();
    glm::vec3 sphere2Pos = sphere2->getCenter();
    float sphere1Radius = sphere1->getRadius();
    float sphere2Radius = sphere2->getRadius();
    float delta = glm::length(sphere1Pos - sphere2Pos);
    if (delta < sphere1Radius + sphere2Radius) {
        glm::vec3 dir = glm::normalize(sphere1Pos - sphere2Pos);
        sphere1->move(sphere1Pos + delta / 2.0f * dir);
        sphere2->move(sphere1Pos - delta / 2.0f * dir);
    }
    // TODO: further physics effect
}

