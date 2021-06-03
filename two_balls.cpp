#include "TxLib.h"

void Balls_Move();
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

  int x2  = 0;
  int y2  = 100;
  int vx2 = 3;
  int vy2 = 5;

  int t  = 0;
  int dt = 1;

  while (!txGetAsyncKeyState (VK_ESCAPE))
  {

     txSetColor     (RGB (t,   t,   0));
     txSetFillColor (RGB (t/2, t/2, 128));

     txCircle (x1, y1, 20);

     x1 += vx1 * dt;
     y1 += vy1 * dt;

     txSetColor     (RGB (0,   t,   t));
     txSetFillColor (RGB (128, t/2, t/2));

     txCircle (x2, y2, 20);

     x2 += vx2 * dt;
     y2 += vy2 * dt;

     t += dt;

     if (x1 > 800)
     {
        vx1 = -vx1;
        x1  = 800;
     }

     if (y1 > 600)
     {
        vy1 = -vy1;
        y1  = 600;
     }

     if (x1 < 0)
     {
        vx1 = -vx1;
        x1  = 0;
     }

     if (y1 < 0)
     {
        vy1 = -vy1;
        y1  = 0;
     }



     if (x2 > 800)
     {
        vx2 = -vx2;
        x2  = 800;
     }

     if (y2 > 600)
     {
        vy2 = -vy2;
        y2  = 600;
     }

     if (x2 < 0)
     {
        vx2 = -vx2;
        x2  = 0;
     }

     if (y2 < 0)
     {
        vy2 = -vy2;
        y2  = 0;
     }

     if(txGetAsyncKeyState (VK_RIGHT))
     {
         vx1++;
         vx2++;
     }

     if(txGetAsyncKeyState (VK_LEFT))
     {
         vx1--;
         vx2--;
     }

     if(txGetAsyncKeyState (VK_UP))
     {
         vy1++;
         vy2++;
     }

     if(txGetAsyncKeyState (VK_DOWN))
     {
         vy1--;
         vy2--;
     }


     txSleep (100);


 }
}
