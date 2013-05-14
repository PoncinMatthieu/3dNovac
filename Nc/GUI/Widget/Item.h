
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

    File Created At:        22/02/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_ITEM_H_
#define NC_GUI_ITEM_H_

#include "Label.h"

namespace Nc
{
    namespace GUI
    {
        /// Label items to be used with widget like ComboBox or TreeWidget.
        class LIB_NC_GUI Item : public Label
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Label, Nc::System::Object, Nc::GUI::Item);

            public:
                Item(const Utils::Unicode::UTF32 &label = "", float fontSize = 15, const Color &fontColor = Color(0,0,0), const std::string &fontName = "arial", Nc::Graphic::PlainTextFormater::Style s = Nc::Graphic::PlainTextFormater::Regular);
                ~Item();

                /** \return a clone of the item. */
                virtual Graphic::ISceneNode         *Clone() const                          {return new Item(*this);}

                /** \return true if the item is selected. */
                bool            Selected() const        {return _selected;}
                /** Select or unselect the item. */
                virtual void    Selected(bool st);
                /** Select the item. */
                void            Select()                {Selected(true);}
                /** Unselect the item. */
                void            Unselect()              {Selected(false);}

                /** Called when the item is double clicked. */
                virtual void    DoubleClicked()         {}

            protected:
                /** Manage item selection. */
                virtual void    MouseButtonEvent(const System::Event &event);

            protected:
                bool        _selected;
        };
    }
}

#endif
