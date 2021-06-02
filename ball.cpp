#include "TxLib.h"
void Ball_Move();
int main()
{
 txCreateWindow (800, 600);
 Ball_Move();

 return(0);
}
void Ball_Move()
{

 int x  = 100;
 int y  = 0;
 int vx = 5;
 int vy = 3;
 int dt = 1;
 int t  = 0;

 while (!txGetAsyncKeyState (VK_ESCAPE))
 {

    txSetColor     (RGB (t,   t,   0));
    txSetFillColor (RGB (t/2, t/2, 128));

    txCircle (x, y, 20);

    x += vx * dt;
    y += vy * dt;

    t += dt;


    if (x > 800)
    {
       vx = -vx;
       x  = 800;
    }

    if (y > 600)
    {
       vy = -vy;
       y  = 600;
    }

    if (x < 0)
    {
       vx = -vx;
       x  = 0;
    }

    if (y < 0)
    {
       vy = -vy;
       y  = 0;
    }


    if(txGetAsyncKeyState (VK_RIGHT)) vx++;

    if(txGetAsyncKeyState (VK_LEFT))  vx--;

    if(txGetAsyncKeyState (VK_UP))    vy++;

    if(txGetAsyncKeyState (VK_DOWN))  vy--;


    txSleep (100);


 }
}
