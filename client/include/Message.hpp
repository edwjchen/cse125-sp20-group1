//
//  Message.hpp
//  Gaia
//
//  Created by peizhenwu on 2020/5/25.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#ifndef Message_hpp
#define Message_hpp

#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <string>
#include <vector>

using namespace std;

struct Message
{
public:
    string status;
    int player;
    int scoreT1;
    int scoreT2;
    string currTime;
    vector<float> height_map;
    int matrix1[16];
    int matrix2[16];
    bool updateTerrain;


    template <class Archive>
    void serialize(
            Archive& ar,
            unsigned int version
            )
    {
        ar & status;
        ar & player;
        ar & scoreT1;
        ar & scoreT2;
        ar & currTime;
        ar & matrix1;
        ar & matrix2;
        ar & updateTerrain;
        ar & height_map;
    }
};

#endif /* Message_hpp */
