#pragma once
#include <SFML/Graphics.hpp>

class Gasolina {
public:
    Gasolina(float x, float y, int lane, float speed, sf::Texture& texture, float scaleFactor);
    
    void update(float dt);
    bool isOutOfBounds(float maxY) const;
    void updateLanePosition(float newX);
    void updateScale(float scaleFactor);
    
    sf::Sprite& getSprite();
    const sf::Sprite& getSprite() const;
    int getLane() const;
    float getY() const;
    float getRecargaAmount() const;

private:
    sf::Sprite mSprite;
    float mSpeed;
    int mLane;
    float mRecargaAmount;
    float mBaseScale;
};
