float Sqr(float x) {
    return x * x;
}

void Load_Texture(Image *img, const char *path) {
    Load_Png_Swap(&img->img, &img->w, &img->h, path);
}

void Map_Texture(Image *img) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->img);
}

void Load_Texture_Num() {
    int Pos[][4] = {
        {0, 0, 20, 46},
        {20, 0, 14, 46},
        {34, 0, 21, 46},
        {55, 0, 19, 46},
        {74, 0, 22, 46},
        {96, 0, 19, 46},
        {115, 0, 22, 46},
        {137, 0, 16, 46},
        {153, 0, 23, 46},
        {176, 0, 22, 46}};
    int *Ptr_Pos;
    Image Img, Img_Tmp;
    Load_Png(&Img.img, &Img.w, &Img.h, "Images/Nums.png");
    int i;
    for (i = 0; i < 10; i++) {
        Ptr_Pos = &Pos[i][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Num[i] = Img_Tmp;
    }
    free(Img.img);
}

void Load_Texture_Goal() {
    Image Img_Goal_Tmp, Img_Goal_Shadow_Tmp, Img_Goal_Center_Tmp;
    Load_Png(&Img_Goal_Tmp.img, &Img_Goal_Tmp.w, &Img_Goal_Tmp.h, "Images/Goal.png");
    Load_Png(&Img_Goal_Center_Tmp.img, &Img_Goal_Center_Tmp.w, &Img_Goal_Center_Tmp.h, "Images/Goal_Center.png");
    Load_Png(&Img_Goal_Shadow_Tmp.img, &Img_Goal_Shadow_Tmp.w, &Img_Goal_Shadow_Tmp.h, "Images/Goal_Shadow.png");
    Create_Image(&Img_Goal, 336, 255);
    Mix_Image(&Img_Goal, &Img_Goal_Shadow_Tmp, 34, 12);
    Mix_Image(&Img_Goal, &Img_Goal_Tmp, 0, 0);
    Mix_Image(&Img_Goal, &Img_Goal_Center_Tmp, 50, 50);
    Swap_Image(Img_Goal.img, Img_Goal.w, Img_Goal.h);
    free(Img_Goal_Tmp.img);
    free(Img_Goal_Shadow_Tmp.img);
    free(Img_Goal_Center_Tmp.img);
}

void Draw_Rect(Rect *Ptr_Rct) {
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f((int)Ptr_Rct->Left, (int)Ptr_Rct->Bottom);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f((int)Ptr_Rct->Right, (int)Ptr_Rct->Bottom);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f((int)Ptr_Rct->Right, (int)Ptr_Rct->Top);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f((int)Ptr_Rct->Left, (int)Ptr_Rct->Top);
    glEnd();
}

void Draw_Power() {
    Map_Texture(&Img_Prog_Power);
    float Offset_TexCoord = Prog_Bar.Offset_2[Prog_Bar.Stt_2] / 25.0f;
    float Offset_Vertex = Prog_Bar.Rct.Bottom + Offset_TexCoord * Img_Prog_Power.h;
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(Prog_Bar.Rct.Left, Prog_Bar.Rct.Bottom);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(Prog_Bar.Rct.Right, Prog_Bar.Rct.Bottom);
    glTexCoord2f(1.0f, Offset_TexCoord);
    glVertex2f(Prog_Bar.Rct.Right, Offset_Vertex);
    glTexCoord2f(0.0f, Offset_TexCoord);
    glVertex2f(Prog_Bar.Rct.Left, Offset_Vertex);
    glEnd();
}

void Draw_Cover() {
    Map_Texture(&Img_Goal_Cover);
    glBegin(GL_POLYGON);
    glTexCoord2f(Cover.TexCoord.Left, Cover.TexCoord.Bottom);
    glVertex2f(Cover.Vertex.Left, Cover.Vertex.Bottom);
    glTexCoord2f(Cover.TexCoord.Right, Cover.TexCoord.Bottom);
    glVertex2f(Cover.Vertex.Right, Cover.Vertex.Bottom);
    glTexCoord2f(Cover.TexCoord.Right, Cover.TexCoord.Top);
    glVertex2f(Cover.Vertex.Right, Cover.Vertex.Top);
    glTexCoord2f(Cover.TexCoord.Left, Cover.TexCoord.Top);
    glVertex2f(Cover.Vertex.Left, Cover.Vertex.Top);
    glEnd();
}

void Set_Goal(Goal *g, int y) {
    int r = 50 + rand() % 12;
    g->y = y;
    g->r = r;
    g->x = rand() % (WIDTH - 2 * (int)g->r) + g->r;
    g->Is_Cover = 0;
    g->Rct.Left = g->x - r;
    g->Rct.Right = g->Rct.Left + 336.0f * r / 64;
    g->Rct.Top = g->y + r;
    g->Rct.Bottom = g->Rct.Top - 255.0f * r / 64;
    g->Rct_Cover.Left = g->x - g->r;
    g->Rct_Cover.Right = g->x + g->r;
    g->Rct_Cover.Bottom = g->y - g->r;
    g->Rct_Cover.Top = g->y + g->r;
}

void Init_Goal() {
    Goal_Current = malloc(sizeof(Goal));
    Current_Pos_Y = 300;
    Set_Goal(Goal_Current, Current_Pos_Y);
    Current_Pos_Y += 260 + rand() % 3 * 10;
    Goal *Goal_Tmp = malloc(sizeof(Goal));
    Set_Goal(Goal_Tmp, Current_Pos_Y);
    Current_Pos_Y += 220 + rand() % 12 * 10;
    Goal_Current->next = Goal_Tmp;
    Goal_Ptr = Goal_Tmp;
    Goal_Tmp = malloc(sizeof(Goal));
    Set_Goal(Goal_Tmp, Current_Pos_Y);
    Goal_Ptr->next = Goal_Tmp;
    Goal_Ptr = Goal_Tmp;
    Goal_Tmp = malloc(sizeof(Goal));
    Current_Pos_Y += 220 + rand() % 12 * 10;
    Set_Goal(Goal_Tmp, Current_Pos_Y);
    Goal_Last = Goal_Tmp;
    Goal_Ptr->next = Goal_Tmp;
    Goal_Ptr = Goal_Tmp;
    Goal_Tmp = malloc(sizeof(Goal));
    Set_Goal(Goal_Tmp, Current_Pos_Y);
    Goal_Ptr->next = Goal_Tmp;
    Goal_Tmp->next = Goal_Current;
    Prog_Bar.Angle = Prog_Bar.Angle_Save = atan2(Ball.x - Goal_Current->x, -Ball.y + Goal_Current->y) * RADIAN;
}

void Create_Goal() {
    Goal_Last = Goal_Last->next;
    Goal_Current = Goal_Current->next;
    Current_Pos_Y += 220 + rand() % 12 * 10;
    Prog_Bar.Angle_Save = atan2(Ball.x - Goal_Current->x, -Ball.y + Goal_Current->y) * RADIAN;
    Set_Goal(Goal_Last, Current_Pos_Y);
}

void Update_Score() {
    int x = Score;
    Score_Length = 0;
    int Size = 0, Size_s;
    do {
        Score_Array[Score_Length] = x % 10;
        Size += Img_Num[Score_Array[Score_Length]].w;
        x /= 10;
        Score_Length++;
    } while (x > 0);
    Score_Start = (90 - Size) / 2.0f;
}

void Load_Menu() {
    Game_Stt = GAME_STT_MENU;
    Menu_Stt = 0;
    Menu_Alpha = 0.0f;
    Menu_Stt_Go = 0;
    Is_Perfect = 0;
    glutPassiveMotionFunc(NULL);
    glutMouseFunc(NULL);
}

void Load_Game() {
    Game_Timer = 0;
    Game_Stt = GAME_STT_IDLE;
    Score = 0;
    Update_Score();
    glutMouseFunc(Mouse_Game);
}

void Init_Game() {
    // GL
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.664f, 0.872f, 0.918f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Game
    Game_Process_Func[GAME_STT_MENU] = Game_Process_Menu;
    Game_Process_Func[GAME_STT_IDLE] = Game_Process_Idle;
    Game_Process_Func[GAME_STT_PREPARE] = Game_Process_Prepare;
    Game_Process_Func[GAME_STT_SHOOT] = Game_Process_Shoot;
    Game_Process_Func[GAME_STT_GOAL] = Game_Process_Goal;
    Game_Process_Func[GAME_STT_DEAD] = Game_Process_Dead;
    Game_Process_Func[GAME_STT_EXIT] = Game_Process_Exit;
    Game_Display_Func[GAME_STT_MENU] = Game_Display_Menu;
    Game_Display_Func[GAME_STT_IDLE] = Game_Display_Idle;
    Game_Display_Func[GAME_STT_PREPARE] = Game_Display_Prepare;
    Game_Display_Func[GAME_STT_SHOOT] = Game_Display_Shoot;
    Game_Display_Func[GAME_STT_GOAL] = Game_Display_Goal;
    Game_Display_Func[GAME_STT_DEAD] = Game_Display_Dead;
    Game_Display_Func[GAME_STT_EXIT] = Game_Display_Exit;
    // Texture
    Load_Texture(&Img_Logo, "Images/Logo.png");
    Load_Texture(&Img_Start_Btn, "Images/Start_Btn.png");
    Load_Texture(&Img_Start, "Images/Start.png");
    Load_Texture(&Img_Start_Center, "Images/Start_Center.png");
    Load_Texture(&Img_Goal, "Images/Goal.png");
    Load_Texture(&Img_Goal_Cover, "Images/Goal_Cover.png");
    Load_Texture(&Img_Ball, "Images/Ball.png");
    Load_Texture(&Img_Prog_Bar, "Images/Progress_Bar.png");
    Load_Texture(&Img_Prog_Power, "Images/Progress_Power.png");
    Load_Texture(&Img_Ripple, "Images/Ripple.png");
    Load_Texture(&Img_Perfect, "Images/Perfect.png");
    Load_Texture_Num();
    //Load_Texture_Goal();
    // Rect
    Ball.w2 = Img_Ball.w / 2;
    Ball.h2 = Img_Ball.h / 2;
    Ball.x = POS_START_X;
    Ball.y = POS_START_Y;
    Rct_Logo.Left = (WIDTH - Img_Logo.w) / 2;
    Rct_Logo.Right = Rct_Logo.Left + Img_Logo.w;
    Rct_Logo.Bottom = 410.0f;
    Rct_Logo.Top = Rct_Logo.Bottom + Img_Logo.h;
    Ball.Rct.Left = POS_START_X - Ball.w2;
    Ball.Rct.Right = Ball.Rct.Left + Img_Ball.w;
    Ball.Rct.Bottom = POS_START_Y - Ball.h2;
    Ball.Rct.Top = Ball.Rct.Bottom + Img_Ball.h;
    Rct_Start.Left = POS_START_X - 77.0f;
    Rct_Start.Right = Rct_Start.Left + 154.0f;
    Rct_Start.Bottom = POS_START_Y - 77.0f;
    Rct_Start.Top = Rct_Start.Bottom + 154.0f;
    Rct_Start_Center.Left = POS_START_X - 15.0f;
    Rct_Start_Center.Right = Rct_Start_Center.Left + 30.0f;
    Rct_Start_Center.Bottom = POS_START_Y - 15.0f;
    Rct_Start_Center.Top = Rct_Start_Center.Bottom + 30.0f;
    Rct_Start_Btn.Left = Ball.x + 10.0f - Img_Start_Btn.w / 2;
    Rct_Start_Btn.Right = Rct_Start_Btn.Left + Img_Start_Btn.w;
    Rct_Start_Btn.Bottom = Ball.y - Img_Start_Btn.h / 2;
    Rct_Start_Btn.Top = Rct_Start_Btn.Bottom + Img_Start_Btn.h;
    Rct_Score.Bottom = 20.0f;
    Rct_Score.Top = Rct_Score.Bottom + Img_Num[0].h;
    Prog_Bar.Rct.Left = 0.0f - Img_Prog_Bar.w / 2;
    Prog_Bar.Rct.Right = Prog_Bar.Rct.Left + Img_Prog_Bar.w;
    Prog_Bar.Rct.Bottom = 24.0f;
    Prog_Bar.Rct.Top = Prog_Bar.Rct.Bottom + Img_Prog_Bar.h;
    // Progress Bar
    int i;
    for (i = 0; i < 25; i++)
        Prog_Bar.Offset_1[i] = -0.5f * i;
    for (i = 25; i < 75; i++)
        Prog_Bar.Offset_1[i] = 0.5f * i - 25.0f;
    for (i = 75; i < 100; i++)
        Prog_Bar.Offset_1[i] = 50.0f - 0.5f * i;
    for (i = 0; i < 99; i++)
        Prog_Bar.Loop_1[i] = i + 1;
    Prog_Bar.Loop_1[99] = 0;
    Prog_Bar.Stt_Func = 1;
    for (i = 0; i < 25; i++)
        Prog_Bar.Offset_2[i] = i;
    for (i = 25; i < 50; i++)
        Prog_Bar.Offset_2[i] = 50 - i;
    for (i = 0; i < 49; i++)
        Prog_Bar.Loop_2[i] = i + 1;
    Prog_Bar.Loop_2[49] = 0;
    // Ripple
    for (i = 0; i < 36; i++)
        Ripple_Offset[0][i] = i * 2.0f;
    for (i = 36; i < 80; i++)
        Ripple_Offset[0][i] = Ripple_Offset[0][i - 1];
    for (i = 0; i < 48; i++)
        Ripple_Alpha[0][i] = 1 - i / 48.0f;
    for (i = 48; i < 80; i++)
        Ripple_Alpha[0][i] = 0.0f;
    for (i = 0; i < 10; i++) {
        Ripple_Offset[1][i] = 0.0f;
        Ripple_Alpha[1][i] = 1.0f;
    }
    for (i = 10; i < 80; i++) {
        Ripple_Offset[1][i] = Ripple_Offset[0][i - 10];
        Ripple_Alpha[1][i] = Ripple_Alpha[0][i - 10];
    }
    // Perfect
    for (i = 0; i < 20; i++) {
        Rct_Perfect_Save[i].Left = -(i + 1) * 2.3f;
        Rct_Perfect_Save[i].Right = (i + 1) * 2.3f;
        Rct_Perfect_Save[i].Bottom = -(i + 1) * 0.3f - 10.0f + 80.0f;
        Rct_Perfect_Save[i].Top = (i + 1) * 0.3f + 10.0f + 80.0f;
        Perfect_Alpha[i] = 1.0f;
    }
    for (i = 20; i < 80; i++) {
        Rct_Perfect_Save[i].Left = -46.0f;
        Rct_Perfect_Save[i].Right = 46.0f;
        Rct_Perfect_Save[i].Bottom = -18.0f + (i - 19) * 1.0f + 80.0f;
        Rct_Perfect_Save[i].Top = 18.0f + (i - 19) * 1.0f + 80.0f;
        Perfect_Alpha[i] = (80 - 1.0f - i) / (80 - 20);
    }
    //
    Init_Sound();
    Init_Goal();
    Load_Menu();
}
