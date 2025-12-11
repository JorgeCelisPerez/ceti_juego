#include "Game.hpp"
#include <iostream>

Game::Game()
    // CAMBIO 1: En SFML 2, VideoMode recibe (ancho, alto) sin llaves {}
    : mWindow(sf::VideoMode::getDesktopMode(), "Traffic Racer - Estructura POO", sf::Style::Fullscreen) 
    , mScrollSpeed(500.f)
    , mIsFullscreen(true)
{
    mWindow.setVerticalSyncEnabled(true);

    // Cargar Textura (Asegúrate de que la ruta sea correcta)
    if (!mRoadTexture.loadFromFile("assets/images/road.png")) {
        std::cerr << "Error: No se pudo cargar assets/images/road.png" << std::endl;
        exit(1);
    }

    mRoad1.setTexture(mRoadTexture);
    mRoad2.setTexture(mRoadTexture);

    // Inicializar escala y posiciones
    updateRoadScale();
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
        }
    }
}

void Game::update(sf::Time dt) {
    float timeSeconds = dt.asSeconds();
    float moveAmount = mScrollSpeed * timeSeconds;

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

    // CAMBIO 2: setPosition usa argumentos directos (x, y) en vez de vectores {}
    mRoad1.setPosition(0.f, 0.f);
    mRoad2.setPosition(0.f, -mTextureHeight);
}