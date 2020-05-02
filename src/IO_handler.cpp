//
//  IO_handler.cpp
//  Gaia
//
//  Created by Marino Wang on 4/18/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#include "IO_handler.hpp"

using namespace std;
namespace pt = boost::property_tree;

IO_handler::IO_handler(int type){
    ctype = type;
}

void IO_handler::SendKeyBoardInput(int direction){
    
    switch(direction){
        case 0:
            currDir = "w";
            break;
        case 1:
            currDir = "a";
            break;
        case 2:
            currDir = "s";
            break;
        case 3:
            currDir = "d";
            break;
        
    }
    ifPressed = true;
}

void IO_handler::SendMouseInput(int leftOrRight, glm::vec2 start, glm::vec2 end){
    if(leftOrRight == 0){
        cerr << "WHY YOU ARE HERE???" << endl;
        return;
    }
    else if(leftOrRight == 1){
        currBut = "l";
        startPos = start;
    }
    else if(leftOrRight == 2){
        currBut = "r";
        endPos = end;
    }
    else{
        cerr << "???????" << endl;
        return;
    }
    ifClicked = true;
}

void IO_handler::SendPackage(chat_client* c){
    pt::ptree root;
    pt::ptree cmd;
    
    pt::ptree cmd_key;
    pt::ptree cmd_mouse;

    if(ifPressed){
        cmd_key.put("key", currDir);
        ifPressed = false;
    }
    if(ifClicked){
        cmd_mouse.put("mouse", currBut);
        ifClicked = false;
    }
    cmd.push_back(std::make_pair("", cmd_key));
    cmd.push_back(std::make_pair("", cmd_mouse));
    
    root.add_child("cmd", cmd);
    
    stringstream ss;
    write_json(ss, root, false);
    c->write(ss.str());
}




