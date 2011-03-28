
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

    File Created At:        04/05/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------


                        Implementation de la classe "gConsole"

                permet l'affichage et la gestion d'une console ingame

Herite de gWindowBox

-----------------------------------------------------------------------------*/

#ifndef NOVAC_GUI_CONSOLE_H_INCLUDED
#define NOVAC_GUI_CONSOLE_H_INCLUDED

#include <list>
#include "WindowBox.h"
#include "WidgetLabeled.h"

namespace Nc
{
    namespace GUI
    {
        class LGUI  Console : public WindowBox, Utils::NonCopyable
        {
            enum Pattern
            {
                Nop,
                TranslateAtFocus
            };

            typedef std::list<Utils::Unicode::UTF32>    ListMsg;

            public:
                Console(Pattern p = TranslateAtFocus);
                virtual ~Console();

                virtual void        Render(Graphic::ISceneGraph *scene);

                virtual Vector2f    GetReelPos() const;

                static void         PushMsg(const std::string &s); // push un msg dans la list de msg
                static void         Write(const std::string msg, bool flush);

            private:
                virtual void Update();

                void KeyboardEvent(const System::Event &event); // receptionne les commandes clavier et l'inscrit dans la console
                void DeleteListFont();
                void ExecCmd(const std::string &cmd);

                static ListMsg                      _listMsg;
                static System::Mutex                _mutexMsg; // protege l'acces a la list de msg
                static ListMsg::reverse_iterator    _itCurrentMsg;

                std::list<Graphic::String*>     _listFont;
                std::list<WidgetLabeled*>       _listLabel;
                WidgetLabeled                   *_labelPrompt;
                WidgetLabeled                   *_labelWrite;
                WidgetLabeled                   *_labelCursor;
                float                           _cursorWidth;

                unsigned int                    _scroll;
                std::string                     _prompt;
                Pattern                         _pattern;
        };
    }
}

#endif

