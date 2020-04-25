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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

using namespace std;
namespace pt = boost::property_tree;

class chat_message
{
public:
    int x1, y1, x2, y2;

    chat_message() : x1(5), x2(5), y1(5), y2(5)
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
        pt::ptree root;
        pt::ptree location;

        pt::ptree obj1;
        pt::ptree obj2;

        obj1.put("x", x1);
        obj1.put("y", y1);
        obj2.put("x", x2);
        obj2.put("y", y2);

        location.push_back(std::make_pair("", obj1));
        location.push_back(std::make_pair("", obj2));
        root.add_child("location", location);

        stringstream ss;
        write_json(ss, root, false);
        return ss.str();
        //cout << ss.str() << endl; // ss.str will be the new string to return

        //return to_string(x1) + "," + to_string(y1) + "," + to_string(x2) + "," + to_string(y2);
    }
};

#endif /* chat_message_hpp */



