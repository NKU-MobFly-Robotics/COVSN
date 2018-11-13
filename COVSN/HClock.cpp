#include "HClock.h"
HClock::HClock()
{

}

HClock::~HClock()
{

}

void HClock::StartClock(void)
{
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nStart);
}
void HClock::EndClock(void)
{
	QueryPerformanceCounter(&nEnd);
}
double HClock::GetTime(void)
{
	return(1000.0*(double)(nEnd.QuadPart - nStart.QuadPart) / (double)nFreq.QuadPart);
}