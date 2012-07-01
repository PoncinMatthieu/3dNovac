
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

    File Created At:        30/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WIDGET_TEXTEDIT_H_
#define NC_GUI_WIDGET_TEXTEDIT_H_

#include "ScrollArea.h"
#include "TextArea.h"

namespace Nc
{
    namespace GUI
    {
        class TextEdit : public ScrollArea
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(ScrollArea, System::Object, Nc::GUI::TextEdit);

            public:
                TextEdit(const Utils::Unicode::UTF32 &text, const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0, 0), const std::string &ttf = "arial");
                ~TextEdit();

                /** \return the attached text area */
                TextArea    *GetTextArea()   {return _textArea;}

            protected:
                /** Render the textArea before disabling the stencil */
                virtual void        RenderChildsEnd(Graphic::SceneGraph *scene);
                /** Render the scroll area */
                virtual void        Draw(Graphic::SceneGraph *scene);

            private:
                TextArea    *_textArea;
        };
    }
}

#endif
