
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
/*-----------------------------------------------------------------------------

    class FPSWidget, print in as a label Widget the Frame Per Second

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_FPSWIDGET_H_
#define NC_GUI_FPSWIDGET_H_

#include "WidgetLabeled.h"

namespace Nc
{
    namespace GUI
    {
        class LGUI FPSWidget : public WidgetLabeled
        {
            public:
                FPSWidget(Corner x = Center, Corner y = Top);

                virtual Widget* Clone() const               {return new FPSWidget(*this);}

                void Render(Graphic::ISceneGraph *scene);

            protected:
                unsigned int    _fps;
        };
    }
}

#endif
