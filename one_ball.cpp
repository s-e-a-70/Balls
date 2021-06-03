#include "TxLib.h"

void Balls_Move();
void Draw_Ball    (int  x, int y, COLORREF border_color, COLORREF fill_color);
void Physics_Ball (int *x, int *y, int *vx, int *vy, int ax, int ay, int dt);
void Control_Ball (int *x, int *y, int *vx, int *vy);

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

    int x  = 100;
    int y  = 0;
    int vx = 5;
    int vy = 3;
    int ax = 0;
    int ay = 1;
    int dt = 1;

    while (!txGetAsyncKeyState (VK_ESCAPE))
    {

        Draw_Ball    (x, y, TX_MAGENTA, TX_LIGHTMAGENTA);
        Physics_Ball (&x, &y, &vx, &vy, ax, ay, dt);
        Control_Ball (&x, &y, &vx, &vy);

        txSleep (100);


    }

}

//-----------------------------------------------------
void Draw_Ball(int x, int y, COLORREF border_color, COLORREF fill_color)
{

    txSetColor     (border_color);
    txSetFillColor (fill_color);

    txCircle (x, y, 20);

}

void Physics_Ball(int *x, int *y, int *vx, int *vy, int ax, int ay, int dt)
{

    *vx += ax*dt;
    *vy += ay*dt;

    *x+= *vx*dt;
    *y+= *vy*dt;

}

void Control_Ball(int *x, int *y, int *vx, int *vy)
{

  if (*x > 800)
    {
       *vx = -(*vx);
       *x  = 800;
    }

    if (*y > 600)
    {
       *vy = -(*vy);
       *y  = 600;
    }

    if (*x < 0)
    {
       *vx = -(*vx);
       *x  = 0;
    }

    if (*y < 0)
    {
       *vy = -(*vy);
       *y  = 0;
    }

    if(txGetAsyncKeyState (VK_RIGHT)) *vx++;
    if(txGetAsyncKeyState (VK_LEFT))  *vx--;
    if(txGetAsyncKeyState (VK_UP))    *vy++;
    if(txGetAsyncKeyState (VK_DOWN))  *vy--;
    if(txGetAsyncKeyState (VK_SPACE)) {*vx = 0; *vy = 0;}

}


