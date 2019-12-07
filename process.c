void Game_Process_Menu() {
    if (Menu_Stt_Go == 0) {
        if (Menu_Alpha < 0.99f) {
            Menu_Alpha += 0.04f;
            if (Menu_Alpha > 0.99f) {
                glutPassiveMotionFunc(Passive_Menu);
                glutMouseFunc(Mouse_Menu);
            }
        }
    } else {
        if (Menu_Alpha > 0.01f) {
            Menu_Alpha -= 0.04f;
            if (Menu_Alpha < 0.01f) {
                Load_Game();
            }
        }
    }
}

void Game_Process_Idle() {
    Game_Timer++;
    if (Game_Timer == TIME_IDLE) {
        Prog_Bar.Stt_Func = 1;
        Prog_Bar.Stt_1 = 0;
        Prog_Bar.Angle = Prog_Bar.Angle_Save;
        Prog_Bar.Alpha = 0.0f;
        Game_Stt = GAME_STT_PREPARE;
    }
}

void Game_Process_Prepare() {
    if (Prog_Bar.Alpha < 0.99f)
        Prog_Bar.Alpha += 0.05f;
    switch (Prog_Bar.Stt_Func) {
    case 1:
        Prog_Bar.Angle = Prog_Bar.Angle_Save + Prog_Bar.Offset_1[Prog_Bar.Stt_1];
        Prog_Bar.Stt_1 = Prog_Bar.Loop_1[Prog_Bar.Stt_1];
        break;
    case 2:
        Prog_Bar.Stt_2 = Prog_Bar.Loop_2[Prog_Bar.Stt_2];
        break;
    }
}

void Game_Process_Shoot() {
    Ball.x += Ball.vx;
    Goal_Ptr = Goal_Current;
    int i;
    for (i = 0; i < 5; i++) {
        Goal_Ptr->y -= Ball.vy;
        Goal_Ptr->Rct.Bottom = Goal_Ptr->y - Goal_Ptr->r;
        Goal_Ptr->Rct.Top = Goal_Ptr->y + Goal_Ptr->r;
        Goal_Ptr->Rct_Center.Bottom = Goal_Ptr->y - 14.0f;
        Goal_Ptr->Rct_Center.Top = Goal_Ptr->y + 14.0f;
        Goal_Ptr = Goal_Ptr->next;
    }
    if (Rct_Start.Top > 0) {
        Rct_Start.Bottom -= Ball.vy;
        Rct_Start.Top -= Ball.vy;
        Rct_Start_Center.Bottom -= Ball.vy;
        Rct_Start_Center.Top -= Ball.vy;
    }
    Current_Pos_Y -= Ball.vy;
    Ball.Rct.Left = Ball.x - Ball.w2;
    Ball.Rct.Right = Ball.Rct.Left + Img_Ball.w;
    Ball.Rct.Bottom = Ball.y - Ball.h2;
    Ball.Rct.Top = Ball.Rct.Bottom + Img_Ball.h;
    Ball.vx *= 0.97f;
    Ball.vy *= 0.97f;
    if (Ball.vx < 0.2f && Ball.vy < 0.2f) {
        Game_Timer = 0;
        if (Sqr(Ball.x - Goal_Current->x) + Sqr(Ball.y - Goal_Current->y) < Sqr(Goal_Current->r - 8.0f)) {
            Cover.Stt = 1;
            Cover.Offset = Goal_Current->r * 0.08f;
            Cover.Vertex.Left = (int)Goal_Current->Rct.Left;
            Cover.Vertex.Right = Goal_Current->Rct.Right;
            Cover.Vertex.Bottom = (int)Goal_Current->Rct.Bottom;
            Cover.Vertex.Top = Cover.Vertex.Bottom + Cover.Stt * Cover.Offset;
            Cover.TexCoord.Left = 0.0f;
            Cover.TexCoord.Right = 1.0f;
            Cover.TexCoord.Bottom = 0.0f;
            Cover.TexCoord.Top = Cover.Stt / 25.0f;
            Goal_Cover = Goal_Current;
            Create_Goal();
            Game_Stt = GAME_STT_GOAL;
        } else {
            Game_Stt = GAME_STT_DEAD;
            Mix_PlayChannel(-1, Sound_Dead, 0);
        }
    }
}

void Game_Process_Goal() {
    if (Cover.Stt < 25) {
        Cover.Stt++;
        Cover.Vertex.Top = Cover.Vertex.Bottom + Cover.Stt * Cover.Offset;
        Cover.TexCoord.Top = Cover.Stt / 25.0f;
        if (Cover.Stt == 25) {
            Goal_Cover->Is_Cover = 1;
            Game_Timer = 0;
            Score++;
            Update_Score();
            Game_Stt = GAME_STT_IDLE;
        }
    }
}

void Game_Process_Dead() {
    if (Game_Timer < 10) {
        Game_Timer++;
        Ball.Rct.Left += 1.6f;
        Ball.Rct.Right -= 1.6f;
        Ball.Rct.Bottom += 1.6f;
        Ball.Rct.Top -= 1.6f;
        if (Game_Timer == 10)
            Dead_Stt = 0;
    }
    if (Game_Timer == 10) {
        int i;
        for (i = 0; i < 2; i++) {
            Rct_Ripple[i].Left = Ball.x - Ripple_Offset[i][Dead_Stt];
            Rct_Ripple[i].Right = Ball.x + Ripple_Offset[i][Dead_Stt];
            Rct_Ripple[i].Bottom = Ball.y - Ripple_Offset[i][Dead_Stt];
            Rct_Ripple[i].Top = Ball.y + Ripple_Offset[i][Dead_Stt];
        }
        Dead_Stt++;
        if (Dead_Stt == 80) {
            Game_Alpha = 1.0f;
            Game_Stt = GAME_STT_EXIT;
        }
    }
}

void Game_Process_Exit() {
    if (Game_Alpha > 0.01f) {
        Game_Alpha -= 0.05f;
        if (Game_Alpha < 0.01f) {
            Goal *Tmp;
            int i;
            for (i = 0; i < 5; i++) {
                Tmp = Goal_Current;
                Goal_Current = Goal_Current->next;
                free(Tmp);
            }
            Rct_Start.Bottom = POS_START_Y - 77.0f;
            Rct_Start.Top = Rct_Start.Bottom + 154.0f;
            Rct_Start_Center.Bottom = POS_START_Y - 15.0f;
            Rct_Start_Center.Top = Rct_Start_Center.Bottom + 30.0f;
            Ball.x = POS_START_X;
            Ball.y = POS_START_Y;
            Ball.Rct.Left = POS_START_X - Ball.w2;
            Ball.Rct.Right = Ball.Rct.Left + Img_Ball.w;
            Ball.Rct.Bottom = POS_START_Y - Ball.h2;
            Ball.Rct.Top = Ball.Rct.Bottom + Img_Ball.h;
            Init_Goal();
            Menu_Alpha = 0.0f;
            Menu_Stt_Go = 0;
            Game_Stt = GAME_STT_MENU;
        }
    }
}
