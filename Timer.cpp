#include "Timer.h"
Timer::Timer() : StartTime(0), Lapse(), TimeLapse(false), LapseTime(0) {}
void Timer::Update() { if (!TimeLapse) return; LapseTime = GetNowCount() - StartTime; Lapse = Gethms(LapseTime); }
hms Timer::Gethms(int t_ms) {
	hms time; div_t res_h, res_m, res_s;
	res_h = div(t_ms, 3600000); time.h = res_h.quot;
	res_m = div(res_h.rem, 60000); time.m = res_m.quot;
	res_s = div(res_m.rem, 1000); time.s = res_s.quot;
	time.ms = res_s.rem; time.mt = t_ms; time.t = 0.001 * t_ms;
	return time;
}
int Timer::DrawLapseTime(int x, int y, unsigned int Color, int Mode) {
	switch (Mode) {
	case Mode::HMSmS: DrawFormatString(x, y, Color, "%2d:%2d:%2d.%3d", Lapse.h, Lapse.m, Lapse.s, Lapse.ms); break;
	case Mode::MSmS: DrawFormatString(x, y, Color, "%2d:%2d.%3d", 60 * Lapse.h + Lapse.m, Lapse.s, Lapse.ms); break;
	case Mode::SmS: DrawFormatString(x, y, Color, "%2d.%3d", 3600 * Lapse.h + 60 * Lapse.m + Lapse.s, Lapse.ms); break;
	case Mode::mS: DrawFormatString(x, y, Color, "%d ms", 3600000 * Lapse.h + 60000 * Lapse.m + 1000 * Lapse.s + Lapse.ms); break;
	case Mode::HMS: DrawFormatString(x, y, Color, "%2d:%2d:%2d", Lapse.h, Lapse.m, Lapse.s); break;
	case Mode::MS: DrawFormatString(x, y, Color, "%2d:%2d", 60 * Lapse.h + Lapse.m, Lapse.s); break;
	case Mode::S: DrawFormatString(x, y, Color, "%d s", 3600 * Lapse.h + 60 * Lapse.m + Lapse.s); break;
	case Mode::HM: DrawFormatString(x, y, Color, "%2d:%2d", Lapse.h, Lapse.m); break;
	default: return -1;
	} return 0;
}
int Timer::DrawLapseTimeToHandle(int x, int y, unsigned int Color, int FontHandle, int Mode) {
	switch (Mode) {
	case Mode::HMSmS: DrawFormatStringToHandle(x, y, Color, FontHandle, "%2d:%2d:%2d.%3d", Lapse.h, Lapse.m, Lapse.s, Lapse.ms); break;
	case Mode::MSmS: DrawFormatStringToHandle(x, y, Color, FontHandle, "%2d:%2d.%3d", 60 * Lapse.h + Lapse.m, Lapse.s, Lapse.ms); break;
	case Mode::SmS: DrawFormatStringToHandle(x, y, Color, FontHandle, "%2d.%3d", 3600 * Lapse.h + 60 * Lapse.m + Lapse.s, Lapse.ms); break;
	case Mode::mS: DrawFormatStringToHandle(x, y, Color, FontHandle, "%d ms",
		3600000 * Lapse.h + 60000 * Lapse.m + 1000 * Lapse.s + Lapse.ms); break;
	case Mode::HMS: DrawFormatStringToHandle(x, y, Color, FontHandle, "%2d:%2d:%2d", Lapse.h, Lapse.m, Lapse.s); break;
	case Mode::MS: DrawFormatStringToHandle(x, y, Color, FontHandle, "%2d:%2d", 60 * Lapse.h + Lapse.m, Lapse.s); break;
	case Mode::S: DrawFormatStringToHandle(x, y, Color, FontHandle, "%d s", 3600 * Lapse.h + 60 * Lapse.m + Lapse.s); break;
	case Mode::HM: DrawFormatStringToHandle(x, y, Color, FontHandle, "%2d:%2d", Lapse.h, Lapse.m); break;
	default: return -1;
	} return 0;
}
Timer::~Timer() {}