#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Enemy {
public:
    Enemy(float x, float y, int lane, float speed, float laneLeft, float laneRight, sf::Vector2f baseSize, float scaleFactor, const sf::Texture& texture, bool rotateSprite = false);
    
    void update(float dt);
    bool isOutOfBounds(float maxY) const;
    void updateLanePosition(float newX, float newLeft, float newRight);
    void updateSize(sf::Vector2f baseSize, float scaleFactor);
    
    sf::Sprite& getSprite();
    const sf::Sprite& getSprite() const;
    int getLane() const;
    float getY() const;

private:
    sf::Sprite mSprite;
    float mSpeed;
    int mLane;
    float mLaneLeft;
    float mLaneRight;
};
