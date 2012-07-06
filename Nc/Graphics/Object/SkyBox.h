
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

    File Created At:        18/05/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NOVAC_GRAPHIC_SKYBOX_H_
#define NOVAC_GRAPHIC_SKYBOX_H_

#include "Object.h"

namespace Nc
{
    namespace Graphic
    {
        class Camera3d;

        /// To render a skybox in background.
        /**
            The skybox needs to have a pointer to the Camera3d and a tab of filename like that:
 \code
    Utils::FileName filenamesSky1[6] =
    {
        ("Nc:Image:sky/space_lf.png"),
        ("Nc:Image:sky/space_rt.png"),
        ("Nc:Image:sky/space_ft.png"),
        ("Nc:Image:sky/space_bk.png"),
        ("Nc:Image:sky/space_dn.png"),
        ("Nc:Image:sky/space_up.png")
    };
 \endcode
        */
        class LGRAPHICS SkyBox : public Object
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Graphic::Object, System::Object, Nc::Graphic::SkyBox);

            public:
                SkyBox(const Utils::FileName filenames[]);
                virtual ~SkyBox();

                virtual ISceneNode      *Clone() const          {return new SkyBox(*this);}

            protected:
                /** Transform the model matrix to be a the eye of the camera. */
                virtual void    TransformModelMatrixToRender(SceneGraph *scene);

                /** Update the geometry of the drawable. */
                void            UpdateGeometry(const Utils::FileName filenames[]);
        };
    }
}

#endif
