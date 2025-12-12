#include "SoundManager.hpp"
#include <iostream>

SoundManager::SoundManager() : mVolume(100.0f), mCurrentEngineLoop(1), mFirstLoopComplete(false), mEngineManuallyPaused(false) {
    // Definir los archivos de loop disponibles
    mEngineLoopFiles = {
        "assets/sounds/car-acceleration-inside.wav",
        "assets/sounds/Car_Engine_Loop.wav"
    };
}

bool SoundManager::init() {
    // Cargar sonido de item
    if (!mItemBuffer.loadFromFile("assets/sounds/Item.wav")) {
        std::cerr << "Error: No se pudo cargar el sonido Item.wav" << std::endl;
        return false;
    }
    
    mItemSound.setBuffer(mItemBuffer);
    mItemSound.setVolume(mVolume);
    
    // Cargar sonido de countdown
    if (!mCountdownBuffer.loadFromFile("assets/sounds/countdown.wav")) {
        std::cerr << "Error: No se pudo cargar el sonido countdown.wav" << std::endl;
        return false;
    }
    
    mCountdownSound.setBuffer(mCountdownBuffer);
    mCountdownSound.setVolume(mVolume);
    
    // Cargar sonido de crash
    if (!mCrashBuffer.loadFromFile("assets/sounds/Crash.wav")) {
        std::cerr << "Error: No se pudo cargar el sonido Crash.wav" << std::endl;
        return false;
    }
    
    mCrashSound.setBuffer(mCrashBuffer);
    mCrashSound.setVolume(mVolume);
    
    // Cargar sonido de game over
    if (!mGameOverBuffer.loadFromFile("assets/sounds/losetrumpet.wav")) {
        std::cerr << "Error: No se pudo cargar el sonido losetrumpet.wav" << std::endl;
        return false;
    }
    
    mGameOverSound.setBuffer(mGameOverBuffer);
    mGameOverSound.setVolume(mVolume);
    
    // Cargar sonido de motor rugiendo (al empezar)
    if (!mEngineRoaringBuffer.loadFromFile("assets/sounds/car-engine-roaring.wav")) {
        std::cerr << "Error: No se pudo cargar el sonido car-engine-roaring.wav" << std::endl;
        return false;
    }
    
    mEngineRoaringSound.setBuffer(mEngineRoaringBuffer);
    mEngineRoaringSound.setVolume(mVolume * 0.6f);
    
    // Cargar el loop de motor inicial (car-acceleration-inside.wav)
    if (!mEngineLoop.openFromFile(mEngineLoopFiles[0])) {
        std::cerr << "Error: No se pudo cargar el loop de motor inicial" << std::endl;
        return false;
    }
    mEngineLoop.setLoop(false);  // Controlamos el loop manualmente
    mEngineLoop.setVolume(mVolume * 0.65f);  // Volumen del motor al 65%
    mEngineDuration = mEngineLoop.getDuration();
    
    std::cout << "SoundManager inicializado correctamente" << std::endl;
    std::cout << "Loop de motor activo: 1 - car-acceleration-inside.wav" << std::endl;
    return true;
}

void SoundManager::playItemSound() {
    mItemSound.play();
}

void SoundManager::playCountdownSound() {
    mCountdownSound.play();
}

void SoundManager::playCrashSound() {
    mCrashSound.play();
}

void SoundManager::playGameOverSound() {
    mGameOverSound.play();
}

void SoundManager::playEngineRoaringSound() {
    mEngineRoaringSound.play();
}

void SoundManager::startEngineLoop() {
    if (mEngineLoop.getStatus() != sf::Music::Playing) {
        mFirstLoopComplete = false;
        mEngineManuallyPaused = false;
        mEngineLoop.setPlayingOffset(sf::Time::Zero);
        mEngineLoop.play();
    }
}

void SoundManager::stopEngineLoop() {
    mEngineLoop.stop();
    mFirstLoopComplete = false;
    mEngineManuallyPaused = false;
}

void SoundManager::pauseEngineLoop() {
    if (mEngineLoop.getStatus() == sf::Music::Playing) {
        mEngineLoop.pause();
        mEngineManuallyPaused = true;
    }
}

void SoundManager::resumeEngineLoop() {
    if (mEngineLoop.getStatus() == sf::Music::Paused) {
        mEngineLoop.play();
        mEngineManuallyPaused = false;
    }
}

void SoundManager::pauseCountdownSound() {
    if (mCountdownSound.getStatus() == sf::Sound::Playing) {
        mCountdownSound.pause();
    }
}

void SoundManager::resumeCountdownSound() {
    if (mCountdownSound.getStatus() == sf::Sound::Paused) {
        mCountdownSound.play();
    }
}

void SoundManager::changeEngineLoop(int loopNumber) {
    if (loopNumber < 1 || loopNumber > 2) return;
    
    bool wasPlaying = (mEngineLoop.getStatus() == sf::Music::Playing);
    mEngineLoop.stop();
    
    mCurrentEngineLoop = loopNumber;
    if (!mEngineLoop.openFromFile(mEngineLoopFiles[loopNumber - 1])) {
        std::cerr << "Error: No se pudo cargar el loop " << loopNumber << std::endl;
        return;
    }
    
    mEngineLoop.setLoop(false);  // Controlamos manualmente
    mEngineLoop.setVolume(mVolume * 0.65f);
    mEngineDuration = mEngineLoop.getDuration();
    mFirstLoopComplete = false;
    
    // Mostrar cuál loop está activo
    std::cout << "Loop de motor cambiado a: " << loopNumber << " - " 
              << mEngineLoopFiles[loopNumber - 1] << std::endl;
    
    if (wasPlaying) {
        mEngineLoop.play();
    }
}

int SoundManager::getCurrentEngineLoop() const {
    return mCurrentEngineLoop;
}

void SoundManager::updateEngineLoop() {
    // Si el motor está sonando, verificar si terminó para reiniciar con recorte
    // NO reiniciar si está pausado manualmente
    if (mEngineLoop.getStatus() == sf::Music::Stopped && !mEngineManuallyPaused) {
        // Verificar si es porque terminó de reproducir (no por pause manual)
        if (!mFirstLoopComplete) {
            // Primera reproducción completa, ahora empezar desde 2.5 segundos
            mFirstLoopComplete = true;
            mEngineLoop.setPlayingOffset(sf::seconds(2.5f));
            mEngineLoop.play();
        } else {
            // Reproducciones subsecuentes, empezar desde 2.5 segundos
            mEngineLoop.setPlayingOffset(sf::seconds(2.5f));
            mEngineLoop.play();
        }
    }
}

void SoundManager::setVolume(float volume) {
    mVolume = volume;
    mItemSound.setVolume(mVolume);
    mCountdownSound.setVolume(mVolume);
    mCrashSound.setVolume(mVolume);
    mGameOverSound.setVolume(mVolume);
    mEngineRoaringSound.setVolume(mVolume * 0.6f);
    mEngineLoop.setVolume(mVolume * 0.65f);  // Motor al 65%
}
