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
{
    mWindow.setVerticalSyncEnabled(true);

    // Cargar Textura (Asegúrate de que la ruta sea correcta)
    if (!mRoadTexture.loadFromFile("assets/images/road.png")) {
        std::cerr << "Error: No se pudo cargar assets/images/road.png" << std::endl;
        exit(1);
    }

    mRoad1.setTexture(mRoadTexture);
    mRoad2.setTexture(mRoadTexture);

    // Configurar jugador (rectángulo placeholder)
    mPlayer.setSize(sf::Vector2f(100.f, 170.f));
    sf::Vector2f pSize = mPlayer.getSize();
    mPlayer.setOrigin(pSize.x * 0.5f, pSize.y * 0.5f);
    mPlayer.setFillColor(sf::Color(255, 80, 80));

    // Inicializar escala y posiciones
    updateRoadScale();

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
    mWindow.draw(mPlayer);

    if (mDebugBounds) {
        float windowHeight = static_cast<float>(mWindow.getSize().y);
        sf::RectangleShape playableRect;
        playableRect.setPosition(mPlayableLeft, 0.f);
        playableRect.setSize(sf::Vector2f(mPlayableRight - mPlayableLeft, windowHeight));
        playableRect.setFillColor(sf::Color(0, 255, 0, 40));
        playableRect.setOutlineThickness(2.f);
        playableRect.setOutlineColor(sf::Color(0, 200, 0, 180));
        mWindow.draw(playableRect);
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

    // CAMBIO 2: setPosition usa argumentos directos (x, y) en vez de vectores {}
    mRoad1.setPosition(0.f, 0.f);
    mRoad2.setPosition(0.f, -mTextureHeight);

    clampPlayer();
}

void Game::clampPlayer() {
    sf::Vector2f pos = mPlayer.getPosition();
    float halfW = mPlayer.getSize().x * 0.5f;

    if (pos.x < mPlayableLeft + halfW) pos.x = mPlayableLeft + halfW;
    if (pos.x > mPlayableRight - halfW) pos.x = mPlayableRight - halfW;

    mPlayer.setPosition(pos.x, pos.y);
}