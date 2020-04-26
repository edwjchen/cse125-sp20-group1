//
//  chat_message.hpp
//  game-server
//
//  Created by peizhenwu on 2020/4/18.
//  Copyright © 2020 peizhenwu. All rights reserved.
//

#ifndef chat_message_hpp
#define chat_message_hpp

#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

class chat_message
{
public:
    int x1, y1, x2, y2;
    
    chat_message() : x1(-10), x2(10), y1(0), y2(0)
    {
    }
    
    void update1(char op){
        switch (op) {
            case 'w':
                y1++;
                break;
            case 'a':
                x1--;
                break;
            case 's':
                y1--;
                break;
            case 'd':
                x1++;
                break;
        }
    }
    
    void update2(char op){
        switch (op) {
            case 'w':
                y2++;
                break;
            case 'a':
                x2--;
                break;
            case 's':
                y2--;
                break;
            case 'd':
                x2++;
                break;
        }
    }

    std::string data()
    {
        return to_string(x1) + "," + to_string(y1) + "," + to_string(x2) + "," + to_string(y2);
    }
};

#endif /* chat_message_hpp */



