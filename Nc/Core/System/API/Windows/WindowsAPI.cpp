
#include <Windows.h>

#include "../SystemAPI.h"

using namespace Nc;

void    System::Sleep(float second)
{ 
    ::Sleep(static_cast<DWORD>(1000 * second));
}

double  System::Time()
{
	static LARGE_INTEGER Frequency;
    static BOOL          UseHighPerformanceTimer = QueryPerformanceFrequency(&Frequency);

    if (UseHighPerformanceTimer)
    {
        // High performance counter available : use it
        LARGE_INTEGER CurrentTime;
        QueryPerformanceCounter(&CurrentTime);
        return static_cast<double>(CurrentTime.QuadPart) / Frequency.QuadPart;
    }
    else // High performance counter not available
        return GetTickCount() * 0.001;        
}

char* System::Strdup(const char *str)
{
    unsigned int len = strlen(str) + 1;
    char *r = (char*)malloc(sizeof(char) * len);
    memcpy(r, str, len);
    return r;
}

unsigned int System::ThreadId()
{
	return GetCurrentThreadId();
}
