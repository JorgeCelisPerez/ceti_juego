#include "SoundManager.hpp"
#include <iostream>

SoundManager::SoundManager() : mVolume(100.0f), mCurrentEngineLoop(1) {
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
    
    // Cargar sonido de nuevo récord
    if (!mNewRecordBuffer.loadFromFile("assets/sounds/new-record.wav")) {
        std::cerr << "Error: No se pudo cargar el sonido new-record.wav" << std::endl;
        return false;
    }
    
    mNewRecordSound.setBuffer(mNewRecordBuffer);
    mNewRecordSound.setVolume(mVolume);
    
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
    mEngineLoop.setLoop(true);  // SFML maneja el loop automáticamente
    mEngineLoop.setVolume(mVolume * 0.65f);  // Volumen del motor al 65%
    
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

void SoundManager::playNewRecordSound() {
    mNewRecordSound.play();
}

void SoundManager::playEngineRoaringSound() {
    mEngineRoaringSound.play();
}

void SoundManager::startEngineLoop() {
    if (mEngineLoop.getStatus() != sf::Music::Playing) {
        mEngineLoop.play();
    }
}

void SoundManager::stopEngineLoop() {
    mEngineLoop.stop();
}

void SoundManager::pauseEngineLoop() {
    if (mEngineLoop.getStatus() == sf::Music::Playing) {
        mEngineLoop.pause();
    }
}

void SoundManager::resumeEngineLoop() {
    if (mEngineLoop.getStatus() == sf::Music::Paused) {
        mEngineLoop.play();
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
    
    mEngineLoop.setLoop(true);  // Loop automático
    mEngineLoop.setVolume(mVolume * 0.65f);
    
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
    // Con setLoop(true), SFML maneja el loop automáticamente
    // Esta función ya no es necesaria pero la dejamos vacía por compatibilidad
}

void SoundManager::setVolume(float volume) {
    mVolume = volume;
    mItemSound.setVolume(mVolume);
    mCountdownSound.setVolume(mVolume);
    mCrashSound.setVolume(mVolume);
    mGameOverSound.setVolume(mVolume);
    mNewRecordSound.setVolume(mVolume);
    mEngineRoaringSound.setVolume(mVolume * 0.6f);
    mEngineLoop.setVolume(mVolume * 0.65f);  // Motor al 65%
}
