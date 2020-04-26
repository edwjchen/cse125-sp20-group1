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

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>


using namespace std;
namespace pt = boost::property_tree;


class chat_message
{
public:
    int x1, y1, x2, y2;

    glm::mat4 transM1, transM2;

    chat_message() : x1(5), x2(5), y1(5), y2(5)
    {
        transM1 = glm::mat4(1.0f);
        transM1[3] = glm::vec4(5,5,0,1);
        transM2 = glm::mat4(1.0f);
        transM2[3] = glm::vec4(-5,-5,0,1);

    }


    void update1(char op){
        switch (op) {
            case 'w':
                y1++;
                transM1[3][1]++;
                break;
            case 'a':
                x1--;
                transM1[3][0]--;
                break;
            case 's':
                y1--;
                transM1[3][1]--;
                break;
            case 'd':
                x1++;
                transM1[3][0]++;
                break;
        }
    }


    void update2(char op){
        switch (op) {
            case 'w':
                y2++;
                transM2[3][1]++;
                break;
            case 'a':
                x2--;
                transM2[3][0]--;
                break;
            case 's':
                y2--;
                transM2[3][1]--;
                break;
            case 'd':
                x2++;
                transM2[3][0]++;
                break;
        }
    }

    std::string data()
    {
        pt::ptree root;
        pt::ptree obj;

        pt::ptree obj1;
        pt::ptree obj2;

        pt::ptree m1;
        pt::ptree m2;

        pt::ptree matrix1[16];
        pt::ptree matrix2[16];

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

        root.add_child("Obj", obj);

        stringstream ss;
        write_json(ss, root, false);
        return ss.str();
    }
};

#endif /* chat_message_hpp */