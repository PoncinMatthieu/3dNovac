
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

GLenum      RasterMode::_currentFace = GL_FRONT_AND_BACK;
GLenum      RasterMode::_currentMode = GL_FILL;
GLfloat     RasterMode::_currentPointSize = 1.f;
GLfloat     RasterMode::_currentLineWidth = 1.f;
GLfloat     RasterMode::_currentPolygonOffsetFactor = 0.f;
GLfloat     RasterMode::_currentPolygonOffsetUnits = 0.f;
bool        RasterMode::_currentDepthTest = true;

RasterMode::RasterMode()
    : _setPolygonMode(false), _face(GL_FRONT_AND_BACK), _mode(GL_FILL),
      _setPointSize(false), _pointSize(1.f),
      _setLineWidth(false), _lineWidth(1.f),
      _setPolygonOffset(false), _polygonOffsetFactor(0.f), _polygonOffsetUnits(0.f),
      _setDepthTest(false), _depthTest(true), _lastDepthTestState(true)
{
}

RasterMode::RasterMode(GLenum face, GLenum mode)
    : _setPolygonMode(false), _face(face), _mode(mode),
      _setPointSize(false), _pointSize(1.f),
      _setLineWidth(false), _lineWidth(1.f),
      _setPolygonOffset(false), _polygonOffsetFactor(0.f), _polygonOffsetUnits(0.f),
      _setDepthTest(false), _depthTest(true), _lastDepthTestState(true)
{
}

void    RasterMode::Enable()
{
    if (_setPolygonMode && (_face != _currentFace || _mode != _currentMode))
    {
        glPolygonMode(_face, _mode);
        _currentFace = _face;
        _currentMode = _mode;
    }
    if (_setPolygonOffset && (_polygonOffsetFactor != _currentPolygonOffsetFactor || _polygonOffsetUnits != _currentPolygonOffsetUnits))
    {
        glPolygonOffset(_polygonOffsetFactor, _polygonOffsetUnits);
        _currentPolygonOffsetFactor = _polygonOffsetFactor;
        _currentPolygonOffsetUnits = _polygonOffsetUnits;
    }
    if (_setPointSize && _pointSize != _currentPointSize)
    {
        glPointSize(_pointSize);
        _currentPointSize = _pointSize;
    }
    if (_setLineWidth && _lineWidth != _currentLineWidth)
    {
        glLineWidth(_lineWidth);
        _currentLineWidth = _lineWidth;
    }
    if (_setDepthTest && _currentDepthTest != _depthTest)
    {
        if (_depthTest)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
        _lastDepthTestState = _currentDepthTest;
        _currentDepthTest = _depthTest;
    }
}

void    RasterMode::Disable()
{
    if (_setPolygonMode && GL_FILL != _currentMode)
    {
        glPolygonMode(_currentFace, GL_FILL);
        _currentMode = GL_FILL;
    }
    if (_setPolygonOffset && (_currentPolygonOffsetFactor != 0 || _currentPolygonOffsetUnits != 0))
    {
        glPolygonOffset(0.f, 0.f);
        _currentPolygonOffsetFactor = 0.f;
        _currentPolygonOffsetUnits = 0.f;
    }
    if (_setPointSize && _currentPointSize != 1.f)
    {
        glPointSize(1);
        _currentPointSize = 1;
    }
    if (_setLineWidth && _currentLineWidth != 1.f)
    {
        glLineWidth(1);
        _currentLineWidth = 1;
    }
    if (_setDepthTest && _currentDepthTest != _lastDepthTestState)
    {
        if (_lastDepthTestState)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
        _currentDepthTest = _lastDepthTestState;
    }
}
