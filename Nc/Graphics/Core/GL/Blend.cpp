
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
    State &s = State::Current();
    _lastBlendState = s.Enabled(Enum::Blend);
    if (_blendState == true)
        s.Enable(Enum::Blend);
    else
        s.Disable(Enum::Blend);
    _lastSFactor = s.CurrentBlendSFactor();
    _lastDFactor = s.CurrentBlendDFactor();
    s.BlendFunc(_sFactor, _dFactor);
}

void Blend::Disable()
{
    State &s = State::Current();
    if (_lastBlendState == true)
        s.Enable(Enum::Blend);
    else
        s.Disable(Enum::Blend);
    s.BlendFunc(_lastSFactor, _lastDFactor);
}
