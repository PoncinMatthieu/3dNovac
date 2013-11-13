
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

#include <Nc/Core/Utils/Utils.h>
#include "FlowControl.h"

using namespace Nc;
using namespace Nc::Net;

FlowControl::FlowControl()
{
    _thresholdRTT = 0.400f;
    _goodCondition = false;
    _penaltyTime = 4.0f;
    _goodConditionTime = 0.0f;
    _penaltyReductionTime = 0.0f;
}

FlowControl::~FlowControl()
{

}

float   FlowControl::SendRate() const
{
    return (_goodCondition) ? 30 : 10;
}

void    FlowControl::Update(float elapsedTime, float clientRTT)
{
    if (_goodCondition)
    {
        // drop to bad mode
        if (clientRTT > _thresholdRTT)
        {
            LOG << "*** dropping to bad mode ***" << std::endl;
            _goodCondition = false;
            if (_goodConditionTime < 10 && _penaltyTime < 60)
            {
                _penaltyTime *= 2;
                if (_penaltyTime > 60)
                    _penaltyTime = 60.0f;
                LOG << "penalty time increased to " << _penaltyTime << std::endl;
            }
            _goodConditionTime = 0.0f;
            _penaltyReductionTime = 0.0f;
            return;
        }

        _goodConditionTime += elapsedTime;
        _penaltyReductionTime += elapsedTime;

        if (_penaltyReductionTime > 10.0f && _penaltyTime > 1.0f)
        {
            _penaltyTime /= 2.0f;
            if (_penaltyTime < 1.0f)
                _penaltyTime = 1.0f;
            LOG << "penalty time reduced to " << _penaltyTime << std::endl;
            _penaltyReductionTime = 0.0f;
        }
    }
    else
    {
        if (clientRTT <= _thresholdRTT)
            _goodConditionTime += elapsedTime;
        else
            _goodConditionTime = 0.0f;

        if (_goodConditionTime > _penaltyTime)
        {
            LOG << "*** upgrading to good mode ***" << std::endl;
            _goodConditionTime = 0.0f;
            _penaltyReductionTime = 0.0f;
            _goodCondition = true;
        }
    }
}
