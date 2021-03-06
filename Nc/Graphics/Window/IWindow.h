
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

    File Created At:        02/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_WINDOW_WINDOW_H_
#define NC_GRAPHICS_WINDOW_WINDOW_H_

#include <iostream>
#include <Nc/Core/Math/Math.h>
#include <Nc/Core/Utils/Mask.h>
#include "../Define.h"

namespace Nc
{
    namespace Graphic
    {
        class GLContext;
        class WindowInput;
        class ICursor;
        class SubWindow;
        class SceneGraphManager;

        /// Interface to create and manage a window for multiarchi.
        /**
            The IWindow taked all the parameters needed to create a window in it's constructor, however, the window will be created only at the call of the method 'Create'.
            The IWindow takes also a SceneGraphManager in it's constructor, however if you gives a null pointer, the window will create a new instance of SceneGraphManager.
            This allow you to define your own scene manager. But be carefull, The scene manager will be deleted in the destructor of the window.
		*/
        class LIB_NC_GRAPHICS IWindow : public Utils::NonCopyable
        {
            public:
                enum Style
                {
                    Resizeable  =   1 << 0,         ///< set the window resizable.
                    Fullscreen  =   1 << 1,         ///< fullscreen mode.
                    Titlebar    =   1 << 2,         ///< show the Titlebar.
                    Closeable   =   1 << 3,         ///< set the window closable.
					Hidden		=	1 << 4			///< the window will be hidden. (can be usefull if you want to resize it after it's creation)
				};

                typedef std::list<SubWindow*>       ListSubWindow;

            public:
				IWindow(SceneGraphManager *sceneGraphManager = NULL);
				IWindow(const std::string &title, const Math::Vector2ui &size, const Utils::Mask<Style> &style, const Utils::FileName &icon, unsigned int antialiasingLevel, SceneGraphManager *sceneGraphManager = NULL);
                virtual ~IWindow();

                /** \return true if the IWindow is create. */
                virtual bool        IsCreate() const  {return _isCreate;}

				/** Create the window by using the predefined parameters */
				void				Create();
                /** Interface to create the window with the suitable attributes. */
                virtual void        Create(const std::string &title, const Math::Vector2ui &size, const Utils::Mask<Style> &style, const Utils::FileName &icon, unsigned int antialiasingLevel) = 0;

                /** Interface to create a Window instance using an existing window (for Qt by exemple). */
                virtual void        UseExistingWindow(void *disp, int winId, const Math::Vector2ui &size, unsigned int antialiasingLevel) = 0;

                /** Create and return a new GLContext associated to the window. */
                virtual GLContext  *CreateGLContext() = 0;

                /** Create and return a GLContext shared with the rendering context (Call the function CreateNewSharedContext of GLContext). */
                virtual GLContext   *CreateSharedContext();

                /** Close the window. */
                virtual void        Close() = 0;

                /** Initialize the scene graph manager. */
                virtual void        InitSceneGraphManager();

                /** Set the enable statement for rendering. If true, the window can be rendered. */
                void				Enable(bool state)					{_isEnable = state;}
                /** \return true if the window can be rendered. */
                inline bool			Enabled() const						{return _isEnable;}

				/** Hide the window. */
				virtual void		Hide() = 0;
				/** Show the window. */
				virtual void		Show() = 0;

                /** Render the scenes by using the given GLContext through the scene graph manager. */
                virtual void        Render(GLContext *context);

                // accessor
                /** Set an Icon to the window with the given image filename. */
                virtual bool                    SetIcon(const Utils::FileName &f) = 0;

                /** Resize the window using the given size. */
                virtual void                    Resize(unsigned int width, unsigned int height) = 0;
                /** Notify a resize of the window. */
                virtual void                    Resized()               {}

                /** \return the width of the window. */
                inline unsigned int		        Width() const           {return _width;}

                /** \return the height of the window. */
                inline unsigned int		        Height() const          {return _height;}

                // /** Set the window size. */
                //inline void                     ResetSize(unsigned int width, unsigned int height)      {_width = width; _height = height;}

                /** \return the OpenGL context associated. */
                inline GLContext                *Context()              {return _context;}

                /** \return the window input associated. */
                inline WindowInput              *Input()                {return _input;}

                /** \return false if the window use an existing window (Created with the method UseExistingWindow). */
                inline bool                     IsOwn()                 {return _own;}

                /** \return the antialiasing level available, this could be smaller than the level you put in the Create method. */
                inline unsigned int             AntialiasingLevel()     {return _antialiasingLevel;}

                /** \return the number of depth bits. */
                inline unsigned int             Depth()					{return _depth;}

                /** \return the number of stencil bits.  */
                inline unsigned int             Stencil()				{return _stencil;}

                /** \return the number of bits per pixel (color buffer). */
                inline unsigned int             BitsPerPixel()			{return _bitsPerPixel;}

                /** \return the default cursor. */
                inline ICursor                  *DefaultCursor()        {return _defaultCursor;}

                /** \return the activated cursor. */
                inline ICursor                  *CurrentCursor()        {return _currentCursor;}

                /** Create and return a New Cursor. */
                virtual ICursor                 *NewCursor() = 0;

                /** \return the SceneGraphManager used to render the scenes. */
                SceneGraphManager               *SceneManager()         {return _sceneGraphManager;}

            protected:
                /** Called by a SubWindow to add it's instance to the list of SubWindow. */
                void                    AddSubWindow(SubWindow *w);
                /** Called by a SubWindow to remove it's instance from the list of SubWindow. */
                void                    RemoveSubWindow(SubWindow *w);

            protected:
                SceneGraphManager       *_sceneGraphManager;    ///< The Scene graph manager that is used to render the scenes.

                bool                    _isCreate;          ///< true if the window has been created.
                bool					_isEnable;			///< statement enabling the rendering of the window.
				std::string				_title;				///< the title of the window.
				Utils::FileName			_icon;				///< the path to the icon of the window.
				unsigned int            _width;             ///< the width of the window.
                unsigned int            _height;            ///< the height of the window.
				Utils::Mask<Style>		_style;				///< the style of the window.
                GLContext               *_context;          ///< the associated OpenGL context.
                WindowInput             *_input;            ///< the window input associated.
                bool                    _own;               ///< false if the window use an existing window (Created with the method UseExistingWindow).
                unsigned int            _antialiasingLevel; ///< the antialising level of the window.
                unsigned int			_depth;				///< number of bits of the depth buffer.
                unsigned int			_stencil;			///< number of bits of the pixel buffer.
                unsigned int			_bitsPerPixel;		///< number of bits of the color buffer.
                ICursor                 *_defaultCursor;    ///< instance of the default cursor of the window.
                ICursor                 *_currentCursor;    ///< instance of the current cursor activated.

            private:
                ListSubWindow           _listSubWindow;     ///< list of subwindow, which will be used for exemple to forward inputs to a subwindow.
                System::Mutex           _mutexListSubWindow;///< mutex protecting the access of the list of subwindow.

                friend class WindowInput;
                friend class SubWindow;
                friend class ICursor;
        };

        typedef Utils::Mask<IWindow::Style>      WindowStyle;
    }
}

#endif
