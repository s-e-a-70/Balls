#include "TxLib.h"
#include <math.h>

struct balls
{
    int x;
    int y;
    int vx;
    int vy;
    int r;
    COLORREF border_color;
    COLORREF fill_color;
    int key_right;
    int key_left;
    int key_up;
    int key_down;
    int key_stop;
    int vectorVx[5];
    int vectorVy[5];
    int average_count;
    int vx_sr;
    int vy_sr;


} ;

void Balls_Game ();
void Move_Ball        (balls *ball, int ax, int ay, int t, int dt);
void Draw_Ball        (balls  ball);
void Draw_Vector      (balls  ball);
void Average_Velosity (balls *ball, int t);
void Physics_Ball     (balls *ball, int ax, int ay, int dt);
void Control_Ball     (balls *ball);
bool Is_Crash         (balls  ball1, balls ball2, int eps);

//-----------------------------------------------------
int main()
{

    txCreateWindow (800, 600);

    Balls_Game();

    return(0);

}

//-----------------------------------------------------
void Balls_Game()
{

    struct balls ball1;
    struct balls ball2;

    ball1.x  = 100;
    ball1.y  = 0;
    ball1.vx = 5;
    ball1.vy = 3;
    ball1.r  = 20;

    ball1.border_color = TX_MAGENTA;
    ball1.fill_color   = TX_LIGHTMAGENTA;

    ball1.key_right = VK_RIGHT;
    ball1.key_left  = VK_LEFT;
    ball1.key_up    = VK_UP;
    ball1.key_down  = VK_DOWN;
    ball1.key_down  = VK_SPACE;

//    ball1.vectorVx       = {0, 0, 0, 0, 0};
//    ball1.vectorVy       = {0, 0, 0, 0, 0};
    ball1.average_count  = 0;
    ball1.vx_sr          = 0;
    ball1.vy_sr          = 0;

    int ax1 = 1;
    int ay1 = 0;


    ball2.x  = 0;
    ball2.y  = 100;
    ball2.vx = 2;
    ball2.vy = 1;
    ball2.r  = 20;

    ball2.border_color = TX_BLUE;
    ball2.fill_color   = TX_LIGHTBLUE;

    ball2.key_right = VK_NUMPAD6;
    ball2.key_left  = VK_NUMPAD4;
    ball2.key_up    = VK_NUMPAD8;
    ball2.key_down  = VK_NUMPAD2;
    ball2.key_stop  = VK_NUMPAD5;

 //   ball1.vectorVx       = {0, 0, 0, 0, 0};
 //   ball1.vectorVy       = {0, 0, 0, 0, 0};
    ball1.average_count  = 0;
    ball1.vx_sr          = 0;
    ball1.vy_sr          = 0;

    int ax2 = 0;
    int ay2 = 1;

    int dt       = 1;
    int t        = 0;
    int count    = 0;


    while (!txGetAsyncKeyState (VK_ESCAPE))
    {

        Draw_Ball        (ball1);
        Draw_Vector      (ball1);
        Physics_Ball     (&ball1, ax1, ay1, dt);
        Control_Ball     (&ball1);
        Average_Velosity (&ball1, t);

        Draw_Ball        (ball2);
        Draw_Vector      (ball2);
        Physics_Ball     (&ball2, ax2, ay2, dt);
        Control_Ball     (&ball2);
        Average_Velosity (&ball2, t);

        if (Is_Crash(ball1, ball2, 40))
            count ++;

        printf("CRASH = %d\r", count);


        txSleep (100);

        t += dt;

    }

}
//-----------------------------------------------------
void Draw_Ball(balls ball)
{

    txSetColor     (ball.border_color);
    txSetFillColor (ball.fill_color);

    txCircle    (ball.x, ball.y, ball.r);

}
//-----------------------------------------------------
void Physics_Ball(balls *ball, int ax, int ay, int dt)
{

    ball->vx += ax*dt;
    ball->vy += ay*dt;

    ball->x += ball->vx*dt;
    ball->y += ball->vy*dt;

    if (ball->x > 800)
    {
        ball->vx = -ball->vx;
        ball->x  = 800;
    }

    if (ball->y > 600)
    {
       ball->vy = -ball->vy;
       ball->y  = 600;
    }

    if (ball->x < 0)
    {
       ball->vx = -ball->vx;
       ball->x  = 0;
    }

    if (ball->y < 0)
    {
       ball->vy = -ball->vy;
       ball->y  = 0;
    }

}
//-----------------------------------------------------
void Control_Ball(balls *ball)
{

    if(txGetAsyncKeyState (ball->key_right)) (ball->vx)++;
    if(txGetAsyncKeyState (ball->key_left))  (ball->vx)--;
    if(txGetAsyncKeyState (ball->key_up))    (ball->vy)++;
    if(txGetAsyncKeyState (ball->key_down))  (ball->vy)--;
    if(txGetAsyncKeyState (ball->key_stop))  {ball->vx = 0; ball->vy = 0;}

}
//-----------------------------------------------------
void Draw_Vector(balls ball)
{

    txSetColor (ball.border_color);

    txLine     (ball.x, ball.y, ball.x + 3 * ball.vx_sr, ball.y + 3 * ball.vy_sr);
    txCircle   (ball.x + 3 * ball.vx_sr, ball.y + 3 * ball.vy_sr, 3);

}
//-----------------------------------------------------
void Average_Velosity(balls *ball, int t)
{

    int svx = 0;
    int svy = 0;
    int i   = 0;

    if (t < 5)
        {
             ball->vx_sr = ball->vx;
             ball->vy_sr = ball->vy;
        }

    ball->vectorVx[ball->average_count] = ball->vx;
    ball->vectorVy[ball->average_count] = ball->vy;

    if (ball->average_count == 4)
    {
        svx = 0;
        svy = 0;

        for (i = 0; i < 5; i++)
        {

            svx += ball->vectorVx[i];
            svy += ball->vectorVy[i];

        }

        ball->vx_sr = (int)(svx/5);
        ball->vy_sr = (int)(svy/5);

        ball->average_count = 0;

    }

    (ball->average_count)++;

}
//-----------------------------------------------------
bool Is_Crash (balls ball1, balls ball2, int eps)
{

    bool crash = false;
    int  l     = 0;

    l = (int)sqrt((ball1.x- ball2.x) * (ball1.x - ball2.x) + (ball1.y - ball2.y) * (ball1.y - ball2.y));

    if (l <= eps)
        crash = true;

    return crash;

}
//-----------------------------------------------------
