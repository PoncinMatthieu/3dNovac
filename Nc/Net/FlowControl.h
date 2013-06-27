
/*-----------------------------------------------------------------------------

	3dNovac Net
	Copyright (C) 2010-2013, The 3dNovac Team

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

    File Created At:        25/06/2013
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_NET_FLOWCONTROL_H_
#define NC_NET_FLOWCONTROL_H_

#include <Nc/Core/Utils/NonCopyable.h>
#include "Define.h"

namespace Nc
{
    namespace Net
    {
        /// Implementation of a simple binary flow control.
        /**
            The condition of a client is based on the metric of his RTT.
            In bad condition, the rate drop down to 10 packet per second.
            In good condition, the rate is up to 30 packet per second.
        */
        class LIB_NC_NET FlowControl : Utils::NonCopyable
        {
            public:
                FlowControl();
                ~FlowControl();

                /** Update the rtt of the client. */
                void    Update(float elapsedTime, float clientRTT);

                /** \return the send rate the application should send over the network to avoid flooding the connection. */
                float   SendRate() const;

            private:
                bool        _goodCondition;                 ///< true if we are in good condition.
                float       _thresholdRTT;                  ///< RTT value on which we will drop in bad condition.
                float       _penaltyTime;
                float       _goodConditionTime;
                float       _penaltyReductionTime;
        };
    }
}

#endif
