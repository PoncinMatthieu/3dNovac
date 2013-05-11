
/*-----------------------------------------------------------------------------

	3dNovac GUI
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "../Look/Looks.h"
#include "ProgressBar.h"

using namespace Nc;
using namespace Nc::GUI;
using namespace Nc::Graphic;

ProgressBar::ProgressBar(const AlignmentMask &alignment, const Vector2i &size, const std::string &looksName)
    : Widget(alignment, size),
    _progressBox(0, 0, size.data[0], size.data[1]),
    _percent(0)
{
    Init(looksName);
}

ProgressBar::~ProgressBar()
{
}

void    ProgressBar::SetProgressBox(const Box2f &b)
{
    if (_progressBox != b)
    {
        _progressBox = b;
        _size = b.Size();
        _stateChanged = true;
    }
}

void    ProgressBar::SetColor(const Color &c)
{
    if (_colorLeft != c || _colorRight != c)
    {
        _colorLeft = _colorRight = c;
        _stateChanged = true;
    }
}

void    ProgressBar::SetColor(const Color &c1, const Color &c2)
{
    if (_colorLeft != c1 || _colorRight != c2)
    {
        _colorLeft = c1;
        _colorRight = c2;
        _stateChanged = true;
    }
}

void    ProgressBar::Percent(float p)
{
    float newP = (p < 100) ? p : 100.f;
    newP = (newP > 0) ? newP : 0.f;

    if (_percent != newP)
    {
        _percent = newP;
        _stateChanged = true;
    }
}

void    ProgressBar::NbEvolution(unsigned int nb)
{
    _nbEvolution = nb;
}

void    ProgressBar::Evolution()
{
    Percent(_percent + (100.f / (float)_nbEvolution));
}

void ProgressBar::Init(const std::string &looksName)
{
    UseLook(new StripLook(looksName + StyleSheet::Name::ProgressBar));

    _colorLeft.Init(0, 1, 0);
    _colorRight.Init(0, 1, 0);
    _nbEvolution = 1;

    // creation des drawable
    _indexDrawable = _drawables.size();
    GL::GeometryBuffer<GL::DefaultVertexType::Colored2d, false> *geometry1 = new GL::GeometryBuffer<GL::DefaultVertexType::Colored2d, false>(GL::Enum::TriangleStrip);
    geometry1->VBO().Init(4, GL::Enum::DataBuffer::StreamDraw);
    _drawables.push_back(new GL::Drawable(geometry1));
    ChooseDefaultMaterial();
}

void ProgressBar::UpdateState()
{
    Widget::UpdateState();
    Array<GL::DefaultVertexType::Colored2d, 4>   verticesProgress;
    verticesProgress[0].Fill(_progressBox.Min(0) + 1, _progressBox.Min(1) + 1, _colorLeft);
    verticesProgress[1].Fill(_progressBox.Min(0) - 1 + (_percent * _progressBox.Length(0) / 100.f), _progressBox.Min(1) + 1, _colorRight);
    verticesProgress[2].Fill(_progressBox.Min(0) + 1, _progressBox.Max(1) - 1, _colorLeft);
    verticesProgress[3].Fill(_progressBox.Min(0) - 1 + (_percent * _progressBox.Length(0) / 100.f), _progressBox.Max(1) - 1, _colorRight);
    static_cast<GL::GeometryBuffer<GL::DefaultVertexType::Colored2d, false>*>(_drawables[_indexDrawable]->Geometry)->VBO().UpdateData(verticesProgress.data);
}

void ProgressBar::Draw(SceneGraph *scene)
{
    GetMaterial()->Render(scene, *_drawables[_indexDrawable]);
    Widget::Draw(scene);
}
