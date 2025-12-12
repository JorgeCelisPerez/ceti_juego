#pragma once
#include <SFML/Graphics.hpp>

class HighScore {
public:
    HighScore();
    
    void checkAndUpdate(int currentScore);
    int getHighScore() const;
    void reset(); // Para resetear completamente
    
    void update(float windowWidth, float windowHeight);
    void update(float windowWidth, float windowHeight, float offsetX, float offsetY);
    void draw(sf::RenderWindow& window);
    void drawDebug(sf::RenderWindow& window);
    void setBaseResolution(float width, float height);
    
private:
    void updateTextPosition(float windowWidth, float windowHeight);
    void updateValue();
    
    sf::Font mFont;
    sf::Text mHighScoreLabel;
    sf::Text mHighScoreValue;
    int mHighScore;
    float mBaseResolutionWidth;
    float mBaseResolutionHeight;
};
