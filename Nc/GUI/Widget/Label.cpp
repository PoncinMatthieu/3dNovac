
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

#include "Label.h"

#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::GUI;

Label::Label(const Utils::Unicode::UTF32 &label, float charSize, const Color &c, const AlignmentMask &alignment, const Vector2i &size, const std::string &ttf, Graphic::PlainTextFormater::Style s)
    : Widget(alignment, size), _label(NULL), _labelAlignment(Left | CenterV)
{
    PaddingH(5);
    CreateLabel(label, charSize, c, ttf, s);
}

Label::~Label()
{
    if (_label != NULL)
        delete _label;
}

Label::Label(const Label &w)
    : Widget(w)
{
    Copy(w);
}

Label &Label::operator = (const Label &w)
{
    if (_label != NULL)
        delete _label;

    Widget::operator = (w);
    Copy(w);
    return *this;
}

void    Label::Copy(const Label &w)
{
    _label = new Graphic::Text(*w._label);
}

void Label::ToString(std::ostream &os) const
{
    Widget::ToString(os);
    os << " Label: " << _label->PlainText();
}

void Label::CreateLabel(const Utils::Unicode::UTF32 &l, float charSize, const Color &c, const std::string &ttf, Graphic::PlainTextFormater::Style s)
{
    if (_label)
        delete _label;
    _label = new Graphic::Text(l, charSize, c, ttf, s);
    _stateChanged = true;
}

void Label::UpdateState()
{
    // update the size of the label on the size of the text if the size if too small
    // (more likely if we didn't set the size at first)
    if (_size[0] < _label->Size()[0])
        _size[0] = _label->Size()[0] + PaddingLeft() + PaddingRight();
    if (_size[1] < _label->Size()[1])
        _size[1] = _label->Size()[1] + PaddingTop() + PaddingBottom();

    Widget::UpdateState();
    UpdateLabel();
}

void Label::UpdateLabel()
{
    _label->Matrix.SetIdentity();

    // manage horizontal alignment
    if (_labelAlignment.Enabled(Left))
    {
        // do nothing
    }
    else if (_labelAlignment.Enabled(CenterH))
    {
        _label->Matrix.Translation((_size.data[0] / 2.f) - (_label->Size().data[0] / 2.f), 0, 0);
    }
    else if (_labelAlignment.Enabled(Right))
    {
        _label->Matrix.Translation(_size.data[0] - _label->Size().data[0], 0, 0);
    }

    // manage vertical alignment
    if (_labelAlignment.Enabled(Bottom))
    {
        // do nothing
    }
    else if (_labelAlignment.Enabled(CenterV))
    {
        _label->Matrix.AddTranslation(0, (_size.data[1] / 2.f) - (_label->Size().data[1] / 2.f), 0);
    }
    else if (_labelAlignment.Enabled(Top))
    {
        _label->Matrix.AddTranslation(0, _size.data[1] - _label->Size().data[1], 0);
    }
}

void Label::Draw(Graphic::SceneGraph *scene)
{
    // draw le widget
    Widget::Draw(scene);

    // draw le label
    _label->RenderNode(scene);
}

void    Label::PosChild(const Widget *w, Vector2i &pos) const
{
    Widget::PosChild(w, pos);

    if (_labelAlignment.Enabled(Left))
    {
        if (w->Alignment().Enabled(Left))
            pos[0] += _label->Size()[0];
        else if (w->Alignment().Enabled(CenterH))
            pos[0] += _label->Size()[0] / 2;
    }
    else if (_labelAlignment.Enabled(Right))
    {
        if (w->Alignment().Enabled(Right))
            pos[0] -= _label->Size()[0];
        else if (w->Alignment().Enabled(CenterH))
            pos[0] -= _label->Size()[0] / 2;
    }
}

void    Label::SizeChild(const Widget *w, Vector2i &size) const
{
    Widget::SizeChild(w, size);
    size[0] -= _label->Size()[0];
}

void    Label::AddBuddy(Widget *w)
{
    w->Percent(Vector2f(100, 100));
    AddChild(w);
}
