
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

using namespace Nc;
using namespace Nc::Graphic;

Sprite::Sprite()
    : Object(), _needUpdate(true), _color(1, 1, 1)
{
}

Sprite::Sprite(const Vector2f &size, const GL::Texture &t, const Box2i &box, const GL::Blend::Pattern &blendPattern)
    : Object(), _needUpdate(true), _color(1, 1, 1)
{
    Initialize(size, box, t, blendPattern);
}

Sprite::Sprite(const Vector2f &size, const GL::Texture &t, float percentBox, const GL::Blend::Pattern &blendPattern)
    : Object(), _needUpdate(true), _color(1, 1, 1)
{
    Initialize(size, Box2i(Vector2f(0,0), Vector2f(t.Size()[0] * percentBox / 100, t.Size()[1] * percentBox / 100)), t, blendPattern);
}

Sprite::Sprite(const GL::Texture &t, const GL::Blend::Pattern &blendPattern)
    : Object(), _needUpdate(true), _color(1, 1, 1)
{
    Initialize(t.Size(), Box2i(Vector2f(0,0), Vector2f(t.Size()[0], t.Size()[1])), t, blendPattern);
}

void Sprite::Initialize(const Vector2f &spriteSize, const Box2i &textureBox, const GL::Texture &t, const GL::Blend::Pattern &blendPattern)
{
    _spriteSize = spriteSize;
    _textureBox = textureBox;

    GL::GeometryBuffer<GL::DefaultVertexType::Textured2d,false> *geo = new GL::GeometryBuffer<GL::DefaultVertexType::Textured2d,false>(GL::Enum::TriangleStrip);
    geo->VBO().Init(4, GL::Enum::DataBuffer::StaticDraw);

    GL::MaterialConfig *conf = new GL::MaterialConfig(blendPattern);
    conf->Textures.InitSize(1);
    conf->Textures[0] = t;

    _drawables.resize(1);
    _drawables[0] = new GL::Drawable(geo, conf);
    ChooseDefaultMaterial();

    _useSceneMaterial = false;
}

void Sprite::Draw(SceneGraph *scene)
{
    if (_needUpdate)
        UpdateGeometry();
    Object::Draw(scene);
}

void Sprite::UpdateGeometry()
{
    const Vector2i  &size = _drawables[0]->Config->Textures[0].Size();

    float           xb = (float)_textureBox.Min(0) / (float)size.data[0];
    float           yb = (float)_textureBox.Min(1) / (float)size.data[1];
    float           xh = (float)_textureBox.Max(0) / (float)size.data[0];
    float           yh = (float)_textureBox.Max(1) / (float)size.data[1];

    Array<GL::DefaultVertexType::Textured2d, 4>   vertices;
    vertices[0].Fill(0, 0, xb, yb, _color);
    vertices[1].Fill(_spriteSize[0], 0, xh, yb, _color);
    vertices[2].Fill(0, _spriteSize[1], xb, yh, _color);
    vertices[3].Fill(_spriteSize[0], _spriteSize[1], xh, yh, _color);

    GL::GeometryBuffer<GL::DefaultVertexType::Textured2d,false> *geo = static_cast<GL::GeometryBuffer<GL::DefaultVertexType::Textured2d,false>*>(_drawables[0]->Geometry);
    geo->VBO().UpdateData(vertices.data);
    _needUpdate = false;
}

void    Sprite::SaveTexture(const Utils::FileName &f)
{
    // retreive the texture from the drawable
    GL::Texture &t = _drawables[0]->Config->Textures[0];

    // save the texture
    t.SaveToFile(f);
}
