
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "FPSWidget.h"

using namespace Nc;
using namespace Nc::GUI;

FPSWidget::FPSWidget(Corner x, Corner y)
    : WidgetLabeled("000", 20, x, y, Vector2i(0,5), Vector2i(0,20))
{
    _size.Data[1] = _label->Size().Data[1];
    LabelColor(Color(1,1,1));
}

void FPSWidget::Draw(Graphic::SceneGraph *scene)
{
    //static unsigned int     frequency = 10; // nous permet d'avoir une frequence de refresh relativement constante
    static unsigned int     i = 0;
    static unsigned int     moyFps = 0;

    // toutes les 50 frames, on reset, et print la moyenne de fps
    if (i >= 50/*frequency*/)
    {
        std::ostringstream FPS;
        //frequency = moyFps / frequency;
        FPS << (moyFps / i);
        Label(FPS.str());
        moyFps = 0;
        i = 0;
    }
    unsigned int fps = Graphic::Engine::GetFPS();
    if (fps > 0)
    {
        moyFps += fps;
        ++i;
    }
    WidgetLabeled::Draw(scene);
}
