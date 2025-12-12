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
      mPlayerSpeed(500.f),
      mRoadMarginTexturePx(800.f),
      mDebugBounds(false),
      mBaseCarSize(80.f, 140.f),
      mReferenceWidth(0.f),
      mCarScaleFactor(1.f),
      mCarrilAncho(0.f),
      mSpawnInterval(1.5f),
      mGasolinaSpawnInterval(3.0f),
      mGasolinaMax(100.0f),
      mGasolinaConsumoRate(5.0f),
      mBoundaryPenalty(10.0f),
      mBoundaryContinuousPenalty(15.0f)
{
    mWindow.setVerticalSyncEnabled(true);
    srand(static_cast<unsigned int>(time(NULL)));

    if (!mRoadTexture.loadFromFile("assets/images/road.png")) { exit(1); }
    if (!mGasolinaTexture.loadFromFile("assets/images/Bidon gas.png")) { exit(1); }
    if (!mBarBackgroundTexture.loadFromFile("assets/images/BarBackground.png")) { exit(1); }
    if (!mBarGlassTexture.loadFromFile("assets/images/BarGlass.png")) { exit(1); }
    if (!mRedBarTexture.loadFromFile("assets/images/RedBar.png")) { exit(1); }

    mRoad1.setTexture(mRoadTexture);
    mRoad2.setTexture(mRoadTexture);
    
    mReferenceWidth = static_cast<float>(mWindow.getSize().x);

    mPlayer.setSize(mBaseCarSize);
    mPlayer.setOrigin(mBaseCarSize.x * 0.5f, mBaseCarSize.y * 0.5f);
    mPlayer.setFillColor(sf::Color(255, 80, 80));

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

    mEnemigos.clear();
    mGasolinas.clear();

    updateRoadScale();
    
    float windowWidth = static_cast<float>(mWindow.getSize().x);
    float windowHeight = static_cast<float>(mWindow.getSize().y);
    mPlayer.setPosition(windowWidth * 0.5f, windowHeight * 0.75f);
    clampPlayer();
    
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
                float halfW = mPlayer.getSize().x * 0.5f;
                bool isCurrentlyTouching = (pos.x - halfW < mPlayableLeft) || (pos.x + halfW > mPlayableRight);

                if (isCurrentlyTouching) {
                    // Si ya estaba tocando, aplica la penalización continua.
                    // Si es la primera vez, aplica la penalización inicial.
                    mGasolinaActual -= (mIsTouchingBoundary ? mBoundaryContinuousPenalty * timeSeconds : mBoundaryPenalty);
                }
                mIsTouchingBoundary = isCurrentlyTouching;

                // Ahora, después de verificar y aplicar la penalización, corregimos la posición.
                clampPlayer();

                mSpawnTimer += timeSeconds;
                if (mSpawnTimer >= mSpawnInterval) {
                    mSpawnTimer = 0.f;
                    int randomLane = rand() % NUM_CARRILES;
                    float laneX = mCarrilLimits[randomLane] + (mCarrilAncho * 0.5f);
                    mEnemigos.emplace_back(laneX, -200.f, randomLane, 400.f, mCarrilLimits[randomLane], mCarrilLimits[randomLane + 1], mBaseCarSize, mCarScaleFactor);
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
            for (auto& enemy : mEnemigos) mWindow.draw(enemy.getShape());
            mWindow.draw(mPlayer);
            mWindow.draw(mBarBackground);
            mWindow.draw(mRedBar);
            mWindow.draw(mBarGlass);

            if (mDebugBounds) {
                // ... (código de debug)
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

    mCarrilAncho = (mPlayableRight - mPlayableLeft) / NUM_CARRILES;
    for (int i = 0; i <= NUM_CARRILES; i++) {
        mCarrilLimits[i] = mPlayableLeft + (i * mCarrilAncho);
    }
    
    sf::Vector2f newPlayerSize(mBaseCarSize.x * mCarScaleFactor, mBaseCarSize.y * mCarScaleFactor);
    mPlayer.setSize(newPlayerSize);
    mPlayer.setOrigin(newPlayerSize.x * 0.5f, newPlayerSize.y * 0.5f);

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
}

void Game::clampPlayer() {
    sf::Vector2f pos = mPlayer.getPosition();
    float halfW = mPlayer.getSize().x * 0.5f;

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