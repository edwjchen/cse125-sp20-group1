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
    currDir = direction;
}

void IO_handler::SendPackage(chat_client* c){
    
    if(currDir != -1){
        c->write(to_string(currDir));
        currDir = -1;
    }
}




