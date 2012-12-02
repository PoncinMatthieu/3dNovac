
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

    File Created At:        09/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WIDGET_FPSWIDGET_H_
#define NC_GUI_WIDGET_FPSWIDGET_H_

#include "Label.h"

namespace Nc
{
    namespace GUI
    {
        /// Define a widget displaying the current frame per second of the Graphic::Engine.
        class LGUI FPSWidget : public Label
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Label, System::Object, Nc::GUI::FPSWidget);

            public:
                FPSWidget(const AlignmentMask &alignment = Center | Top);

                virtual ISceneNode  *Clone() const              {return new FPSWidget(*this);}

            public:
                /** Update the geometry of the fps widget. */
                virtual void UpdateState();
                /** Render the fps. */
                virtual void Draw(Graphic::SceneGraph *scene);
        };
    }
}

#endif
