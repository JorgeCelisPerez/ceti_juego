#include "ControlsDisplay.hpp"
#include <iostream>
#include <cstdlib>

ControlsDisplay::ControlsDisplay() 
    : mBaseResolutionWidth(1920.0f), 
      mBaseResolutionHeight(1080.0f),
      mDebugMode(false),
      mSelectedElement(0),
      mMoveLabelIndex(0),
      mArrowStartIndex(0),
      mPauseLabelIndex(1),
      mPauseKeysStartIndex(4),
      mF11LabelIndex(2),
      mF11KeyIndex(6),
      mEnterKeyIndex(7) {
    
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

void ControlsDisplay::selectElement(int elementIndex) {
    mSelectedElement = elementIndex;
}

void ControlsDisplay::moveSelected(float offsetX, float offsetY) {
    if (mSelectedElement == 0) return;
    
    // 1 = moveLabel, 2 = arrows, 3 = pauseLabel, 4 = pauseKeys, 5 = f11Label, 6 = f11Key
    if (mSelectedElement == 1 && mMoveLabelIndex < mLabels.size()) {
        sf::Vector2f pos = mLabels[mMoveLabelIndex].getPosition();
        mLabels[mMoveLabelIndex].setPosition(pos.x + offsetX, pos.y + offsetY);
    }
    else if (mSelectedElement == 2) {
        // Mover las 4 flechas (indices 0-3 en mKeySprites)
        for (int i = 0; i < 4 && i < mKeySprites.size(); i++) {
            sf::Vector2f pos = mKeySprites[i].getPosition();
            mKeySprites[i].setPosition(pos.x + offsetX, pos.y + offsetY);
        }
    }
    else if (mSelectedElement == 3 && mPauseLabelIndex < mLabels.size()) {
        sf::Vector2f pos = mLabels[mPauseLabelIndex].getPosition();
        mLabels[mPauseLabelIndex].setPosition(pos.x + offsetX, pos.y + offsetY);
    }
    else if (mSelectedElement == 4) {
        // Mover P y ESC (indices 4-5 en mKeySprites)
        for (int i = 4; i < 6 && i < mKeySprites.size(); i++) {
            sf::Vector2f pos = mKeySprites[i].getPosition();
            mKeySprites[i].setPosition(pos.x + offsetX, pos.y + offsetY);
        }
    }
    else if (mSelectedElement == 5 && mF11LabelIndex < mLabels.size()) {
        sf::Vector2f pos = mLabels[mF11LabelIndex].getPosition();
        mLabels[mF11LabelIndex].setPosition(pos.x + offsetX, pos.y + offsetY);
    }
    else if (mSelectedElement == 6 && mF11KeyIndex < mKeySprites.size()) {
        sf::Vector2f pos = mKeySprites[mF11KeyIndex].getPosition();
        mKeySprites[mF11KeyIndex].setPosition(pos.x + offsetX, pos.y + offsetY);
    }
    else if (mSelectedElement == 7 && mEnterKeyIndex < mKeySprites.size()) {
        sf::Vector2f pos = mKeySprites[mEnterKeyIndex].getPosition();
        mKeySprites[mEnterKeyIndex].setPosition(pos.x + offsetX, pos.y + offsetY);
    }
}

void ControlsDisplay::printCoordinates() const {
    std::cout << "\n=== Coordenadas ControlsDisplay ===" << std::endl;
    
    if (mMoveLabelIndex < mLabels.size()) {
        sf::Vector2f pos = mLabels[mMoveLabelIndex].getPosition();
        std::cout << "1. Texto 'Movimiento': X=" << pos.x << ", Y=" << pos.y << std::endl;
    }
    
    if (mKeySprites.size() >= 4) {
        std::cout << "2. Flechas:" << std::endl;
        sf::Vector2f pos0 = mKeySprites[0].getPosition();
        sf::Vector2f pos1 = mKeySprites[1].getPosition();
        sf::Vector2f pos2 = mKeySprites[2].getPosition();
        sf::Vector2f pos3 = mKeySprites[3].getPosition();
        std::cout << "   Arrow Up: X=" << pos0.x << ", Y=" << pos0.y << std::endl;
        std::cout << "   Arrow Left: X=" << pos1.x << ", Y=" << pos1.y << std::endl;
        std::cout << "   Arrow Down: X=" << pos2.x << ", Y=" << pos2.y << std::endl;
        std::cout << "   Arrow Right: X=" << pos3.x << ", Y=" << pos3.y << std::endl;
    }
    
    if (mPauseLabelIndex < mLabels.size()) {
        sf::Vector2f pos = mLabels[mPauseLabelIndex].getPosition();
        std::cout << "3. Texto 'Pausa': X=" << pos.x << ", Y=" << pos.y << std::endl;
    }
    
    if (mKeySprites.size() >= 6) {
        std::cout << "4. Teclas Pausa:" << std::endl;
        sf::Vector2f posP = mKeySprites[4].getPosition();
        sf::Vector2f posEsc = mKeySprites[5].getPosition();
        std::cout << "   P: X=" << posP.x << ", Y=" << posP.y << std::endl;
        std::cout << "   ESC: X=" << posEsc.x << ", Y=" << posEsc.y << std::endl;
    }
    
    if (mF11LabelIndex < mLabels.size()) {
        sf::Vector2f pos = mLabels[mF11LabelIndex].getPosition();
        std::cout << "5. Texto 'Pantalla': X=" << pos.x << ", Y=" << pos.y << std::endl;
    }
    
    if (mF11KeyIndex < mKeySprites.size()) {
        sf::Vector2f pos = mKeySprites[mF11KeyIndex].getPosition();
        std::cout << "6. Tecla F11: X=" << pos.x << ", Y=" << pos.y << std::endl;
    }
    
    if (mEnterKeyIndex < mKeySprites.size()) {
        sf::Vector2f pos = mKeySprites[mEnterKeyIndex].getPosition();
        std::cout << "7. Tecla Enter: X=" << pos.x << ", Y=" << pos.y << std::endl;
    }
    
    std::cout << "===================================" << std::endl;
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
    
    // Dibujar hitboxes si está en modo debug
    if (mDebugMode) {
        // Hitbox para label "Movimiento" (1)
        if (mMoveLabelIndex < mLabels.size()) {
            sf::FloatRect bounds = mLabels[mMoveLabelIndex].getGlobalBounds();
            sf::RectangleShape hitbox;
            hitbox.setPosition(bounds.left, bounds.top);
            hitbox.setSize(sf::Vector2f(bounds.width, bounds.height));
            hitbox.setFillColor(sf::Color(255, 255, 0, mSelectedElement == 1 ? 120 : 60));
            hitbox.setOutlineThickness(2.0f);
            hitbox.setOutlineColor(mSelectedElement == 1 ? sf::Color::Yellow : sf::Color(255, 255, 0, 150));
            window.draw(hitbox);
        }
        
        // Hitbox para flechas (2) - combinar todas
        if (mKeySprites.size() >= 4) {
            float minX = 99999.0f, minY = 99999.0f, maxX = -99999.0f, maxY = -99999.0f;
            for (int i = 0; i < 4; i++) {
                sf::FloatRect bounds = mKeySprites[i].getGlobalBounds();
                minX = std::min(minX, bounds.left);
                minY = std::min(minY, bounds.top);
                maxX = std::max(maxX, bounds.left + bounds.width);
                maxY = std::max(maxY, bounds.top + bounds.height);
            }
            sf::RectangleShape hitbox;
            hitbox.setPosition(minX, minY);
            hitbox.setSize(sf::Vector2f(maxX - minX, maxY - minY));
            hitbox.setFillColor(sf::Color(0, 255, 255, mSelectedElement == 2 ? 120 : 60));
            hitbox.setOutlineThickness(2.0f);
            hitbox.setOutlineColor(mSelectedElement == 2 ? sf::Color::Cyan : sf::Color(0, 255, 255, 150));
            window.draw(hitbox);
        }
        
        // Hitbox para label "Pausa" (3)
        if (mPauseLabelIndex < mLabels.size()) {
            sf::FloatRect bounds = mLabels[mPauseLabelIndex].getGlobalBounds();
            sf::RectangleShape hitbox;
            hitbox.setPosition(bounds.left, bounds.top);
            hitbox.setSize(sf::Vector2f(bounds.width, bounds.height));
            hitbox.setFillColor(sf::Color(255, 0, 255, mSelectedElement == 3 ? 120 : 60));
            hitbox.setOutlineThickness(2.0f);
            hitbox.setOutlineColor(mSelectedElement == 3 ? sf::Color::Magenta : sf::Color(255, 0, 255, 150));
            window.draw(hitbox);
        }
        
        // Hitbox para teclas P/ESC (4) - combinar
        if (mKeySprites.size() >= 6) {
            float minX = 99999.0f, minY = 99999.0f, maxX = -99999.0f, maxY = -99999.0f;
            for (int i = 4; i < 6; i++) {
                sf::FloatRect bounds = mKeySprites[i].getGlobalBounds();
                minX = std::min(minX, bounds.left);
                minY = std::min(minY, bounds.top);
                maxX = std::max(maxX, bounds.left + bounds.width);
                maxY = std::max(maxY, bounds.top + bounds.height);
            }
            sf::RectangleShape hitbox;
            hitbox.setPosition(minX, minY);
            hitbox.setSize(sf::Vector2f(maxX - minX, maxY - minY));
            hitbox.setFillColor(sf::Color(255, 128, 0, mSelectedElement == 4 ? 120 : 60));
            hitbox.setOutlineThickness(2.0f);
            hitbox.setOutlineColor(mSelectedElement == 4 ? sf::Color(255, 128, 0) : sf::Color(255, 128, 0, 150));
            window.draw(hitbox);
        }
        
        // Hitbox para label "Pantalla" (5)
        if (mF11LabelIndex < mLabels.size()) {
            sf::FloatRect bounds = mLabels[mF11LabelIndex].getGlobalBounds();
            sf::RectangleShape hitbox;
            hitbox.setPosition(bounds.left, bounds.top);
            hitbox.setSize(sf::Vector2f(bounds.width, bounds.height));
            hitbox.setFillColor(sf::Color(0, 255, 0, mSelectedElement == 5 ? 120 : 60));
            hitbox.setOutlineThickness(2.0f);
            hitbox.setOutlineColor(mSelectedElement == 5 ? sf::Color::Green : sf::Color(0, 255, 0, 150));
            window.draw(hitbox);
        }
        
        // Hitbox para tecla F11 (6)
        if (mF11KeyIndex < mKeySprites.size()) {
            sf::FloatRect bounds = mKeySprites[mF11KeyIndex].getGlobalBounds();
            sf::RectangleShape hitbox;
            hitbox.setPosition(bounds.left, bounds.top);
            hitbox.setSize(sf::Vector2f(bounds.width, bounds.height));
            hitbox.setFillColor(sf::Color(128, 0, 255, mSelectedElement == 6 ? 120 : 60));
            hitbox.setOutlineThickness(2.0f);
            hitbox.setOutlineColor(mSelectedElement == 6 ? sf::Color(128, 0, 255) : sf::Color(128, 0, 255, 150));
            window.draw(hitbox);
        }
        
        // Hitbox para tecla Enter (7)
        if (mEnterKeyIndex < mKeySprites.size()) {
            sf::FloatRect bounds = mKeySprites[mEnterKeyIndex].getGlobalBounds();
            sf::RectangleShape hitbox;
            hitbox.setPosition(bounds.left, bounds.top);
            hitbox.setSize(sf::Vector2f(bounds.width, bounds.height));
            hitbox.setFillColor(sf::Color(255, 255, 255, mSelectedElement == 7 ? 120 : 60));
            hitbox.setOutlineThickness(2.0f);
            hitbox.setOutlineColor(mSelectedElement == 7 ? sf::Color::White : sf::Color(255, 255, 255, 150));
            window.draw(hitbox);
        }
    }
}
