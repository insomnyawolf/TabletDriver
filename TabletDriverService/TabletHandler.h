#pragma once
#include <thread>
#include <mutex>
#include "Tablet.h"
#include "InputEmulator.h"
#include "Runnable.h"

class TabletHandler : public Runnable {
private:
	Vector2D lastScrollPosition;
	Vector2D scrollStartPosition;
	void ProcessPenButtons(UINT32 *outButtons);
	void ProcessAuxButtons(UINT32 *outButtons);
	void ProcessButtons(UINT32 *outButtons, bool isPen);
	bool IsButtonDown(UINT32 buttons, int buttonIndex);
	bool IsButtonPressed(UINT32 buttons, UINT32 lastButtons, int buttonIndex);
	bool IsButtonReleased(UINT32 buttons, UINT32 lastButtons, int buttonIndex);

public:
	Tablet *tablet;
	thread *tabletInputThread;
	thread *auxInputThread;
	HANDLE timer;
	double timerInterval;
	mutex lock;
	chrono::high_resolution_clock::time_point timeBegin;
	chrono::high_resolution_clock::time_point timeLastTimerProblem;
	TabletState outputState;
	TabletState outputStateWrite;
	InputEmulator inputEmulator;

	bool isTimerTickRunning;

	TabletHandler();
	~TabletHandler();

	bool Start();
	bool Stop();
	bool StartTimer();
	bool StopTimer();
	void ChangeTimerInterval(int newInterval);
	void RunTabletInputThread();
	void RunAuxInputThread();
	void OnTimerTick();
	void WriteOutputState(TabletState *outputState);
	


private:
	static VOID CALLBACK TimerCallback(_In_ PVOID lpParameter, _In_ BOOLEAN TimerOrWaitFired) {
		TabletHandler *tabletHandler = (TabletHandler *)lpParameter;
		tabletHandler->OnTimerTick();
	}
};

