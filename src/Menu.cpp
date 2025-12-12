#include "Menu.hpp"
#include <iostream>
#include <cstdlib>
#include <algorithm>

Menu::Menu(sf::RenderWindow& window) 
    : mWindow(window), 
      mButtonIdleColor(sf::Color::White), 
      mButtonHoverColor(sf::Color(255, 204, 0)),
      mSelectedItemIndex(0),
      mBaseResolutionWidth(static_cast<float>(sf::VideoMode::getDesktopMode().width)),
      mBaseResolutionHeight(static_cast<float>(sf::VideoMode::getDesktopMode().height)) {
    if (!mFont.loadFromFile("assets/fonts/Speed Rusher.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente assets/fonts/Speed Rusher.ttf" << std::endl;
        if (!mFont.loadFromFile("assets/fonts/arial.ttf")) {
            std::cerr << "Error: No se pudo cargar la fuente de fallback arial.ttf" << std::endl;
            exit(1);
        }
    }

    if (!mBackgroundTexture.loadFromFile("assets/images/roadborroso.png")) {
        std::cerr << "Error: No se pudo cargar la textura assets/images/roadborroso.png" << std::endl;
        exit(1);
    }

    if (!mLogoTexture.loadFromFile("assets/images/Logo.png")) {
        std::cerr << "Error: No se pudo cargar la textura assets/images/Logo.png" << std::endl;
        exit(1);
    }

    setupUI();
    update(); // Llama inicial para colorear el primer item
}

void Menu::resize() {
    setupUI();
    update();
}

void Menu::setupUI() {
    sf::Vector2u windowSize = mWindow.getSize();
    float winWidth = static_cast<float>(windowSize.x);
    float winHeight = static_cast<float>(windowSize.y);

    mBackgroundSprite.setTexture(mBackgroundTexture);
    float scale = std::max(winWidth / mBackgroundSprite.getLocalBounds().width, winHeight / mBackgroundSprite.getLocalBounds().height);
    mBackgroundSprite.setScale(scale, scale);
    mBackgroundSprite.setPosition(0, 0);

    // Overlay semi-transparente
    mOverlay.setSize(sf::Vector2f(winWidth, winHeight));
    mOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    // Configurar logo grande centrado en la pantalla
    mLogoSprite.setTexture(mLogoTexture);
    sf::FloatRect logoBounds = mLogoSprite.getLocalBounds();
    float logoScale = std::min(winWidth * 0.9f / logoBounds.width, winHeight * 0.8f / logoBounds.height);
    mLogoSprite.setScale(logoScale, logoScale);
    mLogoSprite.setOrigin(logoBounds.width / 2.0f, logoBounds.height / 2.0f);
    mLogoSprite.setPosition(winWidth / 2.0f, winHeight / 2.0f);
    
    // Limpiar items existentes antes de recrearlos
    mMenuItems.clear();
    
    // Calcular escala para el tamaño de fuente
    float scaleY = winHeight / mBaseResolutionHeight;
    unsigned int buttonSize = static_cast<unsigned int>(60.0f * scaleY);
    
    // Configurar y añadir "Empezar" (a la derecha)
    sf::Text playButton;
    playButton.setFont(mFont);
    playButton.setString("Empezar");
    playButton.setCharacterSize(buttonSize);
    sf::FloatRect playBounds = playButton.getLocalBounds();
    playButton.setOrigin(playBounds.left + playBounds.width / 2.0f, playBounds.top + playBounds.height / 2.0f);
    playButton.setPosition(winWidth * 0.85f, winHeight * 0.45f);
    mMenuItems.push_back(playButton);

    // Configurar y añadir "Salir"
    sf::Text exitButton;
    exitButton.setFont(mFont);
    exitButton.setString("Salir");
    exitButton.setCharacterSize(buttonSize);
    sf::FloatRect exitBounds = exitButton.getLocalBounds();
    exitButton.setOrigin(exitBounds.left + exitBounds.width / 2.0f, exitBounds.top + exitBounds.height / 2.0f);
    exitButton.setPosition(winWidth * 0.85f, winHeight * 0.6f);
    mMenuItems.push_back(exitButton);
}

void Menu::draw() {
    mWindow.draw(mBackgroundSprite);
    mWindow.draw(mOverlay);
    mWindow.draw(mLogoSprite);
    for (const auto& item : mMenuItems) {
        mWindow.draw(item);
    }
}

Menu::MenuAction Menu::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
                moveUp();
                break;
            case sf::Keyboard::Down:
                moveDown();
                break;
            case sf::Keyboard::Enter:
                if (mSelectedItemIndex == 0) {
                    return MenuAction::Play;
                }
                if (mSelectedItemIndex == 1) {
                    return MenuAction::Exit;
                }
                break;
            default:
                break;
        }
    }
    return MenuAction::None;
}

void Menu::update() {
    for (size_t i = 0; i < mMenuItems.size(); ++i) {
        if (i == mSelectedItemIndex) {
            mMenuItems[i].setFillColor(mButtonHoverColor);
        } else {
            mMenuItems[i].setFillColor(mButtonIdleColor);
        }
    }
}

void Menu::moveUp() {
    if (mSelectedItemIndex > 0) {
        mSelectedItemIndex--;
    } else {
        mSelectedItemIndex = mMenuItems.size() - 1;
    }
    update();
}

void Menu::moveDown() {
    if (mSelectedItemIndex < mMenuItems.size() - 1) {
        mSelectedItemIndex++;
    } else {
        mSelectedItemIndex = 0;
    }
    update();
}
