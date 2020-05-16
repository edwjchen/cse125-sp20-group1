//
//  AudioManager.hpp
//  Gaia
//
//  Created by Marino Wang on 5/15/20.
//  Copyright © 2020 SphereEnix. All rights reserved.
//

#ifndef AudioManager_hpp
#define AudioManager_hpp

#include "core.h"



class AudioManager{
public:
    static const std::string BACKGROUND_MUSIC1;
    static const std::string SF1POINTS;
    static const std::string SF2HIT;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffer;

    
    AudioManager();
    void PlaySounds(int index);
    void LoadSounds();

private:
    sf::Music music;
    std::vector<sf::Sound> soundList;
    float maxMusicVolume, maxSFVolume;
    bool muted;
    
};

#endif /* AudioManager_hpp */
