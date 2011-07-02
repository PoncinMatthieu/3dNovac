
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        19/06/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Blend.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::Graphic::GL;

bool                Blend::_currentBlendState = false;
Enum::BlendFactor   Blend::_currentSFactor = Enum::One;
Enum::BlendFactor   Blend::_currentDFactor = Enum::Zero;

void Blend::Enable()
{
    if (_pattern != Disabled)
    {
        if (!_currentBlendState)
        {
            glEnable(Enum::Blend);
            _lastBlendState = _currentBlendState;
            _currentBlendState = true;
        }

        Enum::BlendFactor sfactor = Enum::One, dfactor = Enum::Zero;
        switch (_pattern)
        {
            case Alpha:
                sfactor = Enum::SrcAlpha;
                dfactor = Enum::OneMinusSrcAlpha;
                break;
            case Add:
                sfactor = Enum::SrcAlpha;
                dfactor = Enum::One;
                break;
            case Multiply:
                sfactor = Enum::DstColor;
                dfactor = Enum::Zero;
                break;
            default:
                sfactor = Enum::One;
                dfactor = Enum::Zero;
                break;
        }

        if (sfactor != _currentSFactor || dfactor != _currentDFactor)
        {
            glBlendFunc(sfactor, dfactor);
            _lastSFactor = _currentSFactor;
            _lastDFactor = _currentDFactor;
            _currentSFactor = sfactor;
            _currentDFactor = dfactor;
        }
    }
}

void Blend::Disable()
{
    if (_pattern != Disabled)
    {
        if (!_lastBlendState)
        {
            glDisable(Enum::Blend);
            _currentBlendState = false;
        }
        if (_currentSFactor != _lastSFactor || _currentDFactor != _lastDFactor)
        {
            glBlendFunc(_lastSFactor, _lastDFactor);
            _currentSFactor = _lastSFactor;
            _currentDFactor = _lastDFactor;
        }
    }
}
