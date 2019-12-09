#include "main.h"

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Game_Display_Func[Game_Stt]();
    if (Is_Perfect) {
        glColor4f(1.0f, 1.0f, 1.0f, Perfect_Alpha[Perfect_Stt]);
        Map_Texture(&Img_Perfect);
        Draw_Rect(&Rct_Perfect);
        glColor3fv(Color_White);
    }
    glutSwapBuffers();
}

void Timer(int value) {
    Game_Process_Func[Game_Stt]();
    if (Is_Perfect) {
        Rct_Perfect.Left = Goal_Stand->x + Rct_Perfect_Save[Perfect_Stt].Left;
        Rct_Perfect.Right = Goal_Stand->x + Rct_Perfect_Save[Perfect_Stt].Right;
        Rct_Perfect.Bottom = Goal_Stand->y + Rct_Perfect_Save[Perfect_Stt].Bottom;
        Rct_Perfect.Top = Goal_Stand->y + Rct_Perfect_Save[Perfect_Stt].Top;
        Perfect_Stt++;
        if (Perfect_Stt == 80)
            Is_Perfect = 0;
    }
    glutPostRedisplay();
    glutTimerFunc(INTERVAL, Timer, 0);
}

void Passive_Menu(int x, int y) {
    if (Sqr(x - POS_START_X) + Sqr(HEIGHT - y - POS_START_Y) < 5929.0f)
        Menu_Stt = 1;
    else
        Menu_Stt = 0;
}

void Mouse_Menu(int button, int status, int x, int y) {
    if (Menu_Stt == 1) {
        Mix_PlayChannel(-1, Sound_Play, 0);
        Menu_Stt_Go = 1;
        glutPassiveMotionFunc(NULL);
        glutMouseFunc(NULL);
    }
}

void Mouse_Game(int button, int status, int x, int y) {
    if (Prog_Bar.Alpha > 0.99f && button == GLUT_LEFT_BUTTON) {
        if (status == GLUT_DOWN) {
            Prog_Bar.Stt_2 = 0;
            Prog_Bar.Stt_Func = 2;
        } else if (Prog_Bar.Stt_Func == 2) {
            Game_Stt = GAME_STT_SHOOT;
            Mix_PlayChannel(-1, Sound_Shoot, 0);
            Prog_Bar.Stt_Func = 0;
            Ball.vx = -0.6f * sin(Prog_Bar.Angle / RADIAN) * Prog_Bar.Offset_2[Prog_Bar.Stt_2];
            Ball.vy = 0.6f * cos(Prog_Bar.Angle / RADIAN) * Prog_Bar.Offset_2[Prog_Bar.Stt_2];
        }
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Into The Circle");
    Init_Game();
    glutDisplayFunc(Display);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}
