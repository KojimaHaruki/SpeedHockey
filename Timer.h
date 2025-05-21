#pragma once
#include "DxLib.h"
struct hms {
	int h; // hour
	int m; // min
	int s; // second
	double t; // total second
	int ms; // millisecond
	int mt; // total millisecond
};
class Timer {
public:
	Timer();
	~Timer();
	hms Gethms(int t_ms);
	hms GetLapseTime() { return Lapse; };
	int GetLapseCount() { return LapseTime; }
	int DrawLapseTime(int x, int y, unsigned int Color, int Mode = 0);
	int DrawLapseTimeToHandle(int x, int y, unsigned int Color, int FontHandle, int Mode = 0);
	void Update();
	void Reset() { TimeLapse = false; LapseTime = 0; };
	void Start() { TimeLapse = true; StartTime = GetNowCount(); };
	void Stop() { TimeLapse = false; };
	void Resume() { TimeLapse = true; StartTime = GetNowCount() - LapseTime; };
	struct Mode {
		static constexpr int HMSmS = 0;
		static constexpr int MSmS = 1;
		static constexpr int SmS = 2;
		static constexpr int mS = 3;
		static constexpr int HMS = 4;
		static constexpr int MS = 5;
		static constexpr int S = 6;
		static constexpr int HM = 7;
	};
private:
	int StartTime;
	hms Lapse;
	bool TimeLapse;
	int LapseTime;
};



