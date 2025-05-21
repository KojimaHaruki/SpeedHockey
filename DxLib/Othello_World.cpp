#define _CRT_SECURE_NO_WARNINGS

// program for Othello World ©2022 mini
#include <iostream>
#include <string>
#include <iomanip>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "DxLib.h"
#pragma comment(lib, "winmm.lib")   // MSVC 用
using namespace std;
using std::this_thread::sleep_for;
using namespace std::chrono_literals;
// 指定したファイルのファイルサイズを確認して return で返す
int getFileSize(const char* fileName) {
    FILE* fp = fopen(fileName, "rb");
    long fileSize = 0;
    fseek(fp, 0, SEEK_END); // seek to the end of the file
    fileSize = ftell(fp);  // get the current file pointer 
    rewind(fp);  // rewind to the beginning of the file
    fclose(fp);
    return  fileSize;
}
// バイナリデータをヒープ領域のメモリに読み込む
char* readBinaryFile(const char* fileName) {
    int fileSize = getFileSize(fileName);
    char* fileData = (char*)calloc(sizeof(char), fileSize + 1);
    FILE* fp = fopen(fileName, "rb");
    int resultSize = fread(fileData, sizeof(char), fileSize, fp);
    fclose(fp);
    if (resultSize != fileSize) {
        printf("システム：正しくファイルが読み込めませんでした。サイズが一致しません。\n");
        exit(-1);
    }
    return fileData;
}
// program for Othello World ©2022 mini
int main(void) {
    char* soundFileData = readBinaryFile("Othello_World_BGM.wav");
    PlaySound(soundFileData, NULL, SND_MEMORY | SND_LOOP | SND_SYSTEM | SND_ASYNC);
    // inputed Player names & their length limit
    string input_name[5]; int name_limit = 10;
    // Player name
    string name[5];
    // to decide Players' order
    int order; int roll[4];
    // Player ID to distinguish Players' pieces **every Player ID must be a Natural number
    const int Black = 1; const int White = 2; const int Blue = 3; const int Red = 4; const int Space = 0;
    // the number of game, turn         **initial turn = 0
    int game, turn, Max_turn, Jump_turn;
    // turn time = current time - before time, lapse time = current time - start time
    time_t start_time, before_time, turn_time, lapse_time;
    long int turn_sec, turn_min, turn_hour, lapse_sec, lapse_min, lapse_hour;
    // Player ID to change Player who will attack by turn
    int Player;
    // coordinate name
    string coordinate;
    // x-y coordinate into which coordinate name is converted in order of x:→  y:↓
    int x, y;
    // piece[x][y] = (Player ID of the piece on the Place)
    int** piece = nullptr;
    // judge_piece[x][y] = (the number of pieces the piece on the Place can reverse)
    int** judge_piece = nullptr; // If the Place already has a piece, judge_piece[x][y] = -1
    // piece_record[turn][x][y] = (piece[x][y] in the turn)
    int piece_record[500][20][20];
    // reversed_piece[x][y] = (piece[x][y] which was reversed)
    int** reversed_piece = nullptr;
    // guide option number
    int guide_style, before_style;
    // the size of board
    int n;
    // the number of participants
    int k;
    // various operators
    int i, j, p, q, r, s, t, u, m;
    // the number of pattern where the piece can reverse the other
    int simulation_success, success;
    // the number of time where players can't successively put their pieces
    int failure_succession;
    // the number of pieces  rank[j] = the number of pieces ranked j th
    int num[5]; int rank[4];
    // the number of pieces 1 turn before
    int before_num[5];
    // the number of reversed pieces
    int reversed_num[5];
    // the changeable size of the window     ** (num[Space], height) is up to (133, 25)
    int window_wide = 133; int window_height = 30; int input_wide, input_height;
    // to show the board
    void board(int, int, int, time_t, int**, int**, const int, const int, const int, const int, const int,
        int, int[], string[], int, int, int);
    // to count Player's piece
    int count(int, int, int[], int**, int**);
    // to put Player's piece & reverse the other
    int reverse(int, int, int, const int, int, int**, int**);
    // to convert x-y into coordinate
    string xy_coordinate_name(int, int, int); string num_alphabet(int);
    // to show coordinates where the reverse happened
    int reversed_coordinate(int, int, int**);
    // to calculate coordinates where Player can put piece
    int simulation(int, const int, int, int**, int**);
    // to show "-"      " "               texts for seconds
    int bar(int, int); int space(int, int);
    long int hour_min_sec(time_t, long int&, long int&, long int&);
    coordinate = "N"; guide_style = 3; before_style = 0;
    cout << endl << "Othello World ©2022 mini" << endl;
    for (game = 0;; game++) {
        if ("Y" == coordinate || "R" == coordinate) {
            cout << endl << "Othello World ©2022 mini" << endl << endl
                << "Welcome to Othello World!" << endl << endl
                << "Othello World is n × n Othello! {n = 5 ~ 20 **standard Othello: n = 0}" << endl
                << "What size of Othello would you like to play?" << endl
                << "n = "; cin >> n; // the upper limit of n is 20
            if (n == 0) {
                n = 8; m = 64; k = 2;
            }
            else if (n < 5 || 20 < n) {
                cout << endl << "error: " << n << " is an invalid number!" << endl
                    << "note: You should input n {n = 0 or 5, 6, ~ , 20}\a" << endl;
                continue;
            }
            else {
                for (;;) {
                    cout << endl << "How many participants? {k = 2 ~ 4 (person)}" << endl << "k = ";
                    cin >> k;
                    if (k < 2 || 4 < k) {
                        cout << endl << "error: " << k << " is an invalid number!" << endl
                            << "note: You should input k {k = 2, 3, or 4}\a" << endl;
                        continue;
                    }
                    break;
                }
            }
            if (window_height < 2 * n + 8) {
                window_height = 2 * n + 8;
            }
            if (window_wide < 23 * k + 40) {
                if (k == 4) {
                    window_wide = 23 * k + 40;
                }
                else {
                    window_wide = 110;
                }
            }
            // assure data capacity of various pieces
            piece = new int* [n];
            judge_piece = new int* [n];
            reversed_piece = new int* [n];
            for (i = 0; i < n; i++) {
                piece[i] = new int[n]; judge_piece[i] = new int[n]; reversed_piece[i] = new int[n];
            }
            // assign each player to each color (order : num[Black] → num[White] → num[Blue] → num[Red])
            cout << endl << "Plaese input Player name **the number of characters is up to " << name_limit << endl;
            for (i = 0; i < k; i++) {
                for (;;) {
                    cout << i + 1 << "P: ";
                    cin >> input_name[i];
                    if (input_name[i].length() > name_limit) {
                        cout << endl << "error: the number of characters is up to " << name_limit
                            << endl << "Please rewrite " << i + 1 << "P name\a" << endl << endl;
                    }
                    else {
                        break;
                    }
                }
            }
            // initialize the roll of participants
            roll[0] = 0; roll[1] = 0; roll[2] = 0; roll[3] = 0;
            // initialize random number by the present time
            srand((int)time(NULL));
            // create random number:u = 0 ~ (k - 1)
            u = rand() % k;
            name[Black] = input_name[u]; roll[u] = 1;
            // create random number:u = 0 ~ (k - 2)
            u = rand() % (k - 1);
            for (order = 0, i = 0; i < k; i++) {
                if (roll[i] == 0) {
                    if (order == u) {
                        name[White] = input_name[i]; roll[i] = 1;
                        break;
                    }
                    order++;
                }
            }
            cout << endl << "Battle: " << name[Black] << " ○ v.s. " << name[White] << " ●";
            if (k > 2) { // create random number:u = 0 ~ (k - 3)
                u = rand() % (k - 2);
                for (order = 0, i = 0; i < k; i++) {
                    if (roll[i] == 0) {
                        if (order == u) {
                            name[Blue] = input_name[i]; roll[i] = 1;
                            break;
                        }
                        order++;
                    }
                }
                cout << " v.s. " << name[Blue] << " ☆";
            }
            if (k > 3) {
                for (i = 0; i < k; i++) {
                    if (roll[i] == 0) {
                        name[Red] = input_name[i]; roll[i] = 1;
                        break;
                    }
                }
                cout << " v.s. " << name[Red] << " ★";
            }
            bar(window_wide, 10); sleep_for(2s);
        }
        else if (coordinate != "I") {
            cout << endl << "Would you like to play Othello World";
            if (game > 0) {
                cout << " again";
            }
            cout << "? - Yes[Y] / No[N]" << endl << "  - ";
            cin >> coordinate;
            if ("Y" == coordinate) {
                game--;
                continue;
            }
            else if ("N" == coordinate) {
                exit(0);
            }
            else {
                cout << endl << "error: " << coordinate << " is out of option!" << endl;
                cout << "note: You should input Y / N at the head..." << "\a" << endl;
                continue;
            }
        }
        // initalize pieces
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                piece[i][j] = Space; reversed_piece[i][j] = 0;
            }
        }
        // initalize coordinate  
        coordinate = "Y";
        //put num[Black] & num[White] pieces on the center of board
        if (n % 2 == 0) {
            m = n / 2 - 1; p = m + 1; q = m - 1; r = m + 2; s = m - 2;
            if (k == 2) {
                piece[m][m] = White; piece[p][m] = Black;
                piece[m][p] = Black; piece[p][p] = White;
                num[Black] = 2; num[White] = 2; num[Blue] = 0; num[Red] = 0;
            }
            else if (k == 3) {
                piece[m][q] = Black; piece[p][q] = Black;
                piece[q][m] = White; piece[m][m] = White; piece[p][m] = White; piece[r][m] = White;
                piece[q][p] = Blue; piece[m][p] = Blue; piece[p][p] = Blue; piece[r][p] = Blue;
                piece[m][r] = Black; piece[p][r] = Black;
                num[Black] = 4; num[White] = 4; num[Blue] = 4; num[Red] = 0;
            }
            else {
                piece[q][q] = Red; piece[m][q] = Blue; piece[p][q] = Red; piece[r][q] = Blue;
                piece[q][m] = Black; piece[m][m] = White; piece[p][m] = Black; piece[r][m] = White;
                piece[q][p] = Red; piece[m][p] = Blue; piece[p][p] = Red; piece[r][p] = Blue;
                piece[q][r] = Black; piece[m][r] = White; piece[p][r] = Black; piece[r][r] = White;
                num[Black] = 4; num[White] = 4; num[Blue] = 4; num[Red] = 4;
            }
        }
        else {
            m = (n - 1) / 2; p = m + 1; q = m - 1; r = m + 2; s = m - 2;
            if (k == 2) {
                piece[m][q] = White;
                piece[q][m] = White; piece[m][m] = Black; piece[p][m] = White;
                piece[q][p] = Black;                      piece[p][p] = Black;
                num[Black] = 3; num[White] = 3; num[Blue] = 0; num[Red] = 0;
            }
            else if (k == 3) {
                piece[q][q] = Black; piece[m][q] = Blue; piece[p][q] = White;
                piece[q][m] = White; piece[m][m] = Black; piece[p][m] = Blue;
                piece[q][p] = Blue; piece[m][p] = White; piece[p][p] = Black;
                num[Black] = 3; num[White] = 3; num[Blue] = 3; num[Red] = 0;
            }
            else {
                piece[m][s] = Black;
                piece[q][q] = White; piece[m][q] = Red; piece[p][q] = Blue;
                piece[s][m] = Blue; piece[q][m] = Black;                         piece[p][m] = White; piece[r][m] = Red;
                piece[q][p] = Red; piece[m][p] = Blue; piece[p][p] = Black;
                piece[m][r] = White;
                num[Black] = 3; num[White] = 3; num[Blue] = 3; num[Red] = 3;
            }
        }
        num[Space] = n * n - num[Black] - num[White] - num[Blue] - num[Red];
        start_time = time(NULL); before_time = time(NULL);
        for (Max_turn = 0, turn = 0; num[Space] > 0; turn++) {
            if (turn % k == 0 && num[Black] > 0) {
                Player = Black;
            }
            else if (turn % k == 1 && num[White] > 0) {
                Player = White;
            }
            else if (turn % k == 2 && num[Blue] > 0) {
                Player = Blue;
            }
            else if (turn % k == 3 && num[Red] > 0) {
                Player = Red;
            }
            else {
                if (turn % k == 0 && num[Black] == 0) {
                    cout << name[Black];
                }
                else if (turn % k == 1 && num[White] == 0) {
                    cout << name[White];
                }
                else if (turn % k == 2 && num[Blue] == 0) {
                    cout << name[Blue];
                }
                else {
                    cout << name[Red];
                }
                cout << " has been already eliminated! Skipped...\a";
                sleep_for(2s); start_time += 2; before_time += 2;
                continue;
            }
            if (turn < 500) {
                if (turn > Max_turn) {
                    Max_turn = turn;
                }
                for (i = 0; i < n; i++) {
                    for (j = 0; j < n; j++) {
                        piece_record[turn][i][j] = piece[i][j];
                    }
                }
            }
            else {
                cout << "Turn " << turn + 1 << " can't be recorded because of lack of data capacity!?\a" << endl;
            }
            simulation_success = simulation(n, Space, Player, judge_piece, piece);
            if (simulation_success == 0) {
                board(n, k, turn + 1, start_time, piece, judge_piece,
                    Space, Black, White, Blue, Red, Player, num, name,
                    window_height, window_wide, guide_style);
                cout << "You can't put your piece on any place!? Skipped...\a" << endl;
                sleep_for(2s); start_time += 2; before_time += 2; bar(window_wide, 0);
                failure_succession++;
                if (failure_succession == k) {
                    cout << "All players can't put their piece on any place!?\a\a" << endl;
                    sleep_for(3s); start_time += 3; num[Space] = 0; turn--;
                    continue;
                }
            }
            else {
                failure_succession = 0;
                if (simulation_success == 1) {
                    board(n, k, turn + 1, start_time, piece, judge_piece,
                        Space, Black, White, Blue, Red, Player, num, name,
                        window_height, window_wide, guide_style);
                    if (Player == Black) {
                        cout << "There's only one place where you can put ○ ...";
                    }
                    else if (Player == White) {
                        cout << "There's only one place where you can put ● ...";
                    }
                    else if (Player == Blue) {
                        cout << "There's only one place where you can put ☆ ...";
                    }
                    else {
                        cout << "There's only one place where you can put ★ ...";
                    }
                    for (i = 0; i < n; i++) {
                        for (j = 0; j < n; j++) {
                            if (judge_piece[i][j] > 0) {
                                x = i; y = j;
                            }
                        }
                    }
                    coordinate = xy_coordinate_name(n, x, y);
                    cout << "  Your piece will be automatically put on " << coordinate << endl;
                    sleep_for(2s); start_time += 2; before_time += 2;
                }
                else {
                    cout << endl << "|hint|There're" << setw(4) << simulation_success << " places where ";
                    if (Player == Black) {
                        cout << name[Black] << " can put ○"; space(window_wide - name[Black].length() - 48, 4);
                    }
                    else if (Player == White) {
                        cout << name[White] << " can put ●"; space(window_wide - name[White].length() - 48, 4);
                    }
                    else if (Player == Blue) {
                        cout << name[Blue] << " can put ☆"; space(window_wide - name[Blue].length() - 48, 4);
                    }
                    else {
                        cout << name[Red] << " can put ★"; space(window_wide - name[Red].length() - 48, 4);
                    }
                    board(n, k, turn + 1, start_time, piece, judge_piece,
                        Space, Black, White, Blue, Red, Player, num, name,
                        window_height, window_wide, guide_style);
                    if (guide_style == 3) {
                        cout << "Tutorial: Please input the coordinate in the board where you wanna put your piece!" << endl;
                        sleep_for(4s); start_time += 4; before_time += 4; turn--; bar(window_wide, 0);
                        // initialize guide coordinate
                        guide_style = before_style;
                        continue;
                    }
                    cout << "Where would you like to put your piece? - "; cin >> coordinate;
                    before_style = guide_style;
                    if ("H" == coordinate) {
                        cout << endl << "How high is your suitable window? **default: 30 now: "
                            << window_height << endl << "Height: "; cin >> input_height;
                        if (input_height < 2 * n + 8) {
                            cout << endl << "error: You can't lower window's height any more..." << endl
                                << "note: the minimum height is \a" << 2 * n + 8 << endl;
                            sleep_for(2s); start_time += 2; before_time += 2;
                        }
                        else {
                            window_height = input_height;
                        }
                        turn--; bar(window_wide, 0);
                        continue;
                    }
                    else if ("W" == coordinate) {
                        cout << endl << "How wide is your suitable window? **default: 133 now: "
                            << window_wide << endl << "Wide: "; cin >> input_wide;
                        if (input_wide < 23 * k + 40 || input_wide < 110) {
                            cout << endl << "error: You can't lower window's wide any more..." << endl
                                << "note: the minimum wide is \a";
                            if (k == 4) {
                                cout << 23 * k + 40 << endl;
                            }
                            else {
                                cout << 110 << endl;
                            }
                            sleep_for(2s); start_time += 2; before_time += 2;
                        }
                        else {
                            window_wide = input_wide;
                        }
                        turn--; bar(window_wide, 0);
                        continue;
                    }
                    else if ("I" == coordinate) {
                        guide_style = 0;
                        break;
                    }
                    else if ("R" == coordinate) {
                        break;
                    }
                    else if ("J" == coordinate) {
                        cout << endl << "What turn would you like to jump into? - Turn "; cin >> Jump_turn;
                        if (Jump_turn - 1 > Max_turn) {
                            cout << endl << "error: You can't put the turn Forward!" << endl
                                << "note: You should select the turn you has passed through\a" << endl;
                            sleep_for(2s); start_time += 2; before_time += 2; turn--;
                            continue;
                        }
                        turn = Jump_turn - 1;
                    }
                    else if (coordinate.compare(0, 1, "+") == 0) {
                        for (i = 0; coordinate.compare(i, 1, "+") == 0 && guide_style < 3; i++, guide_style++) {
                        }
                        turn--;
                        continue;
                    }
                    else if (coordinate.compare(0, 1, "-") == 0) {
                        for (i = 0; coordinate.compare(i, 1, "-") == 0 && guide_style > 0; i++, guide_style--) {
                        }
                        turn--;
                        continue;
                    }
                    else if (coordinate.compare(0, 1, "<") == 0) {
                        for (i = 0; coordinate.compare(i, 1, "<") == 0; i++, turn--) {
                        }
                    }
                    else if (coordinate.compare(0, 1, ">") == 0) {
                        for (i = 0; coordinate.compare(i, 1, ">") == 0; i++) {
                            if (turn < Max_turn) {
                                turn++;
                            }
                            else {
                                cout << endl << "error: You can't put the turn forward!" << endl
                                    << "note: You should select the turn you has passed through\a" << endl;
                                sleep_for(2s); before_time += 2; start_time += 2;
                                break;
                            }
                        }
                    }
                    else if ("E" == coordinate) {
                        exit(0);
                    }
                    else if ("S" == coordinate) {
                        continue;
                    }
                    else if ("T" == coordinate) {
                        before_style = guide_style;
                        guide_style = 3; turn--;
                        continue;
                    }
                    if (turn < 0) {
                        coordinate = "R";
                        break;
                    }
                    else if (coordinate.compare(0, 1, "<") == 0 || coordinate.compare(0, 1, ">") == 0 || "J" == coordinate) {
                        for (i = 0; i < n; i++) {
                            for (j = 0; j < n; j++) {
                                piece[i][j] = piece_record[turn][i][j];
                            }
                        }
                        count(n, k, num, judge_piece, piece);
                        turn--;
                        continue;
                    }
                    // change alphabet into x-coordinate & prepare x
                    for (x = -1, i = 0; i < n; i++) {
                        if (coordinate.compare(0, 1, num_alphabet(i)) == 0) {
                            x = i;
                        }
                    }
                    if (x == -1) {
                        cout << endl << "error: " << coordinate << " is an invalid coordinate name!" << endl
                            << "note: You should input alphabet or symbol in the board at the head...\a" << endl;
                        sleep_for(2s); start_time += 2; before_time += 2; turn--;
                        continue;
                    }
                    // change number into y-coordinate & calculate y
                    for (y = -1, i = 0; i < n; i++) {
                        if (coordinate.compare(1, to_string(i + 1).length(), to_string(i + 1)) == 0) {
                            y = i;
                        }
                    }
                    if (y == -1) {
                        cout << endl << "error: " << coordinate << " is an invalid coordinate name!" << endl
                            << "note: You should input number in the board at the second...\a" << endl;
                        sleep_for(2s); start_time += 2; before_time += 2; turn--;
                        continue;
                    }
                }
                success = reverse(n, x, y, Space, Player, reversed_piece, piece);
                if (success < 1) {
                    if (success == 0) {
                        cout << endl << "error: You can't put your piece on " << coordinate << "!" << endl
                            << "note: You should put your piece on the place where it can reverse the other...\a" << endl;
                    }
                    else {
                        cout << endl << "fatal error: There is already a piece on " << coordinate << "!!" << endl
                            << "note: You should put your piece on space...\a\a" << endl;
                    }
                    sleep_for(2s); start_time += 2; before_time += 2; turn--;
                    continue;
                }
                else {
                    turn_time = time(NULL) - before_time; hour_min_sec(turn_time, turn_hour, turn_min, turn_sec);
                    //record the number of pieces & initialize it 
                    for (i = 1; i <= k; i++) {
                        before_num[i] = num[i];
                    }
                    count(n, k, num, judge_piece, piece);
                    for (i = 1; i <= k; i++) {
                        reversed_num[i] = before_num[i] - num[i];
                    }
                    lapse_time = time(NULL) - start_time; hour_min_sec(lapse_time, lapse_hour, lapse_min, lapse_sec);
                    bar(window_wide, 21); cout << "|Turn|Time    |Log"; space(window_wide - 18, 14);
                    bar(6, 23); bar(9, 43); bar(window_wide - 15, 14);
                    cout << "|" << setw(4) << turn + 1 << "|" << setw(2) << lapse_hour
                        << ":" << setw(2) << lapse_min << ":" << setw(2) << lapse_sec << "|";
                    if (Player == Black) {
                        cout << name[Black] << " put ○ on " << coordinate << "!";
                        space(window_wide - name[Black].length() - coordinate.length() - 32, 4);
                        space(6, 19); space(9, 4); cout << " ○ reversed";
                    }
                    else if (Player == White) {
                        cout << name[White] << " put ● on " << coordinate << "!";
                        space(window_wide - name[White].length() - coordinate.length() - 32, 4);
                        space(6, 19); space(9, 4); cout << " ● reversed";
                    }
                    else if (Player == Blue) {
                        cout << name[Blue] << " put ☆ on " << coordinate << "!";
                        space(window_wide - name[Blue].length() - coordinate.length() - 32, 4);
                        space(6, 19); space(9, 4); cout << " ☆ reversed";
                    }
                    else {
                        cout << name[Red] << " put  ★ on " << coordinate << "!";
                        space(window_wide - name[Red].length() - coordinate.length() - 32, 4);
                        space(6, 19); space(9, 4); cout << " ★ reversed";
                    }
                    if (reversed_num[Black] > 0) {
                        cout << setw(4) << reversed_num[Black] << " ○  on ";
                        j = reversed_coordinate(n, Black, reversed_piece) + 48;
                        space(window_wide - j, 4);
                        if (reversed_num[White] > 0 || reversed_num[Blue] > 0 || reversed_num[Red] > 0) {
                            space(6, 19); space(9, 4); space(16, 0);
                        }
                    }
                    if (reversed_num[White] > 0) {
                        cout << setw(4) << reversed_num[White] << " ●  on ";
                        j = reversed_coordinate(n, White, reversed_piece) + 48;
                        space(window_wide - j, 4);
                        if (reversed_num[Blue] > 0 || reversed_num[Red] > 0) {
                            space(6, 19); space(9, 4); space(16, 0);
                        }
                    }
                    if (reversed_num[Blue] > 0) {
                        cout << setw(4) << reversed_num[Blue] << " ☆ on ";
                        j = reversed_coordinate(n, Blue, reversed_piece) + 48;
                        space(window_wide - j, 4);
                        if (reversed_num[Red] > 0) {
                            space(6, 19); space(9, 4); space(16, 0);
                        }
                    }
                    if (reversed_num[Red] > 0) {
                        cout << setw(4) << reversed_num[Red] << " ★ on ";
                        j = reversed_coordinate(n, Red, reversed_piece) + 48;
                        space(window_wide - j, 4);
                    }
                    bar(6, 33); bar(9, 43); bar(window_wide - 15, 14);
                    space(6, 9); space(9, 4); space(10, 0);
                    cout << "Total:" << setw(4) << success + 1 << " piece";
                    if (success == 1) {
                        cout << "  in";
                    }
                    else {
                        cout << "s in";
                    }
                    cout << setw(3) << turn_hour << "h" << setw(3) << turn_min << "min" << setw(3) << turn_sec << "s";
                    space(window_wide - 59, 4);
                    bar(window_wide, 12);
                    before_time = time(NULL);
                }
            }
        }
        if (num[Space] == 0) {
            board(n, k, turn + 1, start_time, piece, judge_piece,
                Space, Black, White, Blue, Red, Player, num, name,
                window_height, window_wide, guide_style);
            cout << endl << "Result: " << name[Black] << " ○ " << num[Black]
                << " v.s. " << name[White] << " ● " << num[White];
            if (k > 2) {
                cout << " v.s. " << name[Blue] << " ☆ " << num[Blue];
            }
            if (k > 3) {
                cout << " v.s. " << name[Red] << " ★ " << num[Red];
            }
            for (i = n * n, j = 0; i >= 0 || j < k; i--) {
                for (int m = 1; m <= k; m++) {
                    if (num[m] == i) {
                        rank[j] = num[m]; j++;
                    }
                }
            }
            bar(window_wide, 31); cout << "|Rank|piece|Player name"; space(window_wide - 23, 4);
            for (j = 0; j < k; j++) {
                bar(6, 33); bar(6, 43); bar(window_wide - 12, 14);
                cout << "|" << setw(4) << j + 1 << "|" << setw(5) << rank[j] << "| "; i = 0;
                if (num[Black] == rank[j]) {
                    cout << setw(10) << name[Black] << " : ○  & "; i++;
                }
                if (num[White] == rank[j]) {
                    cout << setw(10) << name[White] << " : ●  & "; i++;
                }
                if (num[Blue] == rank[j]) {
                    cout << setw(10) << name[Blue] << " : ☆ & "; i++;
                }
                if (num[Red] == rank[j]) {
                    cout << setw(10) << name[Red] << " : ★ & "; i++;
                }
                cout << "\b\b"; space(window_wide - i * 18 - 11, 4);
                for (; rank[j] == rank[j + 1]; j++) {
                }
            }
            bar(window_wide, 32); space(8, 0);
            if (k == 2 && num[Black] == num[White]) {
                cout << "        Draw!";
            }
            else if (k == 3 && num[Black] == num[White] && num[Black] == num[Blue]) {
                cout << "        Draw!";
            }
            else if (num[Black] == num[White] && num[Black] == num[Blue] && num[Black] == num[Red]) {
                cout << "        Draw!";
            }
            else {
                if (num[Black] == rank[0]) {
                    cout << name[Black] << " : ○ & ";
                }
                if (num[White] == rank[0]) {
                    cout << name[White] << " : ● & ";
                }
                if (num[Blue] == rank[0]) {
                    cout << name[Blue] << " : ☆ & ";
                }
                if (num[Red] == rank[0]) {
                    cout << name[Red] << " : ★ & ";
                }
                cout << "\b\bWin!!" << endl;
            }
            coordinate = "N";
        }
    }
    return 0;
}
int count(int n, int k, int num[], int** judge_piece, int** piece) {
    for (int m = 0; m <= k; m++) {
        num[m] = 0;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int m = 0; m <= k; m++) {
                if (piece[i][j] == m) {
                    num[m]++;
                }
            }
        }
    }
    return n * n;
}
long int hour_min_sec(time_t time_sec, long int& hour, long int& min, long int& sec) {
    sec = (long int)time_sec % 60; long int j = (long int)time_sec % 3600;
    hour = (long int)(time_sec - j) / 3600;
    min = (long int)(time_sec - sec) / 60 - hour * 60;
    return (long int)time_sec;
}
// show coordinates of reversed pieces and initalize them 
int reversed_coordinate(int n, int Opponent, int** reversed_piece) {
    int char_num = 0;
    string xy_coordinate_name(int, int, int);
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            if (reversed_piece[x][y] == Opponent) {
                cout << xy_coordinate_name(n, x, y) << ", ";
                char_num = char_num + xy_coordinate_name(n, x, y).length() + 2;
                reversed_piece[x][y] = 0;
            }
        }
    }
    cout << "\b\b!";
    return char_num - 1;
}
// show the board
void board(int n, int k, int turn, time_t start_time, int** piece, int** judge_piece,
    const int Space, const int Black, const int White, const int Blue, const int Red,
    int Player, int num[], string name[],
    int window_height, int window_wide, int guide_style) {
    int x, y, i; int board_height = 2 * n + 1; int board_wide = 4 * n + 1;
    int s = window_height - board_height - 7;
    int n_digit = to_string(n).length();
    int upper_space = (s - s % 2) / 2; int lower_space = (s + s % 2) / 2;
    int t = window_wide - board_wide;
    int left_space = (t - t % 2) / 2; int right_space = (t + t % 2) / 2;
    long int hour_min_sec(time_t, long int&, long int&, long int&);
    time_t lapse_time = time(NULL) - start_time;
    long int lapse_sec, lapse_min, lapse_hour;
    hour_min_sec(lapse_time, lapse_hour, lapse_min, lapse_sec);
    string num_alphabet(int); string xy_coordinate_name(int, int, int);
    int bar(int, int); int space(int, int); int space_line(int, int, int, int);
    bar(window_wide, 31);
    if (guide_style == 3) {
        cout << "| < | > |Skip|Jump| ← about Turn";
        space((window_wide - 2 * n_digit - 120 - window_wide % 2) / 2, 0);
        cout << "Turn" << setw(4) << turn << "Player: " << k << "/4  " << n << " × " << n << " Othello";
        space((window_wide - 2 * n_digit - 120 + window_wide % 2) / 2, 0);
        cout << "about Guide → | - | + | ? | [← [E] |"; bar(25, 38); space(window_wide - 51, 0); bar(26, 37);
        cout << "|↑Back ↑For ↑Skip ↑Jump  **You can also change window's Height[H] & Wide[W]";
        space(window_wide - 104, 0); cout << "Less↑ More↑  ↑Tutoreal ↑Exit|";
        space_line(upper_space - 1, window_wide, 19, 20);
    }
    else {
        cout << "| < | > |Skip|Jump"; space((window_wide - 2 * n_digit - 90 - window_wide % 2) / 2, 3);
        cout << "Turn" << setw(4) << turn << "Player: " << k << "/4   " << n << " × " << n << " Othello  Time:"
            << setw(2) << lapse_hour << ":" << setw(2) << lapse_min << ":" << setw(2) << lapse_sec;
        space((window_wide - 2 * n_digit - 100 + window_wide % 2) / 2, 4);
        cout << " - | + | ? | [← [E] |"; bar(25, 38); space(window_wide - 51, 0);
        bar(26, 27); space_line(upper_space, window_wide, 19, 20);
    }
    space(left_space - 1, 3);
    for (x = 0; x < n; x++) {
        cout << setw(4) << num_alphabet(x);
    }
    space(right_space + 2, 4); space(left_space, 13); bar(board_wide, 1);
    space(right_space, 4); space(left_space - 3, 13); cout << " 1 |";
    for (y = 0; y < n; y++) {
        if (y != 0) {
            space(right_space, 4); space(left_space + 1, 19);
            for (i = 0; i < n - 1; i++) {
                bar(4, 43);
            }
            bar(4, 4); space(right_space, 4); space(left_space - 3, 13); cout << setw(2) << y + 1 << " |";
        }
        for (x = 0; x < n; x++) {
            if (guide_style == 3) {
                cout << setw(3) << xy_coordinate_name(n, x, y) << "|";
            }
            else {
                if (piece[x][y] == Black) {
                    cout << " ○|";
                }
                else if (piece[x][y] == White) {
                    cout << " ●|";
                }
                else if (piece[x][y] == Blue) {
                    cout << " ☆|";
                }
                else if (piece[x][y] == Red) {
                    cout << " ★|";
                }
                else if (judge_piece[x][y] == 0) {
                    cout << "   |";
                }
                else {
                    if (guide_style == 1) {
                        cout << " + |";
                    }
                    else if (guide_style == 2) {
                        cout << "+" << setw(2) << judge_piece[x][y] << "|";
                    }
                    else {
                        cout << "   |";
                    }
                }
            }
        }
    }
    space(right_space, 4); space(left_space, 13); bar(board_wide, 2); space(right_space, 4);
    if (guide_style == 3) {
        space_line(lower_space - 2, window_wide, 19, 20); cout << "|↓Restart ↓Initialize";
        space((window_wide - 90 - window_wide % 2) / 2, 0); cout << "↓ Player name : color & number of piece ↓";
        space((window_wide - 90 + window_wide % 2) / 2, 0); cout << "number of space ↓          |" << endl;
    }
    else {
        space_line(lower_space, window_wide, 19, 20);
    }
    bar(17, 26); space(window_wide - 31, 0); bar(14, 35); cout << "|Restart|Initialize ";
    space((window_wide - 23 * k - 38 - (window_wide - k) % 2) / 2, 3);
    cout << setw(14) << name[Black] << " : ○" << setw(4) << num[Black]
        << setw(14) << name[White] << " : ●" << setw(4) << num[White];
    if (k > 2) {
        cout << setw(14) << name[Blue] << " : ☆" << setw(4) << num[Blue];
    }
    if (k > 3) {
        cout << setw(14) << name[Red] << " : ★" << setw(4) << num[Red];
    }
    space((window_wide - 23 * k - 24 + (window_wide - k) % 2) / 2, 0);
    cout << "/" << setw(4) << num[Space] << " remain  |"; bar(window_wide, 32);
    if (num[Space] == 0) {
        i = 41;
    }
    else if (Player == Black) {
        cout << name[Black] << ", it's your turn!"; i = 41 + name[Black].length();
    }
    else if (Player == White) {
        cout << name[White] << ", it's your turn!"; i = 41 + name[White].length();
    }
    else if (Player == Blue) {
        cout << name[Blue] << ", it's your turn!"; i = 41 + name[Blue].length();
    }
    else {
        cout << name[Red] << ", it's your turn!"; i = 41 + name[Red].length();
    }
    space(window_wide - i, 0); cout << "Othello World ©2022 mini" << endl;
    return;
}
int simulation(int n, const int Space, int Player, int** judge_piece, int** piece) {
    int x, y, z, w, i; int success = 0; // the number of successful case
    // assure data capacity of various pieces
    int** copy_piece = (int**)malloc(n * sizeof(int*)); int** dummy = (int**)malloc(n * sizeof(int*));
    for (i = 0; i < n; i++) {
        copy_piece[i] = (int*)malloc(n * sizeof(int)); dummy[i] = (int*)malloc(n * sizeof(int));
    }
    int reverse(int, int, int, const int, int, int**, int**);
    for (x = 0; x < n; x++) {
        for (y = 0; y < n; y++) {
            for (z = 0; z < n; z++) {
                for (w = 0; w < n; w++) {
                    copy_piece[z][w] = piece[z][w];
                }
            }
            judge_piece[x][y] = reverse(n, x, y, Space, Player, dummy, copy_piece);
            if (judge_piece[x][y] > 0) {
                success++;
            }
        }
    }
    return success;
}
int reverse(int n, int x, int y, const int Space, int Player, int** reversed_piece, int** piece) {
    int i, j; int success = 0; // the number of successful case
    if (piece[x][y] == Space) {
        // check the upper side 
        if (y > 1 && piece[x][y - 1] != Player) {
            for (i = 0; y - i != 0 && piece[x][y - i - 1] != Space; i++) {
                if (piece[x][y - i] != Player && piece[x][y - i - 1] == Player) {
                    // put player's piece
                    piece[x][y] = Player;
                    for (j = 1; j < i + 1; j++) {
                        // record which Player's pieces was reversed & Places where pieces was reversed
                        reversed_piece[x][y - j] = piece[x][y - j];
                        // reverse the others on the upper side
                        piece[x][y - j] = Player;
                    }
                    // record the time of reverse on the upper side
                    success = success + abs(i);
                    break;
                }
            }
        }
        // check the lower side 
        if (y < n - 2 && piece[x][y + 1] != Player) {
            for (i = 0; y + i != n - 1 && piece[x][y + i + 1] != Space; i++) {
                if (piece[x][y + i] != Player && piece[x][y + i + 1] == Player) {
                    // put player's piece
                    piece[x][y] = Player;
                    for (j = 1; j < i + 1; j++) {
                        // record which Player's pieces was reversed & Places where pieces was reversed
                        reversed_piece[x][y + j] = piece[x][y + j];
                        // reverse the others on the lower side
                        piece[x][y + j] = Player;
                    }
                    // record the time of reverse on the lower side
                    success = success + abs(i);
                    break;
                }
            }
        }
        // check the left side 
        if (x > 1 && piece[x - 1][y] != Player) {
            for (i = 0; x - i != 0 && piece[x - i - 1][y] != Space; i++) {
                if (piece[x - i][y] != Player && piece[x - i - 1][y] == Player) {
                    // put player's piece
                    piece[x][y] = Player;
                    for (j = 1; j < i + 1; j++) {
                        // record which Player's pieces was reversed & Places where pieces was reversed
                        reversed_piece[x - j][y] = piece[x - j][y];
                        // reverse the others on the left side
                        piece[x - j][y] = Player;
                    }
                    // record the time of reverse on the left side
                    success = success + abs(i);
                    break;
                }
            }
        }
        // check the right side 
        if (x < n - 2 && piece[x + 1][y] != Player) {
            for (i = 0; x + i != n - 1 && piece[x + i + 1][y] != Space; i++) {
                if (piece[x + i][y] != Player && piece[x + i + 1][y] == Player) {
                    // put player's piece
                    piece[x][y] = Player;
                    for (j = 1; j < i + 1; j++) {
                        // record which Player's pieces was reversed & Places where pieces was reversed
                        reversed_piece[x + j][y] = piece[x + j][y];
                        // reverse the others on the right side
                        piece[x + j][y] = Player;
                    }
                    // record the time of reverse on the right side
                    success = success + abs(i);
                    break;
                }
            }
        }
        // check the upper left side 
        if (x > 1 && y > 1 && piece[x - 1][y - 1] != Player) {
            for (i = 0; x - i != 0 && y - i != 0 && piece[x - i - 1][y - i - 1] != Space; i++) {
                if (piece[x - i][y - i] != Player && piece[x - i - 1][y - i - 1] == Player) {
                    // put player's piece
                    piece[x][y] = Player;
                    for (j = 1; j < i + 1; j++) {
                        // record which Player's pieces was reversed & Places where pieces was reversed
                        reversed_piece[x - j][y - j] = piece[x - j][y - j];
                        // reverse the others on the upper left side
                        piece[x - j][y - j] = Player;
                    }
                    // record the time of reverse on the upper left side
                    success = success + abs(i);
                    break;
                }
            }
        }
        // check the upper right side
        if (x < n - 2 && y > 1 && piece[x + 1][y - 1] != Player) {
            for (i = 0; x + i != n - 1 && y - i != 0 && piece[x + i + 1][y - i - 1] != Space; i++) {
                if (piece[x + i][y - i] != Player && piece[x + i + 1][y - i - 1] == Player) {
                    // put player's piece
                    piece[x][y] = Player;
                    for (j = 1; j < i + 1; j++) {
                        // record which Player's pieces was reversed & Places where pieces was reversed
                        reversed_piece[x + j][y - j] = piece[x + j][y - j];
                        // reverse the others on the upper right side
                        piece[x + j][y - j] = Player;
                    }
                    // record the time of reverse on the upper right side
                    success = success + abs(i);
                    break;
                }
            }
        }
        // check the lower left side 
        if (x > 1 && y < n - 2 && piece[x - 1][y + 1] != Player) {
            for (i = 0; x - i != 0 && y + i != n - 1 && piece[x - i - 1][y + i + 1] != Space; i++) {
                if (piece[x - i][y + i] != Player && piece[x - i - 1][y + i + 1] == Player) {
                    // put player's piece
                    piece[x][y] = Player;
                    for (j = 1; j < i + 1; j++) {
                        // record which Player's pieces was reversed & Places where pieces was reversed
                        reversed_piece[x - j][y + j] = piece[x - j][y + j];
                        // reverse the others on the lower left side
                        piece[x - j][y + j] = Player;
                    }
                    // record the time of reverse on the lower left side
                    success = success + abs(i);
                    break;
                }
            }
        }
        // check the lower right side 
        if (x < n - 2 && y < n - 2 && piece[x + 1][y + 1] != Player) {
            for (i = 0; x + i != n - 1 && y + i != n - 1 && piece[x + i + 1][y + i + 1] != Space; i++) {
                if (piece[x + i][y + i] != Player && piece[x + i + 1][y + i + 1] == Player) {
                    // put player's piece
                    piece[x][y] = Player;
                    for (j = 1; j < i + 1; j++) {
                        // record which Player's pieces was reversed & Places where pieces was reversed
                        reversed_piece[x + j][y + j] = piece[x + j][y + j];
                        // reverse the others on the lower right side
                        piece[x + j][y + j] = Player;
                    }
                    // record the time of reverse on the lower right side
                    success = success + abs(i);
                    break;
                }
            }
        }
    }
    else { // There is already a piece on the Place, so Player can't put player's piece; return -1
        success = -1;
    }
    return success;
}
string xy_coordinate_name(int n, int x, int y) {
    string num_alphabet(int);
    string coordinate;
    // change x-coordinate into alphabet
    coordinate = num_alphabet(x) + to_string(y + 1);
    return coordinate;
}
string num_alphabet(int number) {
    string alphabet;
    // change x-coordinate into alphabet
    switch (number) {
    case  0:
        alphabet = "A";
        break;
    case  1:
        alphabet = "B";
        break;
    case  2:
        alphabet = "C";
        break;
    case  3:
        alphabet = "D";
        break;
    case  4:
        alphabet = "E";
        break;
    case  5:
        alphabet = "F";
        break;
    case  6:
        alphabet = "G";
        break;
    case  7:
        alphabet = "H";
        break;
    case  8:
        alphabet = "I";
        break;
    case  9:
        alphabet = "J";
        break;
    case 10:
        alphabet = "K";
        break;
    case 11:
        alphabet = "L";
        break;
    case 12:
        alphabet = "M";
        break;
    case 13:
        alphabet = "N";
        break;
    case 14:
        alphabet = "O";
        break;
    case 15:
        alphabet = "P";
        break;
    case 16:
        alphabet = "Q";
        break;
    case 17:
        alphabet = "R";
        break;
    case 18:
        alphabet = "S";
        break;
    case 19:
        alphabet = "T";
        break;
    default:
        alphabet = "Z";
        break;
    }
    return alphabet;
}
// font:   0      ↑1      ↓2     ←3     →4      ↖5      ↗6      ↙7      ↘8      ⇔9    
// font:   ~     .~.     '~'     |~     ~|     .~       ~.     '~       ~'      |~|    
// font:  ←10    ↑11     ↓12    ←13    →14     ↖15     ↗16     ↙17     ↘18     ⇔19    
// font: \n~   \n.~.   \n'~'   \n|~     ~|\n \n.~       ~.\n \n'~       ~'\n  \n|~|     
// font:  →20    ↑21     ↓22    ←23    →24     ↖25     ↗26     ↙27     ↘28     ⇔29    
// font:   ~\n   .~.\n   '~'\n   |~+   +~|     .~|     |~.     '~|     |~'      |~|\n 
// font: ⇔30    ↑31     ↓32    ←33    →34     ↖35     ↗36     ↙37     ↘38     ⇔39    
// font: \n~\n \n.~.\n \n'~'\n \n|~+   +~|\n   .~|\n \n|~.     '~|\n \n|~'    \n|~|\n 
// font: ⇔40    ↑41     ↓42    ←43    →44     ↖45     ←46     →47     ↘48     ⇔49   
// font:  <~>   /~|     |~/      ~+   +~       /~|\n  \n~+     +~\n  \n|~/      +~+ 
int first_font(int style) {
    int total_char_num = 0;
    switch (abs(style)) {
    case 10:
    case 30:
        cout << endl; total_char_num--;
    case  4:
    case  6:
    case  8:
    case 14:
    case 16:
    case 18:
    case 43:
    case 46:
        total_char_num++;
        break;
    case 11:
    case 31:
        cout << endl;
    case  1:
    case 21:
        cout << "."; total_char_num = 2;
        break;
    case 12:
    case 32:
        cout << endl;
    case  2:
    case 22:
    case 27:
    case 37:
        cout << "'"; total_char_num = 2;
        break;
    case 13:
        cout << endl;
    case  3:
        cout << "|"; total_char_num = 1;
        break;
    case 15:
        cout << endl;
    case  5:
        cout << ".";  total_char_num = 1;
        break;
    case 17:
        cout << endl;
    case  7:
        cout << "'"; total_char_num = 1;
        break;
    case 19:
    case 33:
    case 36:
    case 38:
    case 39:
    case 48:
        cout << endl;
    case  9:
    case 23:
    case 26:
    case 28:
    case 29:
    case 42:
        cout << "|"; total_char_num = 2;
        break;
    case 24:
    case 34:
    case 49:
        total_char_num++;
    case 44:
    case 47:
        cout << "+"; total_char_num++;
        break;
    case 25:
    case 35:
        cout << "."; total_char_num = 2;
        break;
    case 40:
        cout << "<"; total_char_num = 2;
        break;
    case 41:
    case 45:
        cout << "/"; total_char_num = 2;
        break;
    default:
        break;
    }
    return total_char_num;
}
int last_font(int style) {
    int last_char_num = 0;
    switch (abs(style)) {
    case  1:
    case  6:
    case 11:
    case 26:
    case 36:
        cout << "."; last_char_num = 1;
        break;
    case  2:
    case  8:
    case 12:
    case 28:
    case 38:
        cout << "'"; last_char_num = 1;
        break;
    case  4:
    case  9:
    case 19:
    case 24:
    case 25:
    case 27:
    case 41:
        cout << "|"; last_char_num = 1;
        break;
    case 14:
    case 29:
    case 34:
    case 35:
    case 37:
    case 39:
    case 45:
        cout << "|"; last_char_num = 1;
    case 20:
    case 30:
    case 47:
        cout << endl;
        break;
    case 16:
    case 21:
    case 31:
        cout << "." << endl; last_char_num = 1;
        break;
    case 18:
    case 22:
    case 32:
        cout << "'" << endl; last_char_num = 1;
        break;
    case 23:
    case 33:
    case 43:
    case 46:
    case 49:
        cout << "+"; last_char_num = 1;
        break;
    case 40:
        cout << ">"; last_char_num = 1;
        break;
    case 42:
    case 48:
        cout << "/"; last_char_num = 1;
        break;
    default:
        break;
    }
    return last_char_num;
}
int space(int space_num, int style) { // style ≧ 0: " ", style < 0: endl <<  
    int first_font(int); int last_font(int); int i;
    space_num -= first_font(style);
    if (space_num > 0) {
        for (i = 0; i < space_num; i++) {
            if (style < 0) {
                cout << endl;
            }
            else {
                cout << " ";
            }
        }
    }
    last_font(style);
    return space_num;
}
int bar(int bar_num, int style) { // style ≧ 0: -, style < 0: _ 
    int first_font(int); int last_font(int); int i;
    bar_num -= first_font(style);
    if (bar_num > 0) {
        for (i = 0; i < bar_num; i++) {
            if (style < 0) {
                cout << "_";
            }
            else {
                cout << "-";
            }
        }
    }
    last_font(style);
    return bar_num;
}
int space_line(int line_num, int space_num, int line_style, int edge_style) {
    int space(int, int); int first_font(int); int last_font(int); int i;
    first_font(edge_style);
    if (line_num > 0) {
        for (i = 0; i < line_num; i++) {
            space(space_num, line_style);
        }
    }
    last_font(edge_style);
    return line_num * space_num;
}


