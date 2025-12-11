#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Teclado.hpp"
#include "Enemigo.hpp"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time dt);
    void render();
    void updateRoadScale();
    void clampPlayer();

private:
    // --- Variables de la Ventana ---
    sf::RenderWindow mWindow;
    bool mIsFullscreen;
    
    // --- Recursos (Assets) ---
    sf::Texture mRoadTexture;
    
    // --- Entidades del Juego ---
    // Usamos dos sprites para el fondo infinito
    sf::Sprite mRoad1;
    sf::Sprite mRoad2;
    sf::RectangleShape mPlayer;
    
    // --- Variables de Lógica ---
    float mScrollSpeed;
    float mTextureHeight;
    float mPlayerSpeed;
    float mRoadMarginTexturePx; // margen lateral en píxeles de la textura original
    float mPlayableLeft;
    float mPlayableRight;
    bool mDebugBounds;
    
    // --- Enemigos y Carriles ---
    static const int NUM_CARRILES = 4;
    float mCarrilAncho;
    float mCarrilLimits[NUM_CARRILES + 1];
    std::vector<Enemy> mEnemigos;
    float mSpawnTimer;
    float mSpawnInterval;
};