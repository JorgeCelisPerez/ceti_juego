#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class GameOverScreen {
public:
    GameOverScreen(float width, float height);

    void update(sf::Time dt);
    void draw(sf::RenderWindow& window);
    void reset();
    bool isDone() const;
    void resize(float width, float height);
    
    void moveUp();
    void moveDown();
    int getSelectedItem() const;
    void setScore(int score);

private:
    void setupText(float width, float height);
    void updateSelection();

    sf::Font mFont;
    sf::Text mGameOverText;
    sf::Text mScoreText;
    std::vector<sf::Text> mMenuItems;
    sf::RectangleShape mOverlay;

    sf::Time mTimer;
    sf::Time mDuration;
    bool mFinished;
    
    int mSelectedItemIndex;
    sf::Color mButtonIdleColor;
    sf::Color mButtonHoverColor;
};
