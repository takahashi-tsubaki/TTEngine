#include "FPS.h"

void FPS::FpsControlBegin()
{
	//周波数取得
	QueryPerformanceFrequency(&cpuClock);
	//計測開始時間の初期化
	QueryPerformanceCounter(&timeStart);
}

void FPS::FpsControlEnd()
{
	//今の時間を取得
	QueryPerformanceCounter(&timeEnd);
	//経過時間
	elapsedFrame_ = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(cpuClock.QuadPart);
	//余裕があるときは待つ
	if (elapsedFrame_ < frameTime)
	{
		//sleep時間
		DWORD sleepTime = static_cast<DWORD>((frameTime - elapsedFrame_) * 1000.0f);
		timeBeginPeriod(1);
		//寝る
		Sleep(sleepTime);
		timeEndPeriod(1);
	}

	fps = 1 / elapsedFrame_;
}

void FPS::SetFrameRate(float fps_)
{
	frameTime = 1.0f / fps_;
}

float FPS::GetFrameRate()
{
	return fps;
}

float FPS::GetElapsedFrame()
{
	return elapsedFrame_;
}

void FPS::SetElapsedFrame(float elapsedFrame)
{
	elapsedFrame_ = elapsedFrame; }

FPS* FPS::GetInstance() {
	static FPS instance;
	return &instance;
}

