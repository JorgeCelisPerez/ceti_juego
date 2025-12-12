#ifndef COUNTDOWN_HPP
#define COUNTDOWN_HPP

#include <SFML/Graphics.hpp>

class Countdown {
public:
    Countdown();
    bool init(const sf::Font& font);
    
    void reset();  // Reiniciar el countdown a 3
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
