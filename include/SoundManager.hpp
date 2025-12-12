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
    void playEngineRoaringSound();
    void setVolume(float volume);  // 0.0f a 100.0f
    
    // Sistema de loops de motor
    void startEngineLoop();
    void stopEngineLoop();
    void pauseEngineLoop();
    void resumeEngineLoop();
    void pauseCountdownSound();
    void resumeCountdownSound();
    void changeEngineLoop(int loopNumber);  // 1-7
    int getCurrentEngineLoop() const;
    void updateEngineLoop();  // Actualizar el loop para recortar 1 seg después de la primera reproducción
    
private:
    sf::SoundBuffer mItemBuffer;
    sf::Sound mItemSound;
    
    sf::SoundBuffer mCountdownBuffer;
    sf::Sound mCountdownSound;
    
    sf::SoundBuffer mCrashBuffer;
    sf::Sound mCrashSound;
    
    sf::SoundBuffer mGameOverBuffer;
    sf::Sound mGameOverSound;
    
    sf::SoundBuffer mEngineRoaringBuffer;
    sf::Sound mEngineRoaringSound;
    
    // Sistema de loops de motor (7 opciones)
    std::vector<sf::SoundBuffer> mEngineLoopBuffers;
    sf::Music mEngineLoop;  // Usar Music para loops largos
    int mCurrentEngineLoop;
    std::vector<std::string> mEngineLoopFiles;
    bool mFirstLoopComplete;  // Para detectar cuando termina la primera reproducción
    sf::Time mEngineDuration;  // Duración del audio del motor
    bool mEngineManuallyPaused;  // Para distinguir entre pausa manual y fin natural
    
    float mVolume;
};

#endif
