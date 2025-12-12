#include "HighScore.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

HighScore::HighScore() : mHighScore(0), mIsNewRecord(false), mBaseResolutionWidth(1920.0f), mBaseResolutionHeight(1080.0f) {
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
    
    load();  // Cargar high score guardado
}

void HighScore::setBaseResolution(float width, float height) {
    mBaseResolutionWidth = width;
    mBaseResolutionHeight = height;
}

void HighScore::checkAndUpdate(int currentScore) {
    if (currentScore > mHighScore) {
        mHighScore = currentScore;
        mIsNewRecord = true;
        updateValue();
        save();  // Guardar automáticamente cuando hay nuevo récord
    } else {
        mIsNewRecord = false;
    }
}

int HighScore::getHighScore() const {
    return mHighScore;
}

void HighScore::reset() {
    mHighScore = 0;
    mIsNewRecord = false;
    updateValue();
}

void HighScore::resetHighScore() {
    mHighScore = 0;
    mIsNewRecord = false;
    updateValue();
    save();  // Guardar el reseteo
    std::cout << "High Score reseteado a 0" << std::endl;
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

bool HighScore::isNewRecord() const {
    return mIsNewRecord;
}

void HighScore::save() {
    std::ofstream file("highscore.dat");
    if (file.is_open()) {
        file << mHighScore;
        file.close();
    } else {
        std::cerr << "Error: No se pudo guardar el high score" << std::endl;
    }
}

void HighScore::load() {
    std::ifstream file("highscore.dat");
    if (file.is_open()) {
        file >> mHighScore;
        file.close();
        updateValue();
        std::cout << "High Score cargado: " << mHighScore << std::endl;
    } else {
        // Archivo no existe, es la primera vez
        mHighScore = 0;
        std::cout << "No hay high score guardado, comenzando desde 0" << std::endl;
    }
}

void HighScore::update(float windowWidth, float windowHeight) {
    updateTextPosition(windowWidth, windowHeight);
}

void HighScore::update(float windowWidth, float windowHeight, float offsetX, float offsetY) {
    updateTextPosition(windowWidth, windowHeight);
    // Aplicar offsets directamente a las posiciones calculadas
    sf::Vector2f labelPos = mHighScoreLabel.getPosition();
    sf::Vector2f valuePos = mHighScoreValue.getPosition();
    mHighScoreLabel.setPosition(labelPos.x + offsetX, labelPos.y + offsetY);
    mHighScoreValue.setPosition(valuePos.x + offsetX, valuePos.y + offsetY);
}

void HighScore::updateTextPosition(float windowWidth, float windowHeight) {
    // Calcular escalado basado en la resolución actual vs la base
    float scaleY = windowHeight / mBaseResolutionHeight;
    float scaleX = windowWidth / mBaseResolutionWidth;
    
    // Escalar tamaños de fuente (base: 35 para label, 45 para value)
    unsigned int labelSize = static_cast<unsigned int>(35.0f * scaleY);
    unsigned int valueSize = static_cast<unsigned int>(45.0f * scaleY);
    mHighScoreLabel.setCharacterSize(labelSize);
    mHighScoreValue.setCharacterSize(valueSize);
    
    // Escalar padding (base: 20.0f)
    float padding = 20.0f * scaleX;
    
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
