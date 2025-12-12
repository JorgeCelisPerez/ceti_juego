#ifndef COUNTDOWN_HPP
#define COUNTDOWN_HPP

#include <SFML/Graphics.hpp>
#include "SoundManager.hpp"

class Countdown {
public:
    Countdown();
    bool init(const sf::Font& font);
    
    void reset();  // Reiniciar el countdown a 3
    void start(SoundManager& soundManager);  // Iniciar countdown con sonido
    void update(float deltaTime);
    bool isFinished() const;  // true cuando termina y debe empezar el juego
    void draw(sf::RenderWindow& window);
    void updateScale(float windowWidth, float windowHeight);
    
private:
    sf::Text mCountdownText;
    float mTimer;
    int mNumber;  // 3, 2, 1, 0 (A Correr!)
    bool mFinished;
    
    float mBaseResolutionWidth;
    float mBaseResolutionHeight;
};

#endif
