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


class IO_handler{

public:
    int ctype = -1;       // player type: 0 for ball, 1 for terrian
    IO_handler(int type);
    void PressW();
    void PressA();
    void PressS();
    void PressD();
};


#endif /* IO_handler_hpp */
