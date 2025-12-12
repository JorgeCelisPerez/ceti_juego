#include "GameOver.hpp"
#include <iostream>
#include <cstdlib>

GameOverScreen::GameOverScreen(float width, float height) : mFinished(false), mDuration(sf::seconds(3.0f)) {
    if (!mFont.loadFromFile("assets/fonts/Speed Rusher.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente 'Speed Rusher.ttf' para Game Over." << std::endl;
        // Fallback a arial
        if (!mFont.loadFromFile("assets/fonts/arial.ttf")) {
            std::cerr << "Error: No se pudo cargar la fuente de fallback 'arial.ttf'." << std::endl;
            exit(1);
        }
    }
    setupText(width, height);
    reset();
}

void GameOverScreen::setupText(float width, float height) {
    mGameOverText.setFont(mFont);
    mGameOverText.setString("GAME OVER");
    mGameOverText.setCharacterSize(80);
    mGameOverText.setFillColor(sf::Color::Red);
    mGameOverText.setStyle(sf::Text::Bold);
    
    sf::FloatRect textBounds = mGameOverText.getLocalBounds();
    mGameOverText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    mGameOverText.setPosition(width / 2.0f, height / 2.0f);
}

void GameOverScreen::update(sf::Time dt) {
    if (!mFinished) {
        mTimer += dt;
        if (mTimer >= mDuration) {
            mFinished = true;
        }
    }
}

void GameOverScreen::draw(sf::RenderWindow& window) {
    window.draw(mGameOverText);
}

void GameOverScreen::reset() {
    mFinished = false;
    mTimer = sf::Time::Zero;
}

bool GameOverScreen::isDone() const {
    return mFinished;
}

void GameOverScreen::resize(float width, float height) {
    setupText(width, height);
}
