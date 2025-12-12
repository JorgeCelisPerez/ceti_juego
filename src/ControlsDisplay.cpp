#include "ControlsDisplay.hpp"
#include <iostream>
#include <cstdlib>

ControlsDisplay::ControlsDisplay() 
    : mBaseResolutionWidth(1920.0f), 
      mBaseResolutionHeight(1080.0f),
      mDebugMode(false) {
    
    // Cargar fuente
    if (!mFont.loadFromFile("assets/fonts/Speed Rusher.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente para ControlsDisplay" << std::endl;
        if (!mFont.loadFromFile("assets/fonts/arial.ttf")) {
            std::cerr << "Error: No se pudo cargar fuente de fallback" << std::endl;
            exit(1);
        }
    }
    
    // Cargar texturas de teclas
    if (!mArrowUpTexture.loadFromFile("assets/images/Arrow Up.png")) {
        std::cerr << "Error: No se pudo cargar Arrow Up.png" << std::endl;
    }
    if (!mArrowDownTexture.loadFromFile("assets/images/Arrow Down.png")) {
        std::cerr << "Error: No se pudo cargar Arrow Down.png" << std::endl;
    }
    if (!mArrowLeftTexture.loadFromFile("assets/images/Arrow Left.png")) {
        std::cerr << "Error: No se pudo cargar Arrow Left.png" << std::endl;
    }
    if (!mArrowRightTexture.loadFromFile("assets/images/Arrow Right.png")) {
        std::cerr << "Error: No se pudo cargar Arrow Right.png" << std::endl;
    }
    if (!mPTexture.loadFromFile("assets/images/P.png")) {
        std::cerr << "Error: No se pudo cargar P.png" << std::endl;
    }
    if (!mEscTexture.loadFromFile("assets/images/Escape.png")) {
        std::cerr << "Error: No se pudo cargar Escape.png" << std::endl;
    }
    if (!mF11Texture.loadFromFile("assets/images/F11.png")) {
        std::cerr << "Error: No se pudo cargar F11.png" << std::endl;
    }
    if (!mEnterTexture.loadFromFile("assets/images/Enter.png")) {
        std::cerr << "Error: No se pudo cargar Enter.png" << std::endl;
    }
}

void ControlsDisplay::setDebugMode(bool debug) {
    mDebugMode = debug;
}

void ControlsDisplay::setBaseResolution(float width, float height) {
    mBaseResolutionWidth = width;
    mBaseResolutionHeight = height;
}

void ControlsDisplay::updatePosition(float windowWidth, float windowHeight) {
    setupLayout(windowWidth, windowHeight);
}

void ControlsDisplay::setupLayout(float windowWidth, float windowHeight) {
    // Calcular escala basada en la resolución
    float scaleY = windowHeight / mBaseResolutionHeight;
    float scaleX = windowWidth / mBaseResolutionWidth;
    float scale = scaleY; // Usar escala vertical para mantener proporciones
    
    // Limpiar sprites y labels anteriores
    mKeySprites.clear();
    mLabels.clear();
    
    // Tamaño de las teclas
    float keySize = 50.0f * scale;
    float enterKeySize = 75.0f * scale; // Enter más grande
    float textSize = 40.0f * scaleY;
    
    // === COORDENADAS FIJAS ESCALADAS ===
    
    // Fila 1: Movimiento (texto arriba, flechas abajo)
    
    // Label "Movimiento" arriba
    sf::Text moveLabel;
    moveLabel.setFont(mFont);
    moveLabel.setString("Movimiento");
    moveLabel.setCharacterSize(static_cast<unsigned int>(40.0f * scaleY));
    moveLabel.setFillColor(sf::Color::Black);
    moveLabel.setPosition(50.0f * scaleX, 328.0f * scaleY);
    mLabels.push_back(moveLabel); // Index 0 (Movimiento)
    
    // Flechas con coordenadas fijas escaladas
    sf::Sprite arrowUp(mArrowUpTexture);
    arrowUp.setScale(keySize / arrowUp.getLocalBounds().width, keySize / arrowUp.getLocalBounds().height);
    arrowUp.setPosition(162.5f * scaleX, 396.0f * scaleY);
    mKeySprites.push_back(arrowUp); // Index 0 (Arrow Up)
    
    sf::Sprite arrowLeft(mArrowLeftTexture);
    arrowLeft.setScale(keySize / arrowLeft.getLocalBounds().width, keySize / arrowLeft.getLocalBounds().height);
    arrowLeft.setPosition(102.5f * scaleX, 446.0f * scaleY);
    mKeySprites.push_back(arrowLeft); // Index 1 (Arrow Left)
    
    sf::Sprite arrowDown(mArrowDownTexture);
    arrowDown.setScale(keySize / arrowDown.getLocalBounds().width, keySize / arrowDown.getLocalBounds().height);
    arrowDown.setPosition(162.5f * scaleX, 446.0f * scaleY);
    mKeySprites.push_back(arrowDown); // Index 2 (Arrow Down)
    
    sf::Sprite arrowRight(mArrowRightTexture);
    arrowRight.setScale(keySize / arrowRight.getLocalBounds().width, keySize / arrowRight.getLocalBounds().height);
    arrowRight.setPosition(222.5f * scaleX, 446.0f * scaleY);
    mKeySprites.push_back(arrowRight); // Index 3 (Arrow Right)
    
    // Fila 2: Pausa (texto arriba, teclas P/ESC abajo)
    
    sf::Text pauseLabel;
    pauseLabel.setFont(mFont);
    pauseLabel.setString("Pausa");
    pauseLabel.setCharacterSize(static_cast<unsigned int>(40.0f * scaleY));
    pauseLabel.setFillColor(sf::Color::Black);
    pauseLabel.setPosition(130.0f * scaleX, 521.0f * scaleY);
    mLabels.push_back(pauseLabel); // Index 1 (Pausa)
    
    sf::Sprite pKey(mPTexture);
    pKey.setScale(keySize / pKey.getLocalBounds().width, keySize / pKey.getLocalBounds().height);
    pKey.setPosition(181.5f * scaleX, 596.0f * scaleY);
    mKeySprites.push_back(pKey); // Index 4 (P Key)
    
    sf::Sprite escKey(mEscTexture);
    escKey.setScale(keySize / escKey.getLocalBounds().width, keySize / escKey.getLocalBounds().height);
    escKey.setPosition(241.5f * scaleX, 596.0f * scaleY);
    mKeySprites.push_back(escKey); // Index 5 (ESC Key)
    
    // Fila 3: Pantalla (texto arriba, tecla F11 abajo)
    
    sf::Text f11Label;
    f11Label.setFont(mFont);
    f11Label.setString("Pantalla");
    f11Label.setCharacterSize(static_cast<unsigned int>(40.0f * scaleY));
    f11Label.setFillColor(sf::Color::Black);
    f11Label.setPosition(80.0f * scaleX, 676.0f * scaleY);
    mLabels.push_back(f11Label); // Index 2 (Pantalla)
    
    sf::Sprite f11Key(mF11Texture);
    f11Key.setScale(keySize / f11Key.getLocalBounds().width, keySize / f11Key.getLocalBounds().height);
    f11Key.setPosition(211.5f * scaleX, 756.0f * scaleY);
    mKeySprites.push_back(f11Key); // Index 6 (F11 Key)
    
    // Tecla Enter (más grande)
    sf::Sprite enterKey(mEnterTexture);
    enterKey.setScale(enterKeySize / enterKey.getLocalBounds().width, enterKeySize / enterKey.getLocalBounds().height);
    enterKey.setPosition(336.5f * scaleX, 426.0f * scaleY);
    mKeySprites.push_back(enterKey); // Index 7 (Enter Key)
}

void ControlsDisplay::draw(sf::RenderWindow& window) {
    // Dibujar las teclas y labels
    for (const auto& sprite : mKeySprites) {
        window.draw(sprite);
    }
    for (const auto& label : mLabels) {
        window.draw(label);
    }
    
    // Dibujar hitboxes en modo debug
    if (mDebugMode) {
        // Hitboxes de todos los sprites de teclas
        for (const auto& sprite : mKeySprites) {
            sf::FloatRect bounds = sprite.getGlobalBounds();
            sf::RectangleShape hitbox;
            hitbox.setPosition(bounds.left, bounds.top);
            hitbox.setSize(sf::Vector2f(bounds.width, bounds.height));
            hitbox.setFillColor(sf::Color(0, 255, 255, 60));
            hitbox.setOutlineThickness(2.0f);
            hitbox.setOutlineColor(sf::Color::Cyan);
            window.draw(hitbox);
        }
        
        // Hitboxes de todos los textos/labels
        for (const auto& label : mLabels) {
            sf::FloatRect bounds = label.getGlobalBounds();
            sf::RectangleShape hitbox;
            hitbox.setPosition(bounds.left, bounds.top);
            hitbox.setSize(sf::Vector2f(bounds.width, bounds.height));
            hitbox.setFillColor(sf::Color(255, 255, 0, 60));
            hitbox.setOutlineThickness(2.0f);
            hitbox.setOutlineColor(sf::Color::Yellow);
            window.draw(hitbox);
        }
    }
}
