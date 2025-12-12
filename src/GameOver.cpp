#include "GameOver.hpp"
#include <iostream>
#include <cstdlib>

GameOverScreen::GameOverScreen(float width, float height) 
    : mFinished(false), 
      mDuration(sf::seconds(3.0f)),
      mSelectedItemIndex(0),
      mButtonIdleColor(sf::Color::White),
      mButtonHoverColor(sf::Color(255, 204, 0)) {
    
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
    // Overlay semi-transparente
    mOverlay.setSize(sf::Vector2f(width, height));
    mOverlay.setFillColor(sf::Color(0, 0, 0, 150));
    
    // Título "GAME OVER"
    mGameOverText.setFont(mFont);
    mGameOverText.setString("GAME OVER");
    mGameOverText.setCharacterSize(100);
    mGameOverText.setFillColor(sf::Color::Red);
    mGameOverText.setStyle(sf::Text::Bold);
    
    sf::FloatRect textBounds = mGameOverText.getLocalBounds();
    mGameOverText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    mGameOverText.setPosition(width / 2.0f, height * 0.25f);
    
    // Texto del Score
    mScoreText.setFont(mFont);
    mScoreText.setString("Score: 0");
    mScoreText.setCharacterSize(45);
    mScoreText.setFillColor(sf::Color(255, 204, 0));
    mScoreText.setStyle(sf::Text::Bold);
    
    sf::FloatRect scoreBounds = mScoreText.getLocalBounds();
    mScoreText.setOrigin(scoreBounds.left + scoreBounds.width / 2.0f, scoreBounds.top + scoreBounds.height / 2.0f);
    mScoreText.setPosition(width / 2.0f, height * 0.42f);
    
    // Limpiar opciones existentes
    mMenuItems.clear();
    
    // Configurar opciones del menú
    std::vector<std::string> itemLabels = {"Reintentar", "Salir al Menu"};
    float startY = height / 2.0f + 80.0f;
    float spacing = 120.0f;
    
    for (size_t i = 0; i < itemLabels.size(); i++) {
        sf::Text item;
        item.setFont(mFont);
        item.setString(itemLabels[i]);
        item.setCharacterSize(50);
        sf::FloatRect itemBounds = item.getLocalBounds();
        item.setOrigin(itemBounds.left + itemBounds.width / 2.0f, itemBounds.top + itemBounds.height / 2.0f);
        item.setPosition(width / 2.0f, startY + (i * spacing));
        mMenuItems.push_back(item);
    }
    
    updateSelection();
}

void GameOverScreen::updateSelection() {
    // Actualizar colores según selección
    for (size_t i = 0; i < mMenuItems.size(); i++) {
        if (static_cast<int>(i) == mSelectedItemIndex) {
            mMenuItems[i].setFillColor(mButtonHoverColor);
        } else {
            mMenuItems[i].setFillColor(mButtonIdleColor);
        }
    }
}

void GameOverScreen::moveUp() {
    if (mSelectedItemIndex > 0) {
        mSelectedItemIndex--;
        updateSelection();
    }
}

void GameOverScreen::moveDown() {
    if (mSelectedItemIndex < static_cast<int>(mMenuItems.size()) - 1) {
        mSelectedItemIndex++;
        updateSelection();
    }
}

int GameOverScreen::getSelectedItem() const {
    return mSelectedItemIndex;
}

void GameOverScreen::setScore(int score) {
    mScoreText.setString("Score: " + std::to_string(score));
    
    // Recentrar el texto del score
    sf::FloatRect scoreBounds = mScoreText.getLocalBounds();
    mScoreText.setOrigin(scoreBounds.left + scoreBounds.width / 2.0f, scoreBounds.top + scoreBounds.height / 2.0f);
}

void GameOverScreen::update(sf::Time dt) {
    // Ya no hay temporizador automático, el jugador elige
}

void GameOverScreen::draw(sf::RenderWindow& window) {
    window.draw(mOverlay);
    window.draw(mGameOverText);
    window.draw(mScoreText);
    for (const auto& item : mMenuItems) {
        window.draw(item);
    }
}

void GameOverScreen::reset() {
    mFinished = false;
    mTimer = sf::Time::Zero;
    mSelectedItemIndex = 0;
    updateSelection();
}

bool GameOverScreen::isDone() const {
    return mFinished;
}

void GameOverScreen::resize(float width, float height) {
    setupText(width, height);
}
