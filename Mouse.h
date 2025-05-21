#pragma once

#include "Singleton.h"

class Mouse : public Singleton<Mouse> {

    Mouse();
    friend Singleton< Mouse >;
    private:
        static constexpr int KEY_NUM = 8;	//キー総数
        int _x, _y;
        int _buttonPressingCount[KEY_NUM];//押されカウンタ
        int _buttonReleasingCount[KEY_NUM];//離されカウンタ
        bool isAvailableCode(int keyCode);//keyCodeが有効なキー番号か問う

    public:
        static constexpr int LEFT_CLICK = 0;
        static constexpr int RIGHT_CLICK = 1;
        bool Update();	//更新
        int GetPressingCount(int keyCode);//keyCodeのキーが押されているフレーム数を取得
        int GetReleasingCount(int keyCode);//keyCodeのキーが離されているフレーム数を取得
        int getX() { return _x; }
        int getY() { return _y; }
};
