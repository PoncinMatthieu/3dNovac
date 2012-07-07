
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        21/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_GL_RASTERMODE_H_
#define NC_GRAPHICS_GL_RASTERMODE_H_

#include "GLObject.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// To manipulate opengl rasterizations mode
            /**
                \todo Manage the glHint (for antialiasing), glFog functions

                <div class="title"> Allow you to manage some opengl state </div>
                    - Abstract the glPolygonMode which controls the interpretation of polygons for rasterization.
                    - Abstract the DepthTest and glDepthMask which control the depth buffer
                    - Abstract the glLineWidth and glPointSize
            */
            class RasterMode
            {
                public:
                    RasterMode();
                    RasterMode(Enum::PolygonFace face, Enum::PolygonMode mode);

                    /** Enable the given polygon rasterization mode */
                    void            Enable();
                    /** Disable the given polygon rasterization mode by activating the default rasterization mode */
                    void            Disable();

                    /** \return return the polygon mode status, if true, the polygon mode will be enable in the mehode `Enable`*/
                    inline bool     PolygonModeState()                                                      {return _setPolygonMode;}
                    /** Inhibit the polygon mode */
                    inline void     InhibitPolygonMode()                                                    {_setPolygonMode = false;}
                    /** Set the polygon resterization mode */
                    inline void     SetPolygonMode(Enum::PolygonFace face, Enum::PolygonMode mode)          {_polygonFace = face; _polygonMode = mode; _setPolygonMode = true;}

                    /** Inhibit the PointSize */
                    inline void     InhibitPointSize()                              {_setPointSize = false;}
                    /** Set the point size and line width used to rasterize the points and lines */
                    inline void     SetPointSize(GLfloat pointSize)                 {_pointSize = pointSize; _setPointSize = true;}

                    /** Inhibit the PointSize */
                    inline void     InhibitLineWidth()                              {_setLineWidth = false;}
                    /** Set the point size and line width used to rasterize the points and lines */
                    inline void     SetLineWidth(GLfloat lineWidth)                 {_lineWidth = lineWidth; _setLineWidth = true;}

                    /** Inhibit the polygon offset */
                    inline void     InhibitPolygonOffset()                          {_setPolygonOffset = false;}
                    /** Set the polygon offset parameters */
                    inline void     SetPolygonOffset(GLfloat factor, GLfloat units) {_polygonOffsetFactor = factor; _polygonOffsetUnits = units; _setPolygonOffset = true;}

                    /** Inhibit the depth test */
                    inline void     InhibitDepthTest()                              {_setDepthTest = false;}
                    /** Set the depth test */
                    inline void     SetDepthTest(bool state)                        {_depthTest = state; _setDepthTest = true;}
                    /** Set the depth test */
                    inline void     SetDepthMask(bool state)                        {_depthMask = state; _setDepthTest = true;}

                private:
                    bool                _setPolygonMode;
                    Enum::PolygonFace   _polygonFace;               ///< Specify the polygons that mode applies
                    Enum::PolygonMode   _polygonMode;               ///< Specify how polygons will be rasterized
                    Enum::PolygonFace   _lastPolygonFace;           ///< Specify the last polygons that mode applies
                    Enum::PolygonMode   _lastPolygonMode;           ///< Specify how last polygons was rasterized

                    bool                _setPolygonOffset;
                    GLfloat             _polygonOffsetFactor;       ///< Set the scale and units used to calculate depth values
                    GLfloat             _polygonOffsetUnits;        ///< Set the scale and units used to calculate depth values
                    GLfloat             _lastPolygonOffsetFactor;   ///< Set the last scale and units used to calculate depth values
                    GLfloat             _lastPolygonOffsetUnits;    ///< Set the last scale and units used to calculate depth values

                    bool                _setPointSize;
                    GLfloat             _pointSize;                 ///< Specify the diameter of rasterized points
                    GLfloat             _lastPointSize;             ///< Specify the last diameter of rasterized points

                    bool                _setLineWidth;
                    GLfloat             _lineWidth;                 ///< Specify the width of rasterized lines
                    GLfloat             _lastLineWidth;             ///< Specify the last width of rasterized lines

                    bool                _setDepthTest;              ///< If true, then the _depthTest property will affect the current openglContext
                    bool                _depthTest;                 ///< If false, disable the depth test
                    bool                _depthMask;                 ///< If false, disable the depth mask
                    bool                _lastDepthTestState;        ///< the last state of the depth test before enable it
                    bool                _lastDepthMaskState;        ///< the last state of the depth mask before enable it
            };
        }
    }
}

#endif
