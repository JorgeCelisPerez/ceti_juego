#pragma once
#include <SFML/Graphics.hpp>

class HighScore {
public:
    HighScore();
    
    void checkAndUpdate(int currentScore);
    int getHighScore() const;
    void reset(); // Para resetear completamente
    bool isNewRecord() const;  // Verifica si el último score fue un nuevo récord
    void resetHighScore();  // Resetear el high score a 0
    
    void save();  // Guardar en archivo
    void load();  // Cargar desde archivo
    
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
    bool mIsNewRecord;  // Flag para saber si hubo nuevo récord
    float mBaseResolutionWidth;
    float mBaseResolutionHeight;
};
