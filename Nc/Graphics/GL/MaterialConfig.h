
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

    File Created At:        26/02/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_GL_MATERIALCONFIG_H_
#define NC_GRAPHICS_GL_MATERIALCONFIG_H_

#include "../Define.h"
#include "Blend.h"
#include "RasterMode.h"
#include "Texture.h"
#include "DefaultVertexType.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Define a material config.
            /**
                The MaterialConfig allow to store data like Texture and RasterMode which is material dependant.
            */
            class LIB_NC_GRAPHICS MaterialConfig
            {
                public:
                    MaterialConfig()                                {}
                    MaterialConfig(const Blend::Pattern &p)
                        : _blend(p)                                 {}
                    virtual ~MaterialConfig()                       {}

                    virtual MaterialConfig  *Clone() const                          {return new MaterialConfig(*this);}

                    /** \return the blend mode. */
                    Blend                   &GetBlend()                             {return _blend;}

                    /** \return the raster mode. */
                    RasterMode              &GetRasterMode()                        {return _rasterMode;}

                    /** Enable the default basic config (blend and depthTest). */
                    virtual void Enable()
                    {
                        _blend.Enable();
                        _rasterMode.Enable();
                    }

                    /** Disable the default basic config (blend and depthTest). */
                    virtual void Disable()
                    {
                        _blend.Disable();
                        _rasterMode.Disable();
                    }

                    Array<Texture,0>    Textures;       ///< texture array used to setup textures needed by the rendering material.

                protected:
                    Blend               _blend;         ///< the blend method of the material.
                    RasterMode          _rasterMode;    ///< the raster mode of the material.
            };
        }
    }
}

#endif
