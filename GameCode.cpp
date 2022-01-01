/*********
   CTIS164 - Homework 3
----------
STUDENT : Talha Rehman Abid
SECTION : 4
HOMEWORK: 3
----------
PROBLEMS: None
----------
ADDITIONAL FEATURES: 1) Another ufo chasing the main ROCKET and you have to steer the main rocket away from the other ufo to hit                                         the main ufo and gain points
                     2) If the other ufo hits the rocket it blows up (goes invisible) then you have to launch another rocket
                     3) If the chasing ufo gets hit it blows up as well (goes invisible)
                     4) hold up arrow key for speed boost
                     5) left, right and down key to steer/control the rocket
                     6) timer (remaining time to play)
                     7) f4 to restart
                     8) score per hit
                     9) bonus points added to score when rocket is in speed boost(more impact from more momentum)
                     10) Game over screen
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 700
#define RUNNING_COUNTER 100
#define TIMER_PERIOD  12 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer
#define LOADING_COUNTER 20
#define D2R 0.0174532
#define LOADING 1
#define RUNNING 2
#define OVER 3

/* Global Variables for HW3 */
int load_timer = LOADING_COUNTER;
int timerT = TIMER_PERIOD;
int load_increment = 0.2;
int stat = LOADING; // stat is the state and set to initial state i.e. 0
int t_score = 0; // score of the user at the end of the game
int increment = 0; // timer influencer
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
float ufoAngle = 0;
float rocketAngle;
float rocketAngle2;
float amplitude = 250; //amplitude
int x, y;
double velocity = 3;
int speed = TIMER_PERIOD;
int count = 0; // timer counter
bool disp = false;
typedef struct {
    vec_t velocity;//vector for velocity
    vec_t coordinate; //x,y pos
}object_t; //structure for ufo

typedef struct {
    vec_t coordinate; //x,y pos
    bool visible;
    vec_t velocity;//rocket visible or not
    float angle; //angle of rocket
}rocket_t;

typedef struct {
    vec_t position;
    vec_t N;
} vertex_t;

vec_t mouse_cor; //mouse coordinates
rocket_t rocket = { {0,0}, false,{0,0}, 0 }; //initial values for the rocket
object_t ufo = { {4,0.1}, {x,y} }; //initial position and velocity of the ufo
rocket_t ufo2 = { {-350,-500}, false,{0,0}, 0 }; //initial values for the rocket


//
// to draw circle, center at (x,y)
// radius r
//

void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}
void object(int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();//circular motion object
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}


//
// To display onto window using OpenGL commands
//
void drawAngle() {
    float r = fabs(amplitude); //amplitude
    glColor3f(1, 0, 0);
    //red point on blue circle
    x = (r * cos(ufoAngle * D2R) - 100);
    y = (r * sin(ufoAngle * D2R) + 25);
    object(5);
}
void drawRocket(rocket_t rocket)
{
    if (rocket.visible == true) {
        glColor3f(1, 1, 0);
        vec_t vertex1 = subV(rocket.coordinate, pol2rec({ 50,rocket.angle + 1 }));
        vec_t vertex2 = subV(vertex1, pol2rec({ -10,rocket.angle + 88 }));
        vec_t vertex3 = subV(vertex2, pol2rec({ -41,rocket.angle }));
        vec_t vertex4 = addV(rocket.coordinate, pol2rec({ 9,rocket.angle + 11 }));
        glColor3f(0.2, 0.5, 0.9);
        glBegin(GL_TRIANGLES);
        glVertex2f(rocket.coordinate.x, rocket.coordinate.y);
        glVertex2f(vertex3.x, vertex3.y);
        glVertex2f(vertex4.x, vertex4.y);
        glEnd();
        glColor3f(0.66, 0.7, 0.8);
        glBegin(GL_POLYGON);
        glVertex2f(rocket.coordinate.x, rocket.coordinate.y);
        glVertex2f(vertex1.x, vertex1.y);
        glVertex2f(vertex2.x, vertex2.y);
        glVertex2f(vertex3.x, vertex3.y);
        glVertex2f(rocket.coordinate.x, rocket.coordinate.y);
        glEnd();
    }

}//rocket

void drawRocketTwo(rocket_t rocket)
{
    if (ufo2.visible == true) {
        glColor3f(1, 1, 0);
        vec_t vertex1 = subV(ufo2.coordinate, pol2rec({ 60,ufo2.angle + 1 }));
        vec_t vertex2 = subV(vertex1, pol2rec({ 10,ufo2.angle + 88 }));
        vec_t vertex3 = subV(vertex2, pol2rec({ -31,ufo2.angle }));
        vec_t vertex4 = addV(ufo2.coordinate, pol2rec({ 19,ufo2.angle + 11 }));


    }

}//rocket

void displayloading() {
    glColor3f(0, 0, 0);
    vprint2(-100, 100, 0.3, "LOADING");

    int width = (LOADING_COUNTER - load_timer) * 30;
    glColor3ub(170, 250, 140);
    glRectf(-300, -70, -300 + width, -20);
    vprint2(-100, 310, 0.1, "UFO DESTROYER");
}

void displayClouds(int a, int b) {

    glColor3ub(95, 95, 95); //white
    circle(0 + a, 0 + b, 30); // cloud 1
    circle(-25 + a, b, 20);
    circle(25 + a, -2 + b, 20);
    circle(21 + a, -19 + b, 10);

    circle(0 - 350 + a, 0 + 50 + b, 30);//cloud 2
    circle(-25 - 350 + a, +50 + b, 20);
    circle(25 - 350 + a, -2 + 50 + b, 20);
    circle(21 - 350 + a, -19 + 50 + b, 10);

    circle(0 + 150 + a, 0 + b, 30); //cloud 3
    circle(-25 + 150 + a, b, 20);
    circle(25 + 150 + a, -2 + b, 20);
    circle(21 + 150 + a, -19 + b, 10);

    circle(0 + 300 + a, 0 - 80 + b, 30); //cloud 4
    circle(-25 + 300 + a, -80 + b, 20);
    circle(25 + 300 + a, -80 - 2 + b, 20);
    circle(21 + 300 + a, -80 - 19 + b, 10);

    circle(0 + 480 + a, 0 + b, 30); //cloud 5
    circle(-25 + 480 + a, b, 20);
    circle(25 + 480 + a, -2 + b, 20);
    circle(21 + 480 + a, -19 + b, 10);

    circle(0 - 480 + a, 0 + 20 + b, 30); // cloud 6
    circle(-25 - 480 + a, 20 + b, 20);
    circle(25 - 480 + a, 20 - 2 + b, 20);
    circle(21 - 480 + a, 20 + -19 + b, 10);

    circle(0 - 250 + a, 0 + b, 30); // cloud 1
    circle(-25 - 250 + a, b, 20);
    circle(25 - 250 + a, -2 + b, 20);
    circle(21 - 250 + a, -19 + b, 10);
}

void displayrunning() {
    glColor3ub(204, 194, 183); // moon
    circle(-130, 280, 70);

    glColor3ub(164, 154, 143); // moon spots
    circle(190 - 320, 120 + 140, 20);
    glColor3ub(174, 164, 153); // moon spots
    circle(220 - 320, 160 + 140, 15);
    glColor3ub(174, 164, 153); // moon spots
    circle(170 - 320, 170 + 140, 15);
    glColor3ub(174, 164, 153); // moon spots
    circle(155 - 320, 150 + 140, 10);
    glColor3ub(174, 164, 153); // moon spots
    circle(190 - 320, 170 + 140, 5);
    glColor3ub(240, 196, 325); // star 1
    glBegin(GL_TRIANGLES);
    glVertex2f(-30, 140);
    glVertex2f(-10, 140);
    glVertex2f(-20, 120);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-30, 125);
    glVertex2f(-10, 125);
    glVertex2f(-20, 145);
    glEnd();

    int r = 30;


    glBegin(GL_TRIANGLES);
    glVertex2f(-30 - r, 140 - r);
    glVertex2f(-10 - r, 140 - r);
    glVertex2f(-20 - r, 120 - r);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-30 - r, 125 - r);
    glVertex2f(-10 - r, 125 - r);
    glVertex2f(-20 - r, 145 - r);
    glEnd();

    r += 40;


    glBegin(GL_TRIANGLES);
    glVertex2f(-30 + r, 140 + r);
    glVertex2f(-10 + r, 140 + r);
    glVertex2f(-20 + r, 120 + r);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-30 + r, 125 + r);
    glVertex2f(-10 + r, 125 + r);
    glVertex2f(-20 + r, 145 + r);
    glEnd();

    r += 25;


    glBegin(GL_TRIANGLES);
    glVertex2f(-30 - r, 140);
    glVertex2f(-10 - r, 140);// star 4
    glVertex2f(-20 - r, 120);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-30 - r, 125);
    glVertex2f(-10 - r, 125);
    glVertex2f(-20 - r, 145);
    glEnd();

    r + 15;

    glBegin(GL_TRIANGLES);
    glVertex2f(-30 + r, 140 - r);
    glVertex2f(-10 + r, 140 - r);  //star 5
    glVertex2f(-20 + r, 120 - r);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-30 + r, 125 - r);
    glVertex2f(-10 + r, 125 - r);
    glVertex2f(-20 + r, 145 - r);
    glEnd();


    r += 180;


    glBegin(GL_TRIANGLES);
    glVertex2f(-30 - r, 140);
    glVertex2f(-10 - r, 140);
    glVertex2f(-20 - r, 120); //star 6
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-30 - r, 125);
    glVertex2f(-10 - r, 125);
    glVertex2f(-20 - r, 145);
    glEnd();

    r = -350;

    glBegin(GL_TRIANGLES);
    glVertex2f(-30 + r, 140);
    glVertex2f(-10 + r, 140); //star 7
    glVertex2f(-20 + r, 120);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-30 + r, 125);
    glVertex2f(-10 + r, 125);
    glVertex2f(-20 + r, 145);
    glEnd();

    r = 350;

    glBegin(GL_TRIANGLES);
    glVertex2f(-30 + r, 140);
    glVertex2f(-10 + r, 140);  //star 8 
    glVertex2f(-20 + r, 120);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-30 + r, 125);
    glVertex2f(-10 + r, 125);
    glVertex2f(-20 + r, 145);
    glEnd();

    r = 500;
    int y = -80;

    glBegin(GL_TRIANGLES);
    glVertex2f(-15 + r, 125 + y);
    glVertex2f(5 + r, 125 + y);  // star 9
    glVertex2f(-5 + r, 105 + y);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-15 + r, 110 + y);
    glVertex2f(5 + r, 110 + y);
    glVertex2f(-5 + r, 130 + y);
    glEnd();
}
void displayGradient(int x1, int y1, int w, int h, float r, float g, float b) {
    glBegin(GL_QUADS);
    glColor3f(r, g, b);
    glVertex2f(x1, y1);
    glVertex2f(x1 + w, y1);
    glColor3f(r + 0.2, g + 0.2, b + 0.2);
    glVertex2f(x1 + w, y1 - h);
    glVertex2f(x1, y1 - h);
    glEnd();
    glColor3f(0.1, 0.1, 0.1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x1 + w, y1);
    glVertex2f(x1 + w, y1 - h);
    glVertex2f(x1, y1 - h);
    glEnd();
}

void displayufo2(int ufoX, int ufoY)
{


    //capsule
    glColor3ub(20, 100, 10);
    circle(ufoX + 44, ufoY + 35, 34);


    //ship frame
    glColor3ub(50, 50, 22);
    glBegin(GL_POLYGON);
    glVertex2f(ufoX, ufoY); // 1st left bottom
    glVertex2f(ufoX - 14, ufoY + 14); // side left bottom
    glVertex2f(ufoX, ufoY + 39); // one upper side  2nd vertex
    glVertex2f(ufoX + 19, ufoY + 19); // upper down 3rd


    glVertex2f(ufoX + 79, ufoY + 29); //5th new
    glVertex2f(ufoX + 89, ufoY + 39); //6th indent
    glVertex2f(ufoX + 104, ufoY + 14); // side left bottom
    glVertex2f(ufoX + 89, ufoY + 1); // 7th bottom
    glEnd();

    //bottom hover-enablers
    glColor3ub(47, 79, 79);
    circle(ufoX + 10, ufoY, 11);
    circle(ufoX + 33, ufoY, 11);
    circle(ufoX + 52, ufoY, 11);
    circle(ufoX + 74, ufoY, 11);

    glColor3ub(0.1, 0.1, 0.1);
    circle(ufoX + 10, ufoY, 5);
    circle(ufoX + 30, ufoY, 5);
    circle(ufoX + 52, ufoY, 5);
    circle(ufoX + 74, ufoY, 5);


    //outlines
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(ufoX + 17, ufoY + 24);
    glVertex2f(ufoX + 19, ufoY + 10);
    glVertex2f(ufoX + 71, ufoY + 27);
    glVertex2f(ufoX + 71, ufoY + 10);

    glEnd();

}


void displayufo(int ufoX, int ufoY)
{


    //capsule
    glColor3ub(240, 100, 10);
    circle(ufoX + 44, ufoY + 35, 34);


    //ship frame
    glColor3ub(25, 25, 112);
    glBegin(GL_POLYGON);
    glVertex2f(ufoX, ufoY); // 1st left bottom
    glVertex2f(ufoX - 14, ufoY + 14); // side left bottom
    glVertex2f(ufoX, ufoY + 39); // one upper side  2nd vertex
    glVertex2f(ufoX + 19, ufoY + 19); // upper down 3rd


    glVertex2f(ufoX + 79, ufoY + 29); //5th new
    glVertex2f(ufoX + 89, ufoY + 39); //6th indent
    glVertex2f(ufoX + 104, ufoY + 14); // side left bottom
    glVertex2f(ufoX + 89, ufoY + 1); // 7th bottom
    glEnd();

    //bottom hover-enablers
    glColor3ub(47, 79, 79);
    circle(ufoX + 10, ufoY, 11);
    circle(ufoX + 33, ufoY, 11);
    circle(ufoX + 52, ufoY, 11);
    circle(ufoX + 74, ufoY, 11);

    glColor3ub(0.1, 0.1, 0.1);
    circle(ufoX + 10, ufoY, 5);
    circle(ufoX + 30, ufoY, 5);
    circle(ufoX + 52, ufoY, 5);
    circle(ufoX + 74, ufoY, 5);


    //outlines
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(ufoX + 17, ufoY + 24);
    glVertex2f(ufoX + 19, ufoY + 10);
    glVertex2f(ufoX + 71, ufoY + 27);
    glVertex2f(ufoX + 71, ufoY + 10);

    glEnd();

}

void displayover() {
    glColor3f(0, 0, 0);
    if (t_score > 180) { vprint2(-350, 100, 0.25, "WOLLA! You managed to destroy the Evil UFO!"); }
    else
    {
        vprint2(-350, 100, 0.2, "Game Over! You were not able to destroy the Evil UFO)");
    }
    vprint2(-80, 0, 0.2, "Your Score: %d", t_score);
    vprint(-60, -250, GLUT_BITMAP_8_BY_13, "F4 to Play Again");
}
void display() {
    //
    // clear window to black
    //
    switch (stat) {
    case LOADING: displayloading(); break;
    case RUNNING: {

        glClearColor(0, 0.22, 0.22, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        displayGradient(-900, 450, 2000, 700, 0.17, 0.17, 0.17);
        displayClouds(0, 220);
        displayrunning();
        drawAngle();
        displayufo(x - 20, y - 20);
        drawRocketTwo(ufo2);
        if (ufo2.visible == true)
            displayufo2(ufo2.coordinate.x - 50, ufo2.coordinate.y - 50);
        drawRocket(rocket);
        glColor3f(1, 0.5, 0);

        vprint(-600, -170, GLUT_BITMAP_8_BY_13, "Total Score: %d", t_score);
        vprint(-600, -190, GLUT_BITMAP_8_BY_13, "Remaining Time: %d", 100 - count);
        vprint(-600, -210, GLUT_BITMAP_8_BY_13, "Remaining Score to win: %d", 180 - t_score);
        vprint(-600, -230, GLUT_BITMAP_8_BY_13, "<- arrow to steer left ");
        vprint(-600, -250, GLUT_BITMAP_8_BY_13, "-> arrow to steer right ");
        vprint(-600, -270, GLUT_BITMAP_8_BY_13, "up arrow to boost ");
        vprint(-600, -290, GLUT_BITMAP_8_BY_13, "down arrow to steer down ");
    }break;
    case OVER: displayover();
    }
    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }
    if (key == GLUT_KEY_F4 && stat == OVER) //to restart game when it is over
    {
        stat = RUNNING;
        t_score = 0;
        increment = 0;
    }


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.
    mouse_cor.x = x - winWidth / 2;
    mouse_cor.y = winHeight / 2 - y;

    vec_t a;
    vec_t b;
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && rocket.visible == false) //to display rocket moving towards ufo
    {
        disp = false;
        rocket.visible = true;

        rocket.coordinate = mouse_cor;

        a.x = ufo.coordinate.x - 1 - rocket.coordinate.x;
        a.y = ufo.coordinate.y - 1 - rocket.coordinate.y;

        rocketAngle = atan2(a.y, a.x) / D2R;

        rocket.angle = rocketAngle;

        if (ufo2.visible == false)
        {
            ufo2.visible = true;
            if (mouse_cor.x - 200 > -600 && mouse_cor.x - 200 < 600 && mouse_cor.y - 150 > -350 && mouse_cor.y - 150 < 350)
            {
                ufo2.coordinate.x = mouse_cor.x - 200;
                ufo2.coordinate.y = mouse_cor.y - 150;
            }b.x = rocket.coordinate.x - 1 - ufo2.coordinate.x;
            b.y = rocket.coordinate.y - 1 - ufo2.coordinate.y;
            rocketAngle2 = atan2(b.y, b.x) / D2R;
            ufo2.angle = rocketAngle2;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(speed, onTimer, 0);
    if (stat == LOADING) {
        load_timer -= 0.01;
        if (load_timer > 0) {
            speed = 100;
        }
        else {
            // time expires

            stat = RUNNING;
            speed = TIMER_PERIOD;
        }
    }
    increment += 10;
    count = increment / 350;
    if (stat == RUNNING && count == RUNNING_COUNTER) // to end at 20 seconds
        stat = OVER;
    if (stat == RUNNING) {
        if (timerT > 16)
            t_score = 0;
        timerT = 16;
        // Write your codes here.
        ufoAngle += 0.5;
        if (ufoAngle > 360)
            ufoAngle -= 360;
        if (left) { x -= 40; }
        if (right) { x += 40; };
        ufo.coordinate.x = x;
        ufo.coordinate.y = y;
        vec_t e;
        vec_t f;
        e.x = ufo.coordinate.x - rocket.coordinate.x + 1;
        e.y = ufo.coordinate.y - rocket.coordinate.y + 1;
        f.x = rocket.coordinate.x - ufo2.coordinate.x + 1;
        f.y = rocket.coordinate.y - ufo2.coordinate.x + 1;
        rocketAngle = atan2(e.y, e.x) / D2R;
        rocketAngle2 = atan2(f.y, f.x) / D2R;
        rocket.angle = rocketAngle; //tilt the rocket at the ufo
        ufo2.angle = rocketAngle2;
        rocket.velocity = mulV(velocity, unitV(subV(ufo.coordinate, rocket.coordinate))); //rocketvelocity

        ufo2.velocity = mulV(velocity - 1.3, unitV(subV(rocket.coordinate, ufo2.coordinate)));


        if (rocket.visible == true) {  //chase the ufo
            rocket.coordinate = addV(rocket.coordinate, rocket.velocity);
            ufo2.coordinate = addV(ufo2.coordinate, ufo2.velocity);
            vec_t hit;
            vec_t hit2;
            hit.x = rocket.coordinate.x - ufo.coordinate.x;
            hit.y = rocket.coordinate.y - ufo.coordinate.y;
            hit2.x = ufo2.coordinate.x - rocket.coordinate.x;
            hit2.y = ufo2.coordinate.y - rocket.coordinate.y;


            float d = sqrtf(hit.x * hit.x + hit.y * hit.y);
            float g = sqrtf(hit2.x * hit2.x + hit2.y * hit2.y);
            if (g <= 40) {
                ufo2.visible = false;
                rocket.visible = false;
            }
            if (d <= 20) {

                rocket.visible = false;
                t_score += 5;

            }if (up) //to boost the rocket into the ufo
                velocity += 0.3;
            if (!up)
                velocity = 3;
            if (left&& rocket.coordinate.x >-580)
                rocket.coordinate.x -= 5;// to steer the rocket left
            if (right&&rocket.coordinate.x < 580)
                rocket.coordinate.x += 5;// to steer the rocket right
            if (down &&rocket.coordinate.y>-330)
                rocket.coordinate.y -= 5;// to steer the rocket downwards
        }
        if (t_score > 180) {
            stat = OVER;

        }
    }
    // to refresh the window it calls display() function

    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("CTIS 164 - Talha Rehman Abid - Chase and Hit");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}