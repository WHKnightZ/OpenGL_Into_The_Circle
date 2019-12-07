#include "../Library/loadpng.h"
#include "../Library/process_image.h"
#include <math.h>
#include <time.h>

#include <SDL2/SDL_mixer.h>
#include <GL/glut.h>

#define WIDTH 360
#define HEIGHT 540
#define INTERVAL 15

#define POS_START_X 180.0f
#define POS_START_Y 100.0f

#define TIME_IDLE 10

#define RADIAN 57.29578f

int POS_X, POS_Y;

enum Game_Stt {
    GAME_STT_MENU,
    GAME_STT_IDLE,
    GAME_STT_PREPARE,
    GAME_STT_SHOOT,
    GAME_STT_GOAL,
    GAME_STT_DEAD,
    GAME_STT_EXIT,
};

typedef struct Rect {
    float Left, Right, Bottom, Top;
} Rect;

typedef struct Goal {
    float x, y, r;
    int Is_Cover;
    Rect Rct, Rct_Center, Rct_Shadow;
    struct Goal *next;
} Goal;

struct Ball {
    Rect Rct;
    float x, y, vx, vy;
    float w2, h2;
} Ball;

struct Cover {
    int Stt;
    float Offset;
    Rect TexCoord, Vertex;
} Cover;

struct Progress_Bar {
    Rect Rct;
    float Alpha, Angle, Angle_Save;
    int Stt_Func;
    int Stt_1, Loop_1[100];
    float Offset_1[100];
    int Stt_2, Loop_2[50], Offset_2[50];
} Prog_Bar;

void (*Game_Process_Func[10])();
void (*Game_Display_Func[10])();

float Color_White[3] = {1.0f, 1.0f, 1.0f};
float Menu_Color[2][3] = {{0.80f, 0.18f, 0.18f}, {0.85f, 0.05f, 0.05f}};

int Menu_Stt, Menu_Stt_Go;
float Menu_Alpha;
int Game_Stt, Game_Timer = 0, Dead_Stt;
float Game_Alpha;
Goal *Goal_Current, *Goal_Last, *Goal_Ptr, *Goal_Cover;
Image Img_Logo, Img_Start_Btn, Img_Start, Img_Start_Center;
Image Img_Ball, Img_Goal, Img_Goal_Center, Img_Goal_Shadow, Img_Goal_Cover;
Image Img_Prog_Bar, Img_Prog_Power;
Image Img_Num[10];
Image Img_Ripple;

Rect Rct_Logo, Rct_Start_Btn, Rct_Start, Rct_Start_Center, Rct_Score;
Rect Rct_Ripple[2];
float Ripple_Offset[2][80];
float Ripple_Alpha[2][80];

int Score, Score_Array[3], Score_Length;
float Score_Start;

float Current_Pos_Y;

Mix_Chunk *Sound_Play = NULL;
Mix_Chunk *Sound_Shoot = NULL;
Mix_Chunk *Sound_Dead = NULL;

void Game_Process_Menu();
void Game_Process_Idle();
void Game_Process_Prepare();
void Game_Process_Shoot();
void Game_Process_Goal();
void Game_Process_Dead();
void Game_Process_Exit();
void Game_Display_Menu();
void Game_Display_Idle();
void Game_Display_Prepare();
void Game_Display_Shoot();
void Game_Display_Goal();
void Game_Display_Dead();
void Game_Display_Exit();
void Passive_Menu(int x, int y);
void Mouse_Menu(int button, int status, int x, int y);
void Mouse_Game(int button, int status, int x, int y);

#include "sound.c"
#include "init.c"
#include "process.c"
#include "display.c"
