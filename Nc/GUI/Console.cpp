
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

#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::System;
using namespace Nc::Graphic;
using namespace Nc::GUI;

Console::ListMsg                    Console::_listMsg;
Console::ListMsg::reverse_iterator  Console::_itCurrentMsg = Console::_listMsg.rbegin();
Mutex                               Console::_mutexMsg;
std::string							Console::_currentWritingMsg;

Console::Console(Pattern p)
    : WindowBox(ClassName(), "Console", "Prototype"), _pattern(p)
{
    unsigned short  percent = 15; // hauteur de la console en poucentage

    _margin.Init(5, 5);
    _corner[1] = Bottom;
    _prompt = "[Main]> ";
    _engineName = "Main";

    _size.Data[0] = Window::Width() - 1;
    _size.Data[1] = ((float)(percent * Window::Height()) / 100.0) + _titleHeight;
    _pos[0] = 1; // pour voir tous les bord de la console, on se decale de 1

    _labelPrompt = new WidgetLabeled(_prompt, Vector2f(0, 0), Vector2f(0, 17), Left, Bottom, "Prototype");
    _labelPrompt->LabelColor(Color(1, 1, 1));
    AddChild(_labelPrompt);

    Vector2f size;
    _labelPrompt->GetReelSize(size);
    _labelWrite = new WidgetLabeled("", Vector2f(size.Data[0], 0), Vector2f(0, 17), Left, Bottom, "Prototype");
    _labelWrite->LabelColor(Color(1, 1, 1));
    AddChild(_labelWrite);

    _labelCursor = new WidgetLabeled("_", Vector2f(size.Data[0], 0), Vector2f(0, 17), Left, Bottom, "arial");
    _labelCursor->LabelColor(Color(0, 1, 0));
    AddChild(_labelCursor);

    #ifndef _DEBUG_GUI_DISABLE_CONSOLE_LOGGING
        LOG.SetLoggingFunction(Write); // set le logger de base dans notre console graphique
    #endif
    _drawTitle = true;
}

Console::~Console()
{
    DeleteListFont();
}

void Console::DeleteListFont()
{
    while(!_listFont.empty())
    {
        delete *(_listFont.begin());
        _listFont.pop_front();
    }
}

void Console::Write(const std::string msg, bool flush)
{
    _currentWritingMsg += msg;
    if (flush)
    {
        PushMsg(_currentWritingMsg);
        _currentWritingMsg.clear();
    }
}

void Console::GetReelPos(Vector2f &pos) const
{
    WindowBox::GetReelPos(pos);
    if (_pattern == TranslateAtFocus && !_focus)
        pos.Data[1] -= _size.Data[1] - _titleHeight;
}

void Console::Update()
{
    WindowBox::Update();
    // met a jour la taille de la console en fonction de la taille de la fenetre
    unsigned short  percent = 15; // hauteur de la console en poucentage
    _size.Data[0] = Window::Width() - 1;
    _size.Data[1] = ((float)(percent * Window::Height()) / 100.0) + _titleHeight;

    // met a jour les string de message
    if ((_pattern == TranslateAtFocus && _focus) || (_pattern == Nop))
    {
        TMatrix         m;
        float           textHeight1 = _labelPrompt->GetLabel()->CharSize(), textHeight2 = 14;
        unsigned int    nbMsg = (((float)(_size.Data[1] - _titleHeight - textHeight1)) / textHeight2);

        if (nbMsg != _listFont.size())
        {
            DeleteListFont();
            m.AddTranslation(10., (double)textHeight1 + _margin.Data[1] + _pos.Data[1], 0.);
            for (unsigned int i = 0; i < nbMsg; i++)
            {
                String *newString = new String("", textHeight2, Color(0.8f, 0.8f, 0.8f), "arial");
                newString->Matrix = m;
                _listFont.push_back(newString);
                m.AddTranslation(0., (double)textHeight2, 0.);
            }
        }
        _scroll = 0;
    }
}

void Console::Render(Graphic::SceneGraph *scene)
{
    WindowBox::Render(scene);

    if ((_pattern == TranslateAtFocus && _focus) || (_pattern == Nop))
    {
        _mutexMsg.Lock();
        ListMsg::reverse_iterator itMsg = _listMsg.rbegin();
        for (unsigned int i = 0; i < _scroll; i++)
            if (itMsg != _listMsg.rend())
                ++itMsg;

        for (list<Graphic::String*>::iterator it = _listFont.begin(); it != _listFont.end(); ++it)
            if (itMsg != _listMsg.rend())
            {
                if (*itMsg != (*it)->Text())
                    (*it)->Text(*itMsg);
                (*it)->RenderNode(scene);
                ++itMsg;
            }
        _mutexMsg.Unlock();
    }
}

void Console::KeyboardEvent(const Event &event)
{
    static bool                     lastcmdSet = false;
    static Utils::Unicode::UTF32    cmd;
    char                            c;

    if (!_inhibit && event.Type == Event::KeyPressed)
    {
        if (event.Key.Code == Key::Up || event.Key.Code == Key::Down)
        {
            _mutexMsg.Lock();
            static Utils::Unicode::UTF32   lastcmd;
            if (event.Key.Code == Key::Up)
            {
                if (_itCurrentMsg != _listMsg.rend())
                {
                    if (!lastcmdSet)
                    {
                        lastcmd = cmd;
                        lastcmdSet = true;
                    }
                    ++_itCurrentMsg;
                    if (_itCurrentMsg != _listMsg.rend())
                    {
                        cmd = *_itCurrentMsg;
                        _labelWrite->Label(cmd);
                        Vector2f sizeLabelWrite;
                        Vector2f sizeLabelPrompt;
                        _labelWrite->GetReelSize(sizeLabelWrite);
                        _labelPrompt->GetReelSize(sizeLabelPrompt);
                        _labelCursor->Pos(Vector2f(sizeLabelWrite.Data[0] + sizeLabelPrompt.Data[0], 0));
                    }
                }
            }
            else if (event.Key.Code == Key::Down)
            {
                if (_itCurrentMsg == _listMsg.rbegin())
                {
                    if (lastcmdSet)
                    {
                        cmd = lastcmd;
                        _labelWrite->Label(cmd);
                        Vector2f sizeLabelWrite;
                        Vector2f sizeLabelPrompt;
                        _labelWrite->GetReelSize(sizeLabelWrite);
                        _labelPrompt->GetReelSize(sizeLabelPrompt);
                        _labelCursor->Pos(Vector2f(sizeLabelWrite.Data[0] + sizeLabelPrompt.Data[0], 0));
                        lastcmdSet = false;
                    }
                }
                else
                {
                    --_itCurrentMsg;
                    cmd = *_itCurrentMsg;
                    _labelWrite->Label(cmd);
                    Vector2f sizeLabelWrite;
                    Vector2f sizeLabelPrompt;
                    _labelWrite->GetReelSize(sizeLabelWrite);
                    _labelPrompt->GetReelSize(sizeLabelPrompt);
                    _labelCursor->Pos(Vector2f(sizeLabelWrite.Data[0] + sizeLabelPrompt.Data[0], 0));
                }
            }
            _mutexMsg.Unlock();
        }
        else if (event.Key.Code == Key::PageDown && _scroll > 0) // scroll
            _scroll--;
        else if (event.Key.Code == Key::PageUp)
            _scroll++;
        else if (event.Key.Code == Key::Return && !cmd.empty()) //execute la commande, et vide la string
        {
            LOG << cmd << endl;
            ExecCmd(cmd.ToStdString());
            cmd.clear();
            _labelWrite->Label("");
            lastcmdSet = false;
            Vector2f sizeLabelWrite;
            Vector2f sizeLabelPrompt;
            _labelWrite->GetReelSize(sizeLabelWrite);
            _labelPrompt->GetReelSize(sizeLabelPrompt);
            _labelCursor->Pos(Vector2f(sizeLabelWrite.Data[0] + sizeLabelPrompt.Data[0], 0));
        }
        else if (event.Key.Code == Key::Back && !cmd.empty()) // suppression du dernier caractere
        {
            cmd.erase(cmd.end() - 1);
            _labelWrite->Label(cmd);
            Vector2f sizeLabelWrite;
            Vector2f sizeLabelPrompt;
            _labelWrite->GetReelSize(sizeLabelWrite);
            _labelPrompt->GetReelSize(sizeLabelPrompt);
            _labelCursor->Pos(Vector2f(sizeLabelWrite.Data[0] + sizeLabelPrompt.Data[0], 0));
            lastcmdSet = false;
        }
        else    // sinon on peut recuperer le caractere et le push dans le label
        {
            c = WindowInput::ToChar(event.Key.Code);
            if (c == '\t' || (c >= ' ' && c <= '~'))      // ajout du caractere dans la string de commande
            {
                cmd += c;
                _labelWrite->Label(cmd);
                Vector2f sizeLabelWrite;
                Vector2f sizeLabelPrompt;
                _labelWrite->GetReelSize(sizeLabelWrite);
                _labelPrompt->GetReelSize(sizeLabelPrompt);
                _labelCursor->Pos(Vector2f(sizeLabelWrite.Data[0] + sizeLabelPrompt.Data[0], 0));
            }
            lastcmdSet = false;
        }
    }
}

void Console::PushMsg(const string &s)
{
    _mutexMsg.Lock();
    _listMsg.push_back(s);
    if (_listMsg.size() > 150)
        _listMsg.erase(_listMsg.begin(), _listMsg.end());
    _itCurrentMsg = _listMsg.rbegin();
    _mutexMsg.Unlock();
}

void Console::ExecCmd(const string &cmd)
{
    string cmdName(cmd), engineName = _engineName;
    size_t pos = cmd.find_first_of(":");

	// npos en commentaire a cause d'un bug VC 2010 a l'export de string::npos (et oui encore ce foutu windows)  ref: http://connect.microsoft.com/VisualStudio/feedback/details/562448/std-string-npos-lnk2001-when-inheriting-a-dll-class-from-std-string
	if (pos != /*string::npos*/ (size_t)-1)
    {
        engineName = cmd.substr(0, pos);
        cmdName = cmdName.substr(pos + 1, cmd.length());
    }
    size_t pos2 = cmdName.find_first_of(" ");
    if (pos2 != /*string::npos*/ (size_t)-1)   // execution de la commande avec les arguments
        Engine::Manager::PushEvent(engineName, cmdName.substr(0, pos2), cmdName.substr(pos2+1, /*string::npos*/ -1));
    else                        // execution de la commande sans argument
        Engine::Manager::PushEvent(engineName, cmdName.substr(0, pos2));
}
