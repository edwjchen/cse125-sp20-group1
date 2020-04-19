//
//  IO_handler.cpp
//  Gaia
//
//  Created by Marino Wang on 4/18/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#include "IO_handler.hpp"
#include "my_client.cpp"

using namespace std;

IO_handler::IO_handler(int type){
    ctype = type;
}

void IO_handler::PressW(){
    // TODO:: link with server
    cout << "Press W!" << endl;
}

void IO_handler::PressA(){
    // TODO:: link with server
    cout << "Press A!" << endl;
}

void IO_handler::PressS(){
    // TODO:: link with server
    cout << "Press S!" << endl;
}

void IO_handler::PressD(){
    // TODO:: link with server
    cout << "Press D!" << endl;
}


