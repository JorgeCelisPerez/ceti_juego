#include "GestorTemas.hpp"
#include <iostream>
#include <cstdlib>

GestorTemas::GestorTemas()
    : mTemaActual(0),
      mTemaAnterior(0),
      mScoreThreshold(500),
      mHaCambiado(false),
      mEnTransicion(false),
      mTiempoFade(0.0f),
      mDuracionFade(1.0f),  // 1 segundo total (0.5s fade out, 0.5s fade in)
      mOpacidadFade(0.0f),
      mTexturaYaCambiada(false)
{
    cargarTexturas();
}

void GestorTemas::cargarTexturas() {
    // Orden de los temas (se van intercalando)
    mNombresTemas = {
        "Normal",
        "Desierto",
        "Jungla",
        "Montañas",
        "Nevado",
        "Playa"
    };
    
    // Cargar todas las texturas
    mTexturas.resize(6);
    
    if (!mTexturas[0].loadFromFile("assets/images/road.png")) {
        std::cerr << "Error: No se pudo cargar road.png" << std::endl;
        exit(1);
    }
    if (!mTexturas[1].loadFromFile("assets/images/roadDesierto.png")) {
        std::cerr << "Error: No se pudo cargar roadDesierto.png" << std::endl;
        exit(1);
    }
    if (!mTexturas[2].loadFromFile("assets/images/roadJungla.png")) {
        std::cerr << "Error: No se pudo cargar roadJungla.png" << std::endl;
        exit(1);
    }
    if (!mTexturas[3].loadFromFile("assets/images/roadMontanas.png")) {
        std::cerr << "Error: No se pudo cargar roadMontanas.png" << std::endl;
        exit(1);
    }
    if (!mTexturas[4].loadFromFile("assets/images/roadNevado.png")) {
        std::cerr << "Error: No se pudo cargar roadNevado.png" << std::endl;
        exit(1);
    }
    if (!mTexturas[5].loadFromFile("assets/images/roadPlaya.png")) {
        std::cerr << "Error: No se pudo cargar roadPlaya.png" << std::endl;
        exit(1);
    }
}

void GestorTemas::updateFromScore(int currentScore) {
    mTemaAnterior = mTemaActual;
    calcularTema(currentScore);
    
    if (mTemaActual != mTemaAnterior && !mEnTransicion) {
        // Iniciar transición con fade
        mEnTransicion = true;
        mTiempoFade = 0.0f;
        mTexturaYaCambiada = false;
        std::cout << "\n=== INICIANDO TRANSICION ===" << std::endl;
        std::cout << "Nuevo tema: " << mNombresTemas[mTemaActual] << std::endl;
        std::cout << "Score: " << currentScore << std::endl;
        std::cout << "===========================" << std::endl;
    }
}

void GestorTemas::calcularTema(int score) {
    // Cada 500 puntos cambia de tema, intercalando cíclicamente
    int temaIndex = (score / mScoreThreshold) % mTexturas.size();
    mTemaActual = temaIndex;
}

const sf::Texture& GestorTemas::getTexturaActual() const {
    return mTexturas[mTemaActual];
}

int GestorTemas::getTemaActual() const {
    return mTemaActual;
}

bool GestorTemas::hayCambioTema() const {
    return mHaCambiado;
}

void GestorTemas::updateFade(float deltaTime) {
    if (!mEnTransicion) return;
    
    mTiempoFade += deltaTime;
    float mitadDuracion = mDuracionFade / 2.0f;
    
    if (mTiempoFade <= mitadDuracion) {
        // Fade out (oscurecer)
        mOpacidadFade = (mTiempoFade / mitadDuracion) * 255.0f;
    } else {
        // Fade in (aclarar)
        float tiempoFadeIn = mTiempoFade - mitadDuracion;
        mOpacidadFade = (1.0f - (tiempoFadeIn / mitadDuracion)) * 255.0f;
    }
    
    // Cambiar textura en el punto medio (máxima oscuridad)
    if (mTiempoFade >= mitadDuracion && !mTexturaYaCambiada) {
        mTexturaYaCambiada = true;
        mHaCambiado = true;  // Señal para que Game actualice las texturas
    }
    
    // Terminar transición
    if (mTiempoFade >= mDuracionFade) {
        mEnTransicion = false;
        mTiempoFade = 0.0f;
        mOpacidadFade = 0.0f;
    }
}

bool GestorTemas::estaMostrandoFade() const {
    return mEnTransicion;
}

float GestorTemas::getOpacidadFade() const {
    return mOpacidadFade;
}

void GestorTemas::confirmarCambio() {
    mHaCambiado = false;
}
