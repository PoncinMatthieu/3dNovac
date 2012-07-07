
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

    File Created At:        22/01/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_MATERIAL_IMATERIAL_H_
#define NC_GRAPHIC_MATERIAL_IMATERIAL_H_

#include "../Scene/SceneGraph.h"
#include "../GL/VertexDescriptor.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface to define a Material which is mandated to render drawables.
        /**
            \todo render the drawables in one time not a call of the material for each drawables of the objects, this would improve the number of matrix compute and Ogl call.
        */
        class LGRAPHICS IMaterial
        {
            public:
                IMaterial(const std::string &name)
                    : _name(name)                   {}
                virtual ~IMaterial()                {}

                /** \return the name of the material. */
                inline const std::string   &Name()  {return _name;}

                /** \return true if the material is compatible with the given vertex descriptor. */
                virtual bool    IsCompatible(const GL::VertexDescriptor &)          {return true;}

                /** Configure the given goemetry. */
                virtual bool    Configure(GL::Drawable &drawable) = 0;

                /** Render the given drawable with the model matrix scene. */
                void            Render(SceneGraph *scene, GL::Drawable &drawable)
                {
                    Render(scene, scene->ModelMatrix(), drawable);
                }

                /** Render the given drawable with the given model matrix. */
                virtual void    Render(SceneGraph *scene, const TMatrix &modelMatrix, GL::Drawable &drawable) = 0;

                friend LGRAPHICS std::ostream &operator << (std::ostream &oss, const IMaterial &m)
                {
                    oss << m._name;
                    return oss;
                }

            private:
                std::string     _name;      ///< the name of the material.
        };
    }
}

#endif
