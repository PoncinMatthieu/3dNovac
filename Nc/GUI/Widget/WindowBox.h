
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

#ifndef NC_GUI_WIDGET_WINDOWBOX_H_
#define NC_GUI_WIDGET_WINDOWBOX_H_

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// Defines a windowBox Widget.
        /**
            A windowBox is just a simple widget displaying a title into it's top edge looks.

            \todo Make the WindowBox dragable.
        */
        class LIB_NC_GUI  WindowBox : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::WindowBox);

            public:
                WindowBox(const std::string &title, const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0,0), const std::string &titleTtf = "arial", const std::string &looksName = "");
                WindowBox(const WindowBox &w);
                WindowBox &operator = (const WindowBox &w);
                virtual ~WindowBox();

                virtual ISceneNode  *Clone() const                              {return new WindowBox(*this);}
                virtual void        ToString(std::ostream &os) const;

                /** Change the title of the window. */
                void            ChangeTitle(const std::string &title, float charSize, const std::string &ttf, const AlignmentMask &mask);

            protected:
                /** Update the geometry of the window. */
                virtual void    UpdateState();
                /** Render the window. */
                virtual void    Draw(Graphic::SceneGraph *scene);

            private:
                /** Initialize the widget. */
                void            Init(const std::string &title, float charSize, const std::string &ttf, const AlignmentMask &mask, const std::string &looksName);

                /** Copy the widget properties. */
                void            Copy(const WindowBox &w);

            protected:
                //bool                _dragable;

                Graphic::Text       *_title;            ///< the title of the window.
                AlignmentMask       _titleAlignment;    ///< define the alignment of the title (Left || CenterH || Right).
        };
    }
}

#endif

