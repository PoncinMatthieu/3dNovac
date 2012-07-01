
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

    File Created At:        01/07/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WIDGET_TEXTDOCUMENT_H_
#define NC_GUI_WIDGET_TEXTDOCUMENT_H_

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// Define a widget holding formated text that can be viewed and edit by using a TextEdit.
        /**
            \todo the text are is not editable right now.
        */
        class LGUI TextDocument : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::TextDocument);

            public:
                TextDocument(const Utils::Unicode::UTF32 &text, const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0, 0), const std::string &ttf = "arial");

                virtual ISceneNode              *Clone() const          {return new TextDocument(*this);}

                /** \return the text. */
                const Utils::Unicode::UTF32     &Text() const           {return _string->Text();}
                /** Set the text. */
                void                            Text(const Utils::Unicode::UTF32 &t);

            protected:
                /** Resize the geometry of the text document. */
                virtual void        Resize();
                /** Update the geometry of the text document. */
                virtual void        Update();
                /** Render the text area. */
                virtual void        Draw(Graphic::SceneGraph *scene);


            private:
                Graphic::String         *_string;
                bool                    _editable;
        };
    }
}

#endif
