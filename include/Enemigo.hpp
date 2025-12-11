#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(float x, float y, int lane, float speed, float laneLeft, float laneRight, sf::Vector2f baseSize, float scaleFactor);
    
    void update(float dt);
    bool isOutOfBounds(float maxY) const;
    void updateLanePosition(float newX, float newLeft, float newRight);
    void updateSize(sf::Vector2f baseSize, float scaleFactor);
    
    sf::RectangleShape& getShape();
    const sf::RectangleShape& getShape() const;
    int getLane() const;
    float getY() const;

private:
    sf::RectangleShape mShape;
    float mSpeed;
    int mLane;
    float mLaneLeft;
    float mLaneRight;
};
