
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
        /// a multi-archi Clock, providing elapsedTime in second.
		/**
			The clock can be paused, reset and restart.

			The clock is reset a first time in the constructor,
			you can get the elapsed time since the last reset of the clock by calling the method ElapsedTime.
		*/
        class LCORE Clock
        {
            public:
                Clock()     {Reset();}
                ~Clock()	{};

                friend LCORE std::ostream &operator << (std::ostream &os, const Clock &cl);

                /** Reset the clock. */
                inline void Reset()                     {_paused = false; _pauseTime = 0; _startTime = System::Time();}

                /** Pause the clock, next call restart to pause it. */
                inline void Pause()                     {_paused = true; _startPause = System::Time();}
                /** Restart the clock after you have pause it. */
                inline void Restart()                   {if (_paused) {_paused = false; _pauseTime += (System::Time() - _startPause);}}
                /** \return the total paused time. */
                inline double PauseTime() const         {return _pauseTime + ((_paused) ? ((System::Time() - _startPause)) : 0);}
                /** \return true if the clock is paused. */
                inline bool Paused()                    {return _paused;}

                /** \return the ElapsedTime in second since the last Reset minus the paused time. */
                inline double ElapsedTime() const       {return System::Time() - _startTime - PauseTime();}


            private:
                double  _startTime;         ///< The time when the clock was started.
                double  _startPause;        ///< The time when the clock was paused.
                bool    _paused;            ///< true if the clock is currently paused.
                double  _pauseTime;         ///< The total of time in pause.
        };
    }
}

#endif
