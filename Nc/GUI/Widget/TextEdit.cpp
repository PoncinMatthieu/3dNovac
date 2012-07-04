
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

    File Created At:        30/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "TextEdit.h"
#include "../Look/Looks.h"
#include "../Visitor/Visitors.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::GUI;

TextEdit::TextEdit(const Utils::Unicode::UTF32 &text, const AlignmentMask &alignment, const Vector2i &size, const std::string &ttf, const Utils::Mask<Core::PlainTextFormater::Style> &s)
    : ScrollArea(alignment, size)
{
    _textDocument = new TextDocument(this, text, Left | Top, size, ttf, s);
    _textDocument->Percent(Vector2f(100, 0));
    AddComposedWidget(_textDocument);
    SetView(_textDocument);
}

TextEdit::TextEdit(const TextEdit &edit)
{
    Copy(edit);
}

TextEdit &TextEdit::operator = (const TextEdit &edit)
{
    delete _textDocument;
    Copy(edit);
    return *this;
}

TextEdit::~TextEdit()
{
    delete _textDocument;
}

void TextEdit::Copy(const TextEdit &edit)
{
    _textDocument = static_cast<TextDocument*>(edit._textDocument->Clone());
}

void TextEdit::RenderChildsEnd(Graphic::SceneGraph *scene)
{
    _textDocument->RenderNode(scene);
    ScrollArea::RenderChildsEnd(scene);
}

void    TextEdit::PlainText(const Utils::Unicode::UTF32 &t)
{
    _textDocument->text->PlainText(t);
    Resized();
}

TextEdit::TextDocument::TextDocument(TextEdit *editor, const Utils::Unicode::UTF32 &t, const AlignmentMask &alignment, const Vector2i &size, const std::string &ttf, const Utils::Mask<Core::PlainTextFormater::Style> &s)
    : Widget(alignment, size), _editor(editor)
{
    text = new Graphic::Text(t, 16, Color(1, 1, 1), ttf, s);
    Resize();
}

void    TextEdit::TextDocument::Resize()
{
    Widget::Resize();
    if (static_cast<Graphic::Core::PlainTextFormater*>(text->Formater())->GetDocumentSize() != (_size[0] - PaddingH()))
    {
        static_cast<Graphic::Core::PlainTextFormater*>(text->Formater())->SetDocumentSize(_size[0] - PaddingH());
        _size[1] = text->Size()[1];
        _editor->Resize();
    }
}

void    TextEdit::TextDocument::Update()
{
    Widget::Update();
    text->Matrix.Translation(PaddingLeft(), PaddingBottom(), 0);
}

void    TextEdit::TextDocument::Draw(Graphic::SceneGraph *scene)
{
    Widget::Draw(scene);
    text->RenderNode(scene);
}
