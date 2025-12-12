#include "Countdown.hpp"
#include <string>

Countdown::Countdown() 
    : mTimer(0.0f), 
      mNumber(3), 
      mFinished(false),
      mBaseResolutionWidth(1920.0f),
      mBaseResolutionHeight(1080.0f) {
}

bool Countdown::init(const sf::Font& font) {
    mCountdownText.setFont(font);
    mCountdownText.setString("3");
    mCountdownText.setFillColor(sf::Color::Yellow);
    mCountdownText.setStyle(sf::Text::Bold);
    return true;
}

void Countdown::reset() {
    mTimer = 0.0f;
    mNumber = 3;
    mFinished = false;
    mCountdownText.setString("3");
    mCountdownText.setFillColor(sf::Color::Yellow);
}

void Countdown::start(SoundManager& soundManager) {
    reset();
    soundManager.playCountdownSound();
}

void Countdown::update(float deltaTime) {
    if (mFinished) return;
    
    mTimer += deltaTime;
    
    // Cambiar número cada segundo
    if (mTimer >= 1.0f) {
        mTimer = 0.0f;
        mNumber--;
        
        if (mNumber < 0) {
            mFinished = true;
        } else if (mNumber == 0) {
            mCountdownText.setString("A Correr!");
            mCountdownText.setFillColor(sf::Color::Green);
        } else {
            mCountdownText.setString(std::to_string(mNumber));
            mCountdownText.setFillColor(sf::Color::Yellow);
        }
    }
}

bool Countdown::isFinished() const {
    return mFinished;
}

void Countdown::draw(sf::RenderWindow& window) {
    window.draw(mCountdownText);
}

void Countdown::updateScale(float windowWidth, float windowHeight) {
    // Escalar igual que Score y HighScore
    float scaleX = windowWidth / mBaseResolutionWidth;
    float scaleY = windowHeight / mBaseResolutionHeight;
    
    // Tamaño base del texto
    unsigned int baseSize = 150;
    unsigned int scaledSize = static_cast<unsigned int>(baseSize * scaleY);
    mCountdownText.setCharacterSize(scaledSize);
    
    // Centrar el texto
    sf::FloatRect bounds = mCountdownText.getLocalBounds();
    mCountdownText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    mCountdownText.setPosition(windowWidth / 2.0f, windowHeight / 2.0f);
}

