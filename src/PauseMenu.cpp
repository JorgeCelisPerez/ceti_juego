#include "PauseMenu.hpp"
#include <iostream>
#include <cstdlib>

PauseMenu::PauseMenu(sf::RenderWindow& window) 
    : mWindow(window),
      mButtonIdleColor(sf::Color::White),
      mButtonHoverColor(sf::Color(255, 204, 0)),
      mSelectedItemIndex(0) {
    
    if (!mFont.loadFromFile("assets/fonts/Speed Rusher.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente para PauseMenu" << std::endl;
        if (!mFont.loadFromFile("assets/fonts/arial.ttf")) {
            std::cerr << "Error: No se pudo cargar fuente de fallback" << std::endl;
            exit(1);
        }
    }
    
    setupUI();
    update();
}

void PauseMenu::setupUI() {
    sf::Vector2u windowSize = mWindow.getSize();
    float winWidth = static_cast<float>(windowSize.x);
    float winHeight = static_cast<float>(windowSize.y);
    
    // Overlay semi-transparente
    mOverlay.setSize(sf::Vector2f(winWidth, winHeight));
    mOverlay.setFillColor(sf::Color(0, 0, 0, 180));
    
    // Configurar título "PAUSA"
    mTitle.setFont(mFont);
    mTitle.setString("PAUSA");
    mTitle.setCharacterSize(80);
    mTitle.setFillColor(sf::Color::White);
    mTitle.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = mTitle.getLocalBounds();
    mTitle.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
    mTitle.setPosition(winWidth / 2.0f, winHeight * 0.25f);
    
    // Limpiar items existentes
    mMenuItems.clear();
    
    // Configurar opciones del menú
    std::vector<std::string> itemLabels = {"Reanudar", "Reiniciar", "Salir al Menu"};
    float startY = winHeight / 2.0f - 50.0f;
    float spacing = 110.0f;
    
    for (size_t i = 0; i < itemLabels.size(); i++) {
        sf::Text item;
        item.setFont(mFont);
        item.setString(itemLabels[i]);
        item.setCharacterSize(50);
        sf::FloatRect itemBounds = item.getLocalBounds();
        item.setOrigin(itemBounds.left + itemBounds.width / 2.0f, itemBounds.top + itemBounds.height / 2.0f);
        item.setPosition(winWidth / 2.0f, startY + (i * spacing));
        mMenuItems.push_back(item);
    }
}

void PauseMenu::resize() {
    setupUI();
    update();
}

void PauseMenu::update() {
    // Actualizar colores según selección
    for (size_t i = 0; i < mMenuItems.size(); i++) {
        if (static_cast<int>(i) == mSelectedItemIndex) {
            mMenuItems[i].setFillColor(mButtonHoverColor);
        } else {
            mMenuItems[i].setFillColor(mButtonIdleColor);
        }
    }
}

void PauseMenu::moveUp() {
    if (mSelectedItemIndex > 0) {
        mSelectedItemIndex--;
        update();
    }
}

void PauseMenu::moveDown() {
    if (mSelectedItemIndex < static_cast<int>(mMenuItems.size()) - 1) {
        mSelectedItemIndex++;
        update();
    }
}

int PauseMenu::getSelectedItem() const {
    return mSelectedItemIndex;
}

void PauseMenu::draw() {
    mWindow.draw(mOverlay);
    mWindow.draw(mTitle);
    for (const auto& item : mMenuItems) {
        mWindow.draw(item);
    }
}
