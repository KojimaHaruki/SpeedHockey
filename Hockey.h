#pragma once
// Control No
#define KeyNum              67
#define MaxKeyNum          212
// TwinKey No
#define SINGLE               0
#define SHIFT                1  
#define CTRL                 2  
#define ALT                  3
#include "JISColor.h"
#include "Timer.h"
#include <iostream>
#include <string>
#include <complex>
using namespace std;
struct SceneID {
    static constexpr int Home       = 0; static constexpr int HomeConfig       = -7;
    static constexpr int ModeSelect = 1; static constexpr int ModeSelectConfig = -6;
    static constexpr int CharSelect = 2; static constexpr int CharSelectConfig = -5;
    static constexpr int GameStart  = 3; static constexpr int GameStartConfig  = -4;
    static constexpr int Game       = 4; static constexpr int GameConfig       = -3;
    static constexpr int Pause      = 5; static constexpr int PauseConfig      = -2;
    static constexpr int Result     = 6; static constexpr int ResultConfig     = -1;
    static constexpr int Num        = 7;
};
struct CtrlID {
    static constexpr int QuickMove =  0; static constexpr int Hold      =  1; 
    static constexpr int Home      =  2; static constexpr int Restart   =  3; 
    static constexpr int Init      =  4; static constexpr int Skip      =  5; 
    static constexpr int Exit      =  6; static constexpr int Config    =  7; 
    static constexpr int ChMusic   =  8; static constexpr int ChWindow  =  9; 
    static constexpr int Mute      = 10; static constexpr int Pause     = 11; 
    static constexpr int Start     = 12; static constexpr int Yes       = 13; 
    static constexpr int No        = 14; static constexpr int Back      = 15; 
    static constexpr int Forward   = 16; static constexpr int Down      = 17; 
    static constexpr int Up        = 18; static constexpr int Num       = 19;
};
class Hockey {
public:
    Hockey();
    ~Hockey();
    void BackSystem();
    int DrawScene();
    int ClickBox(int x1, int y1, int x2, int y2);
    int HitCtrlKey(int KeyRole);
private:
    void ChangeWindow(int WindowModeFlag);
    void ChangeKey(int x1, int y1, int x2, int y2, int KeyRole);
    int HitKey(int keyCode);
    void ChangeSound(int SoundModeFlag);
    void ChangeSoundVol(int SoundNo, int Vol);
    void ChangeBGM(int TargetBGM);
    // Icon
    struct IconID {
        static constexpr int Left          =  0; static constexpr int Right          =  1;
        static constexpr int CCWSpin       =  2; static constexpr int CWSpin         =  3;
        static constexpr int SoftDrop      =  4; static constexpr int HardDrop       =  5;
        static constexpr int Ghost         =  6; static constexpr int Skill          =  7;
        static constexpr int Hold          =  8; static constexpr int Home           =  9;
        static constexpr int Restart       = 10; static constexpr int Init           = 11;
        static constexpr int Skip          = 12; static constexpr int Exit           = 13;
        static constexpr int Config        = 14; static constexpr int Music          = 15;
        static constexpr int AnotherWindow = 16; static constexpr int MaximizeWindow = 17; 
        static constexpr int Mute          = 18; static constexpr int Unmute         = 19;
        static constexpr int Pause         = 20; static constexpr int Start          = 21;
        static constexpr int Back          = 22; static constexpr int Forward        = 23;
        static constexpr int Down          = 24; static constexpr int Up             = 25;
        static constexpr int Selected      = 26; static constexpr int Cursor         = 27;
        static constexpr int Num           = 28;
    };
    int Icon[IconID::Num]; 
    // Controls
    const string CtrlName[10] = { "Quick Move", "Hold", "Home", "Restart", "Initialize", "           Skip", 
        "Quit Game", "Config", "Change BGM", "Change Window" };
    const string BoardCtrlName[4] = {  };
    bool Drag;
    // Scene
    int Scene; const string SceneName[3] = { "Home", "Mode Select", "Char Select" };
    // Sounds
    struct SoundID {
        static constexpr int Total = 0;
        static constexpr int BGM = 1;
        static constexpr int SE = 2;
        static constexpr int Num = 3;
    };
    struct SoundVol {
        static constexpr int Min = 0; static constexpr int Max = 200;
    };
    const string SoundName[SoundID::Num] = { "Volume", " BGM", " SE" };
    // Mute
    struct MuteID {
        static constexpr int Mute = 0; static constexpr int Unmute = 1;
        static constexpr int Num = 2;
    };
    int Mute; const string MuteName[MuteID::Num] = { "Mute", "Unmute" };
    // BGM
    struct BGMID {
        static constexpr int DeadOrAlive = 0;
        static constexpr int Chocolate = 1;
        static constexpr int Detect = 2;
        static constexpr int Hometown = 3;
        static constexpr int MemoryOfLegend = 4;
        static constexpr int NothingDies = 5;
        static constexpr int Sorrow = 6;
        static constexpr int VictoryBelongsToYou = 7;
        static constexpr int Num = 8;
    };
    int BGM[BGMID::Num], PlayingBGM, BGMVol;
    const string BGMName[BGMID::Num] = { "Dead or Alive", "Chocolate", "Detect", "Hometown", "Memory of Legend",
                          "Nothing dies in my mind", "Sorrow", "Victory belongs to You" };
    // Playing BGM Mode
    struct BGMModeID {
        static constexpr int Loop = 0;
        static constexpr int Ascending = 1;
        static constexpr int Descending = 2;
        static constexpr int Random = 3;
        static constexpr int Num = 4;
    };
    const string BGMModeName[BGMModeID::Num] = { "Loop", "Ascending order", "Descending order", "Random" };
    // Sound Effects
    struct SEID {
        static constexpr int Num = 4;
        static constexpr int ComboNum = 7;
    };
    int SE[SEID::Num], SEVol;
    // X-Y coordinate  X:Å®  Y:Å´ 
    struct CoordinateSystem {
        complex<double> Field, Screen;
    }; CoordinateSystem Cursor, FieldSize, FieldCorner[2], MalletInitialPos[2];
    static constexpr double Magnification = 200.0;
    
    double FieldGoalWide, ScreenGoalWide;
    // Window
    int WindowMode;
    struct CharID {
        static constexpr int Initial = -11; static constexpr int Skip          = -6;
        static constexpr int John    =   0; static constexpr int BeforeJohn    = -5;
        static constexpr int Komachi =   1; static constexpr int BeforeKomachi = -4; 
        static constexpr int Misuzu  =   2; static constexpr int BeforeMisuzu  = -3;
        static constexpr int Michel  =   3; static constexpr int BeforeMichel  = -2;
        static constexpr int Yoichi  =   4; static constexpr int BeforeYoichi  = -1;
        static constexpr int Num     =   5;
    };
    int CharNo, CharLv, CharImage[CharID::Num];
    const int CharBGM[CharID::Num] = { BGMID::DeadOrAlive, BGMID::NothingDies, BGMID::Chocolate, BGMID::Hometown, BGMID::MemoryOfLegend };
    const string CharName[CharID::Num] = { "  John", "Komachi", " Misuzu", " Michel", " Yoichi" };
    const string CharRole[CharID::Num] = { "Racer", "Model", "Girl", "Witch", "Master" };
    const string CharFeature[CharID::Num] = { "He's fast!!\nExcept that,\nnormal...", "She bother you,\nso you can't\nsee next block!?",
                                    "Holded by her,\nyou can't hold!?", "She recites spell\nto add block!!", "He can use\nall skills!!" }; 
    // Joypad
    static constexpr int MaxJoypadNum = 4;
    int JoypadNum, SideSetJoypadNum, JoypadOrder[MaxJoypadNum];
    struct JoypadSideID {
        static constexpr int Left = 0; static constexpr int Right = 1;
        static constexpr int Num = 2;
    };
    struct JoypadData {
        int No, Side;
        bool IsSideSet;
        DINPUT_JOYSTATE Input;
    }; JoypadData Joypad[MaxJoypadNum], Player[MaxJoypadNum];
    const string JoypadSideName[JoypadSideID::Num] = { "L", "R" };
    struct PhysicalAmount {
        double m = 0.02, r = 0.0575, e = 0.8, dp = 0.0;
        complex<double> p, v;
    }; PhysicalAmount Pack, Mallet[MaxJoypadNum];
    complex<double> ScreenPack, ScreenMallet[MaxJoypadNum], PackInitialPos[MaxJoypadNum];
    const complex<double> O;
    int Score[2];
    // PlayMode
    struct ModeID {
        static constexpr int Initial  = -9; static constexpr int Skip = -5;
        static constexpr int Shuttle  =  0; static constexpr int BeforeShuttle  = -4;
        static constexpr int Marathon =  1; static constexpr int BeforeMarathon = -3;
        static constexpr int Sprint   =  2; static constexpr int BeforeSprint   = -2;
        static constexpr int Battle   =  3; static constexpr int BeforeBattle   = -1;
        static constexpr int Num      =  4;
    };
    int Mode; const string ModeName[ModeID::Num] = { "Singles", "Doubles", "Flood", "Trick Shot" };
    // Progress of Game
    struct PauseID {
        static constexpr int Resume = 0; static constexpr int Pause = 1;
        static constexpr int Num    = 2;
    };
    struct ScreenData {
        complex<double> Center, Size;
    }; 
    int Pause; const string PauseName[PauseID::Num] = { "Pause", "Resume" };
    Timer t; static constexpr int StartCount = 3000;
    // Key
    const int KeyNo[KeyNum] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 28, 29, 30, 31, 32, 
                                33, 34, 35, 36, 37, 38, 42, 44, 45, 46, 47, 48, 49, 50, 54, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 
                                67, 68, 87, 88, 157, 184, 197, 200, 203, 205, 208, 210, 211 };
    int KeyImage[MaxKeyNum]; 
    JISColor c;
    struct ColorData {
        unsigned int Title, Str, Line, Frame, HP, Field, Pack, Char[CharID::Num];
    }; ColorData Color;
    struct FontData {
        int Str, Title, CharName;
    }; FontData Font;
    struct ConfigData {
        ScreenData Screen;
        int SoundVol[SoundID::Num];          // SoundVol[SoundNo] SoundNo 0: Total, 1: BGM, 2: SE
        int BGMMode;                    // BGMStyle 0: Loop, 1: Ascending order, 2: Decending order, 3: Random" 
        int Key[CtrlID::Num];                // Key[KeyRole]
        int TwinKey[CtrlID::Num];            // TwinKey[KeyRole]
        int SceneBGM[3];                 // SceneBGM[SceneNo]
        int MaxCharLv[ModeID::Num][CharID::Num]; // MaxCharLv[Mode][CharNo]
        double Sensitivity[2];
    }; ConfigData cd; 
    const char* DataFileName = "ConfigData.dat";
    // Function of each part
    void Config();
    void Home();
    void ModeSelect();
    void CharSelect();
    void GameStart();
    void InitScene();
    void Game();
    void UpdateScene(int TargetScene);
    int DrawCursor();
    double dot(complex<double> a, complex<double> b) { return real(a) * real(b) + imag(a) * imag(b); }
};
