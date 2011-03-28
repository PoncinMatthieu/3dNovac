
/*--------------------------------------------------------------------------------

	3dNovac Engine
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

    cree le :        13/12/2010

--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------

    Implemente the gestion of a window for windows plateform

--------------------------------------------------------------------------------*/

#ifndef NC_SYSTEM_WWINDOW_H_
#define NC_SYSTEM_WWINDOW_H_

#include "Window.h"
#include "WCursor.h"

namespace Nc
{
    namespace System
    {
        class LSYSTEM WWindow : public Window
        {
            public:
                WWindow();
                virtual ~WWindow();

                virtual void				Create(const std::string &title, const Vector2i &size, unsigned long pattern, const Utils::FileName &icon, unsigned int antialiasingLevel);
                virtual void				UseExistingWindow(void *disp, int winId, const Vector2i &size, unsigned int antialiasingLevel);
                virtual GLRenderer			*CreateRenderer();

                virtual void				Close();

				virtual bool				SetIcon(const Utils::FileName &f);
                virtual void				Resize(unsigned int width, unsigned int height);
				virtual System::ICursor		*NewCursor()										{return new System::Cursor(this);}

            protected:
				void						SwitchToFullscreen(const Vector2i &size);

                static const char *_classNameA;
                HWND          _handle;
                DWORD         _win32Style;
                HICON         _icon;

                friend class WGLRenderer;
                friend class WWindowInput;
				friend class Cursor;
        };
    }
}

#endif
