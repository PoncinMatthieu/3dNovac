
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

#include "MainMenu.h"
#include "GameEngine.h"

using namespace Nc;
using namespace Nc::Utils;
using namespace Nc::System;
using namespace Nc::GUI;
using namespace Nc::Graphic;
using namespace GUIDemo;

MainMenu::MainMenu(Nc::GUI::SceneGraph *gui)
    : _GUI(gui)
{
    Vector2f    winsize(470, 550);
    Vector2f    winsizeMode(470, 200);
    Vector2f    winsizeInGame(300, 110);

// selectMode IWindow
    _selectModeWindow = new WindowBox("Main menu", Center, winsizeMode);

    Widget *selectModeWidget = new Widget(CenterH | Top, Vector2f(winsizeMode[0] - 20, winsizeMode[1] - 20));
    selectModeWidget->Pos(Vector2i(0, 10));
    _selectModeWindow->AddChild(selectModeWidget);

    Button *button = new Button("Single Player", Center, Vector2f(140, 20), "Prototype");
    button->Pos(Vector2f(0, 20));
    button->HandlerEngineName(GameEngine::ClassName());
    button->HandlerId(GameEngine::ModeSinglePlayer);
    selectModeWidget->AddChild(button);
    button = new Button("Mutiplayer", Center, Vector2f(140, 20), "Prototype");
    button->Pos(Vector2f(0, -20));
    button->HandlerEngineName(GameEngine::ClassName());
    button->HandlerId(GameEngine::ModeMultiplayer);
    selectModeWidget->AddChild(button);
    _GUI->AddChild(_selectModeWindow);

    UpdateMode(SelectMode);
}

MainMenu::~MainMenu()
{
}

void MainMenu::UpdateMode(Pattern mode)
{
    _pattern = mode;

    System::Locker l(&_GUI->GetMutex());
    _selectModeWindow->Enable(false);
//    _multiModeWindow->Enable(false);
//    _inGameWindow->Enable(false);
//    _gameOverWidget->Enable(false);
//    _loadingWidget->Enable(false);
    switch (_pattern)
    {
        case SelectMode:
            _selectModeWindow->Enable(true);
        break;
/*
        case InGame:
            _GUI->BringToFront(_inGameWindow);
            _inGameWindow->Enable(true);
        break;

        case GameOver:
            _gameOverWidget->Enable(true);
        break;

        case Loading:
            _loadingWidget->Enable(true);
        break;

        case SelectGame:
        case WaitingRoom:
        case CreateGame:
        {
            _multiModeWindow->Enable(true);
            _selectGameWidget->Enable((_pattern == SelectGame) ? true : false);
            _waitingRoomWidget->Enable((_pattern == WaitingRoom) ? true : false);
            _createGameWidget->Enable((_pattern == CreateGame) ? true : false);
            break;
        }
*/    }
}
