#ifndef HCLOCK_H_
#define HCLOCK_H_
#include <windows.h>
class HClock
{
public:
	HClock();
	~HClock();
public:
	void StartClock(void);
	void EndClock(void);
	double GetTime(void);//in ms
protected:
private:
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nStart;
	LARGE_INTEGER nEnd;
};
#endif