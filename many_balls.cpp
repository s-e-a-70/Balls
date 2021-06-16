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

} ;

void Balls_Move ();
void Draw_Ball        (balls  ball);
void Draw_Vector      (balls  ball);
void Average_Velosity (balls *ball, int vectorVx[5], int vectorVy[5], int *average_count, int *vx_sr, int *vy_sr);
void Physics_Ball     (balls *ball, int ax, int ay, int dt);
void Control_Ball     (balls *ball);
bool Is_Crash         (balls  ball1, balls ball2, int eps);

//-----------------------------------------------------
int main()
{

    txCreateWindow (800, 600);

    Balls_Move();

    return(0);

}

//-----------------------------------------------------
void Balls_Move()
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

    int ax1 = 1;
    int ay1 = 0;

    ball2.x  = 0;
    ball2.y  = 100;
    ball2.vx = 2;
    ball2.vy = 1;
    ball2.r  = 20;

    ball2.border_color = TX_BLUE;
    ball2.fill_color   = TX_LIGHTBLUE;

    int ax2 = 0;
    int ay2 = 1;

    int dt = 1;
    int count = 0;



    int vectorVx[5]    = {0, 0, 0, 0, 0};
    int vectorVy[5]    = {0, 0, 0, 0, 0};
    int average_count  = 0;
    int vx_sr          = 0;
    int vy_sr          = 0;

    vx_sr = ball1.vx;
    vy_sr = ball1.vy;



    while (!txGetAsyncKeyState (VK_ESCAPE))
    {

        Draw_Ball        (ball1);
        Draw_Vector      (ball1);
        Physics_Ball     (&ball1, ax1, ay1, dt);
        Control_Ball     (&ball1);
        Average_Velosity (&ball1, vectorVx, vectorVy, &average_count, &vx_sr, &vy_sr);

        Draw_Ball    (ball2);
        Draw_Vector  (ball2);
        Physics_Ball (&ball2, ax2, ay2, dt);
        Control_Ball (&ball2);

        if (Is_Crash(ball1, ball2, 40))
            count ++;

        printf("CRASH = %d\r", count);



        txSleep (100);


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

    (*ball).vx += ax*dt;
    (*ball).vy += ay*dt;

    (*ball).x += (*ball).vx*dt;
    (*ball).y += (*ball).vy*dt;

     if ((*ball).x > 800)
    {
       (*ball).vx = -((*ball).vx);
      (*ball).x  = 800;
    }

    if ((*ball).y > 600)
    {
       (*ball).vy = -((*ball).vy);
       (*ball).y  = 600;
    }

    if ((*ball).x < 0)
    {
       (*ball).vx = -((*ball).vx);
       (*ball).x  = 0;
    }

    if ((*ball).y < 0)
    {
       (*ball).vy = -((*ball).vy);
       (*ball).y  = 0;
    }

}
//-----------------------------------------------------
void Control_Ball(balls *ball)
{

    if(txGetAsyncKeyState (VK_RIGHT)) ((*ball).vx)++;
    if(txGetAsyncKeyState (VK_LEFT))  ((*ball).vx)--;
    if(txGetAsyncKeyState (VK_UP))    ((*ball).vy)++;
    if(txGetAsyncKeyState (VK_DOWN))  ((*ball).vy)--;
    if(txGetAsyncKeyState (VK_SPACE)) {(*ball).vx = 0; (*ball).vy = 0;}

}
//-----------------------------------------------------
void Draw_Vector(balls ball)
{

    txSetColor     (ball.border_color);

    txLine   (ball.x, ball.y, ball.x + 3 * ball.vx, ball.y + 3 * ball.vy);
    txCircle (ball.x + 3 * ball.vx, ball.y + 3 * ball.vy, 3);

}
//-----------------------------------------------------
void Average_Velosity(balls *ball, int vectorVx[5], int vectorVy[5], int *average_count, int *vx_sr, int *vy_sr)
{

    int svx = 0;
    int svy = 0;
    int i   = 0;


    vectorVx[*average_count] = (*ball).vx;
    vectorVy[*average_count] = (*ball).vy;

    if (*average_count == 4)
    {
        svx = 0;
        svy = 0;

        for (i = 0; i < 5; i++)
        {

            svx += vectorVx[i];
            svy += vectorVy[i];

        }

        *vx_sr = (int)(svx/5);
        *vy_sr = (int)(svy/5);

        *average_count = 0;

    }

    (*average_count)++;

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
