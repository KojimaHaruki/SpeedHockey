#pragma once
class JISColor {
private:
    static constexpr int MainColorNum = 13;
    static constexpr int MajorColorNum = 47;
    static constexpr int ColorNum = 117;
    struct rgbData {
        int r; // Red
        int g; // Green
        int b; // Blue
        unsigned int c; // Color Handle
        bool available; // Color Handle availability
    }; rgbData rgb[ColorNum];
    int MakeColorHandle(int ColorHandleNum);
public:
    JISColor();
    ~JISColor();
    unsigned int CreateColorToHandle(int ColorNo);
    int MakeMainColorHandle() { return MakeColorHandle(MainColorNum); };
    int MakeMajorColorHandle() { return MakeColorHandle(MajorColorNum); };
    int MakeAllColorHandle() { return MakeColorHandle(ColorNum); };
    unsigned int GetColorHandle(int ColorNo);
    // Main Color
    static constexpr int SnowWhite       =   0; static constexpr int White     =  0; static constexpr int W   =  0; // White 
    static constexpr int Grey            =   1; static constexpr int Gray      =  1; static constexpr int GY  =  1; // Grey/Gray
    static constexpr int LampBlack       =   2; static constexpr int Black     =  2; static constexpr int BK  =  2; // Black
    static constexpr int Brown           =   3;                                      static constexpr int BW  =  3; // Brown
    static constexpr int JauneBrillant   =   4; static constexpr int Yellow    =  4; static constexpr int Y   =  4; // Yellow
    static constexpr int Orange          =   5;                                      static constexpr int OR  =  5; // Orange
    static constexpr int Green           =   6;                                      static constexpr int G   =  6; // Green
    static constexpr int Cyan            =   7;                                      static constexpr int C   =  7; // Cyan
    static constexpr int Blue            =   8;                                      static constexpr int B   =  8; // Blue
    static constexpr int Purple          =   9;                                      static constexpr int PP  =  9; // Purple
    static constexpr int Magenta         =  10;                                      static constexpr int M   = 10; // Magenta
    static constexpr int Pink            =  11;                                      static constexpr int PK  = 11; // Pink
    static constexpr int TomatoRed       =  12; static constexpr int Red       = 12; static constexpr int R   = 12; // Red
    // Major Color
    static constexpr int AshGrey         =  13;                                      static constexpr int ASH = 13; // Ash Grey
    static constexpr int Vermillion      =  14; static constexpr int Vermilion = 14; static constexpr int VRM = 14; // Vermilion
    static constexpr int Scarlet         =  15;                                      static constexpr int SC  = 15; // Scarlet
    static constexpr int BurntSienna     =  16;                                      static constexpr int BS  = 16; // Burnt Sienna
    static constexpr int Ivory           =  17;                                      static constexpr int IV  = 17; // Ivory
    static constexpr int RawSienna       =  18;                                      static constexpr int RS  = 18; // Raw Sienna
    static constexpr int Tan             =  19;                                      static constexpr int TN  = 19; // Tan
    static constexpr int Beige           =  20;                                      static constexpr int BG  = 20; // Beige
    static constexpr int GoldenYellow    =  21;                                      static constexpr int GLY = 21; // Golden Yellow
    static constexpr int Sepia           =  22;                                      static constexpr int SP  = 22; // Sepia
    static constexpr int MandarinOrange  =  23;                                      static constexpr int MO  = 23; // Mandarin Orange
    static constexpr int Bronze          =  24;                                      static constexpr int BZ  = 24; // Bronze
    static constexpr int BurntUmber      =  25;                                      static constexpr int BU  = 25; // Burnt Umber
    static constexpr int YellowOcher     =  26;                                      static constexpr int YO  = 26; // Yellow Ocher
    static constexpr int Khaki           =  27;                                      static constexpr int KK  = 27; // Khaki
    static constexpr int Blond           =  28; static constexpr int Blonde    = 28; static constexpr int BL  = 28; // Blond
    static constexpr int RawUmber        =  29;                                      static constexpr int RU  = 29; // Raw Umber
    static constexpr int CanaryYellow    =  30;                                      static constexpr int CRY = 30; // CanaryYellow
    static constexpr int Olive           =  31;                                      static constexpr int OL  = 31; // Olive
    static constexpr int LemonYellow     =  32;                                      static constexpr int LY  = 32; // Lemon Yellow
    static constexpr int EmeraldGreen    =  33;                                      static constexpr int EG  = 33; // Emerald Green
    static constexpr int Viridian        =  34;                                      static constexpr int VRD = 34; // Viridian
    static constexpr int MarineBlue      =  35;                                      static constexpr int MB  = 35; // Marine Blue
    static constexpr int TurquoiseBlue   =  36;                                      static constexpr int TQ  = 36; // Turquoise Blue
    static constexpr int CeruleanBlue    =  37;                                      static constexpr int CRB = 37; // Cerulean Blue
    static constexpr int MidnightBlue    =  38;                                      static constexpr int MID = 38; // Midnight Blue
    static constexpr int CobaltBlue      =  39;                                      static constexpr int COB = 39; // Cobalt Blue
    static constexpr int NavyBlue        =  40;                                      static constexpr int NV  = 40; // Navy Blue
    static constexpr int UltramarineBlue =  41;                                      static constexpr int UB  = 41; // Ultramarine Blue
    static constexpr int Violet          =  42;                                      static constexpr int VIO = 42; // Violet
    static constexpr int Lavender        =  43;                                      static constexpr int LV  = 43; // Lavender
    static constexpr int WineRed         =  44;                                      static constexpr int WR  = 44; // Wine Red
    static constexpr int Carmine         =  45;                                      static constexpr int CM  = 45; // Carmine
    static constexpr int Maroon          =  46;                                      static constexpr int MR  = 46; // Maroon
    // Minor Color
    static constexpr int PearlGray       =  47;
    static constexpr int SilverGray      =  48;
    static constexpr int Terracotta      =  49; 
    static constexpr int SalmonPink      =  50; 
    static constexpr int ShellPink       =  51;
    static constexpr int NailPink        =  52; 
    static constexpr int Chocolate       =  53;
    static constexpr int CarrotOrange    =  54; 
    static constexpr int ChineseRed      =  55; 
    static constexpr int Peach           =  56;
    static constexpr int CocoaBrown      =  57;
    static constexpr int Apricot         =  58;
    static constexpr int EcruBeige       =  59;
    static constexpr int Cork            =  60;
    static constexpr int Buff            =  61;
    static constexpr int Amber           =  62;
    static constexpr int Leghorn         =  63;
    static constexpr int Marigold        =  64;
    static constexpr int NaplesYellow    =  65;
    static constexpr int CreamYellow     =  66;
    static constexpr int ChromeYellow    =  67;
    static constexpr int OliveDrab       =  68;
    static constexpr int OliveGreen      =  69;
    static constexpr int GrassGreen      =  70;
    static constexpr int ChartreuseGreen =  71;
    static constexpr int LeafGreen       =  72;
    static constexpr int SeaGreen        =  73;
    static constexpr int IvyGreen        =  74;
    static constexpr int AppleGreen      =  75;
    static constexpr int MintGreen       =  76;
    static constexpr int CobaltGreen     =  77;
    static constexpr int BottleGreen     =  78;
    static constexpr int ForestGreen     =  79;
    static constexpr int MalachiteGreen  =  80;
    static constexpr int BilliardGreen   =  81;
    static constexpr int PeacockGreen    =  82;
    static constexpr int PeacockBlue     =  83;
    static constexpr int NileBlue        =  84;
    static constexpr int HorizonBlue     =  85;
    static constexpr int SkyBlue         =  86;
    static constexpr int SaxBlue         =  87;
    static constexpr int BabyBlue        =  88;
    static constexpr int IronBlue        =  89; static constexpr int PrussianBlue = 89;
    static constexpr int Hyacinth        =  90;
    static constexpr int OrientalBlue    =  91;
    static constexpr int SkyGray         =  92;
    static constexpr int Wistaria        =  93;
    static constexpr int SlateGray       =  94;
    static constexpr int Pansy           =  95;
    static constexpr int Heliotrope      =  96;
    static constexpr int Mauve           =  97;
    static constexpr int CharcoalGray    =  98;
    static constexpr int SteelGray       =  99;
    static constexpr int Lilac           = 100;
    static constexpr int Orchid          = 101;
    static constexpr int CherryPink      = 102;
    static constexpr int RoseRed         = 103;
    static constexpr int RubyRed         = 104;
    static constexpr int RoseGray        = 105;
    static constexpr int CochinealRed    = 106;
    static constexpr int Strawberry      = 107;
    static constexpr int Burgundy        = 108;
    static constexpr int RosePink        = 109;
    static constexpr int Rose            = 110;
    static constexpr int OldRose         = 111;
    static constexpr int CoralRed        = 112;
    static constexpr int Bordeaux        = 113;
    static constexpr int BabyPink        = 114;
    static constexpr int SignalRed       = 115;
    static constexpr int PoppyRed        = 116;
};

//White(GetColor(241, 241, 241)), SnowWhite(White), W(White),
//PearlGray(GetColor(173, 173, 173)),
//SilverGray(GetColor(160, 160, 160)),
//AshGrey(GetColor(147, 147, 147)),
//Grey(GetColor(121, 121, 121)), Gray(Grey), GY(Grey),
//LampBlack(GetColor(28, 28, 28)), Black(LampBlack), BK(Black),
//Vermilion(GetColor(237, 81, 78)), Vermillion(Vermilion),
//Scarlet(GetColor(222, 67, 53)),
//Terracotta(GetColor(172, 86, 71)),
//SalmonPink(GetColor(255, 165, 148)),
//ShellPink(GetColor(251, 204, 195)),
//NailPink(GetColor(241, 190, 177)),
//Chocolate(GetColor(82, 55, 47)),
//BurntSienna(GetColor(168, 89, 60)),
//CarrotOrange(GetColor(204, 87, 44)),
//ChineseRed(GetColor(255, 93, 32)),
//Peach(GetColor(235, 192, 175)),
//CocoaBrown(GetColor(117, 76, 56)),
//Ivory(GetColor(227, 212, 202)),
//RawSienna(GetColor(187, 100, 33)),
//Brown(GetColor(115, 78, 49)), BW(Brown),
//Apricot(GetColor(221, 162, 115)),
//EcruBeige(GetColor(248, 207, 174)),
//Tan(GetColor(165, 111, 63)),
//Cork(GetColor(165, 129, 97)),
//Beige(GetColor(193, 171, 150)),
//Orange(GetColor(253, 126, 0)), OR(Orange),
//Buff(GetColor(197, 153, 109)),
//GoldenYellow(GetColor(243, 154, 56)),
//Amber(GetColor(179, 125, 64)),
//Sepia(GetColor(74, 59, 42)),
//MandarinOrange(GetColor(253, 149, 30)),
//Bronze(GetColor(129, 90, 43)),
//BurntUmber(GetColor(91, 70, 42)),
//YellowOcher(GetColor(193, 138, 57)),
//Leghorn(GetColor(225, 197, 155)),
//Khaki(GetColor(154, 117, 58)),
//Blond(GetColor(227, 180, 102)), Blonde(Blond),
//Marigold(GetColor(255, 160, 0)),
//NaplesYellow(GetColor(242, 194, 107)),
//CreamYellow(GetColor(232, 213, 175)),
//RawUmber(GetColor(127, 92, 19)),
//ChromeYellow(GetColor(255, 188, 0)),
//OliveDrab(GetColor(104, 98, 78)),
//CanaryYellow(GetColor(247, 213, 78)),
//Yellow(GetColor(255, 204, 0)), Y(Yellow), JauneBrillant(Yellow),
//Olive(GetColor(96, 86, 39)),
//LemonYellow(GetColor(232, 200, 0)),
//OliveGreen(GetColor(86, 88, 56)),
//GrassGreen(GetColor(115, 124, 62)),
//ChartreuseGreen(GetColor(189, 212, 88)),
//LeafGreen(GetColor(135, 157, 78)),
//SeaGreen(GetColor(154, 185, 97)),
//IvyGreen(GetColor(81, 106, 57)),
//AppleGreen(GetColor(176, 211, 168)),
//MintGreen(GetColor(129, 204, 145)),
//Green(GetColor(42, 155, 80)), G(Green),
//CobaltGreen(GetColor(93, 194, 136)),
//EmeraldGreen(GetColor(77, 165, 115)),
//BottleGreen(GetColor(38, 68, 53)),
//ForestGreen(GetColor(62, 121, 92)),
//MalachiteGreen(GetColor(0, 128, 71)),
//Viridian(GetColor(21, 111, 92)),
//BilliardGreen(GetColor(0, 72, 64)),
//PeacockGreen(GetColor(0, 127, 145)),
//PeacockBlue(GetColor(0, 112, 139)),
//NileBlue(GetColor(81, 144, 164)),
//MarineBlue(GetColor(0, 81, 117)),
//TurquoiseBlue(GetColor(57, 158, 204)),
//Cyan(GetColor(33, 157, 221)), C(Cyan),
//CeruleanBlue(GetColor(11, 116, 175)),
//HorizonBlue(GetColor(145, 178, 210)),
//SkyBlue(GetColor(149, 192, 236)),
//MidnightBlue(GetColor(0, 21, 45)),
//Blue(GetColor(49, 112, 185)), B(Blue),
//CobaltBlue(GetColor(40, 99, 171)),
//SaxBlue(GetColor(98, 125, 161)),
//BabyBlue(GetColor(171, 189, 218)),
//IronBlue(GetColor(61, 73, 107)), PrussianBlue(IronBlue),
//Hyacinth(GetColor(117, 134, 187)),
//NavyBlue(GetColor(51, 60, 94)),
//UltramarineBlue(GetColor(65, 79, 163)),
//OrientalBlue(GetColor(55, 67, 143)),
//SkyGray(GetColor(186, 186, 198)),
//Wistaria(GetColor(119, 110, 210)),
//SlateGray(GetColor(86, 85, 94)),
//Pansy(GetColor(64, 49, 126)),
//Heliotrope(GetColor(131, 109, 197)),
//Violet(GetColor(109, 82, 171)),
//Mauve(GetColor(131, 95, 168)),
//Lavender(GetColor(158, 142, 174)),
//CharcoalGray(GetColor(78, 72, 84)),
//SteelGray(GetColor(115, 108, 121)),
//Lilac(GetColor(194, 162, 218)),
//Purple(GetColor(162, 96, 191)), PL(Purple),
//Orchid(GetColor(199, 161, 215)),
//Magenta(GetColor(201, 73, 162)), M(Magenta),
//CherryPink(GetColor(207, 97, 165)),
//RoseRed(GetColor(199, 79, 144)),
//RubyRed(GetColor(185, 30, 104)),
//RoseGray(GetColor(147, 132, 139)),
//CochinealRed(GetColor(175, 49, 104)),
//WineRed(GetColor(131, 39, 78)),
//Strawberry(GetColor(187, 30, 94)),
//Burgundy(GetColor(69, 42, 53)),
//RosePink(GetColor(239, 147, 182)),
//Rose(GetColor(217, 65, 119)),
//OldRose(GetColor(201, 127, 150)),
//CoralRed(GetColor(255, 135, 160)),
//Pink(GetColor(235, 151, 168)), PK(Pink),
//Carmine(GetColor(190, 30, 62)),
//Bordeaux(GetColor(85, 53, 59)),
//BabyPink(GetColor(255, 201, 210)),
//SignalRed(GetColor(206, 49, 74)),
//PoppyRed(GetColor(221, 65, 87)),
//Red(GetColor(222, 66, 76)), TomatoRed(Red), R(Red)
//Maroon(GetColor(104, 42, 43))