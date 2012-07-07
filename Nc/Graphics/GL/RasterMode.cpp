
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

    File Created At:        04/06/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "RasterMode.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::Graphic::GL;

RasterMode::RasterMode()
    : _setPolygonMode(false), _polygonFace(Enum::FrontAndBack), _polygonMode(Enum::Fill),
      _setPolygonOffset(false), _polygonOffsetFactor(0.f), _polygonOffsetUnits(0.f),
      _setPointSize(false), _pointSize(1.f),
      _setLineWidth(false), _lineWidth(1.f),
      _setDepthTest(false), _depthTest(true), _depthMask(true), _lastDepthTestState(true), _lastDepthMaskState(true)
{
}

RasterMode::RasterMode(Enum::PolygonFace face, Enum::PolygonMode mode)
    : _setPolygonMode(false), _polygonFace(face), _polygonMode(mode),
      _setPolygonOffset(false), _polygonOffsetFactor(0.f), _polygonOffsetUnits(0.f),
      _setPointSize(false), _pointSize(1.f),
      _setLineWidth(false), _lineWidth(1.f),
      _setDepthTest(false), _depthTest(true), _depthMask(true), _lastDepthTestState(true), _lastDepthMaskState(true)
{
}

void    RasterMode::Enable()
{
    State &s = State::Current();
    if (_setPolygonMode)
    {
        _lastPolygonFace = s.CurrentPolygonFace();
        _lastPolygonMode = s.CurrentPolygonMode();
        s.PolygonMode(_polygonFace, _polygonMode);
    }
    if (_setPolygonOffset)
    {
        _lastPolygonOffsetFactor = s.CurrentPolygonOffsetFactor();
        _lastPolygonOffsetUnits = s.CurrentPolygonOffsetUnits();
        s.PolygonOffset(_polygonOffsetFactor, _polygonOffsetUnits);
    }
    if (_setPointSize)
    {
        _lastPointSize = s.CurrentPointSize();
        s.PointSize(_pointSize);
    }
    if (_setLineWidth)
    {
        _lastLineWidth = s.CurrentLineWidth();
        s.LineWidth(_lineWidth);
    }
    if (_setDepthTest)
    {
        if (s.Enabled(Enum::DepthTest) != _depthTest)
        {
            _lastDepthTestState = !_depthTest;
            if (_depthTest)
                s.Enable(Enum::DepthTest);
            else
                s.Disable(Enum::DepthTest);
        }
        if (s.CurrentDepthMask() != _depthMask)
        {
            _lastDepthMaskState = !_depthMask;
            s.DepthMask(_depthMask);
        }
    }
}

void    RasterMode::Disable()
{
    State &s = State::Current();
    if (_setPolygonMode)
        s.PolygonMode(_lastPolygonFace, _lastPolygonMode);
    if (_setPolygonOffset)
        s.PolygonOffset(_lastPolygonOffsetFactor, _lastPolygonOffsetUnits);
    if (_setPointSize)
        s.PointSize(_lastPointSize);
    if (_setLineWidth)
        s.LineWidth(_lastLineWidth);
    if (_setDepthTest)
    {
        if (_lastDepthTestState)
            s.Enable(Enum::DepthTest);
        else
            s.Disable(Enum::DepthTest);
        s.DepthMask(_lastDepthMaskState);
    }
}
