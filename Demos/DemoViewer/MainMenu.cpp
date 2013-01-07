
/*-----------------------------------------------------------------------------

	3dNovac DemoViewer
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

#include <Nc/GUI/Widget/ScrollArea.h>
#include <Nc/GUI/Widget/ScrollBar.h>
#include <Nc/GUI/Widget/ComboBox.h>
#include <Nc/GUI/Look/Looks.h>
#include <Nc/GUI/Widget/SubWindow.h>
#include <Nc/GUI/Visitor/Visitors.h>

#include "MainMenu.h"
#include "GameEngine.h"

using namespace Nc;
using namespace Nc::Utils;
using namespace Nc::System;
using namespace Nc::GUI;
using namespace Nc::Graphic;
using namespace DemoViewer;

Nc::GUI::Console	    *MainMenu::_console = NULL;

MainMenu::MainMenu(Nc::GUI::SceneGraph *gui)
    : _GUI(gui)
{
    _currentDemoWindow = NULL;
    _currentItemSelected = NULL;
    imageHeight = 150;

    // create the main layout
    Layout *mainLayout = new Layout(Layout::Horizontal, Center);
    mainLayout->UseLook(new BoxLook("Background"));
    mainLayout->Percent(Vector2f(100, 100));
    _GUI->AddChild(mainLayout);

    // create the description window
    CreateDescriptionPannel(mainLayout);

    // create a central layout
    Layout *centralLayout = new Layout(Layout::Vertical, Center);
    centralLayout->PaddingH(5);
    centralLayout->PaddingV(5);
    centralLayout->Percent(Vector2f(100, 100));
    mainLayout->AddChild(centralLayout);
    mainLayout->SetExpandRatio(centralLayout, 100);

    // create the demo window used to render the demos
    _widgetDemoWindow = new Widget(Center);
    _widgetDemoWindow->UseLook(new BoxLook("Small"));
    _widgetDemoWindow->Percent(Vector2f(100, 100));
    centralLayout->AddChild(_widgetDemoWindow);
    centralLayout->SetExpandRatio(_widgetDemoWindow, 100);

    // create the console at the bottom of the centralLayout
    _console = new Console("Console", CenterH | Bottom, Vector2i(0,200));
    _console->Percent(Vector2f(100, 0));
    _console->AddEventManager(Engine::Manager::GetEngine(GameEngine::ClassName()));
    centralLayout->AddChild(_console);

    // link the logger to the console
    Utils::Logger::Instance().SetLoggingFunction(Write);

    // create the fps widget on top of the main layout
    FPSWidget *fps = new FPSWidget(Right | Top);
    fps->MarginTop(8);
    fps->MarginRight(8);
    _GUI->AddChild(fps);
}

MainMenu::~MainMenu()
{
}

void    MainMenu::AddDemo(const std::string &name)
{
    _demoComboBox->AddItem(new Item(name));
}

Widget  *MainMenu::CreateDescriptionPannel(Layout *parent)
{
    Layout *pannelDescriptionLayout = new Layout(Layout::Vertical, Center, Vector2i(300, 0));
    pannelDescriptionLayout->PaddingH(5);
    pannelDescriptionLayout->PaddingV(5);
    pannelDescriptionLayout->Percent(Vector2f(0, 100));
    parent->AddChild(pannelDescriptionLayout);

    // create the select window with the combobox and the button
    Widget *windowSelectDemo = CreateSelectDemoWindow(pannelDescriptionLayout);
    windowSelectDemo->Percent(Vector2f(100, 0));

    // create the window description area
    WindowBox *winDescArea = new WindowBox("Description", CenterH | Top);
    winDescArea->MarginTop(5);
    winDescArea->Percent(Vector2f(100, 100));
    pannelDescriptionLayout->AddChild(winDescArea);
    pannelDescriptionLayout->SetExpandRatio(winDescArea, 100);

    _layoutWinDesc = new Layout(Layout::Vertical, CenterH | Center);
    _layoutWinDesc->Percent(Vector2f(100, 100));
    winDescArea->AddChild(_layoutWinDesc);

    // create the image widget to show an image of the demo
    _demoImage = new GUI::Image(NULL, CenterH | Top, Vector2i(0, imageHeight));
    _demoImage->Percent(Vector2f(100, 0));
    _layoutWinDesc->AddChild(_demoImage);

    // create the text area to describe the selected demo
    _descriptionTextArea = new TextEdit("", CenterH | Bottom, Vector2i(0, 0), "arial", PlainTextFormater::Regular);
    _descriptionTextArea->MarginTop(5);
    _descriptionTextArea->UseLook(new BoxLook("Small"));
    _descriptionTextArea->Percent(Vector2f(100, 100));
    _layoutWinDesc->AddChild(_descriptionTextArea);
    _layoutWinDesc->SetExpandRatio(_descriptionTextArea, 100);

    return pannelDescriptionLayout;
}

Widget  *MainMenu::CreateSelectDemoWindow(Layout *parent)
{
    Widget *windowSelectDemo = new Widget(CenterH | Top, Vector2i(0,60));
    windowSelectDemo->UseLook(new BoxLook("Widget"));
    parent->AddChild(windowSelectDemo);

    Layout *selectDemoLayout = new Layout(Layout::Horizontal, Center);
    selectDemoLayout->Percent(Vector2f(100, 100));
    windowSelectDemo->AddChild(selectDemoLayout);

    _demoComboBox = new ComboBox(_GUI, Left | CenterV, Vector2i(250,0));
    _demoComboBox->MarginRight(5);
    _demoComboBox->AddEventManager(Engine::Manager::GetEngine(GameEngine::ClassName()));
    _demoComboBox->Percent(Vector2f(100, 0));
    selectDemoLayout->AddChild(_demoComboBox);
    selectDemoLayout->SetExpandRatio(_demoComboBox, 100);

    Button *button = new Button("Start", Center, Vector2i(70, 28), "arial");
    selectDemoLayout->AddChild(button);
    button->AddEventManager(Engine::Manager::GetEngine(GameEngine::ClassName()));
    return windowSelectDemo;
}

GUI::SubWindow      *MainMenu::CreateDemoWindow(IWindow *windowParent)
{
    System::Locker l(&_widgetDemoWindow->GetMutex());
    if (_currentDemoWindow != NULL)
        _widgetDemoWindow->RemoveWidget(_currentDemoWindow);

    _currentDemoWindow = new GUI::SubWindow(windowParent, Center);
    _currentDemoWindow->Percent(Vector2f(100, 100));
    _widgetDemoWindow->AddChild(_currentDemoWindow);
    _currentDemoWindow->Resize();
    return _currentDemoWindow;
}

void    MainMenu::CloseDemoWindow()
{
    if (_currentDemoWindow != NULL)
        _currentDemoWindow->GetSubWindow()->Close();
}

void    MainMenu::DemoSelected()
{
    if (_demoComboBox->CurrentItem() != NULL && _demoComboBox->CurrentItem() != _currentItemSelected)
    {
        _currentItemSelected = _demoComboBox->CurrentItem();

        // unset the sprite
        _demoImage->Sprite(NULL);

        // create the sprite used for the description of the demo
        Utils::FileName pictureFile = "Nc:Image:Demos/" + _currentItemSelected->Data() + ".png";
        if (pictureFile.IsReadable())
        {
            Sprite *sp = new Sprite(Vector2i(0,0), GL::Texture(pictureFile), 100);
            _demoImage->Sprite(sp);
            _demoImage->Size(Vector2i(0, imageHeight));
        }
        else
            _demoImage->Size(Vector2i(0, 0));

        // set the description
        _descriptionTextArea->PlainText(CONFIG->Block("DemoDescriptions")->Block(_currentItemSelected->Data())->Data());

        GUI::Visitor::ResizedAll resized;
        resized(*_layoutWinDesc);
    }
}

void    MainMenu::ExecConsoleCommand()
{
    // print the command, so it will be displayed by the console
    if (!_console->LineEditText().empty())
    {
        LOG << _console->LineEditText() << std::endl;
        _console->ClearLineEdit();
    }
}

void    MainMenu::Write(const std::string &msg, bool flush)
{
    _console->AppendText(msg);
}
