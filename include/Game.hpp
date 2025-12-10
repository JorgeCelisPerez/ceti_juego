#pragma once
#include <SFML/Graphics.hpp>

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time dt);
    void render();

private:
    // --- Variables de la Ventana ---
    sf::RenderWindow mWindow;
    
    // --- Recursos (Assets) ---
    sf::Texture mRoadTexture;
    
    // --- Entidades del Juego ---
    // Usamos dos sprites para el fondo infinito
    sf::Sprite mRoad1;
    sf::Sprite mRoad2;
    
    // --- Variables de Lógica ---
    float mScrollSpeed;
    float mTextureHeight;
};