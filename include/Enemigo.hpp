#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(float x, float y, int lane, float speed, float laneLeft, float laneRight);
    
    void update(float dt);
    bool isOutOfBounds(float maxY) const;
    
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
