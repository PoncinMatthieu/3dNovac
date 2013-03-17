
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

#include <Nc/Core/Engine/Manager.h>
#include "Console.h"
#include "../Look/Looks.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::System;
using namespace Nc::Graphic;
using namespace Nc::GUI;


Console::Console(const AlignmentMask &alignment, const Vector2i &size)
    :   Widget(alignment, size),
        _listener(this)
{
    // create the composed widgets
    _centralLayout = new Layout(Layout::Vertical, Center);
    _centralLayout->Percent(Vector2f(100, 100));

    _textEdit = new TextEdit("");
    _textEdit->UseLook(new BoxLook("Small"));
    _textEdit->Percent(Vector2f(100, 100));
    _textEdit->SetToEndOfText();
    _centralLayout->AddChild(_textEdit);
    _centralLayout->SetExpandRatio(_textEdit, 100);

    _lineEdit = new LineEdit(Left | Bottom, Vector2i(0, 25));
    _lineEdit->Percent(Vector2f(100, 0));
    _lineEdit->AddEventManager(&_listener);
    _centralLayout->AddChild(_lineEdit);

    AddComposedWidget(_centralLayout);
}

Console::~Console()
{
    delete _centralLayout;
}

void    Console::RenderEnd(Graphic::SceneGraph *scene)
{
    _centralLayout->RenderNode(scene);

    Widget::RenderEnd(scene);
}

void    Console::Update(float elapsedTime)
{
    _listener.ExecuteEvents();
    Widget::Update(elapsedTime);
}

void    Console::SendCommand()
{
    SendEvent(Event::ReturnPressed);
}

void    Console::AppendText(const Utils::Unicode::UTF32 &text)
{
    Utils::Unicode::UTF32 t = _textEdit->PlainText();
    t += text;
    _textEdit->PlainText(t);
}
