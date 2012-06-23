
/*-----------------------------------------------------------------------------

	3dNovac GUIDemo
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

    File Created At:        27/09/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <Nc/GUI/ScrollArea.h>
#include <Nc/GUI/ScrollBar.h>
#include <Nc/GUI/ComboBox.h>
#include <Nc/GUI/Looks.h>
#include <Nc/GUI/WidgetSubWindow.h>

#include "MainMenu.h"
#include "GameEngine.h"

using namespace Nc::Utils;
using namespace Nc::System;
using namespace Nc::GUI;
using namespace Nc::Graphic;
using namespace SampleViewer;

MainMenu::MainMenu(Nc::GUI::SceneGraph *gui)
    : _GUI(gui)
{
    _currentSampleWindow = NULL;

    // create the main layout
    Layout *mainLayout = new Layout(Layout::Vertical, Center, Center);
    mainLayout->UseLook(new BoxLook("Background"));
    _GUI->AddChild(mainLayout);

    // create the select window with the combobox and the button
    Widget *windowSelectSample = CreateSelectSampleWindow(mainLayout);

    // create the sample window used to render the samples
    _widgetSampleWindow = new Widget(Center, Center);
    _widgetSampleWindow->UseLook(new BoxLook(""));
    _widgetSampleWindow->Margin(Vector2i(5, 5));
    mainLayout->AddChild(_widgetSampleWindow);

    // set the auto resizing
    mainLayout->Percent(Vector2f(100, 100));
    windowSelectSample->Percent(Vector2f(40, 0));
    mainLayout->SetExpandRatio(_widgetSampleWindow, 100);
    _widgetSampleWindow->Percent(Vector2f(80, 100));

    // create the fps widget on top of the main layout
    _GUI->AddChild(new FPSWidget(Right, Top));

    //_console = new GUI::Console();
    //_scene->AddChild(_console);
}

MainMenu::~MainMenu()
{
}

void    MainMenu::AddSample(const std::string &name)
{
    _comboBox->AddItem(new Item(name));
}

Widget  *MainMenu::CreateSelectSampleWindow(Layout *parent)
{
    Widget *windowSelectSample = new Widget(Center, Center, Vector2i(0,0), Vector2i(0,60));
    windowSelectSample->UseLook(new BoxLook("Window"));
    parent->AddChild(windowSelectSample);

    Layout *selectSampleLayout = new Layout(Layout::Horizontal, Center, Center);
    windowSelectSample->AddChild(selectSampleLayout);

    _comboBox = new ComboBox(_GUI, Center, Center, Vector2i(0,0), Vector2i(250,0));
    selectSampleLayout->AddChild(_comboBox);

    Button *button = new Button("Start Sample", Center, Center, Vector2i(0,0), 20, 15, 8, "arial");
    selectSampleLayout->AddChild(button);
    button->HandlerEngineName(GameEngine::ClassName());
    button->HandlerId(GameEngine::StartSample);

    selectSampleLayout->Percent(Vector2f(95, 100));
    selectSampleLayout->SetExpandRatio(_comboBox, 65);
    _comboBox->Percent(Vector2f(95, 0));
    button->Percent(Vector2f(95, 0));

    return windowSelectSample;
}

WidgetSubWindow    *MainMenu::CreateSampleWindow(Window *windowParent)
{
    if (_currentSampleWindow != NULL)
        _widgetSampleWindow->RemoveWidget(_currentSampleWindow);

    _currentSampleWindow = new WidgetSubWindow(windowParent, Center, Center);
    _widgetSampleWindow->AddChild(_currentSampleWindow);
    _currentSampleWindow->Percent(Vector2f(100, 100));
    return _currentSampleWindow;
}

void    MainMenu::CloseSampleWindow()
{
    if (_currentSampleWindow != NULL)
        _currentSampleWindow->SubWindow()->Close();
}

