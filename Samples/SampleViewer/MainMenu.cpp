
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

#include <Nc/GUI/Widget/ScrollArea.h>
#include <Nc/GUI/Widget/ScrollBar.h>
#include <Nc/GUI/Widget/ComboBox.h>
#include <Nc/GUI/Look/Looks.h>
#include <Nc/GUI/Widget/SubWindow.h>

#include "MainMenu.h"
#include "GameEngine.h"

using namespace Nc;
using namespace Nc::Utils;
using namespace Nc::System;
using namespace Nc::GUI;
using namespace Nc::Graphic;
using namespace SampleViewer;

MainMenu::MainMenu(Nc::GUI::SceneGraph *gui)
    : _GUI(gui)
{
    _currentSampleWindow = NULL;
    _currentItemSelected = NULL;
    imageHeight = 150;

    // create the main layout
    Layout *mainLayout = new Layout(Layout::Horizontal, Center);
    mainLayout->UseLook(new BoxLook("Background"));
    mainLayout->Percent(Vector2f(100, 100));
    _GUI->AddChild(mainLayout);

    // create the description window
    CreateDescriptionPannel(mainLayout);

    // create the sample window used to render the samples
    _widgetSampleWindow = new Widget(Center);
    _widgetSampleWindow->UseLook(new BoxLook());
    _widgetSampleWindow->PaddingH(5);
    _widgetSampleWindow->PaddingV(5);
    _widgetSampleWindow->Percent(Vector2f(100, 100));
    mainLayout->AddChild(_widgetSampleWindow);
    mainLayout->SetExpandRatio(_widgetSampleWindow, 100);

    // create the fps widget on top of the main layout
    _GUI->AddChild(new FPSWidget(Right | Top));

    //_console = new GUI::Console();
    //_scene->AddChild(_console);
}

MainMenu::~MainMenu()
{
}

void    MainMenu::AddSample(const std::string &name)
{
    _sampleComboBox->AddItem(new Item(name));
}

Widget  *MainMenu::CreateDescriptionPannel(Layout *parent)
{
    Layout *pannelDescriptionLayout = new Layout(Layout::Vertical, Center, Vector2i(300, 0));
    pannelDescriptionLayout->PaddingH(5);
    pannelDescriptionLayout->PaddingV(5);
    pannelDescriptionLayout->Percent(Vector2f(0, 100));
    parent->AddChild(pannelDescriptionLayout);

    // create the select window with the combobox and the button
    Widget *windowSelectSample = CreateSelectSampleWindow(pannelDescriptionLayout);
    windowSelectSample->Percent(Vector2f(100, 0));

    // create the window description area
    WindowBox *winDescArea = new WindowBox("Description", CenterH | Top);
    winDescArea->MarginTop(5);
    winDescArea->Percent(Vector2f(100, 100));
    pannelDescriptionLayout->AddChild(winDescArea);
    pannelDescriptionLayout->SetExpandRatio(winDescArea, 100);

    _layoutWinDesc = new Layout(Layout::Vertical, CenterH | Center);
    _layoutWinDesc->Percent(Vector2f(100, 100));
    winDescArea->AddChild(_layoutWinDesc);

    // create the image widget to show an image of the sample
    _sampleImage = new GUI::Image(CenterH | Top, Vector2i(0, imageHeight));
    _sampleImage->Percent(Vector2f(100, 0));
    _layoutWinDesc->AddChild(_sampleImage);

    // create the text area to describe the selected sample
    //Utils::Unicode::UTF32 text(L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. \nProin tempor nulla vitae justo pharetra feugiat. \nPhasellus eget erat velit, id dictum felis. \nVestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; \nUt eget sapien nunc, id pharetra sem. Sed mollis lobortis sem nec ullamcorper. Nullam eget nisi elit, sit amet faucibus quam. Integer dictum varius nulla id aliquet. Vestibulum in tincidunt velit. Aenean egestas hendrerit quam, eu rutrum justo lobortis vitae. Donec vehicula, enim et faucibus commodo, nibh nulla facilisis nisl, ac dapibus enim est ut mauris. Nam urna massa, tincidunt at tempor eu, lacinia sed tortor. Curabitur pretium, nisi et tincidunt tempor, nunc mi interdum elit, vitae lacinia purus tortor ac dolor.\n\nNulla at dolor magna. Sed erat nulla, tincidunt et gravida quis, placerat ut sapien. Cras laoreet tempus rhoncus. Ut ligula lacus, egestas in rhoncus a, molestie ut nulla. Fusce euismod elit sit amet lectus eleifend hendrerit. Maecenas molestie aliquet nisi eu tempor. Fusce quam metus, ullamcorper ac condimentum eget, interdum in velit. Aenean quis enim leo. Quisque mi felis, tristique sit amet tristique at, luctus in erat. In sit amet libero a metus viverra blandit. Phasellus ut porta dui. In hac habitasse platea dictumst. \nPellentesque placerat leo sit amet eros pellentesque ultrices. ");
    //Utils::Unicode::UTF32 text(L"bSplinesDemo");
    Utils::Unicode::UTF32 text(L"3dNovac is a free multimedia C++ Framework. Its main goal is to provide the necessary tools for creating games of all types, while remaining portable and lightweight.\nThe framework will therefore manage the main features required for the development of a complete game (Real-time display, environmental management, network management, audio management).\nWritten in C++ object-oriented, The 3D engine uses only OpenGL and GLSL for rendering and uses only low-level library, to be the most independent and lightest possible.\nTo know more about 3dNovac, you can visit the features page.\nIf you have any questions, comments or remarks, feel free to contact a member of the team by email.");
    _descriptionTextArea = new TextEdit(text, CenterH | Bottom, Vector2i(0, 0), "arial", Core::PlainTextFormater::Underlined);
    _descriptionTextArea->MarginTop(5);
    _descriptionTextArea->Percent(Vector2f(100, 100));
    _descriptionTextArea->UseLook(new BoxLook());
    _layoutWinDesc->AddChild(_descriptionTextArea);
    _layoutWinDesc->SetExpandRatio(_descriptionTextArea, 100);

    return pannelDescriptionLayout;
}

Widget  *MainMenu::CreateSelectSampleWindow(Layout *parent)
{
    Widget *windowSelectSample = new Widget(CenterH | Top, Vector2i(0,60));
    windowSelectSample->UseLook(new BoxLook("Widget"));
    parent->AddChild(windowSelectSample);

    Layout *selectSampleLayout = new Layout(Layout::Horizontal, Center);
    selectSampleLayout->Percent(Vector2f(100, 100));
    windowSelectSample->AddChild(selectSampleLayout);

    _sampleComboBox = new ComboBox(_GUI, Left | CenterV, Vector2i(250,0));
    _sampleComboBox->MarginRight(5);
    _sampleComboBox->HandlerEngineName(GameEngine::ClassName());
    _sampleComboBox->HandlerId(GameEngine::SampleSelected);
    _sampleComboBox->Percent(Vector2f(100, 0));
    selectSampleLayout->AddChild(_sampleComboBox);
    selectSampleLayout->SetExpandRatio(_sampleComboBox, 100);

    Button *button = new Button("Start", Center, Vector2i(70, 28), "arial");
    selectSampleLayout->AddChild(button);
    button->HandlerEngineName(GameEngine::ClassName());
    button->HandlerId(GameEngine::StartSample);
    return windowSelectSample;
}

GUI::SubWindow      *MainMenu::CreateSampleWindow(Window *windowParent)
{
    if (_currentSampleWindow != NULL)
        _widgetSampleWindow->RemoveWidget(_currentSampleWindow);

    _currentSampleWindow = new GUI::SubWindow(windowParent, Center);
    _currentSampleWindow->Percent(Vector2f(100, 100));
    _widgetSampleWindow->AddChild(_currentSampleWindow);
    return _currentSampleWindow;
}

void    MainMenu::CloseSampleWindow()
{
    if (_currentSampleWindow != NULL)
        _currentSampleWindow->GetSubWindow()->Close();
}

void    MainMenu::SampleSelected()
{
    if (_sampleComboBox->CurrentItem() != NULL && _sampleComboBox->CurrentItem() != _currentItemSelected)
    {
        _currentItemSelected = _sampleComboBox->CurrentItem();

        // unset the sprite
        _sampleImage->Sprite(NULL);

        // create the sprite used for the description of the sample
        Utils::FileName pictureFile = "Nc:Image:Samples/" + _currentItemSelected->Data() + ".png";
        if (pictureFile.IsReadable())
        {
            Sprite *sp = new Sprite(Vector2i(0,0), GL::Texture(pictureFile), 100);
            _sampleImage->Sprite(sp);
            _sampleImage->Size(Vector2i(0, imageHeight));
        }
        else
            _sampleImage->Size(Vector2i(0, 0));
        _layoutWinDesc->Resized();
    }
}
