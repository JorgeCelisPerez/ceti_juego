#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class GestorTemas {
public:
    GestorTemas();
    
    void updateFromScore(int currentScore);
    void updateFade(float deltaTime);
    const sf::Texture& getTexturaActual() const;
    int getTemaActual() const;
    bool hayCambioTema() const;
    void confirmarCambio();
    
    // Sistema de fade
    bool estaMostrandoFade() const;
    float getOpacidadFade() const;
    
private:
    void cargarTexturas();
    void calcularTema(int score);
    
    std::vector<sf::Texture> mTexturas;
    std::vector<std::string> mNombresTemas;
    
    int mTemaActual;
    int mTemaAnterior;
    int mScoreThreshold;  // Cada cuántos puntos cambia (500)
    bool mHaCambiado;
    
    // Sistema de fade
    bool mEnTransicion;
    float mTiempoFade;
    float mDuracionFade;  // Duración total del fade (ida y vuelta)
    float mOpacidadFade;
    bool mTexturaYaCambiada;
};
