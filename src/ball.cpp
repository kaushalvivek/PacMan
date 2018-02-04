#include "ball.h"
#include "main.h"

#define pi 3.1415
Ball::Ball(float x, float y, color_t color, float radius) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    yspeed = 0;
    speed = 0;
    
    this->radius = radius;

    static GLfloat g_vertex_buffer_data[10000];
    float theta = 1;
    // float radius = 0.25;
    int j =0;
    for(int i=0;i<360;i++)
    {
        g_vertex_buffer_data[j++] = 0.0f;
            g_vertex_buffer_data[j++] = 0.0f;
            g_vertex_buffer_data[j++] = 0.0f;
            
            g_vertex_buffer_data[j++] = radius*sin((pi*(theta)/180));
            g_vertex_buffer_data[j++] = radius*cos((pi*(theta)/180));
            g_vertex_buffer_data[j++] = 0.0f;

            theta = theta + 1;

            g_vertex_buffer_data[j++] = radius*sin((pi*(theta)/180));
            g_vertex_buffer_data[j++] = radius*cos((pi*(theta)/180));
            g_vertex_buffer_data[j++] = 0.0f;
    }

    this->object = create3DObject(GL_TRIANGLES, 1080, g_vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    // std::cout<<this->position.x<<" "<<this->position.y<<'\n';
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->position.x -= speed;
    this->position.y += yspeed;

    if(this->position.y > -2) {
        yspeed = yspeed - 0.01;
    }
   else {
       yspeed = 0;
       this->position.y = -2;
   }
}

bounding_box_t Ball::bounding_box() {
float x = this->position.x, y = this->position.y;
float radius = this->radius;
bounding_box_t bbox = { x, y, radius};
return bbox;
}
