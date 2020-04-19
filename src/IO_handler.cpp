//
//  IO_handler.cpp
//  Gaia
//
//  Created by Marino Wang on 4/18/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#include "IO_handler.hpp"

using namespace std;

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
        c->write(currDir);
        ifPressed = false;
    }
}




