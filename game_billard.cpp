#define pi 3.14
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

};


struct cues
{
    int x1;
    int y1;
    int x2;
    int y2;
    double length;
    int angle;
    COLORREF border_color;
    COLORREF fill_color;
    int key_right;
    int key_left;
    int key_up;
    int key_down;
    int key_turn_left;
    int key_turn_right;
};

void Came_Billard();
void Draw_Landscape(void);
void Draw_Cue(cues cue);
void Control_Cue(cues *cue);
void Draw_Ball(balls ball);
void Physics_Ball(balls *ball, int dt);
bool Is_Cue_Hit (cues cue, balls ball, int eps);
bool IsLine(int x1, int y1, int x2, int y2, int x3, int y3);
void Cue_Hit(balls *ball, cues cue);
//-----------------------------------------------------
int main()
{

    txCreateWindow (1000, 800);

    Came_Billard();

    return(0);
}

void Came_Billard()
{

    balls ball1;
    cues  cue1;

    cue1.x1     = 100;
    cue1.y1     = 100;
    cue1.length = 200;
    cue1.angle  = 30;
    cue1.border_color = TX_YELLOW;
    cue1.key_right      = VK_RIGHT;
    cue1.key_left       = VK_LEFT;
    cue1.key_up         = VK_UP;
    cue1.key_down       = VK_DOWN;
    cue1.key_turn_right = VK_NUMPAD6;
    cue1.key_turn_left  = VK_NUMPAD4;

    ball1.x  = rand() % 601 + 200;
    ball1.y  = rand() % 601 + 200;
    ball1.vx = 0;
    ball1.vy = 0;
    ball1.r  = 20;
    ball1.border_color = TX_WHITE;
    ball1.fill_color   = TX_WHITE;

    int dt = 1;

    //ball1.vx = 20;
    //ball1.vy = 30;

    Draw_Landscape();

    while (!txGetAsyncKeyState (VK_ESCAPE))
    {

        txClear();

        Draw_Landscape();
        Draw_Ball   (ball1);
        Draw_Cue    (cue1);
        Control_Cue (&cue1);

        if (Is_Cue_Hit(cue1, ball1, 10))
         {
            //ball1.vx += 30;
            //ball1.vy += 30;
            Cue_Hit(&ball1, cue1);
         }

        Physics_Ball(&ball1, dt);
//        printf("%d  %d  %d  %d\n", ball1.x, ball1.y, ball1.vx, ball1.vy);

        txSleep (100);

    }

}

//-----------------------------------------------------
void Draw_Cue(cues cue)
{

    txSetColor (cue.border_color, 10);

    txLine     (cue.x1, cue.y1, cue.x2, cue.y2);

    txSetColor (TX_RED, 10);

    txCircle   (cue.x2, cue.y2, 5);

}
//-----------------------------------------------------
void Draw_Landscape(void)
{

        txSetFillColor (RGB (94, 47, 0));
        txSetColor     (RGB (94, 47, 0), 3);
        txRectangle    (100, 100, 900, 700);

        txSetFillColor (RGB (0, 100, 0));
        txSetColor     (RGB (0, 100, 0), 3);
        txRectangle    (150, 150, 850, 650);

        txSetFillColor (RGB (0, 0, 0));
        txSetColor     (RGB (0, 0, 0), 3);
        txCircle(150, 150, 30);
        txCircle(850, 150, 30);
        txCircle(150, 650, 30);
        txCircle(850, 650, 30);

}
//-----------------------------------------------------
void Control_Cue(cues *cue)
{

    if(txGetAsyncKeyState (cue->key_right))      (cue->x1)     += 10;
    if(txGetAsyncKeyState (cue->key_left))       (cue->x1)     -= 10;
    if(txGetAsyncKeyState (cue->key_up))         (cue->y1)     += 10;
    if(txGetAsyncKeyState (cue->key_down))       (cue->y1)     -= 10;
    if(txGetAsyncKeyState (cue->key_turn_right)) (cue->angle) += 10;
    if(txGetAsyncKeyState (cue->key_turn_left))  (cue->angle) -= 10;

    cue->x2 = cue->x1 + (int)(cue->length * cos(cue->angle * pi / 180));
    cue->y2 = cue->y1 + (int)(cue->length * sin(cue->angle * pi / 180));

}
//-----------------------------------------------------
void Draw_Ball(balls ball)
{

    txSetColor     (ball.border_color);
    txSetFillColor (ball.fill_color);
    txCircle       (ball.x, ball.y, ball.r);

    txSetColor     (TX_BLACK);
    txSetFillColor (TX_BLACK);

}
//-----------------------------------------------------
void Physics_Ball(balls *ball, int dt)
{
    int k = 1;

    if (ball->vx < 0)
        ball->vx += k * dt;
    if (ball->vx > 0)
        ball->vx -= k * dt;

    if (ball->vy < 0)
        ball->vy += k * dt;
    if (ball->vy > 0)
        ball->vy -= k * dt;

    ball->x += ball->vx*dt;
    ball->y += ball->vy*dt;

   // printf("%d  %d\n", ball->vx, ball->vy);


    if (ball->x > 850 - ball->r)
    {
        ball->vx = -ball->vx;
        ball->x  = 850 - ball->r;
    }

    if (ball->y > 650 - ball->r)
    {
       ball->vy = -ball->vy;
       ball->y  = 650 - ball->r;
    }

    if (ball->x < 150 + ball->r)
    {
       ball->vx = -ball->vx;
       ball->x  = 150 + ball->r;
    }

    if (ball->y < 150 + ball->r)
    {
       ball->vy = -ball->vy;
       ball->y  = 150 + ball->r;
    }

}
//-----------------------------------------------------
bool Is_Cue_Hit (cues cue, balls ball, int eps)
{

    bool hit = false;

    int distance = (int)sqrt((ball.x - cue.x2) * (ball.x - cue.x2) + (ball.y - cue.y2) * (ball.y - cue.y2));

    if ((distance <= eps) && IsLine(cue.x1, cue.y1, cue.x2, cue.y2, ball.x, ball.y))
        hit = true;

    return hit;

}
//-----------------------------------------------------
bool IsLine(int x1, int y1, int x2, int y2, int x3, int y3)
{

   if ((x3 - x1) / (x2 - x1) == (y3 - y1) / (y2 - y1))
        return true;
   else return false;

}
//-----------------------------------------------------
void Cue_Hit(balls *ball, cues cue)
{
  ball->vx = cue.x1 - cue.x2;
  ball->vy = cue.y1 - cue.y2;

}
