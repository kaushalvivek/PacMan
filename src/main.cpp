#include "timer.h"
#include "ball.h"
#include "ball.cpp"
#include "color.cpp"
#include "input.cpp"
#include "nonedit.cpp"
#include "other_handlers.cpp"
#include "timer.cpp"

#include <stdlib.h>

using namespace std;
#define pi 3.1415

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
int up_flag = 0;

float score = 0.0;
float life = 600.0;
int level = 1;
float spike_height = -1.85;
float base = 0.01;
//-------------------------
struct object{
  VAO* vao;
};
typedef struct object object;
object ground, pond, trampoline, rectangle, magnet, porcupine;

//Draw Ground
void Ground(object G)
{
  static const GLfloat vertex_buffer_data[] = {
    -4.0f, -2.25f, 0.0f,
    4.0f, -2.25f, 0.0f,
    -4.0f, -4.0f, 0.0f,

    4.0f, -2.25f, 0.0f,
    4.0f, -4.0f, 0.0f,
    -4.0f, -4.0f, 0.0f,
  };
  G.vao = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, COLOR_GREEN, GL_FILL);
  draw3DObject(G.vao);
}

void Pond(object P)
{
    static GLfloat vertex_buffer_data[10000];
    float theta = 1;    
    float radius = 1;
    int j =0;
    for(int i=0;i<180;i++)
    {
        vertex_buffer_data[j++] = 0.0f;
            vertex_buffer_data[j++] = -2.25f;
            vertex_buffer_data[j++] = 0.0f;
            
            vertex_buffer_data[j++] = radius*cos(-1*(pi*(theta)/180));
            vertex_buffer_data[j++] = -2.25+radius*sin(-1*(pi*(theta)/180));
            vertex_buffer_data[j++] = 0.0f;

            theta = theta + 1;

            vertex_buffer_data[j++] = radius*cos(-1*(pi*(theta)/180));
            vertex_buffer_data[j++] = -2.25+radius*sin(-1*(pi*(theta)/180));
            vertex_buffer_data[j++] = 0.0f;
    }

    P.vao = create3DObject(GL_TRIANGLES, 540, vertex_buffer_data, COLOR_BLUE, GL_FILL);
    draw3DObject(P.vao);
}

void Trampoline(object T)
{
    static const GLfloat vertex_buffer_data[] = {
    2.0f, -1.75f, 0.0f, //v1
    2.1f, -1.75f, 0.0f, //v2
    2.0f, -2.25f, 0.0f, //v3

    2.1f, -2.25f, 0.0f,
    2.0f, -2.25f, 0.0f,
    2.1f, -1.75f, 0.0f,
  };
  T.vao = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, COLOR_BROWN, GL_FILL);
  draw3DObject(T.vao);

  static const GLfloat n2_vertex_buffer_data[] = {
    2.0f, -1.75f, 0.0f, //v1
    2.7f, -1.75f, 0.0f, //v2
    2.0f, -1.85f, 0.0f, //v3

    2.7f, -1.85f, 0.0f,
    2.0f, -1.85f, 0.0f,
    2.7f, -1.75f, 0.0f,
  };
  T.vao = create3DObject(GL_TRIANGLES, 6, n2_vertex_buffer_data, COLOR_BROWN, GL_FILL);
  draw3DObject(T.vao);

    
    static const GLfloat n1_vertex_buffer_data[] = {
    2.7f, -1.75f, 0.0f, //v1
    2.8f, -1.75f, 0.0f, //v2
    2.7f, -2.25f, 0.0f, //v3

    2.8f, -2.25f, 0.0f,
    2.7f, -2.25f, 0.0f,
    2.8f, -1.75f, 0.0f,
  };
  T.vao = create3DObject(GL_TRIANGLES, 6, n1_vertex_buffer_data, COLOR_BROWN, GL_FILL);
  draw3DObject(T.vao);
}

void Magnet(object M)
{
    static const GLfloat vertex_buffer_data[] = {
    -3.8f, 1.65f, 0.0f, //v1
    -3.1f, 1.65f, 0.0f, //v2
    -3.8f, 1.55f, 0.0f, //v3
  };
  M.vao = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, COLOR_GRAY, GL_FILL);
  draw3DObject(M.vao);

  static const GLfloat n2_vertex_buffer_data[] = {
    -3.8f, 1.65f, 0.0f, //v1
    -3.8f, 2.5f, 0.0f, //v2
    -3.7f, 2.5f, 0.0f, //v3

    -3.7f, 1.65f, 0.0f,
    -3.8f, 1.65f, 0.0f,
    -3.7f, 2.5f, 0.0f,
  };
  M.vao = create3DObject(GL_TRIANGLES, 6, n2_vertex_buffer_data, COLOR_RED, GL_FILL);
  draw3DObject(M.vao);

    static const GLfloat n1_vertex_buffer_data[] = {
    -3.1f, 2.4f, 0.0f, //v1
    -3.8f, 2.4f, 0.0f, //v2
    -3.8f, 2.5f, 0.0f, //v3

  };
  M.vao = create3DObject(GL_TRIANGLES, 3, n1_vertex_buffer_data, COLOR_GRAY, GL_FILL);
  draw3DObject(M.vao);
}


void Porcupine(object PP)
{
    static const GLfloat vertex_buffer_data[] = {

    -3.0f, -2.25f, 0.0f,
    -2.9f, spike_height, 0.0f,
    -2.8f, -2.25f, 0.0f,

    -2.8f, -2.25f, 0.0f,
    -2.7f, spike_height, 0.0f,
    -2.6f, -2.25f, 0.0f,

    -2.6f, -2.25f, 0.0f,
    -2.5f, spike_height, 0.0f,
    -2.4f, -2.25f, 0.0f,

    -2.4f, -2.25f, 0.0f,
    -2.3f, spike_height, 0.0f,
    -2.2f, -2.25f, 0.0f,

    -2.2f, -2.25f, 0.0f,
    -2.1f, spike_height, 0.0f,
    -2.0f, -2.25f, 0.0f,

    -2.0f, -2.25f, 0.0f,
    -1.9f, spike_height, 0.0f,
    -1.8f, -2.25f, 0.0f,
  };

  PP.vao = create3DObject(GL_TRIANGLES, 18, vertex_buffer_data, COLOR_RED, GL_FILL);
  draw3DObject(PP.vao);
}



Ball ball1;
Ball ball[200];

//---------------------
Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model


    Matrices.model = glm::mat4(1.0f);
    MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        Ground(ground);

    

    Matrices.model = glm::mat4(1.0f);
    MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        Pond(pond);

    ball1.draw(VP);
    for(int i=0;i<200;i++)
    {
        ball[i].draw(VP);
    }

    Matrices.model = glm::mat4(1.0f);
    MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        Trampoline(trampoline);

    Matrices.model = glm::mat4(1.0f);
    MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        Magnet(magnet);

     Matrices.model = glm::mat4(1.0f);
    MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
        Porcupine(porcupine);


    // for translation
    // Matrices.model = glm::mat4(1.0f);
    // glm::mat4 pond_translate = glm::translate (glm::vec3(-2,2,0));    // glTranslatef
    // Matrices.model *= (pond_translate);
    //  MVP = VP * Matrices.model;
    // glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
//    cout<<"Ball co-ords: "<<ball1.position.x<<" "<<ball1.position.y<<endl;
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);

    // For moving ball1 left or right
    if (left) {
        // #Condition to Stop the ball in Trampoline
        if (ball1.position.x > 2.7 && ball1.position.x < 3.05 && ball1.position.y < -1.7) {
            ball1.speed = 0;
        }

        // Enter for pond

        else if (ball1.position.x > -1.2 && ball1.position.x < 0.75 && ball1.position.y < -2) {
            ball1.speed = 0.01;
        }

        else 
            ball1.speed = 0.05;

    }
    else if (right) {

        // Trampoline Stop
        if (ball1.position.x > 1.7 && ball1.position.x < 2.5 && ball1.position.y < -1.7) {
            ball1.speed = 0;
        }

        // Enter for pond

        else if (ball1.position.x > -1.2 && ball1.position.x < 0.75 && ball1.position.y < -2) {
            ball1.speed = -0.01;
        }

        else 
        ball1.speed = -0.05;
    }
    else {
        if (ball1.speed < 0) {
            ball1.speed += 0.001;
        }
        else if (ball1.speed > 0) {
            ball1.speed -= 0.001;
        }
        else {
            ball1.speed = 0;
        }
    }

    // For making ball1 jump
    if (up == GLFW_PRESS and !up_flag) {
        up_flag = 1;
        
        //Trampoline
        if (ball1.position.x > 2 && ball1.position.x < 3) {
            ball1.yspeed = 0.2; 
        }

        // // Pond
        // else if (ball1.position.x > -1.2 && ball1.position.x < 0.75 && ball1.position.y== -2.2) {
        //     ball1.yspeed = 0.05;
        // }

        else 
            ball1.yspeed = 0.15;
    }

    if (up == GLFW_RELEASE) {
        up_flag = 0;
    }
}

int counter = 0;

void tick_elements() {

    counter++;

    char score_print[200];
    sprintf(score_print, "Score : %f  Health : %f Level : %d", score, life, level);
    glfwSetWindowTitle(window, score_print);

    if (score > 50 && score < 100) {
        level = 2;
        spike_height = -1.55;
        base = 0.05;
    }

    if (score > 100) {
        level = 3;
        spike_height = -1.25;
        base = 0.08;
    }


    if (counter == 100) {
        cout << "\n" << "SCORE : " << score << "\n";
        cout  << "LIFE : " << life << "\n";
        counter = 0;
    }

    if (life < 0 ) {
        cout << "\nGAME OVER" ;
        cout << "\n" << "Your Score is : " << score << "\n";
        quit(window);
    }

    ball1.tick();
    // Pond
    if (ball1.position.x > -1.2 && ball1.position.x < 0.75 && ball1.position.y== -2.0) {
            ball1.position.y = -2.2;
    }

    // Magnet
    if (ball1.position.y > 1.5 && ball1.position.y < 2.5) {
        ball1.speed = 0.08;
    }

    //Porcupine
    if (ball1.position.x < -1.6 && ball1.position.x > -3.0 && ball1.position.y < -1.9) {
        life-=20;
    }

    for(int i =0;i<200;i++)
    {
        // ball[i].speed = rand()%100 / 1000.0 + 0.01;
        // ball[i].speed = speed_rand;
        ball[i].yspeed = 0;
        ball[i].tick();

        if ((detect_collision(ball1.bounding_box(),ball[i].bounding_box()))) {
            ball[i].position.x = -100000;
            ball1.yspeed = 0.15;
            score += 10;
        }
    }

    // Trampoline
    if (ball1.position.x > 2 && ball1.position.x < 3 
    && ball1.position.y < -1.7 && ball1.position.y > -1.9 ) {
        ball1.yspeed = 0.30;
    }

//    ball2.tick();
//    if (detect_collision(ball1.bounding_box(), ball2.bounding_box())) {
//        ball1.speed = -ball1.speed;
//    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball1 = Ball (3.8, -2, COLOR_ORANGE, 0.25);

    for (int i = 0; i < 200 ; i++) {
        float y = -3 + rand()%5;
        int x = rand()%330;
        int color_R = rand()%255;
        int color_B = rand()%255;
        int color_G = rand()%255;
        color_t COLOR_RANDOM = {color_R , color_G , color_B };
        float radius_var = (rand()%100)/200.0 + 0.1;
        ball[i] = Ball(x,y+2.2,COLOR_RANDOM,radius_var);
    }

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 700;
    int height = 700;

    window = initGLFW(width, height);

    initGL (window, width, height);
    
    for (int i = 0; i < 200 ; i++) {
    ball[i].speed = rand()%100 / 2000.0 + base;
    }

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        reshapeWindow (window, width, height);
        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (pow(a.x - b.x,2) + pow(a.y - b.y,2) < pow(a.radius + b.radius,2));
}
// -------------------------


// ----------------------------------
void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
