void Draw_Start() {
    if (Rct_Start.Top > 0) {
        Map_Texture(&Img_Start);
        Draw_Rect(&Rct_Start);
        Map_Texture(&Img_Start_Center);
        Draw_Rect(&Rct_Start_Center);
    }
}

void Draw_Goal() {
    Goal_Ptr = Goal_Current;
    int i;
    for (i = 0; i < 5; i++) {
        if (Goal_Ptr->Rct.Bottom <= HEIGHT && Goal_Ptr->Rct.Top >= 0) {
            Map_Texture(&Img_Goal);
            Draw_Rect(&Goal_Ptr->Rct);
            if (Goal_Ptr->Is_Cover) {
                Map_Texture(&Img_Goal_Cover);
                Draw_Rect(&Goal_Ptr->Rct_Cover);
            }
        }
        Goal_Ptr = Goal_Ptr->next;
    }
}

void Draw_Ball() {
    Map_Texture(&Img_Ball);
    Draw_Rect(&Ball.Rct);
}

void Draw_Score() {
    Rct_Score.Right = Score_Start;
    int i;
    for (i = Score_Length - 1; i >= 0; i--) {
        Rct_Score.Left = Rct_Score.Right;
        Rct_Score.Right = Rct_Score.Left + Img_Num[Score_Array[i]].w;
        Map_Texture(&Img_Num[Score_Array[i]]);
        Draw_Rect(&Rct_Score);
    }
}

void Game_Display_Menu() {
    if (Menu_Stt_Go == 1) {
        glColor3f(1.0f, 1.0f, 1.0f);
        Map_Texture(&Img_Start);
        Draw_Rect(&Rct_Start);
        Draw_Goal();
        Draw_Ball();
        glColor4f(1.0f, 1.0f, 1.0f, Menu_Alpha);
    } else {
        glColor4f(1.0f, 1.0f, 1.0f, Menu_Alpha);
        Map_Texture(&Img_Start);
        Draw_Rect(&Rct_Start);
        Draw_Goal();
    }
    Map_Texture(&Img_Logo);
    Draw_Rect(&Rct_Logo);
    glColor4f(Menu_Color[Menu_Stt][0], Menu_Color[Menu_Stt][1], Menu_Color[Menu_Stt][2], Menu_Alpha);
    Map_Texture(&Img_Start_Btn);
    Draw_Rect(&Rct_Start_Btn);
    glColor3fv(Color_White);
}

void Game_Display_Idle() {
    Draw_Start();
    Draw_Goal();
    Draw_Ball();
    Draw_Score();
}

void Game_Display_Prepare() {
    Draw_Start();
    Draw_Goal();
    glTranslatef((int)Ball.x, Ball.y, 0.0f);
    glRotatef(Prog_Bar.Angle, 0.0f, 0.0f, 1.0f);
    glColor4f(1.0f, 1.0f, 1.0f, Prog_Bar.Alpha);
    Map_Texture(&Img_Prog_Bar);
    Draw_Rect(&Prog_Bar.Rct);
    if (Prog_Bar.Stt_Func == 2)
        Draw_Power();
    glColor3fv(Color_White);
    glLoadIdentity();
    Draw_Ball();
    Draw_Score();
}

void Game_Display_Shoot() {
    Game_Display_Idle();
}

void Game_Display_Goal() {
    Draw_Start();
    Draw_Goal();
    Draw_Cover();
    Draw_Ball();
    Draw_Score();
}

void Game_Display_Dead() {
    Game_Display_Idle();
    if (Game_Timer == 10) {
        Map_Texture(&Img_Ripple);
        glColor4f(1.0f, 1.0f, 1.0f, Ripple_Alpha[0][Dead_Stt]);
        Draw_Rect(&Rct_Ripple[0]);
        glColor4f(1.0f, 1.0f, 1.0f, Ripple_Alpha[1][Dead_Stt]);
        Draw_Rect(&Rct_Ripple[1]);
        glColor3fv(Color_White);
    }
}

void Game_Display_Exit() {
    glColor4f(1.0f, 1.0f, 1.0f, Game_Alpha);
    Game_Display_Idle();
    glColor3fv(Color_White);
}
