#include "HighScore.hpp"
#include <iostream>
#include <sstream>

HighScore::HighScore() : mHighScore(0) {
    if (!mFont.loadFromFile("assets/fonts/Speed Rusher.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente para HighScore" << std::endl;
        if (!mFont.loadFromFile("assets/fonts/arial.ttf")) {
            std::cerr << "Error: No se pudo cargar fuente de fallback" << std::endl;
            exit(1);
        }
    }
    
    // Configurar etiqueta "High Score:"
    mHighScoreLabel.setFont(mFont);
    mHighScoreLabel.setCharacterSize(35);
    mHighScoreLabel.setFillColor(sf::Color::White);
    mHighScoreLabel.setStyle(sf::Text::Bold);
    mHighScoreLabel.setString("High Score:");
    
    // Configurar valor numérico
    mHighScoreValue.setFont(mFont);
    mHighScoreValue.setCharacterSize(45);
    mHighScoreValue.setFillColor(sf::Color(255, 215, 0)); // Dorado brillante
    mHighScoreValue.setStyle(sf::Text::Bold);
    mHighScoreValue.setString("0");
}

void HighScore::checkAndUpdate(int currentScore) {
    if (currentScore > mHighScore) {
        mHighScore = currentScore;
        updateValue();
    }
}

int HighScore::getHighScore() const {
    return mHighScore;
}

void HighScore::reset() {
    mHighScore = 0;
    updateValue();
}

void HighScore::updateValue() {
    std::ostringstream oss;
    oss << mHighScore;
    mHighScoreValue.setString(oss.str());
    
    // Actualizar posición para mantenerlo centrado
    sf::FloatRect labelBounds = mHighScoreLabel.getGlobalBounds();
    sf::FloatRect valueBounds = mHighScoreValue.getGlobalBounds();
    float labelCenterX = mHighScoreLabel.getPosition().x + labelBounds.width / 2.0f;
    float valuePosX = labelCenterX - valueBounds.width / 2.0f;
    float valuePosY = mHighScoreLabel.getPosition().y + labelBounds.height + 5.0f;
    mHighScoreValue.setPosition(valuePosX, valuePosY);
}

void HighScore::update(float windowWidth, float windowHeight) {
    updateTextPosition(windowWidth, windowHeight);
}

void HighScore::update(float windowWidth, float windowHeight, float offsetX, float offsetY) {
    updateTextPosition(windowWidth, windowHeight);
    // Aplicar offset
    mHighScoreLabel.move(offsetX, offsetY);
    mHighScoreValue.move(offsetX, offsetY);
}

void HighScore::updateTextPosition(float windowWidth, float windowHeight) {
    float padding = 20.0f;
    
    // Posicionar etiqueta "High Score:" en la esquina superior IZQUIERDA
    mHighScoreLabel.setPosition(padding, padding);
    
    // Posicionar valor numérico debajo de la etiqueta y CENTRADO
    sf::FloatRect labelBounds = mHighScoreLabel.getGlobalBounds();
    sf::FloatRect valueBounds = mHighScoreValue.getGlobalBounds();
    float labelCenterX = mHighScoreLabel.getPosition().x + labelBounds.width / 2.0f;
    float valuePosX = labelCenterX - valueBounds.width / 2.0f;
    float valuePosY = mHighScoreLabel.getPosition().y + labelBounds.height + 5.0f;
    mHighScoreValue.setPosition(valuePosX, valuePosY);
}

void HighScore::draw(sf::RenderWindow& window) {
    window.draw(mHighScoreLabel);
    window.draw(mHighScoreValue);
}

void HighScore::drawDebug(sf::RenderWindow& window) {
    // Dibujar hitbox de la etiqueta
    sf::FloatRect labelBounds = mHighScoreLabel.getGlobalBounds();
    sf::RectangleShape labelBox;
    labelBox.setPosition(labelBounds.left, labelBounds.top);
    labelBox.setSize(sf::Vector2f(labelBounds.width, labelBounds.height));
    labelBox.setFillColor(sf::Color(0, 255, 255, 50));
    labelBox.setOutlineThickness(2.f);
    labelBox.setOutlineColor(sf::Color::Cyan);
    window.draw(labelBox);
    
    // Dibujar hitbox del valor
    sf::FloatRect valueBounds = mHighScoreValue.getGlobalBounds();
    sf::RectangleShape valueBox;
    valueBox.setPosition(valueBounds.left, valueBounds.top);
    valueBox.setSize(sf::Vector2f(valueBounds.width, valueBounds.height));
    valueBox.setFillColor(sf::Color(255, 0, 255, 50));
    valueBox.setOutlineThickness(2.f);
    valueBox.setOutlineColor(sf::Color::Magenta);
    window.draw(valueBox);
}
