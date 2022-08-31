/*********
STUDENT NAME : Alper Celik
HOMEWORK: Homework #3
----------
PROBLEMS: none (I've done all requirements).
----------
ADDITIONAL FEATURES:
* I inspired light simulation and I have added my lightling feature.
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


#define WINDOW_WIDTH  1100
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
bool timerActive = true; //for pause/restart


//Mouse Cursor location's global variables
int xCursor, yCursor; //GLUT version
int xCursorGL, yCursorGL; //OpenGL version

//the bool. function to find is the cursor is inside or outside of the line
bool isInside(int x, int y)
{
    if (x > 100 && x < 1000 && y > 100 && y < 600)
        return true;
    else
        return false;
}

typedef struct //structure of vertex(locations)
{
    double x, y;
}vertex_t;

typedef struct 
{
    vec_t position, pace; //position and speed
}zuper_t;
zuper_t zuperx = { {0,0},{3,0} }; 

vec_t reflectpos;// it simply keeps position of reflection.
int wall = 5;//which wall is hit (assigned garbage value)
int wallcount = 0;// reflection duration


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

//vertex function
void vertex(vertex_t P, vertex_t Tr, double angle)
{
    double xp = (P.x) * cos(angle) - P.y * sin(angle) + Tr.x; 
    double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    glVertex2d(xp, yp);
}

void zuperman()
{

    polar_t pol = rec2pol(zuperx.pace); //rectangular to polar
    pol.angle = (pol.angle - 90) * D2R; // turn angle to radian, -90 to reset default rotation.

    //body
    glColor3ub(0, 92, 252);
    glBegin(GL_POLYGON);
    vertex({ -25, -45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 25, -45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 25, 45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -25, 45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glEnd();

    glColor3ub(252, 27, 0);
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    vertex({ -25, -45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 25, -45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 25, 45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -25, 45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glEnd();

    //symbol
    glColor3ub(255, 195, 0);
    glBegin(GL_POLYGON);
    vertex({ -15, -17 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 0, -25 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 15, -17 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 15, 20 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -15, 20 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glEnd();
    glLineWidth(3);
    glColor3ub(252, 27, 0);
    glBegin(GL_LINE_LOOP);
    vertex({ -15, -17 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 0, -25 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 15, -17 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 15, 20 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -15, 20 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glEnd();
    glColor3ub(252, 27, 0);
    glLineWidth(2);

    //left
    glBegin(GL_LINES);
    vertex({ 9, -12 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -9, -12 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -9, -12 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 9, 4 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 9, 4 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -9, 4 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glEnd();


    glColor3ub(0, 92, 252);
    //left arm and hand
    glBegin(GL_POLYGON);
    vertex({ -35, 45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -25, 45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -25, 0 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -35, 0 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glEnd();
    glColor3ub(252, 27, 0);


    //right
    glBegin(GL_POLYGON);
    vertex({ -35, 5 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -25, 5 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -25, -5 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -35, -5 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glEnd();

    glColor3ub(0, 92, 252);
    //right arm and hand
    glBegin(GL_POLYGON);
    vertex({ 23, 43 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 33, 43 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 33, 88 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 23, 88 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glEnd();

    
    glColor3ub(252, 27, 0);
    glBegin(GL_POLYGON);
    vertex({ 23, 96 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 33, 96 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 33,86 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 23, 86 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glEnd();

    //legs
    glColor3ub(0, 92, 252);
    glBegin(GL_TRIANGLES);
    vertex({ -25, -45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);//left
    vertex({ 0, -45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -13, -85 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glPointSize(5);

    glColor3ub(0, 92, 252);
    vertex({ 0, -45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);//right
    vertex({ 25, -45 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 13, -85 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glColor3f(0, 0, 0);
    glEnd();
    
    glColor3ub(252, 27, 0);
    glBegin(GL_POLYGON); //left foot
    vertex({ -20, -75 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -6, -75 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -6, -89 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -20, -89 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glEnd();
    
    glColor3ub(252, 27, 0);
    glBegin(GL_POLYGON); //right foot
    vertex({ 20, -75 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 6, -75 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 6, -89 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 20, -89 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glEnd(); 

    //backside of the head (hair)
    glColor3ub(150, 75, 0);
    glBegin(GL_POLYGON);
    vertex({ -20, 83 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 20, 83 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ 20,43 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    vertex({ -20, 43 }, { zuperx.position.x,zuperx.position.y }, pol.angle);
    glEnd();

    //angle counter
    glColor3f(1, 1, 1);
    vprint(-417, -300, GLUT_BITMAP_9_BY_15, "%.0f", rec2pol(zuperx.pace).angle);

}

int ifHitsWall(vec_t* pos)
{

    double vx = zuperx.pace.x, vy = zuperx.pace.y; // components of pace(speed)

    if (zuperx.position.x > -410 && zuperx.position.x < -400)
    {
        zuperx.pace = { -vx,vy };
        *pos = zuperx.position; // the position of the zuperman when it hits the wall
        return 0;//left border
    }

    else if (zuperx.position.x < 410 && zuperx.position.x > 400)
    {
        zuperx.pace = { -vx,vy };
        *pos = zuperx.position;
        return 1;//right border
    }

    else if (zuperx.position.y < 210 && zuperx.position.y > 200)
    {
        *pos = zuperx.position;
        zuperx.pace = { vx,-vy };
        return 2;//top border
    }

    else if (zuperx.position.y < -200 && zuperx.position.y > -210)
    {
        zuperx.pace = { vx,-vy };
        *pos = zuperx.position;
        return 3;//bottom border
    }
    else
        return 5;//garbage value, to get rid of the random value that assigned by compiler.

    // I used "return" to take a value for using in my lightning feature.
    //Also, when mouse pointer is outside, reflection is calculated in this function.
}

void ScreenDesign()
{

    glColor3ub(120, 31, 31);
    glRectf(-550, 350, -450, -350); //left rectangular
    glRectf(550, 350, 450, -350); //right rectangular
    glRectf(-550, 350, 550, 250); //top rectangular
    glRectf(-550, -350, 550, -250); //bottom rectangular

    //name&surname and ID
    glColor3ub(93, 225, 35);
    vprint(400, 320, GLUT_BITMAP_HELVETICA_18, "ALPER CELIK");
    vprint(420, 300, GLUT_BITMAP_9_BY_15, "21903022");
    vprint(-515, 305, GLUT_BITMAP_HELVETICA_18, "CTIS 164 - 2");
    //name of the game
    glColor3f(1, 1, 1);
    vprint(-100, 305, GLUT_BITMAP_TIMES_ROMAN_24, "CHASING GAME");

    glColor3ub(222, 252, 2);
    vprint(-480, -300, GLUT_BITMAP_9_BY_15, "Angle: "); //angle counter
    vprint(300, -300, GLUT_BITMAP_8_BY_13, "Mouse Pointer is "); 
    //mouse is inside or outside
    glColor3f(1, 1, 1);
    if (isInside(xCursor, yCursor))
    {
        vprint(435, -300, GLUT_BITMAP_8_BY_13, "inside");

        glColor3ub(222, 252, 2);
        circle(xCursorGL, yCursorGL, 8); //the small circle at the position of the mouse pointer
    }
    else
    {
        vprint(435, -300, GLUT_BITMAP_8_BY_13, "outside");
    }


    //to select timer is 1 or 0
    glColor3ub(222, 252, 2);
    if (timerActive)
        vprint(-170, -300, GLUT_BITMAP_9_BY_15, "Press <s or S> to pause the chasing");
    else
        vprint(-170, -300, GLUT_BITMAP_9_BY_15, "Press <s or S> to restart the chasing");


}

void display() {
    // the background colour
    glClearColor(0 / 255., 252 / 255., 241 / 255., 0);
    glClear(GL_COLOR_BUFFER_BIT);


    ScreenDesign();
    zuperman(); //my shape
    
    //displays lightning feature
    double px = reflectpos.x, py = reflectpos.y;
    if (wallcount >= 0) // if zuperman hits the wall
        if (wall == 0) //left
        {
            glBegin(GL_POLYGON);
            glColor3ub(235, 232, 52);
            glVertex2f(px - 50, py + 20);
            glVertex2f(px - 50, py - 20);
            glColor3ub(120, 31, 31);
            glVertex2f(px - 90, py - 20);
            glVertex2f(px - 90, py + 20);
            glEnd();
        }
        else if (wall == 1) //right
        {
            glBegin(GL_POLYGON);
            glColor3ub(235, 232, 52);
            glVertex2f(px + 50, py + 20);
            glVertex2f(px + 50, py - 20);
            glColor3ub(120, 31, 31);
            glVertex2f(px + 90, py - 20);
            glVertex2f(px + 90, py + 20);
            glEnd();
        }
        else if (wall == 2) //top
        {
            glBegin(GL_POLYGON);
            glColor3ub(235, 232, 52);
            glVertex2f(px - 20, py + 50);
            glVertex2f(px + 20, py + 50);
            glColor3ub(120, 31, 31);
            glVertex2f(px + 20, py + 90);
            glVertex2f(px - 20, py + 90);
            glEnd();
        }
        else  if (wall == 3) //bottom
        {
            glBegin(GL_POLYGON);
            glColor3ub(235, 232, 52);
            glVertex2f(px - 20, py - 50);
            glVertex2f(px + 20, py - 50);
            glColor3ub(120, 31, 31);
            glVertex2f(px + 20, py - 90);
            glVertex2f(px - 20, py - 90);
            glEnd();
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

    //pressing 's' or 'S' makes the timer enable/disable
    if (key == 115 || key == 83) // s or S
    {
        timerActive = !timerActive;
    }

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


    if (timerActive == true) 
    {
        xCursor = x;
        yCursor = y;

        xCursorGL = xCursor - 1100 / 2.; //OpenGL version
        yCursorGL = 700 / 2. - yCursor; //OpenGL version

        //assinging global variables (glut values of cursor) to the boolean function
        if (isInside(xCursor, yCursor))
        {
            //setting angle of the zuperman
            polar_t tmp = rec2pol(zuperx.pace); // it converts rectangular to polar because it is much better to calculate angle, easily.

            tmp.angle = atan2(yCursorGL - zuperx.position.y, xCursorGL - zuperx.position.x) / D2R; //now angle equals which is between mouse and angle
            if (tmp.angle < 0)
                tmp.angle = tmp.angle + 360;
            zuperx.pace = pol2rec(tmp); // now it can be converted to rectangular, again.
        }

    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);

    if (timerActive == true)
    {
        //it changes position
        zuperx.position = addV(zuperx.position, zuperx.pace);

        if (!isInside(xCursor, yCursor)) //mouse pointer is outside
        {
            int tmp = ifHitsWall(&reflectpos); //for the reflection.
            if (tmp != 5) //if it is not garbage value (if it hits the wall)
            {
                wall = tmp; //it assigned the wall that is hit.
                wallcount = 20;
            }

            wallcount--;
            printf("wall:%d   tmp=%d  wallcount:%d \n", wall, tmp, wallcount);//for debugging

        }
        else //mouse pointer is inside
        {
            //character stops when catch the mouse
            polar_t tmp = rec2pol(zuperx.pace); // i need polar to calculate more easily.
            if (sqrtf(powf(xCursorGL - zuperx.position.x, 2) + powf(yCursorGL - zuperx.position.y, 2)) < 20)
            {
                tmp.magnitude = 0; // when zuperman chases the mouse pointer, he stops.
                printf("in if\n"); // for debugging
            }
            else
                tmp.magnitude = 3; //zuperman goes brr.

            zuperx.pace = pol2rec(tmp); //it is returned to rectangular form, again.
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
    glutCreateWindow("'Zuperman tries to chase the mouse, which is a pointer' by Alper Celik");

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
