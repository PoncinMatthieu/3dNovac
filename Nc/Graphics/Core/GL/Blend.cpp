
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

Blend::Blend()
    : _blendState(false), _sFactor(Enum::One), _dFactor(Enum::Zero),
      _lastBlendState(false), _lastSFactor(Enum::One), _lastDFactor(Enum::Zero)
{
}

Blend::Blend(Pattern p)
    : _sFactor(Enum::One), _dFactor(Enum::Zero)
{
    SetPattern(p);
}

void Blend::SetPattern(Pattern p)
{
    switch (p)
    {
        case Alpha:
            _sFactor = Enum::SrcAlpha;
            _dFactor = Enum::OneMinusSrcAlpha;
            _blendState = true;
            break;
        case Add:
            _sFactor = Enum::SrcAlpha;
            _dFactor = Enum::One;
            _blendState = true;
            break;
        case Multiply:
            _sFactor = Enum::DstColor;
            _dFactor = Enum::Zero;
            _blendState = true;
            break;
        default:
            _sFactor = Enum::One;
            _dFactor = Enum::Zero;
            _blendState = false;
            break;
    }
}


void Blend::Enable()
{
    if (_blendState != _currentBlendState)
    {
        if (_blendState == true)
            glEnable(Enum::Blend);
        else
            glDisable(Enum::Blend);
        _lastBlendState = _currentBlendState;
        _currentBlendState = _blendState;
    }
    if (_sFactor != _currentSFactor || _dFactor != _currentDFactor)
    {
        glBlendFunc(_sFactor, _dFactor);
        _lastSFactor = _currentSFactor;
        _lastDFactor = _currentDFactor;
        _currentSFactor = _sFactor;
        _currentDFactor = _dFactor;
    }
}

void Blend::Disable()
{
    if (_lastBlendState != _currentBlendState)
    {
        if (_lastBlendState == true)
            glEnable(Enum::Blend);
        else
            glDisable(Enum::Blend);
        _currentBlendState = _lastBlendState;
    }
    if (_lastSFactor != _currentSFactor || _lastDFactor != _currentDFactor)
    {
        glBlendFunc(_lastSFactor, _lastDFactor);
        _currentSFactor = _lastSFactor;
        _currentDFactor = _lastDFactor;
    }
}
