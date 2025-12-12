#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class PauseMenu {
public:
    PauseMenu(sf::RenderWindow& window);
    
    void draw();
    void moveUp();
    void moveDown();
    int getSelectedItem() const;
    void resize();
    
private:
    void setupUI();
    void update();
    
    sf::RenderWindow& mWindow;
    sf::Font mFont;
    sf::Text mTitle;
    std::vector<sf::Text> mMenuItems;
    sf::RectangleShape mOverlay;
    
    int mSelectedItemIndex;
    sf::Color mButtonIdleColor;
    sf::Color mButtonHoverColor;
};
