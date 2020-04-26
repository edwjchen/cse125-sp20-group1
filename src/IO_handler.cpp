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

void IO_handler::SendInput(int direction){
    
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

void IO_handler::SendPackage(chat_client* c){
    if(ifPressed){
        pt::ptree root;
        pt::ptree cmd;
        
        pt::ptree cmd_key;
        pt::ptree cmd_mouse;
        
        cmd_key.put("key", currDir);
        //cmd_mouse.put("mouse", ...);
        
        cmd.push_back(std::make_pair("", cmd_key));
        cmd.push_back(std::make_pair("", cmd_mouse));
        
        root.add_child("cmd", cmd);
        
        stringstream ss;
        write_json(ss, root, false);
        c->write(ss.str());
        ifPressed = false;
    }
}




