
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

#include "WindowBox.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::GUI;
using namespace Nc::Graphic;

WindowBox::WindowBox(const std::string &title, const std::string &ttf)
    : Widget(ClassName())
{
    Init(title, ttf);
}

WindowBox::WindowBox(const std::string &title, const Vector2f &pos, const Vector2f &size, Corner x, Corner y, Widget *parent)
    : Widget(ClassName(), pos, size, x, y)
{
    Init(title, "arial");
}

WindowBox::WindowBox(const char *className, const std::string &title, const std::string &ttf)
    : Widget(className)
{
    Init(title, ttf);
}

WindowBox::WindowBox(const char *className, const std::string &title, const Vector2f &pos, const Vector2f &size, Corner x, Corner y, Widget *parent)
    : Widget(className, pos, size, x, y)
{
    Init(title, "arial");
}

void WindowBox::ToString(std::ostream &os) const
{
    Widget::ToString(os);
    os << " Title: " << _title->Text();
}

void  WindowBox::Init(const std::string &title, const std::string &ttf)
{
    _titleHeight = 15;
    _title = NULL;
    ChangeTitle(title, ttf);

    //_dragable = true;
    _drawTitle = true;
    _titleColor1 = Color(1.f, 0.788235294f, 0.48627451f);
    _titleColor2 = Color(0, 0, 0);

    _indexDrawable = _drawables.size();
    _drawables.reserve(_indexDrawable+3);
    GL::GeometryBuffer<DefaultVertexType::Colored2d, false> *geometry1 = new GL::GeometryBuffer<DefaultVertexType::Colored2d, false>(GL_TRIANGLE_STRIP);
    GL::GeometryBuffer<DefaultVertexType::Colored2d, false> *geometry2 = new GL::GeometryBuffer<DefaultVertexType::Colored2d, false>(GL_TRIANGLE_STRIP);
    GL::GeometryBuffer<DefaultVertexType::Colored2d, false> *geometry3 = new GL::GeometryBuffer<DefaultVertexType::Colored2d, false>(GL_LINE_LOOP);
    geometry1->VBO().Init(4, GL_STREAM_DRAW);
    geometry2->VBO().Init(4, GL_STREAM_DRAW);
    geometry3->VBO().Init(4, GL_STREAM_DRAW);
    _drawables.push_back(new Drawable(geometry1));
    _drawables.push_back(new Drawable(geometry2));
    _drawables.push_back(new Drawable(geometry3));
    ChooseDefaultMaterial();
    _stateChanged = true;
}


WindowBox::~WindowBox()
{
    delete _title;
}

WindowBox::WindowBox(const WindowBox &w)
    : Widget(w)
{
    Copy(w);
}

WindowBox &WindowBox::operator = (const WindowBox &w)
{
    if (_title != NULL)
        delete _title;

    Widget::operator = (w);
    Copy(w);
    return *this;
}

void    WindowBox::Copy(const WindowBox &w)
{
    _color = w._color;
    //_dragable = w._dragable;
    _title = new Graphic::String(*w._title);
    _titleColor1 = w._titleColor1;
    _titleColor2 = w._titleColor2;
    _titleHeight = w._titleHeight;
    _drawTitle = w._drawTitle;

    _indexDrawable = _drawables.size();
    _drawables.reserve(_indexDrawable+3);
    GL::GeometryBuffer<DefaultVertexType::Colored2d, false> *geometry1 = new GL::GeometryBuffer<DefaultVertexType::Colored2d, false>(GL_TRIANGLE_STRIP);
    GL::GeometryBuffer<DefaultVertexType::Colored2d, false> *geometry2 = new GL::GeometryBuffer<DefaultVertexType::Colored2d, false>(GL_TRIANGLE_STRIP);
    GL::GeometryBuffer<DefaultVertexType::Colored2d, false> *geometry3 = new GL::GeometryBuffer<DefaultVertexType::Colored2d, false>(GL_LINE_LOOP);
    geometry1->VBO().Init(4, GL_STREAM_DRAW);
    geometry2->VBO().Init(4, GL_STREAM_DRAW);
    geometry3->VBO().Init(4, GL_STREAM_DRAW);
    _drawables.push_back(new Drawable(geometry1));
    _drawables.push_back(new Drawable(geometry2));
    _drawables.push_back(new Drawable(geometry3));
    ChooseDefaultMaterial();
    _stateChanged = true;
}

void WindowBox::ChangeTitle(const std::string &title, const std::string &ttf)
{
    if (_title != NULL)
        delete _title;
    _title = new Graphic::String(title.c_str(), _titleHeight, Color(0, 0, 0), ttf);
    _stateChanged = true;
}

void WindowBox::Update()
{
    Widget::Update();

// title
    if (_drawTitle)
    {
        Array<DefaultVertexType::Colored2d, 4>   vertices;
        vertices[0].Fill(0, _size.Data[1] - _titleHeight, _titleColor1);
        vertices[1].Fill(_size.Data[0], _size.Data[1] - _titleHeight, _titleColor2);
        vertices[2].Fill(0, _size.Data[1], _titleColor1);
        vertices[3].Fill(_size.Data[0], _size.Data[1], _titleColor2);
        static_cast<GL::GeometryBuffer<DefaultVertexType::Colored2d, false>*>(_drawables[_indexDrawable]->Geometry)->VBO().UpdateData(vertices.Data);

        _title->Matrix.Translation(5.f, 3.f, 0.f);
        _title->Matrix.AddTranslation(0, _size.Data[1] - _titleHeight, 0);
    }

// box
    Array<DefaultVertexType::Colored2d, 4>   vertices;
    vertices[0].Fill(0, 0, _color);
    vertices[1].Fill(_size.Data[0], 0, _color);
    vertices[2].Fill(0, _size.Data[1] - _titleHeight, _color);
    vertices[3].Fill(_size.Data[0], _size.Data[1] - _titleHeight, _color);
    static_cast<GL::GeometryBuffer<DefaultVertexType::Colored2d, false>*>(_drawables[_indexDrawable+1]->Geometry)->VBO().UpdateData(vertices.Data);

// edge
    Vector2f size(_size.Data[0], _size.Data[1]);
    Color c(1,1,1);

    if (!_focus)
        c = _edgeColor;
    if (!_drawTitle)
        size.Data[1] -= _titleHeight;

    Array<DefaultVertexType::Colored2d, 4>   verticesEdge;
    verticesEdge[0].Fill(0, 0, c);
    verticesEdge[1].Fill(0, size.Data[1], c);
    verticesEdge[2].Fill(size.Data[0], size.Data[1], c);
    verticesEdge[3].Fill(size.Data[0], 0, c);
    static_cast<GL::GeometryBuffer<DefaultVertexType::Colored2d, false>*>(_drawables[_indexDrawable+2]->Geometry)->VBO().UpdateData(verticesEdge.Data);
}

void WindowBox::Draw(Graphic::SceneGraph *scene)
{
    // affichage du titre
    if (_drawTitle)
    {
        GetMaterial()->Render(scene, *_drawables[_indexDrawable]);
        _title->Render(scene);
    }
    // affichage de la box
    GetMaterial()->Render(scene, *_drawables[_indexDrawable+1]);

    // affiche le contour de la box
    GetMaterial()->Render(scene, *_drawables[_indexDrawable+2]);

    Widget::Draw(scene);
}

void WindowBox::TranslateChild(const Corner corner[2], Vector2f &v) const
{
    if (_drawTitle && (corner[1] == Top || corner[1] == Center))
        v[1] = -_titleHeight;
    else if (!_drawTitle && _corner[1] == Bottom)
        v[1] += _titleHeight;
}

void WindowBox::GetReelSize(Vector2f &size) const
{
    Widget::GetReelSize(size);
    if (!_drawTitle)
        size.Data[1] -= _titleHeight;
}
