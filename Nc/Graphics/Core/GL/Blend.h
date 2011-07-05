
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

    File Created At:        27/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_GL_BLEND_H_
#define NC_GRAPHICS_CORE_GL_BLEND_H_

#include "GLObject.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Help to manipulate the blending opengl method
            /**
                \todo manage the transparency color and rate
            */
            class LGRAPHICS  Blend
            {
                public:
                    /// The pattern of the using blend function
                    enum    Pattern
                    {
                        Disabled = 0,
                        Alpha,                  ///< Color Pixel = Src * a + Dest * (1 - a)
                        Add,                    ///< Color Pixel = Src + Dest
                        Multiply                ///< Color Pixel = Src * Dest.
                    };

                public:
                    Blend();
                    Blend(Pattern p);
                    ~Blend()    {}

                    /** Set the blending pattern */
                    void            SetPattern(Pattern p);
                    inline void     InhibitBlending()                                           {_blendState = false;}
                    /** Set the blend factors and active the blending */
                    inline void     SetFactors(Enum::BlendFactor s, Enum::BlendFactor d)        {_sFactor = s; _dFactor = d; _blendState = true;}

                    /** Enable the blending opengl state */
                    void            Enable();
                    /** Disable the blending opengl state */
                    void            Disable();

                protected:
                    bool                        _blendState;            ///< if true, enable the blending
                    Enum::BlendFactor           _sFactor;               ///< sFactor used by the glBlendFunc
                    Enum::BlendFactor           _dFactor;               ///< dFactor used by the glBlendFunc
                    bool                        _lastBlendState;        ///< Last blend state
                    Enum::BlendFactor           _lastSFactor;           ///< Last sFactor used by the glBlendFunc
                    Enum::BlendFactor           _lastDFactor;           ///< Last dFactor used by the glBlendFunc
            };
        }
    }
}

#endif
