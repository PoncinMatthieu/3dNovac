
/*-----------------------------------------------------------------------------

	3dNovac Graphics
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

#include "Sprite.h"

#ifdef _DEBUG
    //#define     _DEBUG_SPRITES
#endif

using namespace Nc;
using namespace Nc::Graphic;

Sprite::Sprite(const Vector2f &size, const GL::Texture &t, const Box2f &box, const GL::Blend::Pattern &blendPattern)
    : Object(ClassName()), _needUpdate(true), _spriteSize(size), _textureBox(box), _color(1, 1, 1)
{
    GL::GeometryBuffer<DefaultVertexType::Textured2d,false> *geo = new GL::GeometryBuffer<DefaultVertexType::Textured2d,false>(GL::Enum::TriangleStrip);
    geo->VBO().Init(4, GL::Enum::StaticDraw);

    MaterialConfig *conf = new MaterialConfig(blendPattern);
    conf->Textures.InitSize(1);
    conf->Textures[0] = t;

    _drawables.resize(1);
    _drawables[0] = new Drawable(geo, conf);
    ChooseDefaultMaterial();
}

void Sprite::Draw(SceneGraph *scene)
{
    if (_needUpdate)
        UpdateGeometry();
    Object::Draw(scene);

/// \todo recode the debug equivalent in opengl 3.3
    #ifdef _DEBUG_SPRITES
    glDisable(GL_TEXTURE_2D);
    Color::Red();
    glBegin(GL_LINE_LOOP);
        glVertex2f(0, 0);
        glVertex2f(0, _spriteSize.Data[1]);
        glVertex2f(_spriteSize.Data[0], _spriteSize.Data[1]);
        glVertex2f(_spriteSize.Data[0], 0);
    glEnd();
    #endif
}

void Sprite::UpdateGeometry()
{
    const Vector2i  &size = _drawables[0]->Config->Textures[0].Size();
    float           xb = _textureBox.Min(0) / (float)size.Data[0];
    float           yb = _textureBox.Min(1) / (float)size.Data[1];
    float           xh = _textureBox.Max(0) / (float)size.Data[0];
    float           yh = _textureBox.Max(1) / (float)size.Data[1];

    Array<DefaultVertexType::Textured2d, 4>   vertices;
    vertices[0].Fill(0, 0, xb, yb, _color);
    vertices[1].Fill(_spriteSize[0], 0, xh, yb, _color);
    vertices[2].Fill(0, _spriteSize[1], xb, yh, _color);
    vertices[3].Fill(_spriteSize[0], _spriteSize[1], xh, yh, _color);

    GL::GeometryBuffer<DefaultVertexType::Textured2d,false> *geo = static_cast<GL::GeometryBuffer<DefaultVertexType::Textured2d,false>*>(_drawables[0]->Geometry);
    geo->VBO().UpdateData(vertices.Data);
    _needUpdate = false;
}


