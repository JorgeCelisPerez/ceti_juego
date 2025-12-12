#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <SFML/Audio.hpp>
#include <map>
#include <string>

class SoundManager {
public:
    SoundManager();
    bool init();  // Cargar todos los sonidos
    
    void playItemSound();
    void playCountdownSound();
    void playCrashSound();
    void playGameOverSound();
    void setVolume(float volume);  // 0.0f a 100.0f
    
private:
    sf::SoundBuffer mItemBuffer;
    sf::Sound mItemSound;
    
    sf::SoundBuffer mCountdownBuffer;
    sf::Sound mCountdownSound;
    
    sf::SoundBuffer mCrashBuffer;
    sf::Sound mCrashSound;
    
    sf::SoundBuffer mGameOverBuffer;
    sf::Sound mGameOverSound;
    
    float mVolume;
};

#endif
