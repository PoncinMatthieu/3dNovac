
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

    File Created At:        28/02/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_SCROLLBAR_H_
#define NC_GUI_SCROLLBAR_H_

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// Enable users to access part of a document that is larger than the widget used to display it
        class LGUI ScrollBar : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::ScrollBar);

            public:
                ScrollBar(Corner x = Left, Corner y = Top, const Vector2i &pos = Vector2i(0, 0), int length = 0, Orientation orientation = Vertical, const std::string &lookName = "");
                ScrollBar(const ScrollBar &sc);
                ScrollBar &operator = (const ScrollBar &sc);
                ~ScrollBar();

                virtual ISceneNode  *Clone() const              {return new ScrollBar(*this);}

                /** Set the total size of the document */
                void        TotalSize(float s)                  {_totalSize = s;}
                /** \return the total size of the document */
                float       TotalSize() const                   {return _totalSize;}

                /** Set the page size of the view */
                void        PageSize(float s)                   {_pageSize = s;}
                /** \return the page size of the slider */
                float       PageSize() const                    {return _pageSize;}

                /** Set the total position in the document */
                void        Position(float s)                   {_position = s;}
                /** \return the position of the slider */
                float       Position() const                    {return _position;}

            protected:
                /** Handler of Mouse motion event */
                virtual void        MouseMotionEvent(const Nc::System::Event &e);
                /** Handler of Mouse button event */
                virtual void        MouseButtonEvent(const Nc::System::Event &e);

                /** Update the sprites */
                virtual void        Update();
                /** Render the scroll area */
                virtual void        Draw(Graphic::SceneGraph *scene);

            private:
                void    Copy(const ScrollBar &sc);

                void    MouseButtonLeft(const Nc::System::Event &event);
                void    MouseButtonRight(const Nc::System::Event &event);
                void    MouseButtonSlider(const Nc::System::Event &event);

                /** \return the slider size */
                float   GetSliderSize();
                /** \return the slider translation value according to the position in the document */
                float   GetSliderTranslation(float position);
                /** \return the position in the document according to the given slider translation */
                float   GetPagePosition(float translation);

            private:
                Orientation         _orientation;               ///< Define the orientation of the scroll bar

                float               _totalSize;                 ///< Define the size of the document (this will allow us to compute the size of the slider)
                float               _pageSize;                  ///< Define the size of the view (this will allow us to compute the size of the slider)
                float               _position;                  ///< Define the position in the document

                bool                _buttonLeftPressed;         ///< true if the button left is pressed
                bool                _buttonRightPressed;        ///< true if the button right is pressed
                bool                _buttonSliderPressed;       ///< true if the slider is pressed

                Vector2i            _lastPosMouse;              ///< record the last position of the mouse

                Graphic::Sprite     *_spriteLeftButton;         ///< sprite of the left button
                Graphic::Sprite     *_spriteRightButton;        ///< sprite of the right button
                Graphic::Sprite     *_spriteSliderBackground;   ///< sprite of the slider's background
                Graphic::Sprite     *_spriteSliderLeft;         ///< sprite of the left of the slider
                Graphic::Sprite     *_spriteSliderMiddle;       ///< sprite of the middle of the slider
                Graphic::Sprite     *_spriteSliderRight;        ///< sprite of the right of the slider
                Graphic::Sprite     *_spriteSliderExpand1;      ///< sprite of the expand region of the slider
                Graphic::Sprite     *_spriteSliderExpand2;      ///< sprite of the expand region of the slider

                Graphic::Sprite     *_spriteLeftButtonPressed;      ///< sprite of the left button pressed
                Graphic::Sprite     *_spriteRightButtonPressed;     ///< sprite of the right button pressed
                Graphic::Sprite     *_spriteSliderLeftPressed;      ///< sprite of the left of the slider pressed
                Graphic::Sprite     *_spriteSliderMiddlePressed;    ///< sprite of the middle of the slider pressed
                Graphic::Sprite     *_spriteSliderRightPressed;     ///< sprite of the right of the slider pressed
                Graphic::Sprite     *_spriteSliderExpand1Pressed;   ///< sprite of the expand region of the slider pressed
                Graphic::Sprite     *_spriteSliderExpand2Pressed;   ///< sprite of the expand region of the slider pressed
        };
    }
}

#endif
