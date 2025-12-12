#include "Menu.hpp"
#include <iostream>
#include <cstdlib>
#include <algorithm>

Menu::Menu(sf::RenderWindow& window) 
    : mWindow(window), 
      mButtonIdleColor(sf::Color::White), 
      mButtonHoverColor(sf::Color(255, 204, 0)),
      mSelectedItemIndex(0) {
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

    setupUI();
    update(); // Llama inicial para colorear el primer item
}

void Menu::setupUI() {
    sf::Vector2u windowSize = mWindow.getSize();
    float winWidth = static_cast<float>(windowSize.x);
    float winHeight = static_cast<float>(windowSize.y);

    mBackgroundSprite.setTexture(mBackgroundTexture);
    float scale = std::max(winWidth / mBackgroundSprite.getLocalBounds().width, winHeight / mBackgroundSprite.getLocalBounds().height);
    mBackgroundSprite.setScale(scale, scale);
    mBackgroundSprite.setPosition(0, 0);

    sf::Vector2f panelSize(winWidth * 0.4f, winHeight * 0.5f);
    mPanel.setSize(panelSize);
    mPanel.setFillColor(sf::Color(70, 130, 180, 240));
    mPanel.setOrigin(panelSize.x / 2, panelSize.y / 2);
    mPanel.setPosition(winWidth / 2, winHeight / 2);

    mPanelShadow.setSize(panelSize);
    mPanelShadow.setFillColor(sf::Color(0, 0, 0, 100));
    mPanelShadow.setOrigin(panelSize.x / 2, panelSize.y / 2);
    mPanelShadow.setPosition(mPanel.getPosition().x + 10, mPanel.getPosition().y + 10);
    
    // Configurar y añadir "Empezar"
    sf::Text playButton;
    playButton.setFont(mFont);
    playButton.setString("Empezar");
    playButton.setCharacterSize(50);
    sf::FloatRect playBounds = playButton.getLocalBounds();
    playButton.setOrigin(playBounds.left + playBounds.width / 2.0f, playBounds.top + playBounds.height / 2.0f);
    playButton.setPosition(winWidth / 2, winHeight / 2 - 50);
    mMenuItems.push_back(playButton);

    // Configurar y añadir "Salir"
    sf::Text exitButton;
    exitButton.setFont(mFont);
    exitButton.setString("Salir");
    exitButton.setCharacterSize(50);
    sf::FloatRect exitBounds = exitButton.getLocalBounds();
    exitButton.setOrigin(exitBounds.left + exitBounds.width / 2.0f, exitBounds.top + exitBounds.height / 2.0f);
    exitButton.setPosition(winWidth / 2, winHeight / 2 + 50);
    mMenuItems.push_back(exitButton);
}

void Menu::draw() {
    mWindow.draw(mBackgroundSprite);
    mWindow.draw(mPanelShadow);
    mWindow.draw(mPanel);
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
