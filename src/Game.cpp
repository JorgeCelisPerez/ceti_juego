#include "Game.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>

Game::Game()
    : mWindow(sf::VideoMode::getDesktopMode(), "Traffic Racer", sf::Style::Fullscreen),
      mMenu(mWindow),
      mGameOverScreen(mWindow.getSize().x, mWindow.getSize().y), // Inicializa la nueva clase
      mPauseMenu(mWindow),
      mGameState(GameState::Menu),
      mIsFullscreen(true),
      mScrollSpeed(550.f),
      mPlayerSpeed(600.f),
      mRoadMarginTexturePx(800.f),
      mDebugBounds(false),
      mBaseCarSize(105.f, 180.f),  // Aumentado para que las imágenes sean más visibles
      mReferenceWidth(0.f),
      mCarScaleFactor(1.f),
      mCarrilAncho(0.f),
      mSpawnInterval(1.5f),
      mTruckSpawnTimer(0.f),
      mTruckSpawnInterval(12.0f),
      mGasolinaSpawnInterval(3.0f),
      mGasolinaMax(100.0f),
      mGasolinaConsumoRate(5.0f),
      mHighScoreInicial(0),
      mBoundaryPenalty(12.0f),
      mBoundaryContinuousPenalty(18.0f),
      mScoreAccumulator(0.0f),
      mScoreOffsetX(-100.0f),
      mScoreOffsetY(20.0f),
      mBarOffsetX(45.0f),
      mBarOffsetY(-10.0f),
      mHighScoreOffsetX(40.0f),
      mHighScoreOffsetY(20.0f),
      mBaseScoreOffsetX(-100.0f),
      mBaseScoreOffsetY(20.0f),
      mBaseHighScoreOffsetX(40.0f),
      mBaseHighScoreOffsetY(20.0f),
      mBaseResolutionWidth(static_cast<float>(sf::VideoMode::getDesktopMode().width)),
      mBaseResolutionHeight(static_cast<float>(sf::VideoMode::getDesktopMode().height)),
      mCarrilesLeftOffset(30.0f),
      mCarrilesRightOffset(-30.0f),
      mDivision1Offset(-6.0f),
      mDivision2Offset(-2.0f),
      mDivision3Offset(6.0f),
      mCombustibleOffsetX(0.0f),
      mCombustibleOffsetY(160.0f)
{
    // Establecer resolución base para escalado de UI
    mScore.setBaseResolution(mBaseResolutionWidth, mBaseResolutionHeight);
    mHighScore.setBaseResolution(mBaseResolutionWidth, mBaseResolutionHeight);
    mControlsDisplay.setBaseResolution(mBaseResolutionWidth, mBaseResolutionHeight);
    
    mWindow.setVerticalSyncEnabled(true);
    srand(static_cast<unsigned int>(time(NULL)));

    if (!mRoadTexture.loadFromFile("assets/images/road.png")) { exit(1); }
    if (!mGasolinaTexture.loadFromFile("assets/images/Bidon gas.png")) { exit(1); }
    if (!mBarBackgroundTexture.loadFromFile("assets/images/BarBackground.png")) { exit(1); }
    if (!mBarGlassTexture.loadFromFile("assets/images/BarGlass.png")) { exit(1); }
    if (!mRedBarTexture.loadFromFile("assets/images/RedBar.png")) { exit(1); }
    if (!mPlayerTexture.loadFromFile("assets/images/Black_viper.png")) { exit(1); }
    if (!mTruckTexture.loadFromFile("assets/images/truck.png")) { exit(1); }
    
    // Cargar fuente para texto Combustible
    if (!mCombustibleFont.loadFromFile("assets/fonts/Speed Rusher.ttf")) {
        if (!mCombustibleFont.loadFromFile("assets/fonts/arial.ttf")) { exit(1); }
    }
    
    // Configurar texto Combustible
    mCombustibleText.setFont(mCombustibleFont);
    mCombustibleText.setString("Combustible");
    mCombustibleText.setCharacterSize(35);
    mCombustibleText.setFillColor(sf::Color::Black);
    
    // Inicializar Countdown
    mCountdown.init(mCombustibleFont);
    
    // Inicializar SoundManager
    mSoundManager.init();
    
    // Iniciar música de fondo en el menú (volumen alto 85%)
    mSoundManager.startBackgroundMusic();
    mSoundManager.setBackgroundMusicVolume(85.0f);
    
    // Cargar texturas de enemigos
    mEnemyTextures.resize(7);
    if (!mEnemyTextures[0].loadFromFile("assets/images/Ambulance.png")) { exit(1); }
    if (!mEnemyTextures[1].loadFromFile("assets/images/Audi.png")) { exit(1); }
    if (!mEnemyTextures[2].loadFromFile("assets/images/Car.png")) { exit(1); }
    if (!mEnemyTextures[3].loadFromFile("assets/images/Mini_truck.png")) { exit(1); }
    if (!mEnemyTextures[4].loadFromFile("assets/images/Mini_van.png")) { exit(1); }
    if (!mEnemyTextures[5].loadFromFile("assets/images/Police.png")) { exit(1); }
    if (!mEnemyTextures[6].loadFromFile("assets/images/taxi.png")) { exit(1); }

    mRoad1.setTexture(mGestorTemas.getTexturaActual());
    mRoad2.setTexture(mGestorTemas.getTexturaActual());
    
    mReferenceWidth = static_cast<float>(mWindow.getSize().x);

    mPlayer.setTexture(mPlayerTexture);
    sf::FloatRect playerBounds = mPlayer.getLocalBounds();
    float playerScaleX = mBaseCarSize.x / playerBounds.width;
    float playerScaleY = mBaseCarSize.y / playerBounds.height;
    mPlayer.setScale(playerScaleX, playerScaleY);
    mPlayer.setOrigin(playerBounds.width * 0.5f, playerBounds.height * 0.5f);

    mBarBackground.setTexture(mBarBackgroundTexture);
    mBarGlass.setTexture(mBarGlassTexture);
    mRedBar.setTexture(mRedBarTexture);
    
    updateRoadScale();
    
    float windowWidth = static_cast<float>(mWindow.getSize().x);
    float windowHeight = static_cast<float>(mWindow.getSize().y);
    mControlsDisplay.updatePosition(windowWidth, windowHeight);
}

void Game::resetHighScore() {
    mHighScore.resetHighScore();
}

void Game::startGame() {
    mGameState = GameState::Countdown;
    mSoundManager.playEngineRoaringSound();  // Sonido de motor rugiendo al empezar
    mCountdown.start(mSoundManager);  // Iniciar countdown con sonido
    mSoundManager.startEngineLoop();  // Iniciar sonido de motor
    mSoundManager.startBackgroundMusic();  // Reiniciar música de fondo
    mSoundManager.setBackgroundMusicVolume(25.0f);  // Bajar música durante el juego
    mGasolinaActual = mGasolinaMax;
    mHighScoreInicial = mHighScore.getHighScore();  // Guardar high score antes de empezar
    mIsTouchingBoundary = false;
    mSpawnTimer = 0.f;
    mGasolinaSpawnTimer = 0.f;
    mScoreAccumulator = 0.0f;
    
    mScore.reset();

    mEnemigos.clear();
    mGasolinas.clear();

    updateRoadScale();
    
    float windowWidth = static_cast<float>(mWindow.getSize().x);
    float windowHeight = static_cast<float>(mWindow.getSize().y);
    mPlayer.setPosition(windowWidth * 0.5f, windowHeight * 0.75f);
    clampPlayer();
    
    mScore.update(windowWidth, windowHeight, mScoreOffsetX, mScoreOffsetY);
    mHighScore.update(windowWidth, windowHeight, mHighScoreOffsetX, mHighScoreOffsetY);
    updateGasolinaBar();
}

void Game::run() {
    sf::Clock clock;
    while (mWindow.isOpen()) {
        sf::Time dt = clock.restart();
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mWindow.close();
        }

        switch (mGameState) {
            case GameState::Menu:
                {
                    Menu::MenuAction action = mMenu.handleEvent(event);
                    if (action == Menu::MenuAction::Play) {
                        startGame();
                    } else if (action == Menu::MenuAction::Exit) {
                        mWindow.close();
                    }
                    
                    // Permitir cambio de pantalla completa en el menú
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F11) {
                        toggleFullscreen();
                        mMenu.resize();
                    }
                    
                    // Resetear high score con tecla Delete en el menú
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Delete) {
                        resetHighScore();
                    }
                }
                break;
            case GameState::Countdown:
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        mGameState = GameState::Menu;
                    }
                }
                break;
            case GameState::Playing:
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::F1) {
                        mDebugBounds = !mDebugBounds;
                        mControlsDisplay.setDebugMode(mDebugBounds);
                        if (mDebugBounds) {
                            std::cout << "\n=== MODO DEBUG: Hitboxes activadas ===" << std::endl;
                        } else {
                            std::cout << "\n=== MODO DEBUG: Desactivado ===" << std::endl;
                        }
                    }
                    if (event.key.code == sf::Keyboard::F11) {
                        toggleFullscreen();
                    }
                    
                    // Pausar con ESC o P
                    if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::P) {
                        mGameState = GameState::Paused;
                        mSoundManager.pauseEngineLoop();
                        mSoundManager.pauseCountdownSound();
                        mSoundManager.setBackgroundMusicVolume(85.0f);  // Subir volumen en pausa
                    }
                }
                break;
            case GameState::Paused:
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::P) {
                        mGameState = GameState::Playing;
                        mSoundManager.resumeEngineLoop();
                        mSoundManager.resumeCountdownSound();
                        mSoundManager.setBackgroundMusicVolume(25.0f);  // Bajar volumen al reanudar
                    }
                    if (event.key.code == sf::Keyboard::Up) {
                        mPauseMenu.moveUp();
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        mPauseMenu.moveDown();
                    }
                    if (event.key.code == sf::Keyboard::Enter) {
                        int selected = mPauseMenu.getSelectedItem();
                        switch (selected) {
                            case 0:  // Reanudar
                                mGameState = GameState::Playing;
                                mSoundManager.resumeEngineLoop();
                                mSoundManager.resumeCountdownSound();
                                mSoundManager.setBackgroundMusicVolume(25.0f);  // Bajar volumen al reanudar
                                break;
                            case 1:  // Reiniciar
                                startGame();
                                break;
                            case 2:  // Salir al Menu
                                mGameState = GameState::Menu;
                                mSoundManager.stopEngineLoop();
                                mSoundManager.startBackgroundMusic();  // Reiniciar música
                                mSoundManager.setBackgroundMusicVolume(85.0f);  // Volumen alto en menú
                                break;
                        }
                    }
                    if (event.key.code == sf::Keyboard::F11) {
                        toggleFullscreen();
                    }                    // Resetear high score con tecla Delete
                    if (event.key.code == sf::Keyboard::Delete) {
                        resetHighScore();
                    }                }
                break;
            case GameState::GameOver:
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) {
                        mGameOverScreen.moveUp();
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        mGameOverScreen.moveDown();
                    }
                    if (event.key.code == sf::Keyboard::Enter) {
                        int selected = mGameOverScreen.getSelectedItem();
                        switch (selected) {
                            case 0:  // Reintentar
                                startGame();
                                break;
                            case 1:  // Salir al Menu
                                mGameState = GameState::Menu;
                                mSoundManager.startBackgroundMusic();  // Reiniciar música
                                mSoundManager.setBackgroundMusicVolume(85.0f);  // Volumen alto en menú
                                break;
                        }
                    }
                    if (event.key.code == sf::Keyboard::F11) {
                        toggleFullscreen();
                    }
                }
                break;
        }
    }
}

void Game::update(sf::Time dt) {
    switch (mGameState) {
        case GameState::Menu:
            // No se necesita update por frame, es manejado por eventos
            break;
        case GameState::Countdown:
            {
                float timeSeconds = dt.asSeconds();
                mCountdown.update(timeSeconds);
                
                // Actualizar escala del countdown según el tamaño de ventana
                float windowWidth = static_cast<float>(mWindow.getSize().x);
                float windowHeight = static_cast<float>(mWindow.getSize().y);
                mCountdown.updateScale(windowWidth, windowHeight);
                
                // Cuando termina el countdown, empezar el juego
                if (mCountdown.isFinished()) {
                    mGameState = GameState::Playing;
                }
            }
            break;
        case GameState::Playing:
            {
                float timeSeconds = dt.asSeconds();
                
                mGasolinaActual -= mGasolinaConsumoRate * mDifficulty.getFuelConsumptionMultiplier() * timeSeconds;
                if (mGasolinaActual <= 0) {
                    mGasolinaActual = 0;
                    int scoreActual = mScore.getScore();
                    bool esNuevoRecord = (scoreActual > mHighScoreInicial);
                    mHighScore.checkAndUpdate(scoreActual); // Actualizar high score
                    mGameOverScreen.setNewRecord(esNuevoRecord); // Pasar si es nuevo récord
                    mGameOverScreen.setScore(scoreActual); // Establecer score en pantalla de Game Over
                    mGameOverScreen.reset(); // Usar la nueva clase
                    mGameState = GameState::GameOver;
                    
                    // Reproducir sonido según si es nuevo récord o no
                    if (esNuevoRecord) {
                        mSoundManager.playNewRecordSound();
                    } else {
                        mSoundManager.playGameOverSound();
                    }
                    
                    mSoundManager.stopEngineLoop();  // Detener sonido de motor
                    mSoundManager.stopBackgroundMusic();  // Detener música de fondo
                    return; 
                }

                // Actualizar high score en tiempo real (solo visualmente, no afecta detección de nuevo récord)
                mHighScore.checkAndUpdate(mScore.getScore());

                float moveAmount = mScrollSpeed * timeSeconds;
                mRoad1.move(0.f, moveAmount);
                mRoad2.move(0.f, moveAmount);
                if (mRoad1.getPosition().y >= mTextureHeight) mRoad1.setPosition(0.f, mRoad2.getPosition().y - mTextureHeight);
                if (mRoad2.getPosition().y >= mTextureHeight) mRoad2.setPosition(0.f, mRoad1.getPosition().y - mTextureHeight);

                float input = getHorizontalInput();
                mPlayer.move(input * mPlayerSpeed * timeSeconds, 0.f);

                sf::Vector2f pos = mPlayer.getPosition();
                sf::FloatRect bounds = mPlayer.getGlobalBounds();
                float halfW = bounds.width * 0.5f;
                bool isCurrentlyTouching = (pos.x - halfW < mPlayableLeft) || (pos.x + halfW > mPlayableRight);

                if (isCurrentlyTouching) {
                    // Si ya estaba tocando, aplica la penalización continua.
                    // Si es la primera vez, aplica la penalización inicial.
                    mGasolinaActual -= (mIsTouchingBoundary ? mBoundaryContinuousPenalty * timeSeconds : mBoundaryPenalty);
                    
                    // Reproducir sonido solo en el primer contacto
                    if (!mIsTouchingBoundary) {
                        mSoundManager.playCrashSound();
                    }
                }
                mIsTouchingBoundary = isCurrentlyTouching;

                // Ahora, después de verificar y aplicar la penalización, corregimos la posición.
                clampPlayer();
                
                // Incrementar puntuación por tiempo sobrevivido (25 puntos por segundo)
                mScoreAccumulator += 25.0f * timeSeconds;
                int pointsToAdd = static_cast<int>(mScoreAccumulator);
                if (pointsToAdd > 0) {
                    mScore.addPoints(pointsToAdd);
                    mScoreAccumulator -= pointsToAdd;
                    
                    // Actualizar dificultad basada en el score (se recalcula continuamente)
                    mDifficulty.updateFromScore(mScore.getScore());
                    
                    // Actualizar tema basado en el score
                    mGestorTemas.updateFromScore(mScore.getScore());
                    mGestorTemas.updateFade(timeSeconds);
                    
                    if (mGestorTemas.hayCambioTema()) {
                        // Aplicar nueva textura a los sprites de la carretera
                        mRoad1.setTexture(mGestorTemas.getTexturaActual());
                        mRoad2.setTexture(mGestorTemas.getTexturaActual());
                        mGestorTemas.confirmarCambio();
                    }
                }

                mSpawnTimer += timeSeconds;
                float adjustedSpawnInterval = mSpawnInterval * mDifficulty.getSpawnRateMultiplier();
                if (mSpawnTimer >= adjustedSpawnInterval) {
                    mSpawnTimer = 0.f;
                    int randomLane = rand() % NUM_CARRILES;
                    int randomTexture = rand() % mEnemyTextures.size();  // Textura aleatoria
                    float laneX = mCarrilLimits[randomLane] + (mCarrilAncho * 0.5f);
                    
                    // Verificar que no haya enemigos muy cerca en ese carril
                    bool canSpawn = true;
                    for (const auto& e : mEnemigos) {
                        if (e.getLane() == randomLane && e.getY() < 300.f) {
                            canSpawn = false;
                            break;
                        }
                    }
                    
                    if (canSpawn) {
                        // Carriles izquierdos (0, 1) están rotados
                        bool isLeftLane = (randomLane < 2);
                        float enemySpeed = 400.f * mDifficulty.getEnemySpeedMultiplier();
                        
                        mEnemigos.emplace_back(laneX, -200.f, randomLane, enemySpeed, mCarrilLimits[randomLane], mCarrilLimits[randomLane + 1], mBaseCarSize, mCarScaleFactor, mEnemyTextures[randomTexture], isLeftLane);
                    }
                }
                
                // Spawn de camiones especiales (raros, abarcan 2 carriles)
                mTruckSpawnTimer += timeSeconds;
                if (mTruckSpawnTimer >= mTruckSpawnInterval) {
                    mTruckSpawnTimer = 0.f;
                    
                    // Decidir si aparece en izquierda (carriles 0-1) o derecha (carriles 2-3)
                    bool spawnLeft = (rand() % 2 == 0);
                    int baseLane = spawnLeft ? 0 : 2;
                    
                    // Verificar que no haya enemigos cerca en los 2 carriles que ocupará
                    bool canSpawn = true;
                    for (const auto& e : mEnemigos) {
                        int enemyLane = e.getLane();
                        if ((enemyLane == baseLane || enemyLane == baseLane + 1) && e.getY() < 400.f) {
                            canSpawn = false;
                            break;
                        }
                    }
                    
                    if (canSpawn) {
                        // Posición X centrada entre dos carriles
                        float truckX = (mCarrilLimits[baseLane] + mCarrilLimits[baseLane + 2]) / 2.0f;
                        
                        // Tamaño especial basado en mBaseCarSize (NO en mCarrilAncho para evitar problemas al escalar)
                        sf::Vector2f truckSize(mBaseCarSize.x * 1.8f, mBaseCarSize.y * 2.2f);
                        
                        // Velocidad y rotación
                        bool rotate = spawnLeft;
                        float truckSpeed = 400.f * mDifficulty.getEnemySpeedMultiplier();
                        
                        // Crear camión (abarca 2 carriles)
                        mEnemigos.emplace_back(truckX, -200.f, baseLane, truckSpeed, 
                                              mCarrilLimits[baseLane], mCarrilLimits[baseLane + 2], 
                                              truckSize, mCarScaleFactor, mTruckTexture, rotate, true);  // true = isTruck
                    }
                }
                
                mGasolinaSpawnTimer += timeSeconds;
                if (mGasolinaSpawnTimer >= mGasolinaSpawnInterval) {
                     mGasolinaSpawnTimer = 0.f;
                    int randomLane = rand() % NUM_CARRILES;
                    float laneX = mCarrilLimits[randomLane] + (mCarrilAncho * 0.5f);
                    mGasolinas.emplace_back(laneX, -200.f, randomLane, 350.f, mGasolinaTexture, mCarScaleFactor);
                }

                for (auto& e : mEnemigos) e.update(timeSeconds);
                for (auto& g : mGasolinas) g.update(timeSeconds);

                float windowHeight = static_cast<float>(mWindow.getSize().y);
                mEnemigos.erase(std::remove_if(mEnemigos.begin(), mEnemigos.end(), [windowHeight](const auto& e) { return e.isOutOfBounds(windowHeight); }), mEnemigos.end());
                mGasolinas.erase(std::remove_if(mGasolinas.begin(), mGasolinas.end(), [windowHeight](const auto& g) { return g.isOutOfBounds(windowHeight); }), mGasolinas.end());

                ColisionManager::checkGasolinaCollisions(mPlayer, mGasolinas, mGasolinaActual, mGasolinaMax, mSoundManager);
                ColisionManager::checkEnemyCollisions(mPlayer, mEnemigos, mGasolinaActual, 32.0f, mSoundManager);
                
                updateGasolinaBar();
            }
            break;
        case GameState::Paused:
            // NO actualizar el loop mientras está pausado
            // Esto previene reinicios cuando el audio termina durante la pausa
            break;
        case GameState::GameOver:
            // El jugador elige qué hacer (Reintentar o Salir)
            break;
    }
}

void Game::render() {
    mWindow.clear();

    switch (mGameState) {
        case GameState::Menu:
            mMenu.draw();
            mControlsDisplay.draw(mWindow);
            break;
        case GameState::Countdown:
            // Dibujar el juego de fondo pero sin mover nada
            mWindow.draw(mRoad1);
            mWindow.draw(mRoad2);
            mWindow.draw(mPlayer);
            
            // Dibujar el contador encima
            mCountdown.draw(mWindow);
            break;
        case GameState::Playing:
        case GameState::GameOver:
            mWindow.draw(mRoad1);
            mWindow.draw(mRoad2);
            for (auto& gasolina : mGasolinas) mWindow.draw(gasolina.getSprite());
            for (auto& enemy : mEnemigos) mWindow.draw(enemy.getSprite());
            mWindow.draw(mPlayer);
            
            // Dibujar hitboxes en modo debug
            if (mDebugBounds) {
                // Hitbox del jugador (100% - tamaño exacto)
                sf::FloatRect playerBounds = mPlayer.getGlobalBounds();
                sf::RectangleShape playerHitbox;
                playerHitbox.setPosition(playerBounds.left, playerBounds.top);
                playerHitbox.setSize(sf::Vector2f(playerBounds.width, playerBounds.height));
                playerHitbox.setFillColor(sf::Color(255, 0, 0, 80));  // Rojo semitransparente
                playerHitbox.setOutlineColor(sf::Color::Red);
                playerHitbox.setOutlineThickness(2.0f);
                mWindow.draw(playerHitbox);
                
                // Hitboxes de enemigos (100% - tamaño exacto)
                for (auto& enemy : mEnemigos) {
                    sf::FloatRect enemyBounds = enemy.getSprite().getGlobalBounds();
                    sf::RectangleShape enemyHitbox;
                    enemyHitbox.setPosition(enemyBounds.left, enemyBounds.top);
                    enemyHitbox.setSize(sf::Vector2f(enemyBounds.width, enemyBounds.height));
                    enemyHitbox.setFillColor(sf::Color(0, 0, 255, 80));  // Azul semitransparente
                    enemyHitbox.setOutlineColor(sf::Color::Blue);
                    enemyHitbox.setOutlineThickness(2.0f);
                    mWindow.draw(enemyHitbox);
                }
            }
            
            mWindow.draw(mBarBackground);
            mWindow.draw(mRedBar);
            mWindow.draw(mBarGlass);
            mWindow.draw(mCombustibleText);
            
            // Dibujar puntuación
            if (mGameState == GameState::Playing) {
                mScore.draw(mWindow);
                mHighScore.draw(mWindow);
                mControlsDisplay.draw(mWindow);
            }

            if (mDebugBounds) {
                // Dibujar debug del score
                mScore.drawDebug(mWindow);
                mHighScore.drawDebug(mWindow);
                
                // Dibujar hitbox del texto Combustible
                sf::FloatRect combustibleBounds = mCombustibleText.getGlobalBounds();
                sf::RectangleShape combustibleHitbox;
                combustibleHitbox.setPosition(combustibleBounds.left, combustibleBounds.top);
                combustibleHitbox.setSize(sf::Vector2f(combustibleBounds.width, combustibleBounds.height));
                combustibleHitbox.setFillColor(sf::Color(255, 165, 0, 50)); // Naranja semi-transparente
                combustibleHitbox.setOutlineThickness(2.f);
                combustibleHitbox.setOutlineColor(sf::Color(255, 165, 0)); // Naranja
                mWindow.draw(combustibleHitbox);
                
                // Dibujar debug de la barra de gasolina
                sf::RectangleShape barDebugBox;
                barDebugBox.setPosition(mBarPosition);
                barDebugBox.setSize(mBarSize);
                barDebugBox.setFillColor(sf::Color(255, 0, 255, 50));
                barDebugBox.setOutlineThickness(3.f);
                barDebugBox.setOutlineColor(sf::Color::Magenta);
                mWindow.draw(barDebugBox);
                
                // Dibujar límites de carriles
                float windowHeight = static_cast<float>(mWindow.getSize().y);
                for (int i = 0; i <= NUM_CARRILES; i++) {
                    sf::RectangleShape carrilLine;
                    carrilLine.setSize(sf::Vector2f(3.f, windowHeight));
                    carrilLine.setPosition(mCarrilLimits[i], 0.f);
                    if (i == 0 || i == NUM_CARRILES) {
                        carrilLine.setFillColor(sf::Color(255, 0, 0, 200)); // Bordes rojos
                    } else {
                        carrilLine.setFillColor(sf::Color(0, 255, 255, 150)); // Líneas cyan
                    }
                    mWindow.draw(carrilLine);
                }
                
                // Dibujar área jugable
                sf::RectangleShape playableArea;
                playableArea.setPosition(mPlayableLeft, 0.f);
                playableArea.setSize(sf::Vector2f(mPlayableRight - mPlayableLeft, windowHeight));
                playableArea.setFillColor(sf::Color(0, 255, 0, 30));
                playableArea.setOutlineThickness(2.f);
                playableArea.setOutlineColor(sf::Color::Green);
                mWindow.draw(playableArea);
            }

            // Dibujar overlay de fade si hay transición de tema
            if (mGestorTemas.estaMostrandoFade()) {
                sf::RectangleShape fadeOverlay;
                fadeOverlay.setSize(sf::Vector2f(static_cast<float>(mWindow.getSize().x), 
                                                 static_cast<float>(mWindow.getSize().y)));
                fadeOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(mGestorTemas.getOpacidadFade())));
                mWindow.draw(fadeOverlay);
            }
            
            if (mGameState == GameState::GameOver) {
                mGameOverScreen.draw(mWindow);
            }
            break;
        case GameState::Paused:
            // Dibujar juego de fondo
            mWindow.draw(mRoad1);
            mWindow.draw(mRoad2);
            mWindow.draw(mPlayer);
            for (auto& e : mEnemigos) mWindow.draw(e.getSprite());
            for (auto& g : mGasolinas) mWindow.draw(g.getSprite());
            mWindow.draw(mBarBackground);
            mWindow.draw(mRedBar);
            mWindow.draw(mBarGlass);
            mWindow.draw(mCombustibleText);
            mScore.draw(mWindow);
            mHighScore.draw(mWindow);
            mControlsDisplay.draw(mWindow);
            
            // Dibujar menú de pausa encima
            mPauseMenu.draw();
            break;
    }

    mWindow.display();
}

void Game::updateRoadScale() {
    sf::Vector2u textureSize = mRoadTexture.getSize();
    float originalWidth = static_cast<float>(textureSize.x);
    float originalHeight = static_cast<float>(textureSize.y);
    float windowWidth = static_cast<float>(mWindow.getSize().x);
    float scale = windowWidth / originalWidth;

    mRoad1.setScale(scale, scale);
    mRoad2.setScale(scale, scale);
    mTextureHeight = originalHeight * scale;
    mRoad1.setPosition(0.f, 0.f);
    mRoad2.setPosition(0.f, -mTextureHeight);

    mPlayableLeft = mRoadMarginTexturePx * scale;
    mPlayableRight = windowWidth - (mRoadMarginTexturePx * scale);
    mCarScaleFactor = windowWidth / mReferenceWidth;

    // Calcular los carriles de forma INDEPENDIENTE del área jugable
    // El área jugable es para el jugador, los carriles son para enemigos/gasolina
    float carrilesLeft = mPlayableLeft + mCarrilesLeftOffset;
    float carrilesRight = mPlayableRight + mCarrilesRightOffset;
    float carrilesWidth = carrilesRight - carrilesLeft;
    
    mCarrilAncho = carrilesWidth / NUM_CARRILES;
    
    // Calcular límites de carriles con offsets individuales
    mCarrilLimits[0] = carrilesLeft;  // Borde izquierdo
    mCarrilLimits[1] = carrilesLeft + mCarrilAncho + mDivision1Offset;  // Primera división
    mCarrilLimits[2] = carrilesLeft + (2 * mCarrilAncho) + mDivision2Offset;  // Centro
    mCarrilLimits[3] = carrilesLeft + (3 * mCarrilAncho) + mDivision3Offset;  // Tercera división
    mCarrilLimits[4] = carrilesRight;  // Borde derecho
    
    sf::Vector2f newPlayerSize(mBaseCarSize.x * mCarScaleFactor, mBaseCarSize.y * mCarScaleFactor);
    sf::FloatRect playerBounds = mPlayer.getLocalBounds();
    float playerScaleX = newPlayerSize.x / playerBounds.width;
    float playerScaleY = newPlayerSize.y / playerBounds.height;
    mPlayer.setScale(playerScaleX, playerScaleY);

    for (auto& enemy : mEnemigos) {
        int lane = enemy.getLane();
        
        if (enemy.isTruck()) {
            // Lógica para trucks (2 carriles)
            float newLeft = mCarrilLimits[lane];
            float newRight = mCarrilLimits[lane + 2];
            float newLaneX = (newLeft + newRight) / 2.0f;  // Centrado entre 2 carriles
            enemy.updateLanePosition(newLaneX, newLeft, newRight);
        } else {
            // Lógica original para enemigos normales (1 carril)
            float newLaneX = mCarrilLimits[lane] + (mCarrilAncho * 0.5f);
            enemy.updateLanePosition(newLaneX, mCarrilLimits[lane], mCarrilLimits[lane+1]);
        }
        
        enemy.updateSize(mBaseCarSize, mCarScaleFactor);
    }
    for (auto& gasolina : mGasolinas) {
        int lane = gasolina.getLane();
        float newLaneX = mCarrilLimits[lane] + (mCarrilAncho * 0.5f);
        gasolina.updateLanePosition(newLaneX);
        gasolina.updateScale(mCarScaleFactor);
    }
    updateGasolinaBar();
    
    // Actualizar posición del score y high score
    float windowHeight = static_cast<float>(mWindow.getSize().y);
    mScore.update(windowWidth, windowHeight, mScoreOffsetX, mScoreOffsetY);
    mHighScore.update(windowWidth, windowHeight, mHighScoreOffsetX, mHighScoreOffsetY);
}

void Game::clampPlayer() {
    sf::Vector2f pos = mPlayer.getPosition();
    sf::FloatRect bounds = mPlayer.getGlobalBounds();
    float halfW = bounds.width * 0.5f;

    if (pos.x < mPlayableLeft + halfW) pos.x = mPlayableLeft + halfW;
    if (pos.x > mPlayableRight - halfW) pos.x = mPlayableRight - halfW;

    mPlayer.setPosition(pos.x, pos.y);
}

void Game::updateGasolinaBar() {
    float windowWidth = static_cast<float>(mWindow.getSize().x);
    float windowHeight = static_cast<float>(mWindow.getSize().y);
    float anchoDisponible = windowWidth - mPlayableRight;
    mBarSize = sf::Vector2f(anchoDisponible * 0.9f, 173.f);
    mBarPosition = sf::Vector2f(mPlayableRight + (anchoDisponible - mBarSize.x) * 0.2f, windowHeight * 0.5f - mBarSize.y * 0.5f);
    
    // Aplicar offset de debug
    mBarPosition.x += mBarOffsetX;
    mBarPosition.y += mBarOffsetY;
    
    float scale = mBarSize.x / mBarBackgroundTexture.getSize().x;
    
    mBarBackground.setPosition(mBarPosition);
    mBarBackground.setScale(scale, scale);
    mBarGlass.setPosition(mBarPosition);
    mBarGlass.setScale(scale, scale);
    
    float porcentaje = mGasolinaActual / mGasolinaMax;
    mRedBar.setPosition(mBarPosition);
    mRedBar.setScale(scale, scale);
    sf::Vector2u redBarTextureSize = mRedBarTexture.getSize();
    mRedBar.setTextureRect(sf::IntRect(0, 0, static_cast<int>(redBarTextureSize.x * porcentaje), redBarTextureSize.y));
    
    // Posicionar texto "Combustible" arriba de la barra con escalado
    float scaleY = windowHeight / mBaseResolutionHeight;
    unsigned int combustibleSize = static_cast<unsigned int>(35.0f * scaleY);
    mCombustibleText.setCharacterSize(combustibleSize);
    
    sf::FloatRect textBounds = mCombustibleText.getLocalBounds();
    float scaledOffsetY = mCombustibleOffsetY * scaleY;
    float textX = mBarPosition.x + (mBarSize.x - textBounds.width) * 0.5f;
    float textY = mBarPosition.y - textBounds.height - (10.0f * scaleY) + scaledOffsetY;
    mCombustibleText.setPosition(textX, textY);
}

void Game::toggleFullscreen() {
    // Guardar posición del jugador y dimensiones ANTES de cambiar ventana
    sf::Vector2f savedPlayerPos = mPlayer.getPosition();
    float savedWindowWidth = static_cast<float>(mWindow.getSize().x);
    float savedWindowHeight = static_cast<float>(mWindow.getSize().y);
    float savedPlayableLeft = mPlayableLeft;
    float savedPlayableRight = mPlayableRight;
    
    mIsFullscreen = !mIsFullscreen;
    
    if (mIsFullscreen) {
        mWindow.create(sf::VideoMode::getDesktopMode(), "Traffic Racer", sf::Style::Fullscreen);
    } else {
        mWindow.create(sf::VideoMode(1280, 720), "Traffic Racer", sf::Style::Default);
    }
    
    mWindow.setVerticalSyncEnabled(true);
    
    // Calcular posiciones relativas con las dimensiones guardadas
    float savedPlayableWidth = savedPlayableRight - savedPlayableLeft;
    float playerRelativeX = (savedPlayerPos.x - savedPlayableLeft) / savedPlayableWidth;
    float playerRelativeY = savedPlayerPos.y / savedWindowHeight;
    
    // Actualizar escala y límites
    updateRoadScale();
    updateGasolinaBar();
    
    // Restaurar jugador con posiciones relativas
    float newWindowWidth = static_cast<float>(mWindow.getSize().x);
    float newWindowHeight = static_cast<float>(mWindow.getSize().y);
    float newPlayableWidth = mPlayableRight - mPlayableLeft;
    
    float newPlayerX = mPlayableLeft + (playerRelativeX * newPlayableWidth);
    float newPlayerY = playerRelativeY * newWindowHeight;
    
    mPlayer.setPosition(newPlayerX, newPlayerY);
    clampPlayer();
    
    // Calcular offsets escalados basados en valores base
    float scaleX = newWindowWidth / mBaseResolutionWidth;
    float scaleY = newWindowHeight / mBaseResolutionHeight;
    
    mScoreOffsetX = mBaseScoreOffsetX * scaleX;
    mScoreOffsetY = mBaseScoreOffsetY * scaleY;
    mHighScoreOffsetX = mBaseHighScoreOffsetX * scaleX;
    mHighScoreOffsetY = mBaseHighScoreOffsetY * scaleY;
    
    // Actualizar UI de las pantallas
    mMenu.resize();
    mPauseMenu.resize();
    mGameOverScreen.resize(newWindowWidth, newWindowHeight);
    mScore.update(newWindowWidth, newWindowHeight, mScoreOffsetX, mScoreOffsetY);
    mHighScore.update(newWindowWidth, newWindowHeight, mHighScoreOffsetX, mHighScoreOffsetY);
    mControlsDisplay.updatePosition(newWindowWidth, newWindowHeight);
}