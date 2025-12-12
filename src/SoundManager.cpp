#include "SoundManager.hpp"
#include <iostream>

SoundManager::SoundManager() : mVolume(100.0f) {
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
    
    std::cout << "SoundManager inicializado correctamente" << std::endl;
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

void SoundManager::setVolume(float volume) {
    mVolume = volume;
    mItemSound.setVolume(mVolume);
    mCountdownSound.setVolume(mVolume);
    mCrashSound.setVolume(mVolume);
    mGameOverSound.setVolume(mVolume);
}
