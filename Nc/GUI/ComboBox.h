
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

#ifndef NC_GUI_COMBOBOX_H_
#define NC_GUI_COMBOBOX_H_

#include "SceneGraph.h"
#include "Widget.h"
#include "Item.h"

namespace Nc
{
    namespace GUI
    {
        class LGUI ComboBox : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(GUI::Widget, System::Object, Nc::GUI::ComboBox);
                typedef std::pair<Item*, Graphic::String*>      StringItem;
                typedef std::list<StringItem>                   ListItem;

            private:
                class ComboBoxUnfoldList : public Widget
                {
                    public:
                        ComboBoxUnfoldList(ComboBox *cb, const AlignmentMask &alignment, const Vector2i &pos, const Vector2i &size);

                        virtual ISceneNode  *Clone() const          {return new ComboBoxUnfoldList(*this);}

                        /** The mouse button handler to choose an item */
                        void                MouseButtonEvent(const System::Event &event);
                        /** Render the unfold list */
                        virtual void        Draw(Graphic::SceneGraph *scene);

                        /** Called when the widget loose the focus */
                        virtual void        LeaveFocus();

                    private:
                        ComboBox    *_cb;
                };


            public:
                ComboBox(GUI::SceneGraph *scene, const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0, 0), float fontSize = 15, const Color &fontColor = Color(0,0,0), const std::string &fontName = "arial", const std::string &looksName = "");
                ComboBox(const ComboBox &cb);
                ComboBox &operator = (const ComboBox &cb);
                ~ComboBox();

                virtual ISceneNode  *Clone() const          {return new ComboBox(*this);}

                void                AddItem(Item *item);

                inline Item         *CurrentItem()          {return (_currentItem) ? _currentItem->first : NULL;}

            protected:
                void                Copy(const ComboBox &cb);

                /** Update the geometry of the combo box */
                virtual void        Update();
                /** Render the combo box */
                virtual void        Draw(Graphic::SceneGraph *scene);

                /** The mouse button handler to choose an item */
                void                MouseButtonEvent(const System::Event &event);

            private:
                SceneGraph          *_scene;
                float               _fontSize;
                Color               _fontColor;
                std::string         _fontName;
                Graphic::Sprite     *_spriteList;

                ListItem            _itemList;
                StringItem          *_currentItem;

                bool                _listUnrolled;
                ComboBoxUnfoldList  *_currentUnfoldList;


                friend class ComboBoxUnfoldList;
        };
    }
}

#endif
