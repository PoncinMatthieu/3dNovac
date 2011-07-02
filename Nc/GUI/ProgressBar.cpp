
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

#include "ProgressBar.h"

using namespace Nc;
using namespace Nc::GUI;
using namespace Nc::Graphic;

ProgressBar::ProgressBar(const Vector2f &pos, const Vector2f &size, Corner x, Corner y, const Utils::FileName &file)
    : Widget(ClassName(), pos, size, x, y),
    _progressBox(0, 0, size.Data[0], size.Data[1]),
    _percent(0)
{
    _colorLeft.Init(0, 1, 0);
    _colorRight.Init(0, 1, 0);
    _nbEvolution = 1;

    // creation des drawable
    _indexDrawable = _drawables.size();
    GL::GeometryBuffer<DefaultVertexType::Colored2d, false> *geometry1 = new GL::GeometryBuffer<DefaultVertexType::Colored2d, false>(GL::Enum::TriangleStrip);
    geometry1->VBO().Init(4, GL::Enum::StreamDraw);
    _drawables.push_back(new Drawable(geometry1));

    GL::GeometryBuffer<DefaultVertexType::Textured2d, false> *geometry2 = new GL::GeometryBuffer<DefaultVertexType::Textured2d, false>(GL::Enum::TriangleStrip);
    geometry2->VBO().Init(4, GL::Enum::StreamDraw);
    Drawable *dr = new Drawable(geometry2);
    dr->Config->Textures.InitSize(1);
    dr->Config->Textures[0].LoadFromFile(file);
    dr->Config->SetBlend(GL::Blend::Alpha);
    _drawables.push_back(dr);
    ChooseDefaultMaterial();
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::Update()
{
    Array<DefaultVertexType::Colored2d, 4>   verticesProgress;
    verticesProgress[0].Fill(_progressBox.Min(0), _progressBox.Min(1), _colorLeft);
    verticesProgress[1].Fill(_progressBox.Min(0) + (_percent * _progressBox.Length(0) / 100.f), _progressBox.Min(1), _colorRight);
    verticesProgress[2].Fill(_progressBox.Min(0), _progressBox.Max(1), _colorLeft);
    verticesProgress[3].Fill(_progressBox.Min(0) + (_percent * _progressBox.Length(0) / 100.f), _progressBox.Max(1), _colorRight);
    static_cast<GL::GeometryBuffer<DefaultVertexType::Colored2d, false>*>(_drawables[_indexDrawable]->Geometry)->VBO().UpdateData(verticesProgress.Data);

    Color   c(1,1,1);
    Array<DefaultVertexType::Textured2d, 4>   verticesTexture;
    verticesTexture[0].Fill(0, 0, 0, 0, c);
    verticesTexture[1].Fill(_size.Data[0], 0, 1, 0, c);
    verticesTexture[2].Fill(0, _size.Data[1], 0, 1, c);
    verticesTexture[3].Fill(_size.Data[0], _size.Data[1], 1, 1, c);
    static_cast<GL::GeometryBuffer<DefaultVertexType::Textured2d, false>*>(_drawables[_indexDrawable+1]->Geometry)->VBO().UpdateData(verticesTexture.Data);
}

void ProgressBar::Draw(SceneGraph *scene)
{
    Widget::Draw(scene);
    GetMaterial()->Render(scene, *_drawables[_indexDrawable]);
    GetMaterial()->Render(scene, *_drawables[_indexDrawable+1]);
}
