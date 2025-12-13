#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
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
    void playNewRecordSound();
    void playEngineRoaringSound();
    void setVolume(float volume);  // 0.0f a 100.0f
    
    // Sistema de loops de motor
    void startEngineLoop();
    void stopEngineLoop();
    void pauseEngineLoop();
    void resumeEngineLoop();
    void pauseCountdownSound();
    void resumeCountdownSound();
    
    // Sistema de música de fondo
    void startBackgroundMusic();
    void stopBackgroundMusic();
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    void setBackgroundMusicVolume(float volume);  // 0.0f a 100.0f
    
private:
    sf::SoundBuffer mItemBuffer;
    sf::Sound mItemSound;
    
    sf::SoundBuffer mCountdownBuffer;
    sf::Sound mCountdownSound;
    
    sf::SoundBuffer mCrashBuffer;
    sf::Sound mCrashSound;
    
    sf::SoundBuffer mGameOverBuffer;
    sf::Sound mGameOverSound;
    
    sf::SoundBuffer mNewRecordBuffer;
    sf::Sound mNewRecordSound;
    
    sf::SoundBuffer mEngineRoaringBuffer;
    sf::Sound mEngineRoaringSound;
    
    // Sistema de loop de motor
    sf::Music mEngineLoop;
    std::vector<std::string> mEngineLoopFiles;
    
    // Música de fondo
    sf::Music mBackgroundMusic;
    
    float mVolume;
};

#endif
