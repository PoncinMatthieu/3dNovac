
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

    File Created At:        29/07/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_EFFECT_RASTEREFFECT_H_
#define NC_GRAPHICS_EFFECT_RASTEREFFECT_H_

#include "../Core/GL/RasterMode.h"
#include "Effect.h"

namespace Nc
{
    namespace Graphic
    {
        /// Manage the rasterisation parameters to draw simple effects like a wireframe effect
        class LGRAPHICS RasterEffect : public Effect
        {
            public:
                NC_UTILS_DEFINE_PARENT_CLASS(Effect);
                NC_UTILS_DEFINE_VISITABLE(System::Object);

            public:
                enum Pattern
                {
                    Points,                 ///< draw only the points of primitives
                    Wireframe,              ///< draw only the edges of primitives
                    Fill,                   ///< default draw
                    FillWireframe,          ///< draw a first pass in Fill mode and a second pass in Wireframe mode with an offset
                    FillWireframeNoDepth    ///< draw a first pass in Fill mode and a second pass in Wireframe mode with an offset and no depth test
                };

            public:
                RasterEffect(Pattern pattern = Fill);

                virtual ISceneNode  *Clone() const                                      {return new RasterEffect(*this);}
                static const char   *ClassName()                                        {return "RasterEffect";}

                /** Set the depth test statement */
                inline void         SetDepthTest(bool state)                            {_mode.SetDepthTest(state);}

                /** Set the effect mode */
                void                SetPattern(Pattern pattern);

                /** Render the childs with the good effect */
                virtual void        Render(SceneGraph *scene);
                /** Render the given \p node with the given \p scene */
                void                Render(Entity *node, SceneGraph *scene);

            private:
                System::Mutex       _mutex;
                GL::RasterMode      _mode;      ///< RasterMode used to render the effect
                GL::RasterMode      _mode2;     ///< a second RasterMode to draw the second pass with an offset
        };
    }
}

#endif
