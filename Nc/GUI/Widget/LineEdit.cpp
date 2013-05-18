
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
    _displayCursor = false;
    PaddingV(5);

    float charSize = 0;
    if (size[1] > 0)
        charSize = size[1] - (PaddingTop() + PaddingBottom());
    _textLeft = new Graphic::Text("", charSize, Color(), ttf);
    _textRight = new Graphic::Text("", charSize, Color(), ttf);
    _cursorText = new Graphic::Text("|", charSize, Color(), ttf);
    _currentOffset = 0;
    _useStencil = true;
}

LineEdit::~LineEdit()
{
    delete _textLeft;
    delete _textRight;
    delete _cursorText;
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
    _textLeft = static_cast<Text*>(w._textLeft->Clone());
    _textRight = static_cast<Text*>(w._textRight->Clone());
    _cursorText = static_cast<Text*>(w._cursorText->Clone());
    _displayCursor = false;
    _currentOffset = 0;
    _editable = w._editable;
}

void    LineEdit::ToString(std::ostream &os) const
{
    Widget::ToString(os);
    os << " Text: " << _textLeft->PlainText() << _textRight->PlainText();
}

void    LineEdit::UpdateState()
{
    Widget::UpdateState();

    // Check the char size of the text (must be equal to the height of the line edit)
    PlainTextFormater *formaterLeft = static_cast<PlainTextFormater*>(_textLeft->Formater());
    PlainTextFormater *formaterRight = static_cast<PlainTextFormater*>(_textRight->Formater());
    PlainTextFormater *formaterUnderscore = static_cast<PlainTextFormater*>(_cursorText->Formater());
    float charSize = _size[1] - (PaddingTop() + PaddingBottom());
    if (charSize != formaterLeft->GetCharSize())
    {
        formaterLeft->SetCharSize(charSize);
        formaterRight->SetCharSize(charSize);
        formaterUnderscore->SetCharSize(charSize);
    }

    // check the offset of the rendered text (in case the text is larger than the line edit)
    float s = _textLeft->Size()[0] + PaddingLeft() + PaddingRight();
    if (s - _currentOffset > _size.data[0])
        _currentOffset = s - _size.data[0];
    else if (s - _currentOffset < (PaddingLeft() + PaddingRight()))
        _currentOffset = _textLeft->Size()[0];

    // set the position of the text depending on the offset and padding
    _textLeft->Matrix.Translation(PaddingLeft() - _currentOffset, (_size.data[1] / 2.f) - (_cursorText->Size().data[1] / 2.f), 0);
    _textRight->Matrix.Translation(PaddingLeft() + _textLeft->Size()[0] - _currentOffset, (_size.data[1] / 2.f) - (_cursorText->Size().data[1] / 2.f), 0);
    _cursorText->Matrix.Translation(PaddingLeft() + _textLeft->Size()[0] - (_cursorText->Size()[0] / 2) - _currentOffset, (_size.data[1] / 2.f) - (_cursorText->Size().data[1] / 2.f), 0);
}

void LineEdit::Draw(Graphic::SceneGraph *scene)
{
    Widget::Draw(scene);

    // render the text like is they are childs
    // (this allow us to use the stencil test on the text.)
    RenderChildsBegin(scene);
    _textLeft->RenderNode(scene);
    _textRight->RenderNode(scene);

    // if we have the focus, we draw the underscore and make it blink
    if (_focus && _editable)
    {
        if (_clock.ElapsedTime() > 1)
        {
            _displayCursor = !_displayCursor;
            _clock.Reset();
        }

        if (_displayCursor)
            _cursorText->RenderNode(scene);
    }
    else
        _displayCursor = false;
    RenderChildsEnd(scene);
}

void LineEdit::KeyboardEvent(const System::Event &event)
{
    if (_editable && event.type == System::Event::TextEntered)
    {
        // 8 is the caracter code for backspace, we delete the last caracter on the left of the cursor
        if (event.text.unicode == 8 && !_textLeft->PlainText().empty())
        {
            Utils::Unicode::UTF32 s = _textLeft->PlainText();
            s.erase(s.end() - 1);
            _textLeft->PlainText(s);
        }
        // 13 is the caracter code for Return/Enter
        else if (event.text.unicode == 13)
        {
            SendEvent(Event::ReturnPressed);
        }
        // add the caracter on the left of the cursor
        else
        {
            PlainTextFormater *formater = static_cast<PlainTextFormater*>(_textLeft->Formater());
            float charSize = formater->GetCharSize(event.text.unicode).data[0];
            if ((event.text.unicode != '\0' && charSize != 0 /*&& _text->Size().data[0] + charSize < _size.data[0]*/) ||
                (charSize == 0 && (event.text.unicode == ' ' || event.text.unicode == '\t')) /*&& _text->Size().data[0] + formater->GetCharSize() < _size.data[0]*/)
            {
                Utils::Unicode::UTF32 s = _textLeft->PlainText();
                s += event.text.unicode;
                _textLeft->PlainText(s);
                SendEvent(Event::TextEntered);
            }
        }
        _stateChanged = true;
    }
    else if (event.type == System::Event::KeyPressed)
    {
        // move the cursor to the left
        if (event.key.code == System::Key::Left && _textLeft->PlainText().size() > 0)
        {
            Utils::Unicode::UTF32 sl = _textLeft->PlainText();
            Utils::Unicode::UTF32 sr = _textRight->PlainText();

            sr.insert(0, &sl[sl.size() - 1], 1);
            sl.erase(sl.end() - 1);
            _textRight->PlainText(sr);
            _textLeft->PlainText(sl);
            _stateChanged = true;
        }
        // move the cursor to the right
        else if (event.key.code == System::Key::Right && _textRight->PlainText().size() > 0)
        {
            Utils::Unicode::UTF32 sl = _textLeft->PlainText();
            Utils::Unicode::UTF32 sr = _textRight->PlainText();

            sl.push_back(sr[0]);
            sr.erase(sr.begin());
            _textRight->PlainText(sr);
            _textLeft->PlainText(sl);
            _stateChanged = true;
        }
    }
}

void    LineEdit::PlainText(const Utils::Unicode::UTF32 &t)
{
    _textLeft->PlainText(t);
    _textRight->PlainText("");
    _stateChanged = true;
}

Utils::Unicode::UTF32   LineEdit::PlainText() const
{
    return _textLeft->PlainText() + _textRight->PlainText();
}
