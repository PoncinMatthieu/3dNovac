
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

    File Created At:        04/07/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_GL_STATE_H_
#define NC_GRAPHICS_CORE_GL_STATE_H_

#include <Nc/Core/Utils/Mask.h>
#include "../GL.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Define a Render state associated to an OpenGL context
            /**
                The render state centralize all the statements and modifiers of the opengl machine state.
                This render state is essantially used to centralize the ogl functions but also to avoid any redundant state changes in the ogl state machine.
                See the opengl documentation for more detail on the opengl functions.

                \warning All opengl functions witch modify the opengl machine state like glEnable() glDisable() should not be direcly call but call with a method of the render state.
            */
            class State : Utils::NonCopyable
            {
                public:
                    typedef Utils::Mask<Enum::BufferBitType, GLbitfield>    BufferBitMask;

                private:
                    typedef std::map<Enum::Capability, bool>                MapCapabilityStatement;

                public:
                    State();
                    ~State();

                    /**
                        \return the current render state
                        Throw an exception if the render state has not been set.
                    */
                    static inline State &Current()                              {if (_current == NULL) throw Utils::Exception("GL::State", "The current render state is not set."); return *_current;}

                    /** print the states */
                    friend std::ostream &operator << (std::ostream &oss, State &s);

                    /**
                        Enable the render state (will lock a mutex and affect the current render state)
                        So the rendering threads will be synchronized.
                     */
                    void                Enable();
                    /** Disable the render state (unset the current render state and unlock the mutex) */
                    void                Disable();

                    /** Initialize the render state with the given associated \p context */
                    void                InitContext(GLContext *context);

                    /** \brief Set the viewport
                        Ogl function:
                            -glViewport()
                    */
                    void                Viewport(int viewportX, int viewportY, int viewportWidth, int viewportHeight);

                    /** \brief Set the clear color state
                        Ogl function:
                            -glClearColor()
                    */
                    void                ClearColor(Color c);
                    /** \brief Clear the buffers
                        Ogl function:
                            -glClear()
                    */
                    inline void         Clear(const BufferBitMask &mask) const                      {glClear(mask.GetMask());}

                    /** \return true if the given \p capability is enabled */
                    inline bool         Enabled(Enum::Capability cp)                                {return _mapCurrentCapabilityStatement[cp];}
                    /** \brief Enable the given \p capability
                        Ogl function:
                            -glEnable()
                    */
                    void                Enable(Enum::Capability cp);
                    /** \brief Disable the given \p capability
                        Ogl function:
                            -glDisable()
                    */
                    void                Disable(Enum::Capability cp);

                    /** \brief Enable or disable the depth mask statement
                        Ogl function:
                            -glDepthMask()
                    */
                    void                DepthMask(bool state);
                    /** \brief Set the depth func
                        Ogl function:
                            -glDepthFunc()
                    */
                    void                DepthFunc(Enum::DepthFunc f);
                    /** \return the current depth mask statement */
                    inline bool         CurrentDepthMask() const                                    {return _currentDepthMask;}

                    /** \brief Set the blend func factors
                        Ogl function:
                            -glBlendFunc()
                    */
                    void                        BlendFunc(Enum::BlendFactor s, Enum::BlendFactor d);
                    /** \return the current source blending factor */
                    inline Enum::BlendFactor    CurrentBlendSFactor() const                         {return _currentBlendSFactor;}
                    /** \return the current destination blending factor */
                    inline Enum::BlendFactor    CurrentBlendDFactor() const                         {return _currentBlendDFactor;}

                    /** \brief Set the Rasterization Polygon mode
                        Ogl function:
                            -glPolygonMode()
                    */
                    void                PolygonMode(Enum::PolygonFace f, Enum::PolygonMode m);
                    /** \brief Set the Rasterization Polygon offset
                        Ogl function:
                            -glPolygonOffset()
                    */
                    void                PolygonOffset(float offsetFactor, float offsetUnits);
                    /** \return the current polygon face */
                    Enum::PolygonFace   CurrentPolygonFace() const                                  {return _currentPolygonFace;}
                    /** \return the current polygon mode */
                    Enum::PolygonMode   CurrentPolygonMode() const                                  {return _currentPolygonMode;}
                    /** \return the current polygon offset factor */
                    inline float        CurrentPolygonOffsetFactor() const                          {return _currentPolygonOffsetFactor;}
                    /** \return the current polygon offset unit */
                    inline float        CurrentPolygonOffsetUnits() const                           {return _currentPolygonOffsetUnits;}

                    /** \brief Set the rasterization point size
                        Ogl function:
                            -glPointSize()
                    */
                    void                PointSize(float s);
                    /** \brief Set the rasterization line width
                        Ogl function:
                            -glLineWidth()
                    */
                    void                LineWidth(float w);
                    /** \return the current point size */
                    inline float        CurrentPointSize() const                                    {return _currentPointSize;}
                    /** \return the current line width */
                    inline float        CurrentLineWidth() const                                    {return _currentLineWidth;}

                private:
                    /** Check the opengl version of the current context */
                    void            CheckGLVersion();

                private:
                    GLContext                   *_context;                      ///< the context connected to the render state

                    Color                       _currentClearColor;             ///< the current clear color used to clear the buffers
                    MapCapabilityStatement      _mapCurrentCapabilityStatement; ///< the list of all capability statement

                    // viewport
                    int                         _currentViewportX;              ///< the current lower x viewport
                    int                         _currentViewportY;              ///< the current lower y viewport
                    int                         _currentViewportWidth;          ///< the current upper x viewport
                    int                         _currentViewportHeight;         ///< the current upper y viewport

                    // depth test
                    bool                        _currentDepthMask;              ///< the current write depth mask statement
                    Enum::DepthFunc             _currentDepthFunc;              ///< the current depth func used by the depth test

                    // blend
                    Enum::BlendFactor           _currentBlendSFactor;           ///< the blend source factor used by the blending function
                    Enum::BlendFactor           _currentBlendDFactor;           ///< the blend destination factor used by the blending function

                    // Polygon mode
                    Enum::PolygonFace           _currentPolygonFace;            ///< the current face used to determine the rasterization mode
                    Enum::PolygonMode           _currentPolygonMode;            ///< the current raterization mode applied to the good face
                    float                       _currentPolygonOffsetFactor;    ///< the current offset factor used to make some polygoon offsets
                    float                       _currentPolygonOffsetUnits;     ///< the current unit factor used to make some polygon offsets
                    float                       _currentPointSize;              ///< the current rasterization size of the points
                    float                       _currentLineWidth;              ///< the current rasterization width of the lines

                    static State                *_current;                      ///< Pointer to the current activated render state
                    static System::Mutex        _mutex;                         ///< The mutex used to protect the render state
                    static unsigned int         _instanceCounter;               ///< An instance used to know how many render state exist
            };
        }
    }
}

#endif
