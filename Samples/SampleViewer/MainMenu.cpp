
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
#include <Nc/GUI/ComboBox.h>
#include <Nc/GUI/ScrollBar.h>

#include "MainMenu.h"
#include "GameEngine.h"

using namespace Nc::Utils;
using namespace Nc::System;
using namespace Nc::GUI;
using namespace Nc::Graphic;

MainMenu::MainMenu(Nc::GUI::SceneGraph *gui)
    : _GUI(gui)
{
    // create a layout to seperate the select list and the sample
    Layout *layout = new Layout(Layout::Horizontal, Left, Top);
    layout->UseLook("Background");
    _GUI->AddChild(layout);

    Layout *layoutChooseSample = new Layout(Layout::Vertical, Center, Center);
    layout->AddChild(layoutChooseSample);

    WidgetLabeled *labelChooseSample = new WidgetLabeled("Choose a sample", 20, Center, Bottom, Vector2i(0, 5));
    labelChooseSample->LabelColor(Color(1,1,1));
    layoutChooseSample->AddChild(labelChooseSample);

    Widget *windowSelectSample = new Widget(Center, Top);
    windowSelectSample->Percent(Vector2f(95, 95));
    windowSelectSample->UseLook("Window");
    layoutChooseSample->AddChild(windowSelectSample);

    windowSelectSample->MarginY(15);
    Button *button = new Button("Start Sample", Center, Top, Vector2i(0,0), 20, 15, 8, "arial");
    windowSelectSample->AddChild(button);

    ComboBox *cb = new ComboBox(_GUI, Center, Top, Vector2i(0,50), Vector2i(250,0));
    cb->AddItem(new Item("plop"));
    cb->AddItem(new Item("nop"));
    cb->AddItem(new Item("top"));
    cb->AddItem(new Item("bottom"));
    windowSelectSample->AddChild(cb);


    ScrollArea *scrollSelectSample = new ScrollArea(Center, Top, Vector2i(0,90), Vector2i(400,400));
    scrollSelectSample->UseLook();
    windowSelectSample->AddChild(scrollSelectSample);

    Vector2i size(1000,1000);
    Sprite *sp = new Sprite(size, GL::Texture(Nc::Utils::FileName("Nc:Image:sky/space_bk.png")), 100.f);
    SpriteWidget *spw = new SpriteWidget(Left, Top, Vector2i(0,0), size);
    spw->AddSprite(sp, Vector2f(0,0));
    scrollSelectSample->AddChild(spw);
    scrollSelectSample->SetView(spw);


/*
    ScrollBar *scrollBar = new ScrollBar(Center, Top, Vector2i(0,110), Vector2i(400,400), Vertical);
    scrollBar->TotalSize(100);
    scrollBar->PageSize(30);
    scrollBar->Position(40);
    windowSelectSample->AddChild(scrollBar);
*/


    Layout *layoutSample = new Layout(Layout::Vertical, Center, Center);
    layout->AddChild(layoutSample);


    WidgetLabeled *labelSample = new WidgetLabeled("Sample", 20, Center, Bottom, Vector2i(0, 5));
    labelSample->LabelColor(Color(1,1,1));
    layoutSample->AddChild(labelSample);


    WindowBox *sampleWindowBox = new WindowBox("Sample", Center, Top);
    sampleWindowBox->DrawTitle(false);
    layoutSample->AddChild(sampleWindowBox);


    // set the ratio and percent at the end, when all childs has been added
    layout->Percent(Vector2f(100, 100));
    layoutChooseSample->SetRatio(labelChooseSample, 5);
    layoutChooseSample->Percent(Vector2f(100, 100));
    layoutSample->SetRatio(labelSample, 20);
    layoutSample->Percent(Vector2f(100, 100));
    windowSelectSample->Percent(Vector2f(90, 95));
    sampleWindowBox->Percent(Vector2f(95, 75));
}

MainMenu::~MainMenu()
{
}

