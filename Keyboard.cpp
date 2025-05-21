#include <DxLib.h>
#include "Keyboard.h"

Keyboard::Keyboard() {
    memset(mKeyPressingCount, 0, sizeof(mKeyPressingCount));
    memset(mKeyReleasingCount, 0, sizeof(mKeyReleasingCount));
}

//更新
bool Keyboard::Update() {
    char nowKeyStatus[KEY_NUM];
    GetHitKeyStateAll(nowKeyStatus);       //今のキーの入力状態を取得
    for (int i = 0; i < KEY_NUM; i++) {
        if (nowKeyStatus[i] != 0) {            //i番のキーが押されていたら
            if (mKeyReleasingCount[i] > 0) {//離されカウンタが0より大きければ
                mKeyReleasingCount[i] = 0;   //0に戻す
            }
            mKeyPressingCount[i]++;          //押されカウンタを増やす
        }
        else {                             //i番のキーが離されていたら
            if (mKeyPressingCount[i] > 0) { //押されカウンタが0より大きければ
                mKeyPressingCount[i] = 0;    //0に戻す
            }
            mKeyReleasingCount[i]++;         //離されカウンタを増やす
        }
    }
    return true;
}
int Keyboard::GetPressingCount(int KeyCode) {
    if (!Keyboard::IsAvailableCode(KeyCode)) {
        return -1;
    }
    return mKeyPressingCount[KeyCode];
}
int Keyboard::GetReleasingCount(int KeyCode) {
    if (!Keyboard::IsAvailableCode(KeyCode)) {
        return -1;
    }
    return mKeyReleasingCount[KeyCode];
}
bool Keyboard::IsAvailableCode(int KeyCode) {
    if (!(0 <= KeyCode && KeyCode < KEY_NUM)) {
        return false;
    }
    return true;
}