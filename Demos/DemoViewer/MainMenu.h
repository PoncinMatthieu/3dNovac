
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

#ifndef SAMPLEVIEWER_MAINMENU_H_
#define SAMPLEVIEWER_MAINMENU_H_

#include <Nc/GUI/GUI.h>
#include <Nc/GUI/Widget/SubWindow.h>
#include <Nc/GUI/Widget/TextEdit.h>

namespace DemoViewer
{
    class MainMenu : Nc::Utils::NonCopyable
    {
        public:
            MainMenu(Nc::GUI::SceneGraph *gui);
            ~MainMenu();

            Nc::GUI::Console            *GetConsole()       {return _console;}

            void                        ExecConsoleCommand();

            void                        AddDemo(const std::string &name);

            void                        DemoSelected();

            const std::string           *Demo() const         {return (_demoComboBox->CurrentItem()) ? &_demoComboBox->CurrentItem()->Data() : NULL;}

            Nc::GUI::SubWindow          *CreateDemoWindow(Nc::Graphic::IWindow *windowParent);

            void                        CloseDemoWindow();

            /** Append messages from the Utils::Logger to the console. */
            static void                 Write(const std::string &msg, bool flush);

        private:
            Nc::GUI::Widget             *CreateDescriptionPannel(Nc::GUI::Layout *parent);
            Nc::GUI::Widget             *CreateSelectDemoWindow(Nc::GUI::Layout *parent);

        private:
            Nc::GUI::SceneGraph         *_GUI;
            static Nc::GUI::Console	    *_console;
            Nc::GUI::ComboBox           *_demoComboBox;
            Nc::GUI::Image              *_demoImage;
            unsigned int                imageHeight;
            Nc::GUI::TextEdit           *_descriptionTextArea;
            Nc::GUI::Layout             *_layoutWinDesc;

            Nc::GUI::Widget             *_widgetDemoWindow;
            Nc::GUI::SubWindow          *_currentDemoWindow;

            Nc::GUI::Item               *_currentItemSelected;
    };
}

#endif
