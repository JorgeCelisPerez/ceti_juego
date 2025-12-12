#pragma once

#include <SFML/Graphics.hpp>

class GameOverScreen {
public:
    GameOverScreen(float width, float height);

    void update(sf::Time dt);
    void draw(sf::RenderWindow& window);
    void reset();
    bool isDone() const;

private:
    void setupText(float width, float height);

    sf::Font mFont;
    sf::Text mGameOverText;

    sf::Time mTimer;
    sf::Time mDuration;
    bool mFinished;
};
