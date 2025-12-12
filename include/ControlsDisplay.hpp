#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class ControlsDisplay {
public:
    ControlsDisplay();
    
    void draw(sf::RenderWindow& window);
    void updatePosition(float windowWidth, float windowHeight);
    void setBaseResolution(float width, float height);
    
    // Métodos para edición manual
    void setDebugMode(bool debug);
    void selectElement(int elementIndex);
    void moveSelected(float offsetX, float offsetY);
    void printCoordinates() const;
    
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
    
    // Variables para edición manual
    bool mDebugMode;
    int mSelectedElement; // 0=none, 1=movLabel, 2=arrows, 3=pauseLabel, 4=pauseKeys, 5=f11Label, 6=f11Key, 7=enterKey
    
    // Indices para identificar elementos en vectores
    int mMoveLabelIndex;
    int mArrowStartIndex;
    int mPauseLabelIndex;
    int mPauseKeysStartIndex;
    int mF11LabelIndex;
    int mF11KeyIndex;
    int mEnterKeyIndex;
};
