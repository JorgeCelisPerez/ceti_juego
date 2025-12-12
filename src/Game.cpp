#include "Game.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>

// La definición de la variable estática debe estar aquí, en el ámbito global.
// Pero como la hemos movido a GameOverScreen, ya no es necesaria aquí.

Game::Game()
    : mWindow(sf::VideoMode::getDesktopMode(), "Traffic Racer", sf::Style::Fullscreen),
      mMenu(mWindow),
      mGameOverScreen(mWindow.getSize().x, mWindow.getSize().y), // Inicializa la nueva clase
      mGameState(GameState::Menu),
      mIsFullscreen(true),
      mScrollSpeed(500.f),
      mPlayerSpeed(600.f),
      mRoadMarginTexturePx(800.f),
      mDebugBounds(false),
      mBaseCarSize(105.f, 180.f),  // Aumentado para que las imágenes sean más visibles
      mReferenceWidth(0.f),
      mCarScaleFactor(1.f),
      mCarrilAncho(0.f),
      mSpawnInterval(1.5f),
      mGasolinaSpawnInterval(3.0f),
      mGasolinaMax(100.0f),
      mGasolinaConsumoRate(5.0f),
      mBoundaryPenalty(10.0f),
      mBoundaryContinuousPenalty(15.0f),
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
      mDivision3Offset(6.0f)
{
    // Establecer resolución base para escalado de UI
    mScore.setBaseResolution(mBaseResolutionWidth, mBaseResolutionHeight);
    mHighScore.setBaseResolution(mBaseResolutionWidth, mBaseResolutionHeight);
    
    mWindow.setVerticalSyncEnabled(true);
    srand(static_cast<unsigned int>(time(NULL)));

    if (!mRoadTexture.loadFromFile("assets/images/road.png")) { exit(1); }
    if (!mGasolinaTexture.loadFromFile("assets/images/Bidon gas.png")) { exit(1); }
    if (!mBarBackgroundTexture.loadFromFile("assets/images/BarBackground.png")) { exit(1); }
    if (!mBarGlassTexture.loadFromFile("assets/images/BarGlass.png")) { exit(1); }
    if (!mRedBarTexture.loadFromFile("assets/images/RedBar.png")) { exit(1); }
    if (!mPlayerTexture.loadFromFile("assets/images/Black_viper.png")) { exit(1); }
    
    // Cargar texturas de enemigos
    mEnemyTextures.resize(7);
    if (!mEnemyTextures[0].loadFromFile("assets/images/Ambulance.png")) { exit(1); }
    if (!mEnemyTextures[1].loadFromFile("assets/images/Audi.png")) { exit(1); }
    if (!mEnemyTextures[2].loadFromFile("assets/images/Car.png")) { exit(1); }
    if (!mEnemyTextures[3].loadFromFile("assets/images/Mini_truck.png")) { exit(1); }
    if (!mEnemyTextures[4].loadFromFile("assets/images/Mini_van.png")) { exit(1); }
    if (!mEnemyTextures[5].loadFromFile("assets/images/Police.png")) { exit(1); }
    if (!mEnemyTextures[6].loadFromFile("assets/images/taxi.png")) { exit(1); }

    mRoad1.setTexture(mRoadTexture);
    mRoad2.setTexture(mRoadTexture);
    
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
}

void Game::startGame() {
    mGameState = GameState::Playing;
    mGasolinaActual = mGasolinaMax;
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
                }
                break;
            case GameState::Playing:
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        mGameState = GameState::Menu;
                    }
                    if (event.key.code == sf::Keyboard::F1) {
                        mDebugBounds = !mDebugBounds;
                        if (mDebugBounds) {
                            std::cout << "\n=== MODO DEBUG ACTIVADO ===" << std::endl;
                            std::cout << "Controles:" << std::endl;
                            std::cout << "  Numpad 8/2/4/6: Mover High Score" << std::endl;
                            std::cout << "  F2: Mostrar valores actuales" << std::endl;
                            std::cout << "\n(Carriles ya fijos)" << std::endl;
                        }
                    }
                    if (event.key.code == sf::Keyboard::F11) {
                        toggleFullscreen();
                    }
                    
                    // Ajustar posición del High Score (Numpad)
                    if (mDebugBounds) {
                        if (event.key.code == sf::Keyboard::Numpad8) mHighScoreOffsetY -= 5.0f;
                        if (event.key.code == sf::Keyboard::Numpad2) mHighScoreOffsetY += 5.0f;
                        if (event.key.code == sf::Keyboard::Numpad4) mHighScoreOffsetX -= 5.0f;
                        if (event.key.code == sf::Keyboard::Numpad6) mHighScoreOffsetX += 5.0f;
                        
                        // --- CARRILES YA FIJOS ---
                        // static int divisionSeleccionada = 2;
                        // if (event.key.code == sf::Keyboard::Num1) {
                        //     divisionSeleccionada = 1;
                        //     std::cout << "División seleccionada: 1 (izquierda)" << std::endl;
                        // }
                        // if (event.key.code == sf::Keyboard::Num2) {
                        //     divisionSeleccionada = 2;
                        //     std::cout << "División seleccionada: 2 (centro)" << std::endl;
                        // }
                        // if (event.key.code == sf::Keyboard::Num3) {
                        //     divisionSeleccionada = 3;
                        //     std::cout << "División seleccionada: 3 (derecha)" << std::endl;
                        // }
                        // if (event.key.code == sf::Keyboard::Q) {
                        //     if (divisionSeleccionada == 1) mDivision1Offset -= 2.0f;
                        //     else if (divisionSeleccionada == 2) mDivision2Offset -= 2.0f;
                        //     else if (divisionSeleccionada == 3) mDivision3Offset -= 2.0f;
                        //     updateRoadScale();
                        // }
                        // if (event.key.code == sf::Keyboard::E) {
                        //     if (divisionSeleccionada == 1) mDivision1Offset += 2.0f;
                        //     else if (divisionSeleccionada == 2) mDivision2Offset += 2.0f;
                        //     else if (divisionSeleccionada == 3) mDivision3Offset += 2.0f;
                        //     updateRoadScale();
                        // }
                        
                        // Mostrar valores actuales
                        if (event.key.code == sf::Keyboard::F2) {
                            std::cout << "\n--- Valores Actuales ---" << std::endl;
                            std::cout << "High Score Offset: X=" << mHighScoreOffsetX << ", Y=" << mHighScoreOffsetY << std::endl;
                            std::cout << "Carriles Left Offset: " << mCarrilesLeftOffset << std::endl;
                            std::cout << "Carriles Right Offset: " << mCarrilesRightOffset << std::endl;
                            std::cout << "División 1 Offset: " << mDivision1Offset << std::endl;
                            std::cout << "División 2 Offset: " << mDivision2Offset << std::endl;
                            std::cout << "División 3 Offset: " << mDivision3Offset << std::endl;
                        }
                    }
                }
                break;
            case GameState::GameOver:
                // La transición es automática
                break;
        }
    }
}

void Game::update(sf::Time dt) {
    switch (mGameState) {
        case GameState::Menu:
            // No se necesita update por frame, es manejado por eventos
            break;
        case GameState::Playing:
            {
                float timeSeconds = dt.asSeconds();
                mGasolinaActual -= mGasolinaConsumoRate * timeSeconds;
                if (mGasolinaActual <= 0) {
                    mGasolinaActual = 0;
                    mHighScore.checkAndUpdate(mScore.getScore()); // Actualizar high score
                    mGameOverScreen.reset(); // Usar la nueva clase
                    mGameState = GameState::GameOver;
                    return; 
                }

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
                }

                mSpawnTimer += timeSeconds;
                float adjustedSpawnInterval = mSpawnInterval * mDifficulty.getSpawnRateMultiplier();
                if (mSpawnTimer >= adjustedSpawnInterval) {
                    mSpawnTimer = 0.f;
                    int randomLane = rand() % NUM_CARRILES;
                    int randomTexture = rand() % mEnemyTextures.size();  // Textura aleatoria
                    float laneX = mCarrilLimits[randomLane] + (mCarrilAncho * 0.5f);
                    
                    // Carriles izquierdos (0, 1) tienen más velocidad y están rotados
                    bool isLeftLane = (randomLane < 2);
                    float speedMultiplier = isLeftLane ? 1.1f : 1.0f;  // 10% más rápido en carriles izquierdos
                    float enemySpeed = 400.f * mDifficulty.getEnemySpeedMultiplier() * speedMultiplier;
                    
                    mEnemigos.emplace_back(laneX, -200.f, randomLane, enemySpeed, mCarrilLimits[randomLane], mCarrilLimits[randomLane + 1], mBaseCarSize, mCarScaleFactor, mEnemyTextures[randomTexture], isLeftLane);
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

                ColisionManager::checkGasolinaCollisions(mPlayer, mGasolinas, mGasolinaActual, mGasolinaMax);
                ColisionManager::checkEnemyCollisions(mPlayer, mEnemigos, mGasolinaActual);

                // Actualizar posiciones con offsets si está en modo debug
                if (mDebugBounds) {
                    float windowWidth = static_cast<float>(mWindow.getSize().x);
                    mScore.update(windowWidth, windowHeight, mScoreOffsetX, mScoreOffsetY);
                    mHighScore.update(windowWidth, windowHeight, mHighScoreOffsetX, mHighScoreOffsetY);
                }
                
                updateGasolinaBar();
            }
            break;
        case GameState::GameOver:
            mGameOverScreen.update(dt);
            if (mGameOverScreen.isDone()) {
                mGameState = GameState::Menu;
            }
            break;
    }
}

void Game::render() {
    mWindow.clear();

    switch (mGameState) {
        case GameState::Menu:
            mMenu.draw();
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
            
            // Dibujar puntuación
            if (mGameState == GameState::Playing) {
                mScore.draw(mWindow);
                mHighScore.draw(mWindow);
            }

            if (mDebugBounds) {
                // Dibujar debug del score
                mScore.drawDebug(mWindow);
                mHighScore.drawDebug(mWindow);
                
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

            if (mGameState == GameState::GameOver) {
                mGameOverScreen.draw(mWindow);
            }
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
        float newLaneX = mCarrilLimits[lane] + (mCarrilAncho * 0.5f);
        enemy.updateLanePosition(newLaneX, mCarrilLimits[lane], mCarrilLimits[lane+1]);
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
    mGameOverScreen.resize(newWindowWidth, newWindowHeight);
    mScore.update(newWindowWidth, newWindowHeight, mScoreOffsetX, mScoreOffsetY);
    mHighScore.update(newWindowWidth, newWindowHeight, mHighScoreOffsetX, mHighScoreOffsetY);
}