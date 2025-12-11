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
    , mGasolinaSpawnTimer(0.f)
    , mGasolinaSpawnInterval(3.0f)
    , mGasolinaActual(100.0f)
    , mGasolinaMax(100.0f)
    , mGasolinaConsumoRate(5.0f)
    , mGameOver(false)
{
    mWindow.setVerticalSyncEnabled(true);

    // Cargar Texturas
    if (!mRoadTexture.loadFromFile("assets/images/road.png")) {
        std::cerr << "Error: No se pudo cargar assets/images/road.png" << std::endl;
        exit(1);
    }
    
    if (!mGasolinaTexture.loadFromFile("assets/images/Bidon gas.png")) {
        std::cerr << "Error: No se pudo cargar assets/images/Bidon gas.png" << std::endl;
        exit(1);
    }
    
    if (!mBarBackgroundTexture.loadFromFile("assets/images/BarBackground.png")) {
        std::cerr << "Error: No se pudo cargar assets/images/BarBackground.png" << std::endl;
        exit(1);
    }
    
    if (!mBarGlassTexture.loadFromFile("assets/images/BarGlass.png")) {
        std::cerr << "Error: No se pudo cargar assets/images/BarGlass.png" << std::endl;
        exit(1);
    }
    
    if (!mRedBarTexture.loadFromFile("assets/images/RedBar.png")) {
        std::cerr << "Error: No se pudo cargar assets/images/RedBar.png" << std::endl;
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
    
    // Configurar barra de gasolina horizontal (entre carretera y borde derecho)
    float anchoDisponible = windowWidth - mPlayableRight;
    mBarSize = sf::Vector2f(anchoDisponible * 0.9f, 173.f);  // 90% del espacio disponible
    mBarPosition = sf::Vector2f(mPlayableRight + (anchoDisponible - mBarSize.x) * 0.2f, windowHeight * 0.5f - mBarSize.y * 0.5f);
    
    mBarBackground.setTexture(mBarBackgroundTexture);
    mBarGlass.setTexture(mBarGlassTexture);
    mRedBar.setTexture(mRedBarTexture);
    
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
                // Guardar posición del jugador y dimensiones ANTES de cambiar ventana
                sf::Vector2f savedPlayerPos = mPlayer.getPosition();
                float savedWindowWidth = static_cast<float>(mWindow.getSize().x);
                float savedWindowHeight = static_cast<float>(mWindow.getSize().y);
                float savedPlayableLeft = mPlayableLeft;
                float savedPlayableRight = mPlayableRight;
                
                // Alternar entre pantalla completa y ventana normal
                mIsFullscreen = !mIsFullscreen;
                if (mIsFullscreen) {
                    mWindow.create(sf::VideoMode::getDesktopMode(), "Traffic Racer - Estructura POO", sf::Style::Fullscreen);
                } else {
                    mWindow.create(sf::VideoMode(1280, 720), "Traffic Racer - Estructura POO", sf::Style::Default);
                }
                mWindow.setVerticalSyncEnabled(true);
                
                // Calcular posiciones relativas con las dimensiones guardadas
                float savedPlayableWidth = savedPlayableRight - savedPlayableLeft;
                float playerRelativeX = (savedPlayerPos.x - savedPlayableLeft) / savedPlayableWidth;
                float playerRelativeY = savedPlayerPos.y / savedWindowHeight;
                
                // Actualizar escala y límites
                updateRoadScale();
                
                // Restaurar jugador con posiciones relativas
                float newWindowWidth = static_cast<float>(mWindow.getSize().x);
                float newWindowHeight = static_cast<float>(mWindow.getSize().y);
                float newPlayableWidth = mPlayableRight - mPlayableLeft;
                
                float newPlayerX = mPlayableLeft + (playerRelativeX * newPlayableWidth);
                float newPlayerY = playerRelativeY * newWindowHeight;
                
                mPlayer.setPosition(newPlayerX, newPlayerY);
                clampPlayer();
            }

            if (event.key.code == sf::Keyboard::F1) {
                mDebugBounds = !mDebugBounds;
            }

        }
    }
}

void Game::update(sf::Time dt) {
    if (mGameOver) return;
    
    float timeSeconds = dt.asSeconds();
    float moveAmount = mScrollSpeed * timeSeconds;

    // Consumir gasolina
    mGasolinaActual -= mGasolinaConsumoRate * timeSeconds;
    if (mGasolinaActual <= 0.0f) {
        mGasolinaActual = 0.0f;
        mGameOver = true;
        return;
    }
    updateGasolinaBar();

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
    
    // Spawn de items de gasolina
    mGasolinaSpawnTimer += timeSeconds;
    if (mGasolinaSpawnTimer >= mGasolinaSpawnInterval) {
        mGasolinaSpawnTimer = 0.f;
        int randomLane = rand() % NUM_CARRILES;
        float laneX = mCarrilLimits[randomLane] + (mCarrilAncho * 0.5f);
        mGasolinas.emplace_back(laneX, -200.f, randomLane, 350.f, mGasolinaTexture, mCarScaleFactor);
    }
    
    // Actualizar enemigos
    for (auto& enemy : mEnemigos) {
        enemy.update(timeSeconds);
    }
    
    // Actualizar gasolinas
    for (auto& gasolina : mGasolinas) {
        gasolina.update(timeSeconds);
    }
    
    // Comprobar colisiones con gasolina
    checkGasolinaCollisions();
    
    // Eliminar enemigos fuera de pantalla
    float windowHeight = static_cast<float>(mWindow.getSize().y);
    mEnemigos.erase(
        std::remove_if(mEnemigos.begin(), mEnemigos.end(),
            [windowHeight](const Enemy& e) { return e.isOutOfBounds(windowHeight); }),
        mEnemigos.end()
    );
    
    // Eliminar gasolinas fuera de pantalla
    mGasolinas.erase(
        std::remove_if(mGasolinas.begin(), mGasolinas.end(),
            [windowHeight](const Gasolina& g) { return g.isOutOfBounds(windowHeight); }),
        mGasolinas.end()
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
    
    // Dibujar items de gasolina
    for (auto& gasolina : mGasolinas) {
        mWindow.draw(gasolina.getSprite());
        
        // DEBUG: Dibujar hitbox
        if (mDebugBounds) {
            sf::FloatRect bounds = gasolina.getSprite().getGlobalBounds();
            sf::RectangleShape hitbox;
            hitbox.setPosition(bounds.left, bounds.top);
            hitbox.setSize(sf::Vector2f(bounds.width, bounds.height));
            hitbox.setFillColor(sf::Color(255, 255, 0, 50));
            hitbox.setOutlineThickness(2.f);
            hitbox.setOutlineColor(sf::Color::Yellow);
            mWindow.draw(hitbox);
        }
    }
    
    // Dibujar enemigos
    for (auto& enemy : mEnemigos) {
        mWindow.draw(enemy.getShape());
    }
    
    mWindow.draw(mPlayer);
    
    // Dibujar UI de gasolina
    mWindow.draw(mBarBackground);
    mWindow.draw(mRedBar);
    mWindow.draw(mBarGlass);

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
    
    // Reposicionar y reescalar enemigos existentes según su carril (mantener Y)
    for (auto& enemy : mEnemigos) {
        int lane = enemy.getLane();
        float newLaneX = mCarrilLimits[lane] + (mCarrilAncho * 0.5f);
        float currentY = enemy.getY();
        enemy.updateLanePosition(newLaneX, mCarrilLimits[lane], mCarrilLimits[lane + 1]);
        enemy.updateSize(mBaseCarSize, mCarScaleFactor);
        // Restaurar Y original
        enemy.getShape().setPosition(newLaneX, currentY);
    }
    
    // Reposicionar gasolinas según su carril (mantener Y)
    for (auto& gasolina : mGasolinas) {
        int lane = gasolina.getLane();
        float newLaneX = mCarrilLimits[lane] + (mCarrilAncho * 0.5f);
        float currentY = gasolina.getY();
        gasolina.updateLanePosition(newLaneX);
        gasolina.updateScale(mCarScaleFactor);
        // Restaurar Y original
        gasolina.getSprite().setPosition(newLaneX, currentY);
    }
    
    // Actualizar barra de gasolina
    updateGasolinaBar();
}

void Game::clampPlayer() {
    sf::Vector2f pos = mPlayer.getPosition();
    float halfW = mPlayer.getSize().x * 0.5f;

    if (pos.x < mPlayableLeft + halfW) pos.x = mPlayableLeft + halfW;
    if (pos.x > mPlayableRight - halfW) pos.x = mPlayableRight - halfW;

    mPlayer.setPosition(pos.x, pos.y);
}

void Game::checkGasolinaCollisions() {
    sf::FloatRect playerBounds = mPlayer.getGlobalBounds();
    
    for (auto it = mGasolinas.begin(); it != mGasolinas.end(); ) {
        sf::FloatRect gasolinaBounds = it->getSprite().getGlobalBounds();
        
        if (playerBounds.intersects(gasolinaBounds)) {
            // Recargar gasolina
            mGasolinaActual += it->getRecargaAmount();
            if (mGasolinaActual > mGasolinaMax) {
                mGasolinaActual = mGasolinaMax;
            }
            updateGasolinaBar();
            
            // Eliminar el item recolectado
            it = mGasolinas.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::updateGasolinaBar() {
    float windowWidth = static_cast<float>(mWindow.getSize().x);
    float windowHeight = static_cast<float>(mWindow.getSize().y);
    
    // Actualizar posición de la barra horizontal (entre carretera y borde derecho)
    float anchoDisponible = windowWidth - mPlayableRight;
    mBarSize = sf::Vector2f(anchoDisponible * 0.9f, 173.f);
    mBarPosition = sf::Vector2f(mPlayableRight + (anchoDisponible - mBarSize.x) * 0.2f, windowHeight * 0.5f - mBarSize.y * 0.5f);
    
    // Calcular escalas uniformes para todas las capas
    float scaleX_BG = mBarSize.x / mBarBackgroundTexture.getSize().x;
    float scaleY_BG = mBarSize.y / mBarBackgroundTexture.getSize().y;
    
    float scaleX_Glass = mBarSize.x / mBarGlassTexture.getSize().x;
    float scaleY_Glass = mBarSize.y / mBarGlassTexture.getSize().y;
    
    // Configurar fondo (background)
    mBarBackground.setPosition(mBarPosition);
    mBarBackground.setScale(scaleX_BG, scaleY_BG);
    
    // Configurar cristal (glass) - misma posición y altura
    mBarGlass.setPosition(mBarPosition);
    mBarGlass.setScale(scaleX_Glass, scaleY_Glass);
    
    // Configurar barra roja horizontal (crece de izquierda a derecha)
    float porcentaje = mGasolinaActual / mGasolinaMax;
    float barraAncho = mBarSize.x * porcentaje;
    
    // Asegurar que no se salga del lado izquierdo
    if (barraAncho < 0.1f) barraAncho = 0.1f;
    
    // La barra roja empieza desde la izquierda y crece hacia la derecha - misma altura que el fondo
    mRedBar.setPosition(mBarPosition.x, mBarPosition.y);
    mRedBar.setScale(
        barraAncho / mRedBarTexture.getSize().x,
        scaleY_BG  // Usar la misma escala vertical que el fondo
    );
}