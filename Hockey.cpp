#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#pragma comment(lib, "winmm.lib")   // MSVC —p
#include "Mouse.h"
#include "Keyboard.h"
#include "Hockey.h"
#include "resource.h"
// Initialize various Value
Hockey::Hockey() : Scene(SceneID::HomeConfig), PlayingBGM(BGMID::NothingDies), FieldGoalWide(0.46), 
ScreenGoalWide(Magnification * FieldGoalWide), WindowMode(FALSE), CharNo(CharID::Initial), CharLv(0), 
JoypadNum(0), SideSetJoypadNum(0),Joypad(), Score(), Mode(ModeID::Initial), 
Pause(PauseID::Resume), Mute(MuteID::Mute), Drag(false), KeyImage() {
    SetWindowIconID(IDI_ICON1); SetWindowText("Speed Hockey"); SetLogDrawOutFlag(FALSE); if (DxLib_Init()) exit(-1);
    // Set Screen Size
    cd.Screen.Center._Val[0] = 450; cd.Screen.Center._Val[1] = 250;
    cd.Screen.Size._Val[0] = 900; cd.Screen.Size._Val[1] = 500;
    SetGraphMode(real(cd.Screen.Size), imag(cd.Screen.Size), 32);
    // Set Field Size
    FieldSize.Field._Val[0] = 2.54; FieldSize.Field._Val[1] = 1.44;
    FieldSize.Screen = Magnification * FieldSize.Field;
    FieldCorner[0].Field = -0.5 * FieldSize.Field; 
    FieldCorner[1].Field = 0.5 * FieldSize.Field;
    // Set Pack & Mallet
    Pack.r = 0.05;
    MalletInitialPos[0].Field._Val[0] = 0.5 * FieldGoalWide + 2.0 * (Pack.r + Mallet[0].r);
    MalletInitialPos[1].Field._Val[0] = -0.5 * FieldGoalWide - 2.0 * (Pack.r + Mallet[1].r);
    PackInitialPos[0]._Val[0] = 0.5 * FieldGoalWide;
    PackInitialPos[1]._Val[0] = -0.5 * FieldGoalWide;
    for (int i = 0; i < 2; i++) {
        FieldCorner[i].Screen = cd.Screen.Center + Magnification * FieldCorner[i].Field;
        MalletInitialPos[i].Screen = cd.Screen.Center + Magnification * MalletInitialPos[i].Field;
        Mallet[i].p = PackInitialPos[i]; cd.Sensitivity[i] = 0.025;
    }
    // initialize random number by the lapse time in Windows
    SRand(GetNowCount());
    ChangeWindow(WindowMode);
    // Load Color
    Color.Title = c.CreateColorToHandle(c.SkyGray);
    Color.Str = c.CreateColorToHandle(c.W);
    Color.Line = c.CreateColorToHandle(c.BK);
    Color.Frame = c.CreateColorToHandle(c.CocoaBrown);
    Color.HP = c.CreateColorToHandle(c.CherryPink);
    Color.Field = c.CreateColorToHandle(c.BilliardGreen);
    Color.Pack = c.CreateColorToHandle(c.OR);
    Color.Char[CharID::John] = c.CreateColorToHandle(c.Y);
    Color.Char[CharID::Komachi] = c.CreateColorToHandle(c.C);
    Color.Char[CharID::Misuzu] = c.CreateColorToHandle(c.Strawberry);
    Color.Char[CharID::Michel] = c.CreateColorToHandle(c.VIO);
    Color.Char[CharID::Yoichi] = c.CreateColorToHandle(c.COB);
    // Load Music
    for (int i = 0; i < BGMID::Num; i++) BGM[i] = LoadSoundMemByResource(MAKEINTRESOURCE(IDR_WAVE1 + i), "WAVE");
    for (int i = 0; i < MaxJoypadNum; i++) { Joypad[i].No = DX_INPUT_PAD1 + i; }
    /*FILE* icdf = fopen(DataFileName, "rb"); if (icdf == NULL) InitScene(); else { fread(&cd, sizeof(cd), 1, icdf); fclose(icdf); }*/
    for (int i = 0; i < ModeID::Num; i++) {
        for (int j = 0; j < CharID::Num; j++) {
            cd.MaxCharLv[i][j] = 0;
        }
    }
    cd.SceneBGM[SceneID::Home] = BGMID::NothingDies; cd.SceneBGM[SceneID::ModeSelect] = BGMID::Hometown;
    cd.SceneBGM[SceneID::CharSelect] = BGMID::Detect;
    cd.BGMMode = 0; for (int i = 0; i < SoundID::Num; i++) cd.SoundVol[i] = SoundVol::Max;
    ChangeSoundVol(SoundID::Total, cd.SoundVol[SoundID::Total]);
    cd.Key[CtrlID::Hold] = KEY_INPUT_W;    cd.Key[CtrlID::Home] = KEY_INPUT_T;
    cd.Key[CtrlID::Restart] = KEY_INPUT_R;    cd.Key[CtrlID::Init] = KEY_INPUT_I;
    cd.Key[CtrlID::Skip] = KEY_INPUT_X;    cd.Key[CtrlID::Exit] = KEY_INPUT_Q;
    cd.Key[CtrlID::Config] = KEY_INPUT_C;    cd.Key[CtrlID::ChMusic] = KEY_INPUT_M;
    cd.Key[CtrlID::ChWindow] = KEY_INPUT_F11;  cd.Key[CtrlID::Mute] = KEY_INPUT_ESCAPE;
    cd.Key[CtrlID::Pause] = KEY_INPUT_E;
    cd.Key[CtrlID::Back] = KEY_INPUT_U;    cd.Key[CtrlID::Forward] = KEY_INPUT_O;
    cd.Key[CtrlID::Up] = KEY_INPUT_4;    cd.Key[CtrlID::Down] = KEY_INPUT_3;
    for (int i = 0; i < CtrlID::Num; i++) { cd.TwinKey[i] = SINGLE; }
    cd.Key[CtrlID::Start] = KEY_INPUT_SPACE; cd.Key[CtrlID::Yes] = KEY_INPUT_RETURN; cd.Key[CtrlID::No] = KEY_INPUT_BACK;
    ChangeSoundVol(0, cd.SoundVol[0]); UpdateScene(SceneID::Home);
}
void Hockey::ChangeWindow(int WindowModeFlag) {
    ChangeWindowMode(WindowModeFlag); SetDrawScreen(DX_SCREEN_BACK);
    // Load Image
    for (int i = 0; i < KeyNum; i++) KeyImage[KeyNo[i]] = LoadGraphToResource(MAKEINTRESOURCE(KeyNo[i]), "PNG");
    for (int i = 0; i < CharID::Num; i++) CharImage[i] = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG68 + i), "PNG");
    for (int i = 0; i < IconID::Num - WindowModeFlag; i++) Icon[i] = LoadGraphToResource(MAKEINTRESOURCE(IDB_PNG74 + i), "PNG");
    // Load Font
    Font.Title = CreateFontToHandle("Brush Script MT Italic", 60, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
    Font.Str = CreateFontToHandle("Calisto MT Bold", 18, 5, DX_FONTTYPE_NORMAL);
    Font.CharName = CreateFontToHandle("Arial Rounded MT Bold", 30, 2, DX_FONTTYPE_ANTIALIASING_EDGE);
    WindowMode = GetWindowModeFlag(); return;
}
void Hockey::Config() {
    DrawBox(0, 25, 640, 455, Color.Char[0], TRUE); DrawBox(0, 25, 412, 325, Color.Char[1], TRUE);
    DrawStringToHandle(5, 25, "Music", Color.Line, Font.Title);
    DrawStringToHandle(412, 25, "System", Color.Str, Font.Title);
    DrawStringToHandle(5, 180, "Play Style", Color.Line, Font.Str);
    DrawStringToHandle(115, 180, (": " + BGMModeName[cd.BGMMode]).c_str(), Color.Line, Font.Str);
    DrawStringToHandle(0, 210, "Scene Theme", Color.Line, Font.Str);
    for (int i = 0; i < 2; i++) {
        ChangeKey(349 + 31 * i, 53, 380 + 31 * i, 83, CtrlID::Down + i);
        DrawGraph(352 + 31 * i, 55, KeyImage[cd.Key[CtrlID::Down + i]], TRUE);
        DrawGraph(355 + 25 * i, 175, Icon[IconID::Down + i], TRUE);
        DrawGraph(420 + 110 * i, 395, Icon[IconID::Hold + 14 * i], TRUE);
        ChangeKey(497 + 110 * i, 393, 528 + 110 * i, 423, CtrlID::Hold + 14 * i);
        DrawGraph(500 + 110 * i, 395, KeyImage[cd.Key[CtrlID::Hold + 14 * i]], TRUE);
    } DrawStringToHandle(450, 400, "Hold             Back", Color.Str, Font.Str);
    ChangeKey(607, 423, 638, 453, CtrlID::Forward); DrawGraph(610, 425, KeyImage[cd.Key[CtrlID::Forward]], TRUE);
    DrawStringToHandle(450, 430, "Forward", Color.Str, Font.Str); DrawGraph(420, 425, Icon[IconID::Forward], TRUE);
    for (int i = 0; i < SoundID::Num; i++) {
        DrawStringToHandle(0, 90 + 30 * i, SoundName[i].c_str(), Color.Line, Font.Str);
        DrawBox(100, 95 + 30 * i, 100 + cd.SoundVol[i], 105 + 30 * i, Color.Str, TRUE);
        DrawBox(100 + cd.SoundVol[i], 95 + 30 * i, 300, 105 + 30 * i, Color.Line, TRUE);
        DrawBox(95 + cd.SoundVol[i], 90 + 30 * i, 105 + cd.SoundVol[i], 110 + 30 * i, Color.Char[0], TRUE);
        DrawFormatStringToHandle(315, 90 + 30 * i, Color.Line, Font.Str, "%3d", cd.SoundVol[i]);
        DrawStringToHandle(5, 240 + 30 * i, SceneName[i].c_str(), Color.Line, Font.Str);
        DrawStringToHandle(115, 240 + 30 * i, (": " + BGMName[cd.SceneBGM[i]]).c_str(), Color.Line, Font.Str);
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) { DrawGraph(355 + 25 * j, 85 + 30 * i + 150 * k, Icon[IconID::Down + j], TRUE); }
        }
        if (ClickBox(355, 85 + 30 * i, 380, 110 + 30 * i) > 10) ChangeSoundVol(i, cd.SoundVol[i] - 1);
        else if (ClickBox(380, 85 + 30 * i, 405, 110 + 30 * i) > 10) ChangeSoundVol(i, cd.SoundVol[i] + 1);
        else if (ClickBox(355, 85 + 30 * i, 380, 110 + 30 * i) == 1) ChangeSoundVol(i, cd.SoundVol[i] - 4);
        else if (ClickBox(380, 85 + 30 * i, 405, 110 + 30 * i) == 1) ChangeSoundVol(i, cd.SoundVol[i] + 4);
        if (ClickBox(99, 85 + 30 * i, 301, 115 + 30 * i) > 0) ChangeSoundVol(i, real(Cursor.Screen) - 100);
        if (ClickBox(355, 235 + 30 * i, 405, 260 + 30 * i) == 1) {
            PlaySoundMem(SE[1], DX_PLAYTYPE_BACK);
            if (real(Cursor.Screen) < 380) { cd.SceneBGM[i] += BGMID::Num - 1; }
            else { cd.SceneBGM[i]++; } cd.SceneBGM[i] %= BGMID::Num;
        }
    }
    for (int i = 0; i < 7; i++) {
        DrawGraph(420, 95 + 30 * i, Icon[IconID::Home + i], TRUE);
        DrawStringToHandle(450, 100 + 30 * i, CtrlName[IconID::Home + i].c_str(), Color.Str, Font.Str);
    }
    DrawGraph(420, 305, Icon[IconID::AnotherWindow + WindowMode], TRUE);
    DrawStringToHandle(450, 310, CtrlName[CtrlID::ChWindow].c_str(), Color.Str, Font.Str);
    DrawGraph(420, 335, Icon[IconID::Mute + Mute + 1], TRUE);
    DrawStringToHandle(450, 340, MuteName[Mute].c_str(), Color.Str, Font.Str);
    DrawGraph(420, 365, Icon[IconID::Pause + Pause + 2], TRUE);
    DrawStringToHandle(450, 370, PauseName[Pause].c_str(), Color.Str, Font.Str);
    for (int i = 0; i < 4; i++) {
        DrawStringToHandle(5, 340 + 30 * i, BoardCtrlName[i].c_str(), Color.Str, Font.Str);
        DrawStringToHandle(56, 340 + 30 * i, (":      " + CtrlName[2 * i]).c_str(), Color.Str, Font.Str);
        DrawStringToHandle(287, 340 + 30 * i, CtrlName[1 + 2 * i].c_str(), Color.Str, Font.Str);
        for (int j = 0; j < 2; j++) {
            ChangeKey(224 + 160 * j, 333 + 30 * i, 255 + 160 * j, 363 + 30 * i, j + 2 * i);
            DrawGraph(227 + 160 * j, 335 + 30 * i, KeyImage[cd.Key[j + 2 * i]], TRUE);
            DrawGraph(64 + 194 * j, 335 + 30 * i, Icon[j + 2 * i], TRUE);
        }
    }
    for (int i = 0; i < 10; i++) {
        ChangeKey(607, 93 + 30 * i, 638, 123 + 30 * i, CtrlID::Home + i);
        DrawGraph(610, 95 + 30 * i, KeyImage[cd.Key[CtrlID::Home + i]], TRUE);
    }
    if (HitCtrlKey(CtrlID::Back) == 1 || ClickBox(30, 0, 55, 25) == 1) UpdateScene(Scene + SceneID::Num);
    else if (ClickBox(355, 175, 405, 200) == 1) {
        PlaySoundMem(SE[1], DX_PLAYTYPE_BACK);
        if (real(Cursor.Screen) < 380) { cd.BGMMode += BGMModeID::Num - 1; }
        else { cd.BGMMode++; } cd.BGMMode %= BGMModeID::Num;
        if (!cd.BGMMode) { PlaySoundMem(BGM[PlayingBGM], DX_PLAYTYPE_BACK | DX_PLAYTYPE_LOOP); }
        else { PlaySoundMem(BGM[PlayingBGM], DX_PLAYTYPE_BACK); }
    } return;
}
int Hockey::HitKey(int KeyCode) { return Keyboard::GetInstance()->GetPressingCount(KeyCode); }
int Hockey::HitCtrlKey(int KeyRole) {
    switch (cd.TwinKey[KeyRole]) {
    case SINGLE: return HitKey(cd.Key[KeyRole]);
    case SHIFT:  return HitKey(KEY_INPUT_LSHIFT) + HitKey(KEY_INPUT_RSHIFT);
    case CTRL:   return HitKey(KEY_INPUT_LCONTROL) + HitKey(KEY_INPUT_RCONTROL);
    case ALT:    return HitKey(KEY_INPUT_LALT) + HitKey(KEY_INPUT_RALT);
    default: exit(-1);
    }
}
void Hockey::ChangeKey(int x1, int y1, int x2, int y2, int KeyRole) {
    if (real(Cursor.Screen) > x1 && imag(Cursor.Screen) > y1 && real(Cursor.Screen) < x2 && imag(Cursor.Screen) < y2) {
        DrawBox(x1, y1, x2, y2, Color.HP, TRUE);
        for (int i = 0; i < KeyNum; i++) {
            if (HitKey(KeyNo[i]) > 0) {
                for (int j = 0; j < CtrlID::Num; j++) {
                    if (KeyNo[i] == cd.Key[j]) { DrawBox(x1, y1, x2, y2, Color.Pack, TRUE); return; }
                }
                cd.Key[KeyRole] = KeyNo[i]; PlaySoundMem(SE[1], DX_PLAYTYPE_BACK);
                if (cd.Key[KeyRole] == KEY_INPUT_LSHIFT || cd.Key[KeyRole] == KEY_INPUT_RSHIFT) { cd.TwinKey[KeyRole] = SHIFT; }
                else if (cd.Key[KeyRole] == KEY_INPUT_LCONTROL || cd.Key[KeyRole] == KEY_INPUT_RCONTROL) { cd.TwinKey[KeyRole] = CTRL; }
                else if (cd.Key[KeyRole] == KEY_INPUT_LALT || cd.Key[KeyRole] == KEY_INPUT_RALT) { cd.TwinKey[KeyRole] = ALT; }
                else { cd.TwinKey[KeyRole] = 0; } return;
            }
        }
    } return;
}
void Hockey::ChangeSound(int SoundModeFlag) {
    if (!SoundModeFlag) StopSoundMem(BGM[PlayingBGM]);
    else {
        ChangeVolumeSoundMem(BGMVol, BGM[PlayingBGM]);
        if (!cd.BGMMode) { PlaySoundMem(BGM[PlayingBGM], DX_PLAYTYPE_BACK | DX_PLAYTYPE_LOOP, 0); }
        else { PlaySoundMem(BGM[PlayingBGM], DX_PLAYTYPE_BACK, 0); }
    } Mute = CheckSoundMem(BGM[PlayingBGM]); return;
}
void Hockey::ChangeSoundVol(int SoundNo, int Vol) {
    if (Vol > SoundVol::Max) cd.SoundVol[SoundNo] = SoundVol::Max;
    else if (Vol < SoundVol::Min) cd.SoundVol[SoundNo] = SoundVol::Min; else cd.SoundVol[SoundNo] = Vol;
    switch (SoundNo) {
    case SoundID::Total: case SoundID::BGM:
        BGMVol = (int)(0.0064 * cd.SoundVol[SoundID::Total] * cd.SoundVol[SoundID::BGM]);
        ChangeVolumeSoundMem(BGMVol, BGM[PlayingBGM]); if (SoundNo == SoundID::BGM) return;
    case SoundID::SE: SEVol = (int)(0.0064 * cd.SoundVol[SoundID::Total] * cd.SoundVol[SoundID::SE]);
        for (int i = 0; i < SEID::Num; i++) { ChangeVolumeSoundMem(SEVol, SE[i]); } return;
    default: exit(-1);
    }
}
void Hockey::ChangeBGM(int TargetBGM) {
    StopSoundMem(BGM[PlayingBGM]); PlayingBGM = TargetBGM; ChangeVolumeSoundMem(BGMVol, BGM[PlayingBGM]);
    if (Mute) {
        if (!cd.BGMMode) { PlaySoundMem(BGM[PlayingBGM], DX_PLAYTYPE_BACK | DX_PLAYTYPE_LOOP); }
        else { PlaySoundMem(BGM[PlayingBGM], DX_PLAYTYPE_BACK); }
    } return;
}
int Hockey::ClickBox(int x1, int y1, int x2, int y2) {
    if (real(Cursor.Screen) > x1 && imag(Cursor.Screen) > y1 && real(Cursor.Screen) < x2 && imag(Cursor.Screen) < y2)
        return Mouse::GetInstance()->GetPressingCount(Mouse::LEFT_CLICK);
    return -1;
}
void Hockey::Home() {
    DrawBox(0, 0, real(cd.Screen.Size), imag(cd.Screen.Size), Color.Char[CharID::Yoichi], TRUE);
    for (int i = 0; i < CharID::Num; i++) DrawGraph(160 * i, 30, CharImage[i], TRUE);
    // Draw Field
    DrawBoxAA(real(FieldCorner[0].Screen), imag(FieldCorner[0].Screen), real(FieldCorner[1].Screen), imag(FieldCorner[1].Screen),
        Color.Field, TRUE);
    // Draw Line on the Field
    DrawBoxAA(real(FieldCorner[0].Screen), imag(FieldCorner[0].Screen), real(FieldCorner[1].Screen), imag(FieldCorner[1].Screen),
        Color.Str, FALSE);
    DrawLine(real(cd.Screen.Center), imag(FieldCorner[0].Screen), real(cd.Screen.Center), imag(FieldCorner[1].Screen), Color.Str);
    DrawCircleAA(real(cd.Screen.Center), imag(cd.Screen.Center), 0.5 * ScreenGoalWide, 100, Color.Str, FALSE);
    if (!Mouse::GetInstance()->GetPressingCount(Mouse::LEFT_CLICK)) Drag = false;
    else if (norm(Cursor.Field - Pack.p) <= Pack.r * Pack.r) Drag = true;
    if (abs(real(Cursor.Field)) <= real(FieldCorner[1].Field) - Pack.r && 
        abs(imag(Cursor.Field)) <= imag(FieldCorner[1].Field) - Pack.r && Drag) Pack.p = Cursor.Field;
    JoypadNum = GetJoypadNum();
    for (int i = 0; i < JoypadNum; i++) {
        GetJoypadDirectInputState(Joypad[i].No, &Joypad[i].Input);
        if (!Joypad[i].IsSideSet) {
            if (Joypad[i].Input.Buttons[8] == 128) { 
                int tmp = Joypad[i].No; Joypad[i] = Joypad[SideSetJoypadNum];
                Joypad[SideSetJoypadNum].Side = JoypadSideID::Left; Joypad[SideSetJoypadNum].IsSideSet = true; 
                Joypad[SideSetJoypadNum].No = tmp; SideSetJoypadNum++;
            }
            else if (Joypad[i].Input.Buttons[9] == 128) { 
                int tmp = Joypad[i].No; Joypad[i] = Joypad[SideSetJoypadNum];
                Joypad[SideSetJoypadNum].Side = JoypadSideID::Right; Joypad[SideSetJoypadNum].IsSideSet = true;
                Joypad[SideSetJoypadNum].No = tmp; SideSetJoypadNum++;
            }
        }
        Mallet[i].dp = cd.Sensitivity[i] + cd.Sensitivity[i] * Joypad[i].Input.Buttons[14] / 256.0;
    }
    for (int i = 0; i < SideSetJoypadNum; i++) {
        DrawFormatStringToHandle(real(cd.Screen.Size) - 150, 390 + 20 * i, Color.Str, Font.Str,
            "%dP (%s) OK!", i + 1, JoypadSideName[Joypad[i].Side].c_str());
    }
    for (int i = SideSetJoypadNum; i < JoypadNum; i++) {
        DrawFormatStringToHandle(real(cd.Screen.Size) - 150, 390 + 20 * i, Color.Str, Font.Str,
            "%dP (%s)", i + 1, JoypadSideName[Joypad[i].Side].c_str());
    }
    switch (JoypadNum) {
    case 0: DrawStringToHandle(20, 400, "None of Joy-Con is connected", Color.Str, Font.Str); break;
    case 1: 
        if (!Joypad[0].IsSideSet) { 
            DrawStringToHandle(real(cd.Screen.Center) - 160, imag(cd.Screen.Center) + 160, 
                "A Joy-Con is connected!\nPlease press +/- button to set it!!", Color.Str, Font.Str);
        } break;
    default: 
        if (SideSetJoypadNum < JoypadNum) { 
            DrawFormatStringToHandle(real(cd.Screen.Center) - 160, imag(cd.Screen.Center) + 160, Color.Str, Font.Str, 
                "%d  Joy-Cons are connected!\nPress +/- buttons to set them!!", JoypadNum); break;
        }
        DrawStringToHandle(real(cd.Screen.Center) - 200, imag(cd.Screen.Center) + 180,
            "You can change the order of Joy-Cons by press", Color.Str, Font.Str);
        DrawGraph(real(cd.Screen.Center) + 230, imag(cd.Screen.Center) + 180, KeyImage[cd.Key[CtrlID::No]], TRUE);
        if (HitCtrlKey(CtrlID::No) == 1) {
            SideSetJoypadNum = 0; for (int i = 0; i < JoypadNum; i++) { Joypad[i].IsSideSet = false; }
        } break;
    }
    if (SideSetJoypadNum == JoypadNum) {
        DrawGraph(real(cd.Screen.Center) - 55, imag(cd.Screen.Center) + 150, KeyImage[cd.Key[CtrlID::Start]], TRUE);
        DrawStringToHandle(real(cd.Screen.Center) - 25, imag(cd.Screen.Center) + 150, "space", Color.Str, Font.Str);
        /*DrawStringToHandle(real(cd.Screen.Center) - 110, imag(cd.Screen.Center) + 150,
            "Press                         to start...", Color.Str, Font.Str);
        if (HitCtrlKey(CtrlID::Start) == 1) { UpdateScene(SceneID::ModeSelect); return; }*/
    }
    for (int i = 0; i < JoypadNum; i += 2) {
        if (Joypad[i].Input.POV[0] != -1) {
            Mallet[i].v = polar(Mallet[i].dp, Joypad[i].Side * M_PI + M_PI * Joypad[i].Input.POV[0] / 18000.0);
            if (real(Mallet[i].p) <= Mallet[i].r) {
                Mallet[i].p._Val[0] = Mallet[i].r;
                if (real(Mallet[i].v) < 0) { Mallet[i].v._Val[0] = 0; }
            }
            else if (real(Mallet[i].p) >= real(FieldCorner[1].Field) - Mallet[i].r) {
                Mallet[i].p._Val[0] = real(FieldCorner[1].Field) - Mallet[i].r;
                if (real(Mallet[i].v) > 0) { Mallet[i].v._Val[0] = 0; }
            }
            if (imag(Mallet[i].p) >= imag(FieldCorner[1].Field) - Mallet[i].r) {
                Mallet[i].p._Val[1] = imag(FieldCorner[1].Field) - Mallet[i].r;
                if (imag(Mallet[i].v) > 0) { Mallet[i].v._Val[1] = 0; }
            }
            else if (imag(Mallet[i].p) <= imag(FieldCorner[0].Field) + Mallet[i].r) {
                Mallet[i].p._Val[1] = imag(FieldCorner[0].Field) + Mallet[i].r;
                if (imag(Mallet[i].v) < 0) { Mallet[i].v._Val[1] = 0; }
            }
        } else { Mallet[i].v = O; }
    }
    for (int i = 1; i < JoypadNum; i += 2) {
        if (Joypad[i].Input.POV[0] != -1) {
            Mallet[i].v = polar(Mallet[i].dp, Joypad[i].Side * M_PI + M_PI * Joypad[i].Input.POV[0] / 18000.0);
            if (real(Mallet[i].p) >= -Mallet[i].r) {
                Mallet[i].p._Val[0] = -Mallet[i].r;
                if (real(Mallet[i].v) > 0) { Mallet[i].v._Val[0] = 0; }
            }
            else if (real(Mallet[i].p) <= real(FieldCorner[0].Field) + Mallet[i].r) {
                Mallet[i].p._Val[0] = real(FieldCorner[0].Field) + Mallet[i].r;
                if (real(Mallet[i].v) < 0) { Mallet[i].v._Val[0] = 0; }
            }
            if (imag(Mallet[i].p) >= imag(FieldCorner[1].Field) - Mallet[i].r) {
                Mallet[i].p._Val[1] = imag(FieldCorner[1].Field) - Mallet[i].r;
                if (imag(Mallet[i].v) > 0) { Mallet[i].v._Val[1] = 0; }
            }
            else if (imag(Mallet[i].p) <= imag(FieldCorner[0].Field) + Mallet[i].r) {
                Mallet[i].p._Val[1] = imag(FieldCorner[0].Field) + Mallet[i].r;
                if (imag(Mallet[i].v) < 0) { Mallet[i].v._Val[1] = 0; }
            }
        }
        else { Mallet[i].v = O; }
    }
    DrawStringToHandle(real(cd.Screen.Center) - 30, 40, " VS ", Color.Str, Font.CharName);
    for (int i = 0; i < 2; i++) {
        DrawLineAA(real(MalletInitialPos[i].Screen), imag(FieldCorner[0].Screen), 
            real(MalletInitialPos[i].Screen), imag(FieldCorner[1].Screen), Color.Str);
        // Draw Score
        DrawFormatStringToHandle(real(cd.Screen.Center) + 30 - 100 * i, 40, Color.Char[i], Font.CharName, "%2d", Score[i]);
    }
    // Draw Title
    DrawStringToHandle(real(cd.Screen.Center) - 140, imag(cd.Screen.Center) - 30, "Speed Hockey", Color.Title, Font.Title);
    // Draw Goal
    DrawBoxAA(real(FieldCorner[0].Screen) - 2.0 * Pack.r * Magnification, imag(cd.Screen.Center) - 0.5 * ScreenGoalWide,
        real(FieldCorner[0].Screen), imag(cd.Screen.Center) + 0.5 * ScreenGoalWide, Color.Field, TRUE);
    DrawBoxAA(real(FieldCorner[0].Screen) - 2.0 * Pack.r * Magnification, imag(cd.Screen.Center) - 0.5 * ScreenGoalWide,
        real(FieldCorner[0].Screen), imag(cd.Screen.Center) + 0.5 * ScreenGoalWide, Color.Str, FALSE);
    DrawBoxAA(real(FieldCorner[1].Screen), imag(cd.Screen.Center) - 0.5 * ScreenGoalWide,
        real(FieldCorner[1].Screen) + 2.0 * Pack.r * Magnification, imag(cd.Screen.Center) + 0.5 * ScreenGoalWide, Color.Field, TRUE);
    DrawBoxAA(real(FieldCorner[1].Screen), imag(cd.Screen.Center) - 0.5 * ScreenGoalWide,
        real(FieldCorner[1].Screen) + 2.0 * Pack.r * Magnification, imag(cd.Screen.Center) + 0.5 * ScreenGoalWide, Color.Str, FALSE);
    // Draw Pack
    ScreenPack = cd.Screen.Center + Magnification * Pack.p;
    DrawCircleAA(real(ScreenPack), imag(ScreenPack), Magnification* Pack.r, 100, Color.Pack);
    DrawCircleAA(real(ScreenPack), imag(ScreenPack), Magnification* Pack.r, 100, Color.Line, FALSE);
    DrawCircleAA(real(ScreenPack), imag(ScreenPack), 0.7 * Magnification * Pack.r, 100, Color.Line, FALSE);
    complex<double> D, I(0, 1), DI; double d = 0.0, Boundary = 0.0, CollisionDepth = 0.0; bool Collision = false;
    for (int i = 0; i < JoypadNum; i++) {
        // Draw Mallet
        Mallet[i].p += Mallet[i].v;
        ScreenMallet[i] = cd.Screen.Center + Magnification * Mallet[i].p;
        DrawFormatStringToHandle(real(ScreenMallet[i]), imag(ScreenMallet[i]) - 30, Color.Char[i], Font.Str, "%dP", i + 1);
        DrawCircleAA(real(ScreenMallet[i]), imag(ScreenMallet[i]), Magnification * Mallet[i].r, 100, Color.Char[i]);
        DrawCircleAA(real(ScreenMallet[i]), imag(ScreenMallet[i]), Magnification * Mallet[i].r, 100, Color.Line, FALSE);
        DrawCircleAA(real(ScreenMallet[i]), imag(ScreenMallet[i]), 0.6 * Magnification * Mallet[i].r, 100, Color.Line, FALSE);
        // Calculate Physics of Pack
        D = Pack.p - Mallet[i].p; DI = D * I; d = abs(D);
        Boundary = Pack.r + Mallet[i].r; CollisionDepth = Boundary - d;
        if (CollisionDepth >= 0.0) {
            Collision = true;
            if (!Joypad[i].Input.Buttons[15]) {
                Pack.v = -(Pack.e * (dot(Pack.v, D) - dot(Mallet[i].v, D)) / norm(D)) * D
                    + ((dot(Pack.v, DI) - dot(Mallet[i].v, DI)) / norm(DI)) * DI + Mallet[i].v;
                Pack.p += Pack.v + (CollisionDepth / d) * D;
            }
            else { Pack.v = Mallet[i].v; Pack.p += Pack.v; }
        }
        // Draw Joypad Input State
        /*DrawFormatStringToHandle(0, 100 + 200 * i, Color.Str, Font.Str, "X:%d, Y:%d, Z:%d", Input[i].X, Input[i].Y, Input[i].Z);
        DrawFormatStringToHandle(0, 120 + 200 * i, Color.Str, Font.Str, "Rx:%d, Ry:%d, Rz:%d", Input[i].Rx, Input[i].Ry, Input[i].Rz);
        DrawFormatStringToHandle(0, 140 + 200 * i, Color.Str, Font.Str, "POV 0:%d, 1:%d, 2:%d, 3:%d",
            Input[i].POV[0], Input[i].POV[1], Input[i].POV[2], Input[i].POV[3]);
        DrawStringToHandle(0, 160 + 200 * i, "Button", Color.Str, Font.Str);
        for (int j = 0; j < 8; j++) {
            DrawStringToHandle(80 + 50 * j, 160 + 200 * i, (to_string(j) + ":" + to_string(Input[i].Buttons[j]) + ",").c_str(), Color.Str, Font.Str);
            DrawStringToHandle(80 + 50 * j, 180 + 200 * i, (to_string(j + 8) + ":" + to_string(Input[i].Buttons[j + 8]) + ",").c_str(), Color.Str, Font.Str);
            DrawStringToHandle(80 + 50 * j, 200 + 200 * i, (to_string(j + 16) + ":" + to_string(Input[i].Buttons[j + 16]) + ",").c_str(), Color.Str, Font.Str);
            DrawStringToHandle(80 + 50 * j, 220 + 200 * i, (to_string(j + 24) + ":" + to_string(Input[i].Buttons[j + 24]) + ",").c_str(), Color.Str, Font.Str);
        }*/
    }
    if (real(Pack.p) > 0.0) {
        Boundary = real(FieldCorner[1].Field) - Pack.r; CollisionDepth = real(Pack.p) - Boundary;
        if (CollisionDepth >= 0.0) {
            if (abs(imag(Pack.p)) >= 0.5 * FieldGoalWide) {
                Collision = true; Pack.p._Val[0] = Boundary - Pack.e * CollisionDepth; Pack.v._Val[0] = -Pack.e * real(Pack.v);
            }
        }
    }
    else {
        Boundary = real(FieldCorner[0].Field) + Pack.r; CollisionDepth = Boundary - real(Pack.p);
        if (CollisionDepth >= 0.0) {
            if (abs(imag(Pack.p)) >= 0.5 * FieldGoalWide) {
                Collision = true; Pack.p._Val[0] = Boundary + Pack.e * CollisionDepth; Pack.v._Val[0] = -Pack.e * real(Pack.v);
            }
        }
    }
    if (imag(Pack.p) > 0.0) {
        Boundary = imag(FieldCorner[1].Field) - Pack.r; CollisionDepth = imag(Pack.p) - Boundary;
        if (CollisionDepth >= 0.0) {
            Collision = true; Pack.p._Val[1] = Boundary - Pack.e * CollisionDepth; Pack.v._Val[1] = -Pack.e * imag(Pack.v);
        }
    }
    else {
        Boundary = imag(FieldCorner[0].Field) + Pack.r; CollisionDepth = Boundary - imag(Pack.p);
        if (CollisionDepth >= 0.0) {
            Collision = true; Pack.p._Val[1] = Boundary + Pack.e * CollisionDepth; Pack.v._Val[1] = -Pack.e * imag(Pack.v);
        }
    }
    if (!Collision) Pack.p += Pack.v; 
    if (real(Pack.p) <= real(FieldCorner[0].Field)) {
        Score[0]++; Pack.p = PackInitialPos[1]; Pack.v = O;
        for (int i = 0; i < JoypadNum; i++) { Mallet[i].p = MalletInitialPos[i % 2].Field; }
    }
    else if (real(Pack.p) >= real(FieldCorner[1].Field)) { 
        Score[1]++; Pack.p = PackInitialPos[0]; Pack.v = O; 
        for (int i = 0; i < JoypadNum; i++) { Mallet[i].p = MalletInitialPos[i % 2].Field; }
    }
    return;
}
void Hockey::ModeSelect() {
    if (HitCtrlKey(CtrlID::Back) == 1 || ClickBox(30, 0, 55, 25) == 1) { UpdateScene(SceneID::Home); return; }
    if (Mode >= -ModeID::Num && (HitCtrlKey(CtrlID::Forward) == 1 || ClickBox(55, 0, 80, 25) == 1)) Mode += ModeID::Num;
    if (ClickBox(565, 430, 640, 455) == 1 || HitCtrlKey(CtrlID::Skip) == 1) Mode += ModeID::Num;
    if (Mode == ModeID::Skip) Mode = ModeID::Battle;
    if (Mode >= ModeID::Shuttle && Mode <= ModeID::Battle) { UpdateScene(SceneID::CharSelect); return; }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            DrawBox(real(cd.Screen.Center) * i, imag(cd.Screen.Center) * j, 
                real(cd.Screen.Center) * (i + 1), imag(cd.Screen.Center) * (j + 1), Color.Char[i + 2 * j], TRUE);
            DrawGraph(real(cd.Screen.Center) * i - 40, 25 + imag(cd.Screen.Center) * j, CharImage[i + 2 * j], TRUE);
            if (i == 1 && !j) { DrawGraph(real(cd.Screen.Center) + 50, 25, CharImage[4], TRUE); }
            DrawStringToHandle(real(cd.Screen.Center) * i + 200, 80 + imag(cd.Screen.Center) * j, 
                ModeName[i + 2 * j].c_str(), Color.Str, Font.Title);
            if (ClickBox(real(cd.Screen.Center) * i, 25 + imag(cd.Screen.Center) * j, 
                real(cd.Screen.Center) * (i + 1), 25 + imag(cd.Screen.Center) * (j + 1)) == 1) {
                Mode = i + 2 * j; UpdateScene(SceneID::CharSelect); return;
            }
        }
    } 
    DrawBox(0, imag(cd.Screen.Center) - 10, real(cd.Screen.Size), imag(cd.Screen.Center) + 10, Color.Char[4], TRUE);
    DrawStringToHandle(real(cd.Screen.Center) - 100, imag(cd.Screen.Center) - 10, "Select play mode...", Color.Str, Font.Str);
    DrawStringToHandle(200, 400, "Deal with a flood of packs", Color.Str, Font.Str);
    DrawStringToHandle(real(cd.Screen.Center) + 200, 400, "Make a goal with\navoiding obstacles", Color.Str, Font.Str);
    return;
}
void Hockey::CharSelect() {
    for (int i = 0; i < CharID::Num; i++) {
        DrawBox(160 * i, 25, 160 * (i + 1), 235, Color.Char[i], TRUE); DrawGraph(160 * i - 50, 30, CharImage[i], TRUE);
        DrawStringToHandle(160 * i + 15, 200, CharName[i].c_str(), Color.Str, Font.CharName);
        if (ClickBox(160 * i, 25, 160 * (i + 1), 235) == 1) { if (CharNo != i) { ChangeBGM(CharBGM[i]); } CharNo = i; }
        if (CharNo == i) DrawGraph(160 * i, 60, Icon[IconID::Selected], TRUE);
    } DrawBox(320, 235, 480, 455, Color.Title, TRUE);
    DrawFormatStringToHandle(427, 245, c.GetColorHandle(c.BK), Font.Str, "Lv.%d", CharLv + 1);
    if (ClickBox(565, 430, 640, 455) == 1 || HitCtrlKey(CtrlID::Skip) == 1) {
        DrawBox(480, 235, 640, 455, c.GetColorHandle(c.COB), TRUE);
        if (CharNo < 0) { CharNo += CharID::Num; } if (CharNo == CharID::Skip) { CharNo = CharID::Yoichi; }
        UpdateScene(SceneID::GameStart);
    }
    else if (CharNo < 0) {
        DrawBox(480, 235, 640, 455, c.GetColorHandle(c.COB), TRUE);
        DrawStringToHandle(485, 260, "Select\n    characters...", Color.Str, Font.Str);
        if (HitCtrlKey(CtrlID::Back) == 1 || ClickBox(30, 0, 55, 25) == 1) { UpdateScene(SceneID::ModeSelect); }
        else if (CharNo >= -CharID::Num && (HitCtrlKey(CtrlID::Forward) == 1 || ClickBox(55, 0, 80, 25) == 1)) {
            CharNo += CharID::Num; ChangeBGM(CharBGM[CharNo]);
        }
    }
    else {
        DrawGraph(270, 240, CharImage[CharNo], TRUE); DrawBox(480, 235, 640, 455, c.GetColorHandle(c.COB), TRUE);
        for (int i = 0; i < 2; i++) { DrawGraph(605, 340 + 55 * i, KeyImage[cd.Key[CtrlID::Yes + i]], TRUE); }
        DrawStringToHandle(320, 245, (CharRole[CharNo]).c_str(), c.GetColorHandle(c.BK), Font.Str);
        DrawStringToHandle(485, 245, CharFeature[CharNo].c_str(), Color.Str, Font.Str);
        DrawStringToHandle(485, 360, "Game Start!!", Color.Str, Font.Str);
        DrawStringToHandle(485, 400, "Cancel...", Color.Str, Font.Str); DrawGraph(55, 0, Icon[IconID::Forward], TRUE);
        DrawStringToHandle(335, 410, CharName[CharNo].c_str(), Color.Str, Font.CharName);
        DrawFormatStringToHandle(485, 320, Color.Str, Font.Str, "Your Max Lv.%d", cd.MaxCharLv[Mode][CharNo] + 1);
        if (ClickBox(320, 235, 480, 455) == 1) { CharLv = (CharLv + 1) % (cd.MaxCharLv[Mode][CharNo] + 1); }
        if (HitCtrlKey(CtrlID::No) == 1 || HitCtrlKey(CtrlID::Back) == 1 || ClickBox(30, 0, 55, 25) == 1) {
            CharNo -= CharID::Num; ChangeBGM(cd.SceneBGM[SceneID::CharSelect]);
        }
        else if (HitCtrlKey(CtrlID::Forward) == 1 || ClickBox(55, 0, 80, 25) == 1 ||
            HitCtrlKey(CtrlID::Yes) == 1 || ClickBox(55, 0, 80, 25) == 1) {
            UpdateScene(SceneID::GameStart);
        }
    } return;
}
void Hockey::GameStart() {
    DrawBox(0, 25, 640, 455, Color.Char[CharNo], TRUE); DrawGraph(260, 40, CharImage[CharNo], TRUE);
    DrawGraph(55, 0, Icon[IconID::Forward], TRUE);
    DrawStringToHandle(100, 300, ("V.S.   " + CharName[CharNo]).c_str(), Color.Str, Font.Title);
    t.Update();
    DrawFormatStringToHandle(100, 400, Color.Str, Font.Str, "Game start in %d s without control", StartCount / 1000 - t.GetLapseTime().s);
    if (HitCtrlKey(CtrlID::Back) == 1 || ClickBox(30, 0, 55, 25) == 1) UpdateScene(SceneID::CharSelect);
    if (HitCtrlKey(CtrlID::Forward) == 1 || ClickBox(55, 0, 80, 25) == 1 ||
        HitCtrlKey(CtrlID::Skip) == 1 || ClickBox(565, 430, 640, 455) == 1 || t.GetLapseCount() >= StartCount) {
        Scene = SceneID::Game; InitScene(); Pause = 0; t.Start();// Forward, Skip or 3s passed
    } return;
}
void Hockey::InitScene() {
    switch (Scene) {
    case SceneID::Home: case SceneID::ModeSelect: Mode = ModeID::Initial;
    case SceneID::CharSelect: CharLv = 0; CharNo = CharID::Initial;
    case SceneID::GameStart: case SceneID::Game:
    case SceneID::Pause: case SceneID::Result: if (Scene != SceneID::Home) break;
    default:
        for (int i = 0; i < ModeID::Num; i++) {
            for (int j = 0; j < CharID::Num; j++) {
                cd.MaxCharLv[i][j] = 0;
            }
        }
        cd.SceneBGM[SceneID::Home] = BGMID::DeadOrAlive; cd.SceneBGM[SceneID::ModeSelect] = BGMID::Hometown;
        cd.SceneBGM[SceneID::CharSelect] = BGMID::Detect;
        cd.BGMMode = 0; for (int i = 0; i < SoundID::Num; i++) { cd.SoundVol[i] = SoundVol::Max; }
        ChangeSoundVol(SoundID::Total, cd.SoundVol[SoundID::Total]);
        cd.Key[CtrlID::Hold] = KEY_INPUT_W;    cd.Key[CtrlID::Home] = KEY_INPUT_T;
        cd.Key[CtrlID::Restart] = KEY_INPUT_R;    cd.Key[CtrlID::Init] = KEY_INPUT_I;
        cd.Key[CtrlID::Skip] = KEY_INPUT_X;    cd.Key[CtrlID::Exit] = KEY_INPUT_Q;
        cd.Key[CtrlID::Config] = KEY_INPUT_C;    cd.Key[CtrlID::ChMusic] = KEY_INPUT_M;
        cd.Key[CtrlID::ChWindow] = KEY_INPUT_F11;  cd.Key[CtrlID::Mute] = KEY_INPUT_ESCAPE;
        cd.Key[CtrlID::Pause] = KEY_INPUT_E;
        cd.Key[CtrlID::Back] = KEY_INPUT_U;    cd.Key[CtrlID::Forward] = KEY_INPUT_O;
        cd.Key[CtrlID::Up] = KEY_INPUT_4;    cd.Key[CtrlID::Down] = KEY_INPUT_3;
        for (int i = 0; i < CtrlID::Num; i++) { cd.TwinKey[i] = SINGLE; } break;
    } UpdateScene(Scene); return;
}
void Hockey::Game() {
    DrawBox(0, 25, 640, 455, Color.Char[CharNo], TRUE); DrawGraph(-70, 60, CharImage[CharNo], TRUE);
    DrawBox(110, 30, 160, 135, Color.Frame, TRUE); DrawBox(160, 30, 635, 450, Color.Frame, TRUE);
}
int Hockey::DrawScene() {
    switch (Scene) {
    case SceneID::Home: Home(); break;
    case SceneID::ModeSelect: ModeSelect(); break;
    case SceneID::CharSelect: CharSelect(); break;
    case SceneID::GameStart: GameStart(); break;
    case SceneID::Game: case SceneID::Pause: case SceneID::Result: Game(); break;
    default: Config(); break;
    } 
    DrawBox(0, 0, real(cd.Screen.Size), 25, Color.Frame, TRUE);
    DrawBox(0, imag(cd.Screen.Size) - 25, real(cd.Screen.Size), imag(cd.Screen.Size), Color.Frame, TRUE);
    if (Scene > SceneID::Home && Scene < SceneID::Game) { DrawGraph(565, 430, Icon[IconID::Skip], TRUE); }
    switch (Scene) {
    case SceneID::GameConfig: case SceneID::PauseConfig: case SceneID::ResultConfig:
    case SceneID::Game: case SceneID::Pause: case SceneID::Result:
        DrawGraph(490, 0, Icon[IconID::Ghost], TRUE); DrawGraph(515, 0, Icon[IconID::Skill], TRUE);
        DrawGraph(540, 0, Icon[IconID::Pause + Pause], TRUE);
    case SceneID::ModeSelectConfig: case SceneID::CharSelectConfig: case SceneID::GameStartConfig:
    case SceneID::CharSelect: case SceneID::GameStart:
        DrawGraph(0, imag(cd.Screen.Size) - 25, Icon[IconID::Restart], TRUE);
    default:
        DrawGraph(0, 0, Icon[IconID::Home], TRUE); DrawGraph(25, imag(cd.Screen.Size) - 25, Icon[IconID::Init], TRUE);
        DrawGraph(590, 0, Icon[IconID::AnotherWindow + WindowMode], TRUE); DrawGraph(85, 0, Icon[IconID::Mute + Mute], TRUE);
        DrawGraph(615, 0, Icon[IconID::Exit], TRUE); DrawGraph(30, 0, Icon[IconID::Back], TRUE);
        DrawGraph(565, 0, Icon[IconID::Config], TRUE); DrawGraph(50, imag(cd.Screen.Size) - 25, Icon[IconID::Music], TRUE);
        DrawStringToHandle(75, imag(cd.Screen.Size) - 20, BGMName[PlayingBGM].c_str(), Color.Str, Font.Str);
        // Draw Product's Name
        DrawCircle(real(cd.Screen.Size) - 107, imag(cd.Screen.Size) - 11, 10, Color.Str, FALSE);
        DrawStringToHandle(real(cd.Screen.Size) - 250, imag(cd.Screen.Size) - 20, "Speed Hockey  C  2024 mini", Color.Str, Font.Str); break;
    }
    switch (Scene) {
    case SceneID::Home:      DrawStringToHandle(115, 5, "Home", Color.Str, Font.Str); break;
    case SceneID::ModeSelect:
        if (Mode >= -ModeID::Num) { DrawGraph(55, 0, Icon[CtrlID::Forward], TRUE); }
        DrawStringToHandle(115, 5, "Mode Select", Color.Str, Font.Str); break;
    case SceneID::CharSelect:
        if (CharNo >= -CharID::Num) { DrawGraph(55, 0, Icon[IconID::Forward], TRUE); }
        DrawStringToHandle(115, 5, (ModeName[Mode] + " >> Charcter Select").c_str(), Color.Str, Font.Str); break;
    case SceneID::GameStart:
        DrawStringToHandle(115, 5, (ModeName[Mode] + " >> " + CharName[CharNo] + " >> Game Start").c_str(), Color.Str, Font.Str); break;
    case SceneID::Game: case SceneID::Pause: case SceneID::Result:
        DrawStringToHandle(115, 5, ModeName[Mode].c_str(), Color.Str, Font.Str);                           break;
    default:              DrawStringToHandle(115, 5, "Config", Color.Str, Font.Str);                                         break;                                                                 break;
    }
    DrawCursor(); return Scene;
}
void Hockey::UpdateScene(int TargetScene) {
    switch (TargetScene) {
    case SceneID::Home: 
        for (int i = 0; i < 2; i++) { Score[i] = 0; }
        if (Scene) ChangeBGM(cd.SceneBGM[SceneID::Home]); break;
    case SceneID::ModeSelect:
        if (Scene != SceneID::ModeSelect) ChangeBGM(cd.SceneBGM[SceneID::ModeSelect]); if (Mode >= 0) Mode -= ModeID::Num; break;
    case SceneID::CharSelect: if (CharNo >= 0 && Scene >= 0 && Scene != SceneID::GameStart) { CharNo -= CharID::Num; CharLv = 0; }
                            if (Scene > SceneID::CharSelect) ChangeBGM(CharBGM[CharNo]);
                            else if (Scene < SceneID::CharSelect) ChangeBGM(cd.SceneBGM[SceneID::CharSelect]); break;
    case SceneID::GameStart: t.Start(); if (Scene != SceneID::GameStart) ChangeBGM(CharBGM[CharNo]); break;
    case SceneID::Game: if (Scene < 0) { ChangeBGM(CharBGM[CharNo]); t.Resume(); break; }
                      break;
    case SceneID::Pause: Pause = 1; if (Scene < 0) ChangeBGM(CharBGM[CharNo]); break;
    case SceneID::Result: if (cd.MaxCharLv[Mode][CharNo] >= 9) cd.MaxCharLv[Mode][CharNo] = 9; ChangeBGM(8); t.Stop(); break;
    default: if (Scene >= 0) ChangeBGM(7); break;
    } Scene = TargetScene; return;
}
void Hockey::BackSystem() {
    Keyboard::GetInstance()->Update();
    switch (Scene) {
    case SceneID::Game:
        t.Update();
    case SceneID::Pause:
        if (ClickBox(540, 0, 565, 25) == 1 || HitCtrlKey(CtrlID::Pause) == 1) {
            if (!Pause) { UpdateScene(SceneID::Pause); t.Stop(); return; } Pause = 0; t.Resume(); Scene = SceneID::Game; return;
        }
    case SceneID::Result:
        if (ClickBox(30, 0, 55, 25) == 1 || HitCtrlKey(CtrlID::Back) == 1) { UpdateScene(SceneID::GameStart); return; }
    case SceneID::ModeSelectConfig: case SceneID::CharSelectConfig: case SceneID::GameStartConfig: case SceneID::GameConfig:
    case SceneID::PauseConfig: case SceneID::ResultConfig: case SceneID::CharSelect: case SceneID::GameStart:
        if (ClickBox(0, 455, 25, 480) == 1 || HitCtrlKey(CtrlID::Restart) == 1) { UpdateScene(SceneID::ModeSelect); return; }
    default:
        if (ClickBox(25, 455, 50, 480) == 1 || HitCtrlKey(CtrlID::Init) == 1) InitScene();
        else if (ClickBox(0, 0, 28, 25) == 1 || HitCtrlKey(CtrlID::Home) == 1) UpdateScene(SceneID::Home);
        else if (ClickBox(50, 455, 75, 480) == 1 || HitCtrlKey(CtrlID::ChMusic) == 1) ChangeBGM((PlayingBGM + 1) % BGMID::Num);
        else if (ClickBox(565, 0, 590, 25) == 1 || HitCtrlKey(CtrlID::Config) == 1) {
            if (Scene == SceneID::Game) { Pause = 1; Scene = SceneID::Pause; t.Stop(); }
            if (Scene >= 0) { UpdateScene(Scene - SceneID::Num); }
            else { UpdateScene(Scene + SceneID::Num); }
        }
        else if (ClickBox(590, 0, 615, 25) == 1 || HitCtrlKey(CtrlID::ChWindow) == 1) ChangeWindow((WindowMode + 1) % 2);
        else if (ClickBox(85, 0, 110, 25) == 1 || HitCtrlKey(CtrlID::Mute) == 1) ChangeSound((Mute + 1) % 2);
        else if (HitCtrlKey(CtrlID::Down) > 10) ChangeSoundVol(SoundID::Total, cd.SoundVol[SoundID::Total] - 1);
        else if (HitCtrlKey(CtrlID::Up) > 10) ChangeSoundVol(SoundID::Total, cd.SoundVol[SoundID::Total] + 1);
        else if (HitCtrlKey(CtrlID::Down) == 1) ChangeSoundVol(SoundID::Total, cd.SoundVol[SoundID::Total] - 4);
        else if (HitCtrlKey(CtrlID::Up) == 1) ChangeSoundVol(SoundID::Total, cd.SoundVol[SoundID::Total] + 4);
        else if (Mute && !CheckSoundMem(BGM[PlayingBGM])) {
            switch (cd.BGMMode) {
            case BGMModeID::Ascending: ChangeBGM((PlayingBGM + 1) % BGMID::Num); return;
            case BGMModeID::Descending: ChangeBGM((PlayingBGM + BGMID::Num - 1) % BGMID::Num); return;
            case BGMModeID::Random: ChangeBGM(GetRand(BGMID::Num - 1));
            default: return;
            }
        }
    } return;
}
int Hockey::DrawCursor() {
    Mouse::GetInstance()->Update(); 
    Cursor.Screen._Val[0] = Mouse::GetInstance()->getX(); Cursor.Screen._Val[1] = Mouse::GetInstance()->getY();
    Cursor.Field = 5e-3 * (Cursor.Screen - cd.Screen.Center);
    return DrawGraph(real(Cursor.Screen) - 15, imag(Cursor.Screen) - 15, Icon[IconID::Cursor], TRUE);
}
Hockey::~Hockey() {
    FILE* ocdf = fopen(DataFileName, "wb+"); fwrite(&cd, sizeof(cd), 1, ocdf); fclose(ocdf);
    PlaySound(NULL, 0, 0); InitSoundMem(); DxLib_End();
}

