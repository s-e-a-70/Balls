#define pi 3.14
#include "TxLib.h"
#include <math.h>

struct balls
{
    int x;
    int y;
    float vx;
    float vy;
    int r;
    COLORREF border_color;
    COLORREF fill_color;
    bool active;

};

struct controls
{
    int key_right;
    int key_left;
    int key_up;
    int key_down;
    int key_turn_left;
    int key_turn_right;
};

struct cues
{
    int number;
    int x1;
    int y1;
    int x2;
    int y2;
    float length;
    int angle;
    COLORREF border_color;
    COLORREF fill_color;
    controls control;
    bool active;

};

struct scores
{
    int player1;
    int player2;
};





void Came_Billard();
void Draw_Landscape(void);
void Draw_Cue(cues cue, int t);
void Move_Cue(cues *cue);
void Control_Cue(cues *cue, balls *ball);
void Draw_Ball(balls ball);
void Control_Ball(balls *ball, bool *cue_change);
void Physics_Ball(balls *ball, int dt);
bool IsLine(int x1, int y1, int x2, int y2, int x3, int y3);
void Cue_Hit(balls *ball, cues *cue);bool Is_Cue_Hit (cues *cue, balls *ball, int eps);

void Cue_Init(cues *cue, int number, bool active);
void Ball_Init(balls *ball, double vx, double vy, int r, COLORREF border_color, COLORREF fill_color, bool active);
void Ball_Delete (balls *ball);
void Draw_Score(scores score);
void Control_Game(balls *ball, cues *cue1, cues *cue2, scores *score, bool *cue_change, int *last_active_cue);

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
    cues  cue2;
    scores score;

    Cue_Init (&cue1, 1, true);
    Cue_Init (&cue2, 2, false);

    Ball_Init(&ball1, 0, 0, 20,  TX_WHITE, TX_WHITE, true);

    int dt = 1;
    int t  = 0;
    int last_active_cue = 0;
    bool cue_change = false;

    score.player1 = 0;
    score.player2 = 0;

    Draw_Landscape();

    while (!txGetAsyncKeyState (VK_ESCAPE))
    {

        txClear();

        Draw_Score(score);

        Draw_Landscape();

        Draw_Ball   (ball1);
        Control_Ball(&ball1, &cue_change);

        Draw_Cue    (cue1, t);
        Control_Cue (&cue1, &ball1);
        Move_Cue    (&cue1);


        Draw_Cue    (cue2, t);
        Control_Cue (&cue2, &ball1);
        Move_Cue    (&cue2);


        Physics_Ball(&ball1, dt);

        Control_Game(&ball1, &cue1, &cue2, &score, &cue_change, &last_active_cue);

        t += dt;

        txSleep (100);

    }

}

//-----------------------------------------------------
void Draw_Cue(cues cue, int t)
{

    if (cue.active && (t % 10 == 0))
    {
        txSetColor (TX_ORANGE, 20);
        txLine     (cue.x1, cue.y1, cue.x2, cue.y2);
    }

    txSetColor (cue.border_color, 10);

    txLine     (cue.x1, cue.y1, cue.x2, cue.y2);

    txSetColor (TX_LIGHTRED, 10);

    txCircle   (cue.x2, cue.y2, 3);

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
        txCircle (150, 150, 30);
        txCircle (500, 150, 30);
        txCircle (850, 150, 30);
        txCircle (150, 650, 30);
        txCircle (500, 650, 30);
        txCircle (850, 650, 30);

}
//-----------------------------------------------------
void Control_Ball(balls *ball, bool *cue_change)
{
    if (txGetAsyncKeyState (VK_F1))
        Ball_Init(ball, 0,   0,   20,  TX_WHITE, TX_WHITE, true);

  //  if  (((ball->x >= 150 && ball->x <= 180)||(ball->x >= 500 && ball->x <= 530)||(ball->x >= 850 && ball->x <= 880))&&((ball->y >= 150 && ball->y <= 180)||(ball->y >= 650 && ball->y <= 680)))

    if (txGetPixel(ball->x, ball->y) == TX_BLACK)
    {
        ball->active = false;
        *cue_change = true;
    }

    if  ((ball->vx == 0) && (ball->vy == 0))
        *cue_change = true;



}
        //-----------------------------------------------------
void Move_Cue(cues *cue)
{
    if (cue->active)
    {

        if (txGetAsyncKeyState (cue->control.key_right))      (cue->x1)     += 10;
        if (txGetAsyncKeyState (cue->control.key_left))       (cue->x1)     -= 10;
        if (txGetAsyncKeyState (cue->control.key_up))         (cue->y1)     -= 10;
        if (txGetAsyncKeyState (cue->control.key_down))       (cue->y1)     += 10;
        if (txGetAsyncKeyState (cue->control.key_turn_right)) (cue->angle)  += 10;
        if (txGetAsyncKeyState (cue->control.key_turn_left))  (cue->angle)  -= 10;

        cue->x2 = cue->x1 + (int)(cue->length * cos(cue->angle * pi / 180));
        cue->y2 = cue->y1 + (int)(cue->length * sin(cue->angle * pi / 180));


    }

}
//-----------------------------------------------------
void Control_Cue(cues *cue, balls *ball)
{
    int num = cue->number;
    bool active = cue->active;

   if (cue->active)
   {
        if (Is_Cue_Hit(cue, ball, 10))
            {
                Cue_Hit  (ball, cue);
                Cue_Init (cue, num, !active);
            }

    }
    else
    {
        Cue_Init (cue, num, active);
    }

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

    ball->x += ROUND(ball->vx*dt);
    ball->y += ROUND(ball->vy*dt);

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
bool Is_Cue_Hit (cues *cue, balls *ball, int eps)
{

    bool hit = false;

    int distance = (int)sqrt((ball->x - cue->x2) * (ball->x - cue->x2) + (ball->y - cue->y2) * (ball->y - cue->y2)) - ball->r;

    if ((distance <= eps) && IsLine(cue->x1, cue->y1, cue->x2, cue->y2, ball->x, ball->y))
        hit = true;

    return hit;

}
//-----------------------------------------------------
bool IsLine(int x1, int y1, int x2, int y2, int x3, int y3)
{

   if ((x3 - x1) / (x2 - x1) == (y3 - y1) / (y2 - y1))
    //if ((x3 - x1) * (y2 - y1) == (x2 - x1) * (y3 - y1))
        return true;
   else return false;

}
//-----------------------------------------------------
void Cue_Hit(balls *ball, cues *cue)
{
  ball->vx = (float)(cue->x2 - cue->x1);//sqrt((cue.x1 - cue.x2) * (cue.x1 - cue.x2) + (cue.y1 - cue.y2) * (cue.y1 - cue.y2));
  ball->vy = (float)(cue->y2 - cue->y1);//sqrt((cue.x1 - cue.x2) * (cue.x1 - cue.x2) + (cue.y1 - cue.y2) * (cue.y1 - cue.y2));

}
//-----------------------------------------------------
void Cue_Init(cues *cue, int number, bool active)
{

    cue->number = number;

    switch (cue->number)
    {
        case 1:
        {
          cue->x1     = 0;
          cue->y1     = 0;
          cue->length = 200;
          cue->angle  = 30;
          cue->border_color = TX_YELLOW;
          cue->control.key_right      = VK_RIGHT;
          cue->control.key_left       = VK_LEFT;
          cue->control.key_up         = VK_UP;
          cue->control.key_down       = VK_DOWN;
          cue->control.key_turn_right = VK_NUMPAD6;
          cue->control.key_turn_left  = VK_NUMPAD4;
          cue->active = active;
          break;
        }

        case 2:
        {
          cue->x1     = 1000;
          cue->y1     = 0;
          cue->length = 200;
          cue->angle  = 150;
          cue->border_color = TX_WHITE;
          cue->control.key_right      = VK_RIGHT;
          cue->control.key_left       = VK_LEFT;
          cue->control.key_up         = VK_UP;
          cue->control.key_down       = VK_DOWN;
          cue->control.key_turn_right = VK_NUMPAD6;
          cue->control.key_turn_left  = VK_NUMPAD4;
          cue->active = active;
          break;
        }
    }

    cue->x2 = cue->x1 + (int)(cue->length * cos(cue->angle * pi / 180));
    cue->y2 = cue->y1 + (int)(cue->length * sin(cue->angle * pi / 180));

}
//-----------------------------------------------------
void Ball_Init (balls *ball, double vx, double vy, int r, COLORREF border_color, COLORREF fill_color, bool active)
{

    ball->x  = random (150, 850);
    ball->y  = random (150, 650);
    ball->vx = vx;
    ball->vy = vy;
    ball->r  = r;
    ball->border_color = border_color;
    ball->fill_color   = fill_color;
    ball->active       = active;

}
//-----------------------------------------------------
void Ball_Delete (balls *ball)
{
    //ball->x  = 0;
   // ball->y  = 0;
   ball->vx = 0;
   ball->vy = 0;
    //ball->r  = 0;

   while (ball->r >= 0)
   {
        ball->r -= 1;
        Draw_Ball(*ball);
        txSleep (100);
   }

    ball->border_color = TX_BLACK;
    ball->fill_color   = TX_BLACK;

}
//-----------------------------------------------------
void Draw_Score(scores score)
{
    char s[50];
    sprintf(s, "%d : %d\n", score.player1, score.player2);

    txSetColor      (TX_WHITE);

    txSelectFont    ("Times New Roman", 50, 0, FW_BOLD);

    txTextOut       (450,  30, s);

}
//-----------------------------------------------------
void Control_Game(balls *ball, cues *cue1, cues *cue2, scores *score, bool *cue_change, int *last_active_cue)
{
  if (ball->active == 0)
  {
     Ball_Delete (ball);
    *last_active_cue == 1? score->player1 ++ :score->player2 ++;
    *last_active_cue = 0;

  }

  if (*cue_change == true)
  {

   *last_active_cue == 1? cue1->active = true : cue2->active = true;
   *cue_change = false;
   *last_active_cue = 0;

  }



}
