
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

#ifndef NC_GRAPHICS_CORE_GL_RASTERMODE_H_
#define NC_GRAPHICS_CORE_GL_RASTERMODE_H_

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
                    - Abstract the glPolygonMode witch controls the interpretation of polygons for rasterization.
                    - Abstract the glDepthTest witch control the depth buffer
                    - Abstract the glLineWidth and glPointSize

                <div class="title"> Polygon Mode </div>
                For the glPolygonMode, from the OpenGL documumentation "http://www.opengl.org/sdk/docs/man3/":

                Three modes are defined and can be specified in mode:
                    - GL_POINT
                    - GL_LINE
                    - GL_FILL
            */
            class RasterMode
            {
                public:
                    RasterMode();
                    RasterMode(GLenum face, GLenum mode);

                    /** Enable the given polygon rasterization mode */
                    void            Enable();
                    /** Disable the given polygon rasterization mode by activating the default rasterization mode */
                    void            Disable();

                    /** Inhibit the polygon mode */
                    inline void     InhibitPolygonMode()                            {_setPolygonMode = false;}
                    /** Set the polygon resterization mode */
                    inline void     SetPolygonMode(GLenum face, GLenum mode)        {_face = face; _mode = mode; _setPolygonMode = true;}

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

                private:
                    bool            _setPolygonMode;
                    /** Must be GL_FRONT_AND_BACK for front- and back-facing polygons. */
                    GLenum          _face;                  ///< SpecifY the polygons that mode applies
                    /** Accepted values are GL_POINT, GL_LINE, and GL_FILL. The initial value is GL_FILL for both front- and back-facing polygons. */
                    GLenum          _mode;                  ///< SpecifY how polygons will be rasterized

                    bool            _setPointSize;
                    GLfloat         _pointSize;             ///< Specify the diameter of rasterized points

                    bool            _setLineWidth;
                    GLfloat         _lineWidth;             ///< Specify the width of rasterized lines

                    bool            _setPolygonOffset;
                    GLfloat         _polygonOffsetFactor;   ///< Set the scale and units used to calculate depth values
                    GLfloat         _polygonOffsetUnits;    ///< Set the scale and units used to calculate depth values

                    bool            _setDepthTest;          ///< If true, then the _depthTest property will affect the current openglContext
                    bool            _depthTest;             ///< If false, disable the depth test
                    bool            _lastDepthTestState;    ///< the last state of the depth test before enable it

                    static GLenum   _currentFace;
                    static GLenum   _currentMode;
                    static GLfloat  _currentPointSize;
                    static GLfloat  _currentLineWidth;
                    static GLfloat  _currentPolygonOffsetFactor;
                    static GLfloat  _currentPolygonOffsetUnits;
                    static bool     _currentDepthTest;
            };
        }
    }
}

#endif
