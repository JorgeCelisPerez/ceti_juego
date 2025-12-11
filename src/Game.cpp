#include "Game.hpp"
#include <iostream>

Game::Game()
    // CAMBIO 1: En SFML 2, VideoMode recibe (ancho, alto) sin llaves {}
    : mWindow(sf::VideoMode::getDesktopMode(), "Traffic Racer - Estructura POO", sf::Style::Fullscreen) 
    , mScrollSpeed(500.f)
    , mIsFullscreen(true)
    , mPlayerSpeed(500.f)
    , mRoadMarginTexturePx(800.f)
    , mDebugBounds(false)
    , mBaseCarSize(80.f, 140.f)
    , mReferenceWidth(0.f)
    , mCarScaleFactor(1.f)
    , mCarrilAncho(0.f)
    , mSpawnTimer(0.f)
    , mSpawnInterval(1.5f)
{
    mWindow.setVerticalSyncEnabled(true);

    // Cargar Textura (Asegúrate de que la ruta sea correcta)
    if (!mRoadTexture.loadFromFile("assets/images/road.png")) {
        std::cerr << "Error: No se pudo cargar assets/images/road.png" << std::endl;
        exit(1);
    }

    mRoad1.setTexture(mRoadTexture);
    mRoad2.setTexture(mRoadTexture);
    
    // Guardar ancho de referencia (fullscreen inicial)
    mReferenceWidth = static_cast<float>(mWindow.getSize().x);

    // Configurar jugador (rectángulo placeholder con tamaño base)
    mPlayer.setSize(mBaseCarSize);
    sf::Vector2f pSize = mPlayer.getSize();
    mPlayer.setOrigin(pSize.x * 0.5f, pSize.y * 0.5f);
    mPlayer.setFillColor(sf::Color(255, 80, 80));

    // Inicializar escala y posiciones
    updateRoadScale();
    
    // Calcular límites de carriles
    mCarrilAncho = (mPlayableRight - mPlayableLeft) / NUM_CARRILES;
    for (int i = 0; i <= NUM_CARRILES; i++) {
        mCarrilLimits[i] = mPlayableLeft + (i * mCarrilAncho);
    }

    float windowWidth = static_cast<float>(mWindow.getSize().x);
    float windowHeight = static_cast<float>(mWindow.getSize().y);
    mPlayer.setPosition(windowWidth * 0.5f, windowHeight * 0.75f);
    clampPlayer();
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
    // CAMBIO 3: La forma clásica de manejar eventos en SFML 2.x
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        // "Si el tipo de evento es cerrar..."
        if (event.type == sf::Event::Closed) {
            mWindow.close();
        }
        // "Si el tipo de evento es presionar tecla..."
        else if (event.type == sf::Event::KeyPressed) {
            // CAMBIO 4: Accedemos a event.key.code y usamos sf::Keyboard::Escape (sin "Key::")
            if (event.key.code == sf::Keyboard::Escape) {
                // Alternar entre pantalla completa y ventana normal
                mIsFullscreen = !mIsFullscreen;
                if (mIsFullscreen) {
                    mWindow.create(sf::VideoMode::getDesktopMode(), "Traffic Racer - Estructura POO", sf::Style::Fullscreen);
                } else {
                    mWindow.create(sf::VideoMode(1280, 720), "Traffic Racer - Estructura POO", sf::Style::Default);
                }
                mWindow.setVerticalSyncEnabled(true);
                updateRoadScale();
            }

            if (event.key.code == sf::Keyboard::F1) {
                mDebugBounds = !mDebugBounds;
            }

        }
    }
}

void Game::update(sf::Time dt) {
    float timeSeconds = dt.asSeconds();
    float moveAmount = mScrollSpeed * timeSeconds;

    // Input horizontal del jugador
    float input = getHorizontalInput();
    float deltaX = input * mPlayerSpeed * timeSeconds;
    mPlayer.move(deltaX, 0.f);
    clampPlayer();
    
    // Spawn de enemigos
    mSpawnTimer += timeSeconds;
    if (mSpawnTimer >= mSpawnInterval) {
        mSpawnTimer = 0.f;
        int randomLane = rand() % NUM_CARRILES;
        float laneX = mCarrilLimits[randomLane] + (mCarrilAncho * 0.5f);
        mEnemigos.emplace_back(laneX, -200.f, randomLane, 400.f, mCarrilLimits[randomLane], mCarrilLimits[randomLane + 1], mBaseCarSize, mCarScaleFactor);
    }
    
    // Actualizar enemigos
    for (auto& enemy : mEnemigos) {
        enemy.update(timeSeconds);
    }
    
    // Eliminar enemigos fuera de pantalla
    float windowHeight = static_cast<float>(mWindow.getSize().y);
    mEnemigos.erase(
        std::remove_if(mEnemigos.begin(), mEnemigos.end(),
            [windowHeight](const Enemy& e) { return e.isOutOfBounds(windowHeight); }),
        mEnemigos.end()
    );

    // Mover hacia abajo
    mRoad1.move(0.f, moveAmount);
    mRoad2.move(0.f, moveAmount);

    // Lógica de bucle infinito
    if (mRoad1.getPosition().y >= mTextureHeight) {
        mRoad1.setPosition(0.f, mRoad2.getPosition().y - mTextureHeight);
    }

    if (mRoad2.getPosition().y >= mTextureHeight) {
        mRoad2.setPosition(0.f, mRoad1.getPosition().y - mTextureHeight);
    }
}

void Game::render() {
    mWindow.clear();
    mWindow.draw(mRoad1);
    mWindow.draw(mRoad2);
    
    // Dibujar enemigos
    for (auto& enemy : mEnemigos) {
        mWindow.draw(enemy.getShape());
    }
    
    mWindow.draw(mPlayer);

    if (mDebugBounds) {
        float windowHeight = static_cast<float>(mWindow.getSize().y);
        
        // Dibujar rectángulo del área jugable
        sf::RectangleShape playableRect;
        playableRect.setPosition(mPlayableLeft, 0.f);
        playableRect.setSize(sf::Vector2f(mPlayableRight - mPlayableLeft, windowHeight));
        playableRect.setFillColor(sf::Color(0, 255, 0, 40));
        playableRect.setOutlineThickness(2.f);
        playableRect.setOutlineColor(sf::Color(0, 200, 0, 180));
        mWindow.draw(playableRect);
        
        // Dibujar límites de carriles
        for (int i = 1; i < NUM_CARRILES; i++) {
            sf::RectangleShape linea;
            linea.setSize(sf::Vector2f(2.f, windowHeight));
            linea.setPosition(mCarrilLimits[i], 0.f);
            linea.setFillColor(sf::Color(200, 100, 0, 150));
            mWindow.draw(linea);
        }
    }

    mWindow.display();
}

void Game::updateRoadScale() {
    // Obtener tamaño original de la imagen (2752x1536)
    sf::Vector2u textureSize = mRoadTexture.getSize();
    float originalWidth = static_cast<float>(textureSize.x);
    float originalHeight = static_cast<float>(textureSize.y);

    // Calcular escala para que ocupe todo el ancho de la ventana (usando tamaño real)
    float windowWidth = static_cast<float>(mWindow.getSize().x);
    float scaleX = windowWidth / originalWidth;
    float scaleY = windowWidth / originalWidth;  // Mantener proporción con respecto al ancho

    // Aplicar escala a ambos sprites
    mRoad1.setScale(scaleX, scaleY);
    mRoad2.setScale(scaleX, scaleY);

    // Calcular la altura escalada para la lógica de scroll
    mTextureHeight = originalHeight * scaleY;

    float marginScaled = mRoadMarginTexturePx * scaleX;
    mPlayableLeft = marginScaled;
    mPlayableRight = windowWidth - marginScaled;
    
    // Calcular factor de escala para coches
    mCarScaleFactor = windowWidth / mReferenceWidth;

    // CAMBIO 2: setPosition usa argumentos directos (x, y) en vez de vectores {}
    mRoad1.setPosition(0.f, 0.f);
    mRoad2.setPosition(0.f, -mTextureHeight);
    
    // Recalcular límites de carriles
    mCarrilAncho = (mPlayableRight - mPlayableLeft) / NUM_CARRILES;
    for (int i = 0; i <= NUM_CARRILES; i++) {
        mCarrilLimits[i] = mPlayableLeft + (i * mCarrilAncho);
    }
    
    // Actualizar tamaño del jugador con nueva escala
    sf::Vector2f newPlayerSize(mBaseCarSize.x * mCarScaleFactor, mBaseCarSize.y * mCarScaleFactor);
    mPlayer.setSize(newPlayerSize);
    mPlayer.setOrigin(newPlayerSize.x * 0.5f, newPlayerSize.y * 0.5f);
    
    // Reposicionar y reescalar enemigos existentes según su carril
    for (auto& enemy : mEnemigos) {
        int lane = enemy.getLane();
        float newLaneX = mCarrilLimits[lane] + (mCarrilAncho * 0.5f);
        enemy.updateLanePosition(newLaneX, mCarrilLimits[lane], mCarrilLimits[lane + 1]);
        enemy.updateSize(mBaseCarSize, mCarScaleFactor);
    }
    
    // Reposicionar jugador en Y (mantener en 75% de la nueva altura)
    sf::Vector2f playerPos = mPlayer.getPosition();
    float newWindowHeight = static_cast<float>(mWindow.getSize().y);
    mPlayer.setPosition(playerPos.x, newWindowHeight * 0.75f);

    clampPlayer();
}

void Game::clampPlayer() {
    sf::Vector2f pos = mPlayer.getPosition();
    float halfW = mPlayer.getSize().x * 0.5f;

    if (pos.x < mPlayableLeft + halfW) pos.x = mPlayableLeft + halfW;
    if (pos.x > mPlayableRight - halfW) pos.x = mPlayableRight - halfW;

    mPlayer.setPosition(pos.x, pos.y);
}