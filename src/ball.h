#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color, float radius);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float speed;
    double yspeed;
    float radius;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
