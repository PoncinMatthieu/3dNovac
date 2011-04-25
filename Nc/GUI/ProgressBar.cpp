
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

ProgressBar::ProgressBar(const Vector2f &pos, const Vector2f &size, Corner x, Corner y, Widget *parent, const Utils::FileName &file)
    : Widget(pos, size, x, y, parent),
    _materialTexture(&Material<BasicVertexType::Textured2d>::Instance()),
    _progressBox(0, 0, size.Data[0], size.Data[1]),
    _percent(0)
{
    _colorLeft.Init(0, 1, 0);
    _colorRight.Init(0, 1, 0);
    _nbEvolution = 1;

    _drawableProgress.GetVBO().Init(4, GL_STREAM_DRAW);
    _materialColored2d->Configure(_drawableProgress);
    _drawableProgress.SetPrimitiveType(GL_TRIANGLE_STRIP);

    _drawableTexture.GetVBO().Init(4, GL_STREAM_DRAW);
    _materialTexture->Configure(_drawableTexture);
    _drawableTexture.SetPrimitiveType(GL_TRIANGLE_STRIP);
    _drawableTexture.texture.LoadFromFile(file);
    _drawableTexture.SetBlend(GL::Blend::Alpha);
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::Update()
{
    Array<BasicVertexType::Colored2d, 4>   verticesProgress;
    verticesProgress[0].Fill(_progressBox.Min(0), _progressBox.Min(1), _colorLeft);
    verticesProgress[1].Fill(_progressBox.Min(0) + (_percent * _progressBox.Length(0) / 100.f), _progressBox.Min(1), _colorRight);
    verticesProgress[2].Fill(_progressBox.Min(0), _progressBox.Max(1), _colorLeft);
    verticesProgress[3].Fill(_progressBox.Min(0) + (_percent * _progressBox.Length(0) / 100.f), _progressBox.Max(1), _colorRight);
    _drawableProgress.GetVBO().UpdateData(verticesProgress.Data);

    Array<BasicVertexType::Textured2d, 4>   verticesTexture;
    Color   c(1,1,1);
    verticesTexture[0].Fill(0, 0, 0, 0, c);
    verticesTexture[1].Fill(_size.Data[0], 0, 1, 0, c);
    verticesTexture[2].Fill(0, _size.Data[1], 0, 1, c);
    verticesTexture[3].Fill(_size.Data[0], _size.Data[1], 1, 1, c);
    _drawableTexture.GetVBO().UpdateData(verticesTexture.Data);
}

void ProgressBar::Draw(ISceneGraph *scene)
{
    _materialColored2d->Render(scene, _drawableProgress);
    _materialTexture->Render(scene, _drawableTexture);
}
