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
    int key_right;
    int key_left;
    int key_up;
    int key_down;
    int key_stop;
};

struct cues
{
    int x;
    int y;
    double length;
    int angle;
    COLORREF border_color;
    COLORREF fill_color;
};

void Draw_Landscape(void);


int main()
{
    balls ball1, ball2;
    cues cue1;

    Draw_Landscape();

    return(0);
}

//-----------------------------------------------------
void Draw_Ball(balls ball)
{

    txSetColor     (ball.border_color);
    txSetFillColor (ball.fill_color);

    txCircle    (ball.x, ball.y, ball.r);

}
//-----------------------------------------------------
void Draw_Cue(cues cue)
{

    txSetColor     (cue.border_color);

    txSetFillColor ( cue.fill_color);

    txLine     (cue.x, cue.y, cue.x + (int)(cue.length * cos(cue.angle * pi / 180)), cue.y + (int)(cue.length * sin(cue.angle * pi / 180)));


}
//-----------------------------------------------------
void Draw_Landscape(void)
{

        txCreateWindow (1000, 800);

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



//-----------------------------------------------------



}


