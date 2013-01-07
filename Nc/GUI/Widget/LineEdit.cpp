
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

#include "LineEdit.h"
#include "../Look/Looks.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::GUI;
using namespace Nc::Graphic;

LineEdit::LineEdit(const AlignmentMask &alignment, const Vector2i &size, const std::string &ttf, const std::string &looksName)
    : Widget(alignment, size)
{
    UseLook(new StripLook(looksName + StyleSheet::Name::LineEdit));

    _editable = true;
    _fontUnderscoreDisplayed = false;
    PaddingV(5);
    _font = new Graphic::Text("", size[1] - (PaddingTop() + PaddingBottom()), Color(), ttf);
    _fontUnderscore = new Graphic::Text("|", size[1] - (PaddingTop() + PaddingBottom()), Color(), ttf);
}

LineEdit::~LineEdit()
{
    delete _font;
    delete _fontUnderscore;
}

LineEdit::LineEdit(const LineEdit &w)
    : Widget(w)
{
    Copy(w);
}

LineEdit &LineEdit::operator = (const LineEdit &w)
{
    Widget::operator = (w);
    Copy(w);
    return *this;
}

void    LineEdit::Copy(const LineEdit &w)
{
    _font = static_cast<Text*>(w._font->Clone());
    _fontUnderscore = static_cast<Text*>(w._fontUnderscore->Clone());
    _fontUnderscoreDisplayed = false;
    _editable = w._editable;
}

void    LineEdit::ToString(std::ostream &os) const
{
    Widget::ToString(os);
    os << " Text: " << _font->PlainText();
}

void    LineEdit::UpdateState()
{
    Widget::UpdateState();

    PlainTextFormater *formater = static_cast<PlainTextFormater*>(_font->Formater());
    PlainTextFormater *formaterUnderscore = static_cast<PlainTextFormater*>(_fontUnderscore->Formater());

    float charSize = _size[1] - (PaddingTop() + PaddingBottom());
    if (charSize != formater->GetCharSize())
    {
        formater->SetCharSize(charSize);
        formaterUnderscore->SetCharSize(charSize);
    }
    _font->Matrix.Translation(PaddingLeft(), (_size.data[1] / 2.f) - (_font->Size().data[1] / 2.f), 0);

    _fontUnderscore->Matrix.Translation(PaddingLeft() + _font->Size()[0], (_size.data[1] / 2.f) - (_font->Size().data[1] / 2.f), 0);
}

void LineEdit::Draw(Graphic::SceneGraph *scene)
{
    Widget::Draw(scene);
    _font->RenderNode(scene);

    // if we have the focus, we draw the underscore and make it blink
    if (_focus && _editable)
    {
        if (_clock.ElapsedTime() > 1)
        {
            _fontUnderscoreDisplayed = !_fontUnderscoreDisplayed;
            _clock.Reset();
        }

        if (_fontUnderscoreDisplayed)
            _fontUnderscore->RenderNode(scene);
    }
    else
        _fontUnderscoreDisplayed = false;

}

void LineEdit::KeyboardEvent(const System::Event &event)
{
    if (_editable && event.type == System::Event::TextEntered)
    {
        // 8 is the caracter code for backspace, we delete the last caracter
        if (event.text.unicode == 8 && !_font->PlainText().empty())
        {
            Utils::Unicode::UTF32 s = _font->PlainText();
            s.erase(s.end() - 1);
            _font->PlainText(s);
        }
        else
        {
            PlainTextFormater *formater = static_cast<PlainTextFormater*>(_font->Formater());
            float charSize = formater->GetCharSize(event.text.unicode).data[0];
            if (event.text.unicode != '\0' && charSize != 0 && _font->Size().data[0] + charSize < _size.data[0])
            {
                if (event.text.unicode == '\n')
                {
                    SendEvent(Event::ReturnPressed);
                }
                else // ajout du caractere dans la string
                {
                    Utils::Unicode::UTF32 s = _font->PlainText();
                    s += event.text.unicode;
                    _font->PlainText(s);
                }
            }
        }
        _stateChanged = true;
    }
}
