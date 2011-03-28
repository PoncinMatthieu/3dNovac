
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

    File Created At:        28/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

    class ProgessBar

-----------------------------------------------------------------------------*/

#ifndef NOVAC_GRAPHIC_GUI_PROGRESS_BAR_H_
#define NOVAC_GRAPHIC_GUI_PROGRESS_BAR_H_

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        class LGUI  ProgressBar : public Widget
        {
            public:
                ProgressBar(const Vector2f &pos = Vector2f(0, 0), const Vector2f &size = Vector2f(10, 10), Corner x = Top, Corner y = Left,
                            Widget *parent = NULL, const std::string &file = "ProgressBar.png");
                virtual ~ProgressBar();

                virtual Widget* Clone() const                                   {return new ProgressBar(*this);}

                void    SetProgressBox(const Box2f &b)                          {_progressBox = b;}
                void    SetColor(const Color &c1, const Color &c2)              {_colorLeft = c1; _colorRight = c2; _stateChange = true;}

                void    Percent(float p)                                        {_percent = p; _stateChange = true;}
                void    NbEvolution(unsigned int nb)                            {_nbEvolution = nb;}
                void    Evolution()                                             {_percent += 100.f / (float)_nbEvolution; _stateChange = true;}

            protected:
                virtual void Update();
                virtual void Draw(Graphic::ISceneGraph *scene);

                Graphic::MaterialConfig<Graphic::BasicVertexType::Colored2d>                _configProgress;
                Graphic::GL::GeometryBuffer<Graphic::BasicVertexType::Colored2d, false>     _geometryProgress;

                Graphic::Material<Graphic::BasicVertexType::Textured2d>                     *_materialTexture;
                Graphic::MaterialConfig<Graphic::BasicVertexType::Textured2d>               _configTexture;
                Graphic::GL::GeometryBuffer<Graphic::BasicVertexType::Textured2d, false>    _geometryTexture;

                Box2f                   _progressBox;
                unsigned int            _nbEvolution;
                float                   _percent;      // progression in percent
                Color                   _colorLeft;
                Color                   _colorRight;
        };
    }
}

#endif
