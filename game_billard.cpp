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

struct buttons
{
    int x;
    int y;
    int lx;
    int ly;
    COLORREF border_color;
    COLORREF fill_color;
    char *s;
};


void Came_Billard();
void Draw_Landscape(void);
void Draw_Cue(cues cue, int t);
void Move_Cue(cues *cue);
void Control_Cue(cues *cue, balls *ball);
void Draw_Ball(balls ball);
void Control_Balls (balls *ball1, balls *ball2);
void Physics_Ball(balls *ball, int dt);
bool Is_Line(int x1, int y1, int x2, int y2, int x3, int y3);
void Cue_Hit(balls *ball, cues *cue);bool Is_Cue_Hit (cues *cue, balls *ball, int eps);

void Cue_Init(cues *cue, int number, bool active);
void Ball_Init(balls *ball, double vx, double vy, int r, COLORREF border_color, COLORREF fill_color, bool active);
void Ball_Delete (balls *ball);
void Draw_Score(scores score);
void Control_Game(balls *ball1, balls *ball2, cues *cue1, cues *cue2, scores *score);
bool Is_Ball_In_Pocket(balls *ball, cues *cue1, cues *cue2, scores *score);
bool Is_Collision (balls *ball1, balls *ball2);
balls To_New_SK (balls ball, float fi);
float Angle (float x1, float y1, float x2, float y2);
balls To_Old_SK (balls ball, float fi);
void Control_Button(balls *ball1, balls *ball2);
void Button_Init (buttons *button, int x, int y, int lx, int ly, COLORREF border_color, COLORREF fill_color, char *s);
void Draw_Button(buttons button);



//-----------------------------------------------------
int main()
{

    txCreateWindow (1000, 800);

    Came_Billard();

    return(0);
}
//-----------------------------------------------------
void Came_Billard()
{

    balls ball1;
    balls ball2;
    cues  cue1;
    cues  cue2;
    scores score;

    buttons button_F1;
    buttons button_ESC;

    Cue_Init (&cue1, 1, true);
    Cue_Init (&cue2, 2, false);

    Ball_Init(&ball1, 0, 0, 20,  TX_WHITE, TX_WHITE, true);
    Ball_Init(&ball2, 0, 0, 20,  TX_WHITE, TX_WHITE, true);

    Button_Init(&button_F1, 100, 720, 150, 50, TX_WHITE, TX_RED, "F1 - NewGame");
    Button_Init(&button_ESC, 800, 720, 150, 50, TX_WHITE, TX_RED, "ESC - Exit");

    int dt = 1;
    int t  = 0;

    score.player1 = 0;
    score.player2 = 0;

    Draw_Landscape();

    while (!txGetAsyncKeyState (VK_ESCAPE))
    {

        txClear();

        Draw_Landscape ();

        Draw_Button (button_F1);
        Draw_Button (button_ESC);

        Draw_Ball     (ball1);
        Draw_Ball     (ball2);
        Control_Balls (&ball1, &ball2);

        Draw_Cue    (cue1, t);
        Move_Cue    (&cue1);
        Control_Cue (&cue1, &ball1);
        Control_Cue (&cue1, &ball2);

        Draw_Cue    (cue2, t);
        Move_Cue    (&cue2);
        Control_Cue (&cue2, &ball1);
        Control_Cue (&cue2, &ball2);

        Physics_Ball(&ball1, dt);
        Physics_Ball(&ball2, dt);

        Control_Game(&ball1, &ball2, &cue1, &cue2, &score);

        Control_Button(&ball1, &ball2);

        Draw_Score(score);

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
        txSetFillColor (TX_RED);
        txSetColor     (TX_RED);
        txRectangle    (0, 0, 1000, 800);

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
void Control_Balls(balls *ball1, balls *ball2)
{

    if (Is_Collision(ball1, ball2))
    {

       txMessageBox("Hooray","Collision",MB_OK);

       balls newball1;
       balls newball2;

       float x = ball2->vx - ball1->vx;
       float y = ball2->vy - ball1->vy;


       float fi = Angle (1.0, 0.0, x, y);
       newball1 = To_New_SK (*ball1, fi);
       newball2 = To_New_SK (*ball2, fi);


       printf("%f  %f  %f  %f\n", ball1->vx, ball1->vy, ball2->vx, ball2->vy);


       float psi = Angle (1.0, 0.0, newball1.vx, newball1.vy);
       float v1x = newball1.vx * cos(psi);
       float v1y = newball1.vx * sin(psi);

       psi = Angle (1.0, 0.0, newball2.vx, newball2.vy);
       float v2x = newball1.vx * cos(psi);
       float v2y = newball1.vx * sin(psi);

       newball1.vx = v2x;
       newball1.vy = v2y;

       newball2.vx = v1x;
       newball2.vy = v1y;

       *ball1 = To_Old_SK (newball1, fi);
       *ball2 = To_Old_SK (newball2, fi);


   }


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

        cue->x2 = cue->x1 + (int)(cue->length * cos(cue->angle * txPI / 180));
        cue->y2 = cue->y1 + (int)(cue->length * sin(cue->angle * txPI / 180));


    }

}
//-----------------------------------------------------
void Control_Cue(cues *cue, balls *ball)
{
    int num = cue->number;
    bool active = cue->active;

   if (cue->active == true)
   {
        if (Is_Cue_Hit(cue, ball, 10))
            {
                Cue_Hit  (ball, cue);
                Cue_Init (cue, num, active);
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

    //txSetColor     (TX_BLACK);
    //txSetFillColor (TX_BLACK);

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

    if ((distance <= eps) && Is_Line(cue->x1, cue->y1, cue->x2, cue->y2, ball->x, ball->y))
        hit = true;

    return hit;

}
//-----------------------------------------------------
bool Is_Line(int x1, int y1, int x2, int y2, int x3, int y3)
{

   if ((x3 - x1) / (x2 - x1) == (y3 - y1) / (y2 - y1))
        return true;
   else return false;

}
//-----------------------------------------------------
void Cue_Hit(balls *ball, cues *cue)
{
  ball->vx = (float)(cue->x2 - cue->x1);//sqrt((cue->x1 - cue->x2) * (cue->x1 - cue->x2) + (cue->y1 - cue->y2) * (cue->y1 - cue->y2)) * 20;
  ball->vy = (float)(cue->y2 - cue->y1);//sqrt((cue->x1 - cue->x2) * (cue->x1 - cue->x2) + (cue->y1 - cue->y2) * (cue->y1 - cue->y2)) * 20;

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

    cue->x2 = cue->x1 + (int)(cue->length * cos(cue->angle * txPI / 180));
    cue->y2 = cue->y1 + (int)(cue->length * sin(cue->angle * txPI / 180));

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
   /*int t = 0;
   HDC bang = txLoadImage("bang.bmp");

   int sizeX = txGetExtentX(bang)/6;
   int sizeY = txGetExtentY(bang);
   int startX = ball->x - 10;
   int startY = ball->y - 10;

    while (t <= 10)
    {
        txTransparentBlt(txDC(), startX, startY, sizeX, 0, bang, (t % 6) * sizeX, 0);
         t++;
         txSleep(100);
    } */

   ball->x  = 0;
   ball->y  = 0;
   ball->vx = 0;
   ball->vy = 0;
   ball->r  = 0;
   ball->active = false;

//    txDeleteDC(bang);

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
void Control_Game(balls *ball1, balls *ball2, cues *cue1, cues *cue2, scores *score)
{

    if (Is_Ball_In_Pocket(ball1, cue1, cue2, score) || Is_Ball_In_Pocket(ball2, cue1, cue2, score))
    {

        cue1->active = !cue1->active;
        cue2->active = !cue2->active;

    }

}
//-----------------------------------------------------

bool Is_Ball_In_Pocket(balls *ball, cues *cue1, cues *cue2, scores *score)
{
    bool res = false;

    if ((txGetPixel(ball->x, ball->y) == TX_BLACK) && (ball->active == true))
    {

        Ball_Delete (ball);

        if (cue1->active == true)
        {

          score->player1 ++;
        }

        else if (cue2->active == true)
             {

               score->player2 ++;
             }

        res = true;

     }

     return res;

}
//-----------------------------------------------------
bool Is_Collision (balls *ball1, balls *ball2)
{

    bool res = false;

    float distance = sqrt((ball1->x - ball2->x) * (ball1->x - ball2->x) + (ball1->y - ball2->y) * (ball1->y - ball2->y));

    if ((ROUND (distance) <= 2 * ball1->r) && (ball1->active == true) && (ball2->active == true))
        res = true;

    return res;

}
//-----------------------------------------------------
float Angle (float x1, float y1, float x2, float y2)
{

    return acos((x1 * x2 + y1 * y2)/(sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2)));

}
//-----------------------------------------------------
balls To_New_SK (balls ball, float fi)
{
    balls newball;

    newball.x = ball.x  * cos(fi) - ball.y * sin(fi);
    newball.y = ball.x  * sin(fi) + ball.y * cos(fi);

    return newball;

}
//-----------------------------------------------------
balls To_Old_SK (balls ball, float fi)
{
    balls newball;

    newball.x = ball.x  * cos(fi) + ball.y * sin(fi);
    newball.y = -ball.x  * sin(fi) + ball.y * cos(fi);

    return newball;

}
//-----------------------------------------------------
void Draw_Button(buttons button)
{
 txSetColor     (button.border_color, 2);
 txSetFillColor (button.fill_color);

 txRectangle    (button.x, button.y, button.x + button.lx, button.y + button.ly);

 txSelectFont   ("Times New Roman", 20, 0, FW_BOLD);
 txDrawText     (button.x, button.y, button.x + button.lx, button.y + button.ly, button.s);

}
//-----------------------------------------------------
void Button_Init (buttons *button, int x, int y, int lx, int ly, COLORREF border_color, COLORREF fill_color, char *s)
{

    button->x  = x;
    button->y  = y;
    button->lx = lx;
    button->ly = ly;
    button->border_color = border_color;
    button->fill_color   = fill_color;
    button->s = s;

}
//-----------------------------------------------------
void Control_Button(balls *ball1, balls *ball2)
{

    if (txGetAsyncKeyState (VK_F1))
    {
        Ball_Init(ball1, 0,   0,   20,  TX_WHITE, TX_WHITE, true);
        Ball_Init(ball2, 0,   0,   20,  TX_WHITE, TX_WHITE, true);
    }

}
//-----------------------------------------------------

