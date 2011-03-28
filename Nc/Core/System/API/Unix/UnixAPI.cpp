
/*--------------------------------------------------------------------------------

	3dNovac Utils
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

--------------------------------------------------------------------------------*/

#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include "../API.h"

using namespace Nc;
using namespace Nc::Utils;

void    System::Sleep(float second)
{
    usleep(static_cast<unsigned long>(second * 1000000));
}

double  System::Time()
{
    struct timeval  t = {0, 0};

    if (gettimeofday(&t, NULL) == -1)
        throw Exception("System::Time", strerror(errno));
	return t.tv_sec + (((double)t.tv_usec)*1.e-6);
}

char* System::Strdup(const char *str)
{
    return strdup(str);
}

unsigned int System::ThreadId()
{
	return pthread_self();
}
