
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

bool        Blend::_currentBlendState = false;
GLenum      Blend::_currentSFactor = GL_ONE;
GLenum      Blend::_currentDFactor = GL_ZERO;

void Blend::Enable()
{
    if (_pattern != Disabled)
    {
        if (!_currentBlendState)
        {
            glEnable(GL_BLEND);
            _lastBlendState = _currentBlendState;
            _currentBlendState = true;
        }

        GLenum sfactor = GL_ONE, dfactor = GL_ZERO;
        switch (_pattern)
        {
            case Alpha:
                sfactor = GL_SRC_ALPHA;
                dfactor = GL_ONE_MINUS_SRC_ALPHA;
                break;
            case Add:
                sfactor = GL_SRC_ALPHA;
                dfactor = GL_ONE;
                break;
            case Multiply:
                sfactor = GL_DST_COLOR;
                dfactor = GL_ZERO;
                break;
            default:
                sfactor = GL_ONE;
                dfactor = GL_ZERO;
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
            glDisable(GL_BLEND);
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
