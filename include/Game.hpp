#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Teclado.hpp"
#include "Enemigo.hpp"
#include "Gasolina.hpp"
#include "Colisiones.hpp"
#include "Menu.hpp"
#include "GameOver.hpp"
#include "Score.hpp"
#include "HighScore.hpp"
#include "Difficulty.hpp"
#include "PauseMenu.hpp"
#include "ControlsDisplay.hpp"
#include "GestorTemas.hpp"
#include "Countdown.hpp"

class Game {
public:
    Game();
    void run();
    void resetHighScore();  // Método público para resetear high score

private:
    void processEvents();
    void update(sf::Time dt);
    void render();

    void startGame();
    void updateRoadScale();
    void clampPlayer();
    void updateGasolinaBar();
    void toggleFullscreen();

private:
    enum class GameState { Menu, Countdown, Playing, Paused, GameOver };
    GameState mGameState;
    
    // --- Countdown ---
    Countdown mCountdown;

    // --- Ventana y Pantallas ---
    sf::RenderWindow mWindow;
    Menu mMenu;
    GameOverScreen mGameOverScreen;
    PauseMenu mPauseMenu;
    Score mScore;
    HighScore mHighScore;
    ControlsDisplay mControlsDisplay;
    DifficultyManager mDifficulty;
    GestorTemas mGestorTemas;
    bool mIsFullscreen;
    
    // --- Recursos (Assets) ---
    sf::Texture mRoadTexture;
    sf::Texture mGasolinaTexture;
    sf::Texture mBarBackgroundTexture;
    sf::Texture mBarGlassTexture;
    sf::Texture mRedBarTexture;
    sf::Texture mPlayerTexture;
    sf::Texture mTruckTexture;
    std::vector<sf::Texture> mEnemyTextures;
    
    // --- Entidades del Juego ---
    sf::Sprite mRoad1;
    sf::Sprite mRoad2;
    sf::Sprite mPlayer;
    
    // --- Variables de Lógica ---
    float mScrollSpeed;
    float mTextureHeight;
    float mPlayerSpeed;
    float mRoadMarginTexturePx;
    float mPlayableLeft;
    float mPlayableRight;
    bool mDebugBounds;
    
    // --- Lógica de Colisión con Límites ---
    float mBoundaryPenalty;
    float mBoundaryContinuousPenalty;
    bool mIsTouchingBoundary;
    
    // --- Escalado de Coches ---
    sf::Vector2f mBaseCarSize;
    float mReferenceWidth;
    float mCarScaleFactor;
    
    // --- Enemigos y Carriles ---
    static const int NUM_CARRILES = 4;
    float mCarrilAncho;
    float mCarrilLimits[NUM_CARRILES + 1];
    std::vector<Enemy> mEnemigos;
    float mSpawnTimer;
    float mSpawnInterval;
    float mTruckSpawnTimer;
    float mTruckSpawnInterval;
    
    // --- Sistema de Gasolina ---
    std::vector<Gasolina> mGasolinas;
    float mGasolinaSpawnTimer;
    float mGasolinaSpawnInterval;
    float mGasolinaActual;
    float mGasolinaMax;
    float mGasolinaConsumoRate;
    int mHighScoreInicial;  // Para detectar nuevo récord al final
    
    // --- UI Barra de Gasolina ---
    sf::Sprite mBarBackground;
    sf::Sprite mBarGlass;
    sf::Sprite mRedBar;
    sf::Vector2f mBarPosition;
    sf::Vector2f mBarSize;
    
    // --- Texto Combustible ---
    sf::Font mCombustibleFont;
    sf::Text mCombustibleText;
    float mCombustibleOffsetX;
    float mCombustibleOffsetY;
    
    // --- Sistema de Puntuación ---
    float mScoreAccumulator;
    
    // --- Ajustes de UI (para debug) ---
    float mScoreOffsetX;
    float mScoreOffsetY;
    float mBarOffsetX;
    float mBarOffsetY;
    float mHighScoreOffsetX;
    float mHighScoreOffsetY;
    
    // --- Valores base para escalado de offsets ---
    float mBaseScoreOffsetX;
    float mBaseScoreOffsetY;
    float mBaseHighScoreOffsetX;
    float mBaseHighScoreOffsetY;
    float mBaseResolutionWidth;
    float mBaseResolutionHeight;
    
    // --- Ajustes de carriles (para debug) ---
    float mCarrilesLeftOffset;    // Offset del inicio de los carriles
    float mCarrilesRightOffset;   // Offset del fin de los carriles
    float mDivision1Offset;       // Offset de la primera división
    float mDivision2Offset;       // Offset de la segunda división (centro)
    float mDivision3Offset;       // Offset de la tercera división
};
