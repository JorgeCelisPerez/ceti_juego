#pragma once
#include <SFML/Graphics.hpp>

class Score {
public:
    Score();
    
    void addPoints(int points);
    void reset();
    int getScore() const;
    
    void update(float windowWidth, float windowHeight);
    void update(float windowWidth, float windowHeight, float offsetX, float offsetY);
    void draw(sf::RenderWindow& window);
    void drawDebug(sf::RenderWindow& window);
    
private:
    void updateTextPosition(float windowWidth, float windowHeight);
    
    sf::Font mFont;
    sf::Text mScoreLabel;
    sf::Text mScoreValue;
    int mCurrentScore;
};
