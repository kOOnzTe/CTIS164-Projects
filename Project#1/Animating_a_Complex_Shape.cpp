/*********
STUDENT NAME : Alper Celik
HOMEWORK: Homework #1
----------
PROBLEMS: none.
----------
ADDITIONAL FEATURES:
    *Four clouds and one complex sun that have different speeds have been added. Additionally, these complex shapes are NOT stable.
    *Wings' color can be changed manually if you press 'c' or 'C'. In addition, RGB color codes can be seen on the screen.
    *You can fully stop the rocket by using <space> if you wish.
    *In the beginning, an important message has been given (with using message func). Also, all additional commands have been added to the screen.
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
#include <time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  9 // Period for the timer.
#define TIMER_ON      1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

bool timerActivator = true;

/* Other Global Variables, modes (which are constants) */
#define START 0
#define MANUAL 1
#define AUTONOMOUS 2

#define NOTSET 00
#define LEFT 01
#define RIGHT 02
#define UP 03
#define DOWN 04


int mode = START; //initial mode
int direction = NOTSET; //initial direction
int red = 161, green = 22, blue = 22; //initial wings' color.

//modes and directions
char modes[3][31] = { "NOT CHOSEN ","MANUAL", "AUTONOMOUS" };
char directions[5][20] = { "NOT SET","LEFT","RIGHT","UP", "DOWN" };

// whole rocket's initial position
int xR = 0, yR = 0;
int radiusR = 26; //glass' initial radius
// sun and cloud's initial position
double xC = 0, xS = 0;

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
void message()
{
    //background
    glColor3ub(122, 37, 37);
    glRectf(-250, -200, 280, -100);
    //message
    glColor3ub(52, 195, 235);
    vprint2(-230, -150, 0.2, "Peace at Home, Peace in the World.");
    vprint2(130, -180, 0.2, "ATATURK");
}
void rocket()
{
    //rectangular skeleton
    glColor3ub(122, 191, 190);
    glRectf(xR + -80, yR + -30, xR + 80, yR + 30);

    //head of the rocket (triangle)
    glColor3ub(161, 22, 22);
    glBegin(GL_TRIANGLES);
    glVertex2f(xR + 80, yR + 30);
    glVertex2f(xR + 80, yR + -30);
    glVertex2f(xR + 170, yR + 0);
    glEnd();

    //endpoint of head of the rocket 
    glPointSize(7);
    glColor3f(0, 0, 0);
    glBegin(GL_POINTS);
    glVertex2f(xR + 163, yR + 0);
    glEnd();

    //small wing #1
    glColor3ub(red, green, blue);
    glBegin(GL_TRIANGLES);
    glVertex2f(xR + 50, yR + 30);
    glVertex2f(xR + 50, yR + 60);
    glVertex2f(xR + 80, yR + 30);
    glEnd();
    //small wing #2
    glColor3ub(red, green, blue);
    glBegin(GL_TRIANGLES);
    glVertex2f(xR + 50, yR + -30);
    glVertex2f(xR + 50, yR + -60);
    glVertex2f(xR + 80, yR + -30);
    glEnd();

    //big wing #1
    glColor3ub(red, green, blue);
    glBegin(GL_TRIANGLES);
    glVertex2f(xR + -80, yR + 30);
    glVertex2f(xR + -30, yR + 30);
    glVertex2f(xR + -120, yR + 60);
    glEnd();
    //big wing #2
    glColor3ub(red, green, blue);
    glBegin(GL_TRIANGLES);
    glVertex2f(xR + -80, yR + -30);
    glVertex2f(xR + -30, yR + -30);
    glVertex2f(xR + -120, yR + -60);
    glEnd();

    //minibox and lines (symbol/logo of this rocket) 
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(xR + 50, yR + 10);
    glVertex2f(xR + 50, yR + -10);
    glVertex2f(xR + 70, yR + -10);
    glVertex2f(xR + 70, yR + 10);
    glEnd();

    glLineWidth(2);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(xR + 50, yR + -10);
    glVertex2f(xR + 70, yR + 10);
    glEnd();

    glLineWidth(2);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(xR + 70, yR + -10);
    glVertex2f(xR + 50, yR + 10);
    glEnd();
    glLineWidth(1);

    //glass door
    glColor3f(0, 0, 1);
    circle(xR, yR, radiusR + 2);
    glColor3f(1, 1, 1);
    circle(xR, yR, radiusR - 1);
    //frame of glass door
    glColor3ub(19, 117, 76);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xR + -25, yR + 0);
    glVertex2f(xR + 0, yR + 25);
    glVertex2f(xR + 25, yR + 0);
    glVertex2f(xR + 0, yR + -25);
    glEnd();
    //handle of glass door
    glPointSize(4);
    glColor3ub(19, 117, 76);
    glBegin(GL_POINTS);
    glVertex2f(xR + 0, yR + -19);
    glEnd();

    //external fire, orange
    glColor3ub(255, 72, 0);
    glBegin(GL_POLYGON);
    glVertex2f(xR + -80, yR + -30);
    glVertex2f(xR + -120, yR + -20);
    glVertex2f(xR + -140, yR + 0);
    glVertex2f(xR + -120, yR + 20);
    glVertex2f(xR + -80, yR + 30);
    glEnd();
    //internal fire, yellow
    glColor3f(1, 1, 0);
    glBegin(GL_POLYGON);
    glVertex2f(xR + -80, yR + -25);
    glVertex2f(xR + -100, yR + -20);
    glVertex2f(xR + -120, yR + 0);
    glVertex2f(xR + -100, yR + 20);
    glVertex2f(xR + -80, yR + 25);
    glEnd();

    //back of rocket (line)
    glLineWidth(3);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(xR + -80, yR + 30);
    glVertex2f(xR + -80, yR + -30);
    glEnd();

    //front of rocket (line) 
    glLineWidth(2);
    glColor3f(1, 0, 1);
    glBegin(GL_LINES);
    glVertex2f(xR + 80, yR + 30);
    glVertex2f(xR + 80, yR + -30);
    glEnd();
    glLineWidth(1);

    //support equipments/strip of rocket(backside)
    glColor3ub(27, 1, 41);
    glBegin(GL_LINE_STRIP);
    glVertex2f(xR + -80, yR + 30);
    glVertex2f(xR + -70, yR + 20);
    glVertex2f(xR + -60, yR + 0);
    glVertex2f(xR + -70, yR + -20);
    glVertex2f(xR + -80, yR + -30);
    glEnd();

    glPointSize(4);
    glColor3ub(41, 39, 1);
    glBegin(GL_POINTS);
    glVertex2f(xR + -70, yR + 20);
    glVertex2f(xR + -60, yR + 0);
    glVertex2f(xR + -70, yR + -20);
    glEnd();
}
void display_background() {

    //name-surname, id
    glColor3f(0, 0, 0);
    vprint(-484, 284, GLUT_BITMAP_9_BY_15, "ALPER CELIK");
    vprint(-474, 266, GLUT_BITMAP_9_BY_15, "21903022");

    glColor3ub(255, 255, 170);
    vprint(-150, 0, GLUT_BITMAP_HELVETICA_18, "CLICK to create/relocate the Rocket.");


    //sun
    glColor3ub(254, 210, 0);
    circle(xS + 0, 222, 33);
    //sun's mouth
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(xS + -18, 205);
    glVertex2f(xS + 18, 205);
    glVertex2f(xS + 10, 198);
    glVertex2f(xS + -10, 198);
    glEnd();
    //sun's red lips
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xS + -18, 205);
    glVertex2f(xS + 18, 205);
    glVertex2f(xS + 10, 198);
    glVertex2f(xS + -10, 198);
    glEnd();
    //sun's eye sockets
    glColor3f(1, 1, 1);
    circle(xS + -12, 228, 6);
    circle(xS + 13, 228, 6);
    //sun's blue pupils
    glColor3ub(17, 51, 242);
    circle(xS + -12, 227, 2);
    circle(xS + 13, 227, 2);

    //clouds (left to right)
    glColor3ub(220, 232, 229);
    circle(xC + -324, 191, 25);
    circle(xC + -381, 191, 25);
    circle(xC + -352, 197, 28);
    circle(xC + -353, 186, 28);

    circle(xC + -123, 151, 26);
    circle(xC + -177, 151, 26);
    circle(xC + -146, 156, 29);
    circle(xC + -145, 146, 29);

    circle(xC + 115, 146, 26);
    circle(xC + 60, 147, 26);
    circle(xC + 91, 151, 29);
    circle(xC + 88, 141, 29);

    circle(xC + 325, 202, 25);
    circle(xC + 271, 203, 25);
    circle(xC + 298, 207, 28);
    circle(xC + 301, 198, 28);
}
void display_background2()
{

    //name-surname, id
    glColor3f(0, 0, 0);
    vprint(-484, 284, GLUT_BITMAP_9_BY_15, "ALPER CELIK");
    vprint(-474, 266, GLUT_BITMAP_9_BY_15, "21903022");

    // command and background of it
    glColor3f(1, 1, 1);
    glRectf(-340, -299, 340, -260);
    glColor3ub(52, 195, 235);
    vprint(-310, -285, GLUT_BITMAP_HELVETICA_18, "Please press <F1> to switch the mode and Arrow Keys to control the Rocket.");

    glColor3ub(40, 41, 1);
    vprint(-480, -240, GLUT_BITMAP_9_BY_15, "Color codes of the wings: Red= %d Green= %d Blue= %d", red, green, blue);

    glColor3ub(66, 117, 19);
    vprint(92, -210, GLUT_BITMAP_9_BY_15, "'c' or 'C': change wings' color");
    vprint(92, -225, GLUT_BITMAP_9_BY_15, "Left Click : change rocket's location");
    vprint(92, -240, GLUT_BITMAP_9_BY_15, "Space Bar : stop/start the animation (lock)");

    //sun
    glColor3ub(254, 210, 0);
    circle(xS + 0, 222, 33);
    //sun's mouth
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(xS + -18, 205);
    glVertex2f(xS + 18, 205);
    glVertex2f(xS + 10, 198);
    glVertex2f(xS + -10, 198);
    glEnd();
    //sun's red lips
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xS + -18, 205);
    glVertex2f(xS + 18, 205);
    glVertex2f(xS + 10, 198);
    glVertex2f(xS + -10, 198);
    glEnd();
    //sun's eye sockets
    glColor3f(1, 1, 1);
    circle(xS + -12, 228, 6);
    circle(xS + 13, 228, 6);
    //sun's blue pupils
    glColor3ub(17, 51, 242);
    circle(xS + -12, 227, 2);
    circle(xS + 13, 227, 2);

    //clouds (left to right)
    glColor3ub(220, 232, 229);
    circle(xC + -324, 191, 25);
    circle(xC + -381, 191, 25);
    circle(xC + -352, 197, 28);
    circle(xC + -353, 186, 28);

    circle(xC + -123, 151, 26);
    circle(xC + -177, 151, 26);
    circle(xC + -146, 156, 29);
    circle(xC + -145, 146, 29);

    circle(xC + 115, 146, 26);
    circle(xC + 60, 147, 26);
    circle(xC + 91, 151, 29);
    circle(xC + 88, 141, 29);

    circle(xC + 325, 202, 25);
    circle(xC + 271, 203, 25);
    circle(xC + 298, 207, 28);
    circle(xC + 301, 198, 28);
}
//
// To display onto window using OpenGL commands
//
void display() {

    glClearColor(52 / 255., 195 / 255., 235 / 255., 0); //background color
    glClear(GL_COLOR_BUFFER_BIT);

    //mode transitions
    switch (mode)
    {
    case START:
        display_background();
        message();
        break;
    case MANUAL:
        display_background2();
        rocket();
        break;
    case AUTONOMOUS:
        display_background2();
        rocket();
        break;
    }

    //Mode and Direction
    glColor3ub(245, 66, 66);
    vprint(290, 284, GLUT_BITMAP_9_BY_15, "Mode: %s", modes[mode]);
    vprint(290, 266, GLUT_BITMAP_9_BY_15, "Direction: %s", directions[direction]);

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


    //to change color
    if (key == 'c' || key == 'C')
    {
        red = rand() % 256;
        green = rand() % 256;
        blue = rand() % 256;
    }

    //activate or deactivate the timerActivator.
    if (key == ' ')
    {
        timerActivator = !timerActivator;
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
    //mode changing
    if (key == GLUT_KEY_F1)
    {
        if (mode == MANUAL)
            mode = AUTONOMOUS;

        else if (mode == AUTONOMOUS)
            mode = MANUAL;
    }

    //MANUAL move
    if (key == GLUT_KEY_UP)
    {
        if (timerActivator)
        {
            direction = UP;
            if (mode == MANUAL)
            {
                if (yR + radiusR + 35 < winHeight / 2)
                    yR = yR + 3;
            }

        }
    }
    if (key == GLUT_KEY_DOWN)
    {
        if (timerActivator)
        {
            direction = DOWN;
            if (mode == MANUAL)
            {
                if (-yR + 118 / 2 < winHeight / 2)
                    yR = yR - 3;
            }
        }
    }
    if (key == GLUT_KEY_RIGHT)
    {
        if (timerActivator)
        {
            direction = RIGHT;
            if (mode == MANUAL)
            {
                if (xR + radiusR + 143 < winWidth / 2)
                    xR = xR + 3;
            }
        }
    }
    if (key == GLUT_KEY_LEFT)
    {
        if (timerActivator)
        {
            direction = LEFT;
            if (mode == MANUAL)
            {
                if (-xR + radiusR + 116 < winWidth / 2)
                    xR = xR - 3;
            }
        }
    }

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
    //each new click relocates the object on the screen.
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
    {
        while (mode == START)
            mode = MANUAL;

        if (mode == MANUAL || mode == AUTONOMOUS)
        {
            xR = x - winWidth / 2;
            yR = winHeight / 2 - y;
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

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);

    //clouds' speed
    xC = xC + 0.03;
    //sun's speed
    xS = xS + 0.007;

    //AUTONOMOUS location changes and returning stage
    if (timerActivator == true)
    {
        if (mode == AUTONOMOUS)
        {
            if (yR + radiusR + 34 < winHeight / 2 && direction == UP)
                yR++;
            else if (-yR + 118 / 2 < winHeight / 2 && direction == DOWN)
                yR--;
            else if (xR + radiusR + 143 < winWidth / 2 && direction == RIGHT)
                xR++;
            else if (-xR + radiusR + 116 < winWidth / 2 && direction == LEFT)
                xR--;
            else
                if (direction == UP)
                    direction = DOWN;
                else if (direction == RIGHT)
                    direction = LEFT;
                else if (direction == LEFT)
                    direction = RIGHT;
                else if (direction == DOWN)
                    direction = UP;
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
    glutCreateWindow("Homework #1 by Alper Celik");

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
    //for color changing.
    srand(time(0));

    glutMainLoop();
}
