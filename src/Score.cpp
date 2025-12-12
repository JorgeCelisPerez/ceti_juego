#include "Score.hpp"
#include <iostream>
#include <sstream>

Score::Score() : mCurrentScore(0) {
    if (!mFont.loadFromFile("assets/fonts/Speed Rusher.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente para Score" << std::endl;
        if (!mFont.loadFromFile("assets/fonts/arial.ttf")) {
            std::cerr << "Error: No se pudo cargar fuente de fallback" << std::endl;
            exit(1);
        }
    }
    
    // Configurar etiqueta "Score:"
    mScoreLabel.setFont(mFont);
    mScoreLabel.setCharacterSize(35);
    mScoreLabel.setFillColor(sf::Color::White);
    mScoreLabel.setStyle(sf::Text::Bold);
    mScoreLabel.setString("Score:");
    
    // Configurar valor numérico
    mScoreValue.setFont(mFont);
    mScoreValue.setCharacterSize(45);
    mScoreValue.setFillColor(sf::Color(255, 204, 0)); // Amarillo dorado
    mScoreValue.setStyle(sf::Text::Bold);
    mScoreValue.setString("0");
}

void Score::addPoints(int points) {
    mCurrentScore += points;
    std::ostringstream oss;
    oss << mCurrentScore;
    mScoreValue.setString(oss.str());
    // Actualizar posición del valor para mantenerlo centrado ya que el ancho cambia
    sf::FloatRect labelBounds = mScoreLabel.getGlobalBounds();
    sf::FloatRect valueBounds = mScoreValue.getGlobalBounds();
    float labelCenterX = mScoreLabel.getPosition().x + labelBounds.width / 2.0f;
    float valuePosX = labelCenterX - valueBounds.width / 2.0f;
    float valuePosY = mScoreLabel.getPosition().y + labelBounds.height + 5.0f;
    mScoreValue.setPosition(valuePosX, valuePosY);
}

void Score::reset() {
    mCurrentScore = 0;
    mScoreValue.setString("0");
}

int Score::getScore() const {
    return mCurrentScore;
}

void Score::update(float windowWidth, float windowHeight) {
    updateTextPosition(windowWidth, windowHeight);
}

void Score::update(float windowWidth, float windowHeight, float offsetX, float offsetY) {
    updateTextPosition(windowWidth, windowHeight);
    // Aplicar offset
    mScoreLabel.move(offsetX, offsetY);
    mScoreValue.move(offsetX, offsetY);
}

void Score::updateTextPosition(float windowWidth, float windowHeight) {
    float padding = 20.0f;
    
    // Posicionar etiqueta "Score:" en la esquina superior derecha
    sf::FloatRect labelBounds = mScoreLabel.getGlobalBounds();
    mScoreLabel.setPosition(windowWidth - labelBounds.width - padding, padding);
    
    // Posicionar valor numérico debajo de la etiqueta y CENTRADO
    sf::FloatRect valueBounds = mScoreValue.getGlobalBounds();
    float labelCenterX = mScoreLabel.getPosition().x + labelBounds.width / 2.0f;
    float valuePosX = labelCenterX - valueBounds.width / 2.0f;
    float valuePosY = mScoreLabel.getPosition().y + labelBounds.height + 5.0f;
    mScoreValue.setPosition(valuePosX, valuePosY);
}

void Score::draw(sf::RenderWindow& window) {
    window.draw(mScoreLabel);
    window.draw(mScoreValue);
}

void Score::drawDebug(sf::RenderWindow& window) {
    // Dibujar hitbox de la etiqueta
    sf::FloatRect labelBounds = mScoreLabel.getGlobalBounds();
    sf::RectangleShape labelBox;
    labelBox.setPosition(labelBounds.left, labelBounds.top);
    labelBox.setSize(sf::Vector2f(labelBounds.width, labelBounds.height));
    labelBox.setFillColor(sf::Color(0, 255, 0, 50));
    labelBox.setOutlineThickness(2.f);
    labelBox.setOutlineColor(sf::Color::Green);
    window.draw(labelBox);
    
    // Dibujar hitbox del valor
    sf::FloatRect valueBounds = mScoreValue.getGlobalBounds();
    sf::RectangleShape valueBox;
    valueBox.setPosition(valueBounds.left, valueBounds.top);
    valueBox.setSize(sf::Vector2f(valueBounds.width, valueBounds.height));
    valueBox.setFillColor(sf::Color(255, 255, 0, 50));
    valueBox.setOutlineThickness(2.f);
    valueBox.setOutlineColor(sf::Color::Yellow);
    window.draw(valueBox);
}
