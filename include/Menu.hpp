#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Menu {
public:
    enum class MenuAction { None, Play, Exit };

    Menu(sf::RenderWindow& window);
    void draw();
    MenuAction handleEvent(sf::Event& event);
    void update();
    void resize();

private:
    void moveUp();
    void moveDown();
    void setupUI();

    sf::RenderWindow& mWindow;
    sf::Font mFont;
    sf::Texture mBackgroundTexture;
    sf::Sprite mBackgroundSprite;
    sf::Texture mLogoTexture;
    sf::Sprite mLogoSprite;

    sf::RectangleShape mOverlay;

    std::vector<sf::Text> mMenuItems;
    int mSelectedItemIndex;

    sf::Color mButtonIdleColor;
    sf::Color mButtonHoverColor;
};
