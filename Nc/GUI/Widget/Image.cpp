
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

#include "Image.h"

using namespace Nc;
using namespace Nc::GUI;

Image::Image(const AlignmentMask &alignment, const Vector2i &size)
    : Widget(alignment, size), _sprite(NULL), _spriteAlignment(Center), _spriteResizable(true), _keepRatio(true)
{
}

Image::~Image()
{
    if (_sprite != NULL)
        delete _sprite;
}

void    Image::Sprite(Graphic::Sprite *sp)
{
    System::Locker l(&_spriteMutex);
    if (_sprite != NULL)
        delete _sprite;
    _sprite = sp;
    _stateChanged = true;
}

void    Image::Update()
{
    if (_sprite != NULL)
    {
        // compute sprite size
        if (_spriteResizable)
        {
            if (_keepRatio)
            {
                Vector2i initialSize = _sprite->TextureBox().Size();

                int diff1 = _size[0] - initialSize.Data[0];
                int diff2 = _size[1] - initialSize.Data[1];
                if (diff1 <= diff2)
                {
                    initialSize.Data[0] += diff1;
                    initialSize.Data[1] += diff1;
                }
                else
                {
                    initialSize.Data[0] += diff2;
                    initialSize.Data[1] += diff2;
                }
                _sprite->Size(initialSize);
            }
            else
            {
                _sprite->Size(_size);
            }
        }

        // align sprite
        _sprite->Matrix.SetIdentity();

        // manage horizontal alignment
        if (_spriteAlignment.Enabled(Left))
        {
            // do nothing
        }
        else if (_spriteAlignment.Enabled(CenterH))
        {
            _sprite->Matrix.Translation((_size.Data[0] / 2.f) - (_sprite->Size().Data[0] / 2.f), 0, 0);
        }
        else if (_spriteAlignment.Enabled(Right))
        {
            _sprite->Matrix.Translation(_size.Data[0] - _sprite->Size().Data[0], 0, 0);
        }

        // manage vertical alignment
        if (_spriteAlignment.Enabled(Bottom))
        {
            // do nothing
        }
        else if (_spriteAlignment.Enabled(CenterV))
        {
            _sprite->Matrix.AddTranslation(0, (_size.Data[1] / 2.f) - (_sprite->Size().Data[1] / 2.f), 0);
        }
        else if (_spriteAlignment.Enabled(Top))
        {
            _sprite->Matrix.AddTranslation(0, _size.Data[1] - _sprite->Size().Data[1], 0);
        }
    }
}

void    Image::Draw(Graphic::SceneGraph *scene)
{
    Widget::Draw(scene);
    System::Locker l(&_spriteMutex);
    if (_sprite != NULL)
        _sprite->RenderNode(scene);
}

void    Image::PosChild(const Widget *w, Vector2i &pos) const
{
    Widget::PosChild(w, pos);

    if (_sprite != NULL)
    {
        if (_spriteAlignment.Enabled(Left))
        {
            if (w->Alignment().Enabled(Left))
                pos[0] += _sprite->Size()[0];
            else if (w->Alignment().Enabled(CenterH))
                pos[0] += _sprite->Size()[0] / 2;
        }
        else if (_spriteAlignment.Enabled(Right))
        {
            if (w->Alignment().Enabled(Right))
                pos[0] -= _sprite->Size()[0];
            else if (w->Alignment().Enabled(CenterH))
                pos[0] -= _sprite->Size()[0] / 2;
        }
    }
}

void    Image::SizeChild(const Widget *w, Vector2i &size) const
{
    Widget::SizeChild(w, size);
    if (_sprite != NULL)
        size[0] -= _sprite->Size()[0];
}
