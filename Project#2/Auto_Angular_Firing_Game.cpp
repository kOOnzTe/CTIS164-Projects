/*********
STUDENT NAME : Alper Celik
HOMEWORK: Homework #2
----------
PROBLEMS: none.
----------
ADDITIONAL FEATURES:
* Menu Screen has added.
* from Beggining to Ending connection has added.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  700
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

//modes
#define MENU 0
#define START 1
#define EVENT 2
#define GAMEOVER 3
#define DURATION 60

void TimerOn(int v); //my timer function.
int mode = MENU; // initial mode
float timeCounter = DURATION;
int red = 23, green = 31, blue = 34; //initial colour codes, will be changed.

typedef struct
{
    double x, y;
} point_t;

typedef struct
{
    point_t pos;   // car's position
    double   angle; // view angle 
    double   r;
} artillery_t;

typedef struct
{
    point_t pos;
    double angle;
    bool active; //activation
} fire_t;

artillery_t artillery = { {0, 0}, 0, 20 };

fire_t fire = { { 0, 0 }, 0, false }; //starts with "false".

//position of the car
int carposx = 100, carposy = 50;

//flag value of hitting
int flag = 1;

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

void panzer()
{
    //skeleton
    glColor3f(0, 0, 0);
    circle(0, 0, 40);

    //armor
    glColor3f(1, 0, 1);
    circle_wire(0, 0, 40);

    glColor3f(1, 1, 0);
    circle_wire(0, 0, 41);

    //upper-shields
    glColor3ub(158, 158, 158);
    circle(10, 10, 20);
    circle(-10, -10, 20);
    circle(-10, 10, 20);
    circle(10, -10, 20);
}

void panzer_advance(artillery_t ta)
{
    
    //tank's barrel
    glColor3ub(131, 30, 22);
    glLineWidth(4);
    glBegin(GL_LINES);
    glVertex2f(ta.pos.x, ta.pos.y); //starting point (0,0)
    glVertex2f(ta.pos.x + 63 * cos(ta.angle * D2R), ta.pos.y + 63 * sin(ta.angle * D2R)); // (x,y) ---> (x1,y1)  ending point of the line    x1: x + r*cosangle    y1: y + r*sinangle
    glEnd();

    glLineWidth(1); //stabilizing line width to 1

    //tank turret
    glColor3ub(204, 119, 118);
    circle(ta.pos.x, ta.pos.y, ta.r);
    glColor3ub(64, 4, 4);
    circle_wire(ta.pos.x, ta.pos.y, ta.r);

    //angle counter
    glColor3ub(222, 186, 186);
    vprint(ta.pos.x - 13, ta.pos.y - 6, GLUT_BITMAP_9_BY_15, "%.0f", ta.angle);
}

void car()
{
    int xC = carposx, yC = carposy; //these variables are assigned to the structure.

    //skeleton
    glColor3ub(red, green, blue);
    glBegin(GL_POLYGON);
    glVertex2f(xC + 50, yC + 0);
    glVertex2f(xC + -50, yC + 0);
    glVertex2f(xC + -50, yC + 40);
    glVertex2f(xC + -42, yC + 51);
    glVertex2f(xC + 42, yC + 51);
    glVertex2f(xC + 50, yC + 40);
    glEnd();

    //wheels
    glColor3f(0, 0, 1);
    circle(xC + -25, yC + 0, 15);
    glColor3f(1, 1, 1);
    circle(xC + -25, yC + 0, 11);

    glColor3f(0, 0, 1);
    circle(xC + 25, yC + 0, 15);
    glColor3f(1, 1, 1);
    circle(xC + 25, yC + 0, 11);

    //rims of car
    glPointSize(5);
    glColor3f(0, 0, 0);
    glBegin(GL_POINTS);
    glVertex2f(xC + -25, yC + 0);
    glVertex2f(xC + 25, yC + 0);
    glEnd();

    //car doors
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
    glVertex2f(xC + -25, yC + 50);
    glVertex2f(xC + -25, yC + 15);

    glVertex2f(xC + 0, yC + 50);
    glVertex2f(xC + 0, yC + 0);

    glVertex2f(xC + 25, yC + 50);
    glVertex2f(xC + 25, yC + 15);
    glEnd();

    //door handles
    glColor3f(1, 1, 1);
    glRectf(xC + -22, yC + 31, xC + -18, yC + 36);
    glColor3ub(52, 195, 235);

    glColor3f(1, 1, 1);
    glRectf(xC + 22, yC + 31, xC + 18, yC + 36);

    //exhaust pipe
    glColor3f(1, 1, 1);
    glRectf(xC + 50, yC + 10, xC + 57, yC + 14);
    glColor3ub(52, 195, 235);

    //car glass
    glColor3ub(161, 22, 22);
    glBegin(GL_TRIANGLES);
    glVertex2f(xC + -25, yC + 50);
    glVertex2f(xC + 0, yC + 80);
    glVertex2f(xC + 25, yC + 50);
    glEnd();


    glColor3ub(red, green, blue);
    glBegin(GL_TRIANGLES);
    glVertex2f(xC + -16, yC + 54);
    glVertex2f(xC + 0, yC + 74);
    glVertex2f(xC + 16, yC + 54);
    glEnd();

    //car's spoiler
    glColor3ub(red, green, blue);
    glBegin(GL_QUADS);
    glVertex2f(xC + 43, yC + 49);
    glVertex2f(xC + 43, yC + 40);
    glVertex2f(xC + 60, yC + 50);
    glVertex2f(xC + 60, yC + 59);
    glEnd();
}

void Fire(fire_t f)
{

    if (f.active)
    { //if I activate the fire
        glColor3f(1., 0., 0.);
        circle(f.pos.x, f.pos.y, 5); // r=5 red bullet 
    }

}

void randCar()
{
    carposx = rand() % 600 - 310; // max location - half (depends on the lines length)
    carposy = rand() % 600 - 340;

    //with random colours
    red = rand() % 256;
    green = rand() % 256;
    blue = rand() % 256;
}

void displayMenu()
{

    glClearColor(199 / 255., 209 / 255., 209 / 255., 0); // background
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3ub(122, 191, 190);
    glBegin(GL_QUADS);
    glVertex2f(-90, 90);
    glVertex2f(-90, -170);
    glVertex2f(92, 90);
    glVertex2f(92, -170);
    glEnd();

    // abbreviation of my name as a symbol.
    glColor3f(1, 1, 1);
    glLineWidth(3);
    vprint2(-80, -25, 1, "AC");

    glColor3ub(122, 191, 190);
    vprint2(-220, 150, 0.3, "Welcome to the game!");
    //commands for user
    vprint2(-120, -250, 0.2, "press <p> to play!");
    vprint2(-150, -300, 0.2, "press <esc> to escape :/");
}

void displayStart()
{
    // lines
    glLineWidth(15);
    glColor3ub(122, 37, 37);
    glBegin(GL_LINES);
    glVertex2f(350, 290);
    glVertex2f(350, -275);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-350, 290);
    glVertex2f(-350, -275);
    glEnd();

    //background of words #1
    glColor3ub(122, 37, 37);
    glRectf(-350, 290, 350, 350);

    //name-surname and ID
    glColor3f(1, 1, 1);
    vprint(220, 330, GLUT_BITMAP_9_BY_15, "ALPER CELIK");
    vprint(230, 310, GLUT_BITMAP_9_BY_15, "21903022");
    vprint(-330, 310, GLUT_BITMAP_TIMES_ROMAN_24, "CTIS 164 - 02");

    //background of words #2
    glColor3ub(122, 37, 37);
    glRectf(-350, -400, 350, -275);

    glColor3ub(235, 235, 180);
    vprint(-130, -320, GLUT_BITMAP_HELVETICA_18, " Press LEFT CLICK to START!!! ");

}

void displayEvent()
{
    // lines
    glLineWidth(15);
    glColor3ub(122, 37, 37);
    glBegin(GL_LINES);
    glVertex2f(350, 290);
    glVertex2f(350, -275);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-350, 290);
    glVertex2f(-350, -275);
    glEnd();

    glLineWidth(2);
    //background of words #1
    glColor3ub(122, 37, 37);
    glRectf(-350, 290, 350, 350);

    //name-surname and ID
    glColor3f(1, 1, 1);
    vprint(220, 330, GLUT_BITMAP_9_BY_15, "ALPER CELIK");
    vprint(230, 310, GLUT_BITMAP_9_BY_15, "21903022");
    vprint(-330, 310, GLUT_BITMAP_TIMES_ROMAN_24, "CTIS 164 - 02");

    //background of words #2
    glColor3ub(122, 37, 37);
    glRectf(-350, -400, 350, -275);

    if (mode != GAMEOVER)
    {
        // time counter
        glColor3ub(90, 100, 140);
        vprint(-340, -310, GLUT_BITMAP_HELVETICA_18, "time: ");
        vprint2(-295, -312, 0.18, "%0.2f", timeCounter);

    }
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //

    // background colour
    glClearColor(10. / 255, 194. / 255, 188. / 255, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    //mode transitions
    if (mode == MENU)
        displayMenu();
    else if (mode == START)
        displayStart();
    else if (mode == EVENT)
    {
        displayEvent();
        car();
        panzer();
        panzer_advance(artillery);
        Fire(fire);
    }
    else if (mode == GAMEOVER)
    {
        displayEvent();
        car();
        panzer();
        panzer_advance(artillery);
        Fire(fire);
        glColor3ub(46, 40, 40);
        vprint(-70, -70, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER");
        vprint(-160, -110, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS <T> TO TRY AGAIN!");
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

    // change MENU to START screen
    if (key == 'p' || key == 'P')
        if (mode == MENU)
            mode = START;

    // change GAMEOVER to MENU screen
    if (key == 'T' || key == 't')
        if (mode == GAMEOVER)
            mode = MENU;

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
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
    {
        if (mode == START) {
            mode = EVENT;
            timeCounter = DURATION; //time counter starts
            glutTimerFunc(1000, TimerOn, 0);
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
void TimerOn(int v)
{
    if (mode == EVENT)
    {
        timeCounter -= TIMER_PERIOD / 1000.; //to count the time that remains

        if (timeCounter > 0)
        {

            // calculation of the slope (I used arctangent to calculate)
            fire.angle = atan2(carposy - artillery.pos.y, carposx - artillery.pos.x) / D2R;

            //refleshing the angle counter (adding the period, 360 degrees)
            if (fire.angle < 0)
                fire.angle += 360;

            if (artillery.angle > 360) //for positive rotation
                artillery.angle = 0;
            if (artillery.angle < 0) //for negative rotation
                artillery.angle = 360;

            //my algorithm for choosing the closest distance
            if (flag == 1)
            {
               //absolute value of subtracting the angles is needed to find the closest distance. 
                if (fire.angle < artillery.angle - 0.01) // quadrants
                {
                    if(fabs(fire.angle - artillery.angle) < 180) 
                        artillery.angle--;
                    if (fabs(fire.angle - artillery.angle) > 180)
                        artillery.angle++;
                }
                
                if (fire.angle -0.01 > artillery.angle ) // another quadrants
                {
                    if (fabs(fire.angle - artillery.angle) < 180)
                        artillery.angle++;
                    if (fabs(fire.angle - artillery.angle) > 180)
                        artillery.angle--;
                }

            }

            //if the angle between target and main = angle of main
            if (flag == 1 && (fire.angle - 1.3 < artillery.angle && fire.angle + 1.3 > artillery.angle)) //1.3 --> so small values to check
            {
                fire.active = true; //active                
                //repositioning
                fire.pos.x = 0;
                fire.pos.y = 0;
                flag = 0;
            }

            //changes active fire/bullets position
            if (fire.active)
            {
                //changing fire position's calculation (way of fire/bullet)
                fire.pos.x += 10 * cos(fire.angle * D2R);
                fire.pos.y += 10 * sin(fire.angle * D2R);
            }

            //check if bullet hits the car and makes it reflesh (0,0)
            if (carposy + 30 > fire.pos.y && carposy - 30 < fire.pos.y) // +30 -30 increases the range
            {
                flag = 1;

                fire.active = false;
                fire.pos.x = 0;
                fire.pos.y = 0;

                randCar(); //calling random location and colours.
            }


            glutTimerFunc(TIMER_PERIOD, TimerOn, 0);
        }

        else
        {
            // time expires and new mode has come.
            mode = GAMEOVER;
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
    glutCreateWindow("Homework #2 Angular Firing Game by Alper Celik");

    srand(time(NULL));

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
    glutTimerFunc(TIMER_PERIOD, TimerOn, 0);
#endif

    Init();

    glutMainLoop();
}
