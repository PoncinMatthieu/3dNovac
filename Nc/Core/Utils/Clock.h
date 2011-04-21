
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, The 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/


#ifndef NC_CORE_UTILS_CLOCK_H_
#define NC_CORE_UTILS_CLOCK_H_

#include <iostream>
#include <string.h>
#include <sys/timeb.h>

#include "../System/API/API.h"

namespace Nc
{
    namespace Utils
    {
        /// a multi-archi Clock, provide elapsedTime in second
        class LCORE Clock
        {
            public:
                Clock()	    {Reset();}
                ~Clock()	{};

                friend std::ostream &operator << (std::ostream &os, const Clock &cl)
                {
                    os << "time = " << cl.ElapsedTime() << "s";
                    return os;
                }

                /// Reset the clock
                inline void Reset()                     {_pauseTime = 0; _startTime = System::Time();}

                /// Pause the clock, next call restart to pause it
                inline void Pause()                     {_startPause = System::Time() - _startTime;}

                /// Restart the clock after you have pause it
                inline void Restart()                   {_pauseTime += System::Time() - _startPause;}

                /// Get ElapsedTime in second since the last Reset
                inline double ElapsedTime() const       {return System::Time() - _startTime - _pauseTime;}

                /// Get the pause time
                inline double PauseTime() const         {return _pauseTime;}

            private:
                double  _startTime;         ///< The time when the clock was started
                double  _startPause;        ///< The time when the clock was paused
                double  _pauseTime;         ///< The totol of time in pause
        };
    }
}

#endif
