
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

#include "Text.h"
#include "../Core/PlainTextFormater.h"

using namespace std;
using namespace Nc::Graphic;
using namespace Nc;

Text::Text(const Utils::Unicode::UTF32 &text, Core::ITextFormater *textFormater)
    : Object(), _text(text), _textFormater(NULL)
{
    Formater(textFormater);
    _useSceneMaterial = false;
}

Text::Text(const Utils::Unicode::UTF32 &text, float charSize, const Color &color, const std::string &ttf, const Utils::Mask<Core::PlainTextFormater::Style> &style)
    : Object(), _text(text), _textFormater(NULL)
{
    Formater(new Core::PlainTextFormater(charSize, color, ttf, style));
    _useSceneMaterial = false;
}

Text::Text(const Text &text)
{
    _text = text._text;
    Formater(text._textFormater->Clone());
}

Text &Text::operator = (const Text &text)
{
    _text = text._text;
    Formater(text._textFormater->Clone());
    return *this;
}

Text::~Text()
{
    delete _textFormater;
}

void    Text::PlainText(const Utils::Unicode::UTF32 &text)
{
    _text = text;
    _textFormater->TextChanged();
}

const Vector2f      &Text::Size()
{
    if (_textFormater->SizeChanged())
    {
        System::Locker l(&_mutex);
        _textFormater->ComputeSize(_size, _text);
    }
    return _size;
}

void    Text::Formater(Core::ITextFormater *f)
{
    if (_textFormater != NULL && _textFormater != f)
    {
        delete _textFormater;
        _textFormater = NULL;
    }

    if (_textFormater != f)
    {
        _textFormater = f;
        _textFormater->InitDrawables(_drawables);
        ChooseDefaultMaterial();
    }
}

void    Text::TransformModelMatrixToRender(SceneGraph *scene)
{
    if (!_text.empty())
        scene->ModelMatrix() *= Matrix * _matrixText;
}

void    Text::Render(SceneGraph *scene)
{
    if (!_text.empty())    // No text, no rendering :)
    {
        if (_textFormater->DrawablesChanged())
        {
            System::Locker l(&_mutex);
            _textFormater->ComputeDrawables(_drawables, _matrixText, _text);
        }
        Object::Render(scene);
    }
}


