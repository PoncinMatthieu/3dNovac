
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

#ifndef NC_GUI_CONSOLE_H_
#define NC_GUI_CONSOLE_H_

#include <list>
#include "WindowBox.h"
#include "WidgetLabeled.h"

namespace Nc
{
    namespace GUI
    {
        /// To manage a console
        class LGUI  Console : public WindowBox, Utils::NonCopyable
        {
            /// The pattern of the console
            enum Pattern
            {
                Nop,
                TranslateAtFocus        ///< if set, then the console will translate itsel when she has the focus
            };

            typedef std::list<Utils::Unicode::UTF32>    ListMsg;    ///< The list of message in UTF-32

            public:
                Console(Pattern p = TranslateAtFocus);
                virtual ~Console();

                static const char   *ClassName()            {return "Console";}

                /** Render the console */
                virtual void        Render(Graphic::SceneGraph *scene);

                /** Return the reel position of the console */
                virtual void        GetReelPos(Vector2f &pos) const;

                /** Push a message in the message list of the console */
                static void         PushMsg(const std::string &s);
                /** Write a message for the Utils::Logger */
                static void         Write(const std::string msg, bool flush);

            private:
                /** Update the geometry of the console */
                virtual void Update();

                /** Keyboadr event Handler */
                void KeyboardEvent(const System::Event &event); // receptionne les commandes clavier et l'inscrit dans la console
                /** Delete the list of font used to render the msg */
                void DeleteListFont();

                /** Execute a command by sending an event the good engine */
                void ExecCmd(const std::string &cmd);

                /** Update the position of the cursor */
                void UpdateCursorPosition();

                static ListMsg                      _listMsg;           ///< The message list
                static System::Mutex                _mutexMsg;          ///< Protect the acces of the message list
                static ListMsg::reverse_iterator    _itCurrentMsg;      ///< the current message, to scroll the messages
				static std::string					_currentWritingMsg;	///< The message that is save between a Write and a flush

                std::list<Graphic::String*>         _listFont;          ///< list of string used to render the messages
                WidgetLabeled                       *_labelPrompt;      ///< the label used to render the prompt
                WidgetLabeled                       *_labelWrite;       ///< the label used to render the command text
                WidgetLabeled                       *_labelCursor;      ///< the label used to render the cursor
                float                               _cursorWidth;       ///< posittion of the cursor

                unsigned int                        _scroll;            ///< scroll position
                std::string                         _prompt;            ///< prompt string
                Pattern                             _pattern;           ///< the pattern of the console
        };
    }
}

#endif

