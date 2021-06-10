#include "TxLib.h"
#include <math.h>

void Balls_Move();
void Draw_Ball    (int x, int y, COLORREF border_color, COLORREF fill_color);
void Draw_Vector  (int x, int y, int vx, int vy, COLORREF border_color);
void Average_Velosity (int vx, int vy, int vectorVx[5], int vectorVy[5], int *average_count, int *vx_sr, int *vy_sr);
void Physics_Ball (int *x, int *y, int *vx, int *vy, int ax, int ay, int dt);
void Control_Ball (int *vx, int *vy);

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

    int x1  = 100;
    int y1  = 0;
    int vx1 = 5;
    int vy1 = 3;
    int ax1 = 1;
    int ay1 = 0;

    int x2  = 0;
    int y2  = 100;
    int vx2 = 2;
    int vy2 = 1;
    int ax2 = 0;
    int ay2 = 0;

    int dt = 1;
    int count = 0;
    int l = 0;
    int lmin = 1000;

    int vectorVx[5] = {0, 0, 0, 0, 0};
    int vectorVy[5] = {0, 0, 0, 0, 0};
    int average_count  = 0;
    int vx_sr          = 0;
    int vy_sr          = 0;

    vx_sr = vx1;
    vy_sr = vy1;

    while (!txGetAsyncKeyState (VK_ESCAPE))
    {

        Draw_Ball        (x1, y1, TX_MAGENTA, TX_LIGHTMAGENTA);
        Draw_Vector      (x1, y1, vx_sr, vx_sr, TX_MAGENTA);
        Physics_Ball     (&x1, &y1, &vx1, &vy1, ax1, ay1, dt);
        Control_Ball     (&vx1,&vy1);
        Average_Velosity (vx1, vy1, vectorVx, vectorVy, &average_count, &vx_sr, &vy_sr);

        Draw_Ball    (x2, y2, TX_BLUE, TX_LIGHTBLUE);
        Draw_Vector  (x2, y2, vx2, vy2, TX_MAGENTA);
        Physics_Ball (&x2, &y2, &vx2, &vy2, ax2, ay2, dt);
        Control_Ball (&vx2, &vy2);

        if ((x1 == x2) && (y1 == y2))
            count++;

        l = (int)sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

        if ( l < lmin )
            lmin = l;


        txSleep (100);


    }

}

//-----------------------------------------------------
void Draw_Ball(int x, int y, COLORREF border_color, COLORREF fill_color)
{

    txSetColor     (border_color);
    txSetFillColor (fill_color);

    txCircle    (x, y, 20);

}

void Physics_Ball(int *x, int *y, int *vx, int *vy, int ax, int ay, int dt)
{

    *vx += ax*dt;
    *vy += ay*dt;

    *x+= *vx*dt;
    *y+= *vy*dt;

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

}

void Control_Ball(int *vx, int *vy)
{

    if(txGetAsyncKeyState (VK_RIGHT)) (*vx)++;
    if(txGetAsyncKeyState (VK_LEFT))  (*vx)--;
    if(txGetAsyncKeyState (VK_UP))    (*vy)++;
    if(txGetAsyncKeyState (VK_DOWN))  (*vy)--;
    if(txGetAsyncKeyState (VK_SPACE)) {*vx = 0; *vy = 0;}

}

void Draw_Vector(int x, int y, int vx, int vy, COLORREF border_color)
{

    txSetColor     (border_color);

    txLine   (x, y, x + 3 * vx, y + 3 * vy);
    txCircle (x + 3 * vx, y + 3 * vy, 3);

}

void Average_Velosity(int vx, int vy, int vectorVx[5], int vectorVy[5], int *average_count, int *vx_sr, int *vy_sr)
{

    int svx = 0;
    int svy = 0;
    int i   = 0;


    vectorVx[*average_count] = vx;
    vectorVy[*average_count] = vy;

    if (*average_count == 5)
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
