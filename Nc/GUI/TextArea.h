
/*-----------------------------------------------------------------------------

	3dNovac Graphics
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

    File Created At:        18/12/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_TEXTAREA_H_
#define NC_GUI_TEXTAREA_H_

#include "ScrollArea.h"

namespace Nc
{
    namespace GUI
    {
        class LGUI TextArea : public ScrollArea
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(ScrollArea, System::Object, Nc::GUI::TextArea);

            public:
                TextArea(Corner x = Top, Corner y = Left, const Vector2i &pos = Vector2i(0, 0), const Vector2i &size = Vector2i(0, 0));

                virtual ISceneNode  *Clone() const          {return new TextArea(*this);}

            private:
                Utils::Unicode::UTF32   _text;
                bool                    _editable;
        };
    }
}

#endif
