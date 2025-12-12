#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class ControlsDisplay {
public:
    ControlsDisplay();
    
    void draw(sf::RenderWindow& window);
    void updatePosition(float windowWidth, float windowHeight);
    void setBaseResolution(float width, float height);
    void setDebugMode(bool debug);
    
private:
    void setupLayout(float windowWidth, float windowHeight);
    
    sf::Font mFont;
    sf::RectangleShape mBackground;
    
    // Texturas y sprites para las teclas
    sf::Texture mArrowUpTexture;
    sf::Texture mArrowDownTexture;
    sf::Texture mArrowLeftTexture;
    sf::Texture mArrowRightTexture;
    sf::Texture mPTexture;
    sf::Texture mEscTexture;
    sf::Texture mF11Texture;
    sf::Texture mEnterTexture;
    
    std::vector<sf::Sprite> mKeySprites;
    std::vector<sf::Text> mLabels;
    
    float mBaseResolutionWidth;
    float mBaseResolutionHeight;
    bool mDebugMode;
};
