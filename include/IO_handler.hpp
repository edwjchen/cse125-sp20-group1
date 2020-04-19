//
//  IO_handler.hpp
//  Gaia
//
//  Created by Marino Wang on 4/18/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#ifndef IO_handler_hpp
#define IO_handler_hpp

#include <stdio.h>
#include <iostream>
#include "my_client.hpp"
#include <string>


class IO_handler{

private:
    int ctype = -1;
    int currDir = -1;
    
public:
    
    IO_handler(int type);
    
    void SendInput(int direction);
    void SendPackage(chat_client* c);
    void PressW();
    void PressA();
    void PressS();
    void PressD(chat_client* c);
    
};


#endif /* IO_handler_hpp */
