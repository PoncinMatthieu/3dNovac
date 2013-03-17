
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

#ifndef NC_GUI_WIDGET_CONSOLE_H_
#define NC_GUI_WIDGET_CONSOLE_H_

#include <list>
#include "WindowBox.h"
#include "Label.h"
#include "TextEdit.h"
#include "LineEdit.h"
#include "Layout.h"


namespace Nc
{
    namespace GUI
    {
        /// To manage a console, contain a text edit to display text with a line edit to send commands.
        /**
            Send a ReturnPressed event when a command is sent, the line edit and text edit will stay unchanged until you modify it.
        */
        class LGUI  Console : public Widget, Utils::NonCopyable
        {
            private:
                /// Used to manage event of composed widgets.
                class Listener : public Nc::Engine::EventManager
                {
                    public:
                        Listener(Console *console)
                            : _console(console)
                        {
                            _throwErrorAtUnknownCommand = false;
                            AddNewCmd(Event::ReturnPressed,    (Nc::Engine::CmdFunction)&Listener::ReturnPressedCmd);
                        }

                    protected:
                        void    ReturnPressedCmd(Nc::Engine::IEvent *e)
                        {
                            _console->SendCommand();
                        }

                    protected:
                        Console *_console;
                };

            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::Console);

            public:
                Console(const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0,0));
                virtual ~Console();

                /** \return the line edit content. */
                const Utils::Unicode::UTF32     &LineEditText() const       {return _lineEdit->PlainText();}
                /** Clear the line edit. Most likely called after you receive a ReturnPressed event. */
                void                            ClearLineEdit()             {_lineEdit->PlainText("");}

                /** \return the text hold by the text edit. */
                const Utils::Unicode::UTF32     &TextEditText() const       {return _textEdit->PlainText();}
                /** Clear the text edit. */
                void                            ClearTextEdit()             {_textEdit->PlainText("");}
                /** Append the given \p text to the text edit. */
                void                            AppendText(const Utils::Unicode::UTF32 &text);

                /** receive events from composed widgets. */
                virtual void        Update(float elapsedTime);

            protected:
                /** Render the root composed widgets. */
                virtual void        RenderEnd(Graphic::SceneGraph *scene);

                /** Send the command. */
                virtual void        SendCommand();

            protected:
                Listener                    _listener;          ///< listen composed widget to manage correctly the console.
                Layout                      *_centralLayout;    ///< root layout.
                TextEdit                    *_textEdit;         ///< the text edit used to display infos.
                LineEdit                    *_lineEdit;         ///< the line edit used to send commands.

                friend class Listener;
        };
    }
}

#endif

