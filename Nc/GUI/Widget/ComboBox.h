
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

    File Created At:        21/02/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WIDGET_COMBOBOX_H_
#define NC_GUI_WIDGET_COMBOBOX_H_

#include "../SceneGraph.h"
#include "Item.h"

namespace Nc
{
    namespace GUI
    {
        /// Define a comboBox Widget.
        /**
            A comboBox show a dropdown list when you click on it allow to select an item.
            The comboBox use a list of Item object to select.

            Once an item is choosed, the combobox send the event ItemChanged.

            \todo correct dropdown list which should start below the box, to be able to show the currently selected item.
        */
        class LIB_NC_GUI ComboBox : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(GUI::Widget, System::Object, Nc::GUI::ComboBox);

            private:
                /// Inner class used to display the dropdown list.
                class ComboBoxUnfoldList : public Widget
                {
                    public:
                        ComboBoxUnfoldList(ComboBox *cb, const AlignmentMask &alignment, const Vector2i &pos, const Vector2i &size);

                        virtual ISceneNode  *Clone() const          {return new ComboBoxUnfoldList(*this);}

                        /** The mouse button handler to choose an item. */
                        void                MouseButtonEvent(const System::Event &event);
                        /** Render the unfold list. */
                        virtual void        Draw(Graphic::SceneGraph *scene);

                        /** Called when the widget loose the focus. */
                        virtual void        LeaveFocus();

                    private:
                        ComboBox    *_cb;       ///< pointer to the comboBox.
                        bool        _new;       ///< used to avoid managing the first button release event, and close the list immediatly.
                };


            public:
                ComboBox(GUI::SceneGraph *scene, const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0, 0), const std::string &looksName = "");
                ComboBox(const ComboBox &cb);
                ComboBox &operator = (const ComboBox &cb);
                ~ComboBox();

                virtual ISceneNode  *Clone() const          {return new ComboBox(*this);}

                /** Add an item to the list, every item will be deleted in the destructor of the comboBox. */
                void                AddItem(Item *item);

                /** \return the pointer to the current item selected. */
                inline Item         *CurrentItem()          {return _currentItem;}

            protected:
                void                Copy(const ComboBox &cb);

                /** Update the geometry of the combo box. */
                virtual void        UpdateState();
                /** Render the combo box. */
                virtual void        Draw(Graphic::SceneGraph *scene);

                /** The mouse button handler to choose an item. */
                void                MouseButtonEvent(const System::Event &event);

                /** Send an event to notify a changement of item. */
                void                ItemChoosed(Item *item);

            private:
                SceneGraph          *_scene;
                Graphic::Sprite     *_spriteList;

                ListPItem           _itemList;
                Item                *_currentItem;

                bool                _listUnrolled;
                ComboBoxUnfoldList  *_currentUnfoldList;


                friend class ComboBoxUnfoldList;
        };
    }
}

#endif
