#pragma once

#include "Singleton.h"

class Keyboard : public Singleton<Keyboard> {

	Keyboard();
	friend Singleton< Keyboard >;

public:
	bool Update();
	int GetPressingCount(int KeyCode);
	int GetReleasingCount(int KeyCode);

private:
	static const int KEY_NUM = 256;
	int mKeyPressingCount[KEY_NUM];
	int mKeyReleasingCount[KEY_NUM];

	bool IsAvailableCode(int KeyCode);
};