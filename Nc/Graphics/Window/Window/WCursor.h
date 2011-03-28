
/**-------------------------------------------------------------------------------

	3dNovac System
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

    cree le :        07/02/2011

--------------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------------

    Provide an implementation of cursor for a windows system
	TODO: Actualy just provide the default cursor (loading not implemented)

--------------------------------------------------------------------------------*/

#ifndef NC_SYSTEM_XCURSOR_H_
#define NC_SYSTEM_XCURSOR_H_

#include "../Define.h"
#include "ICursor.h"

namespace Nc
{
    namespace System
    {
        class WWindow;

        class LSYSTEM Cursor : public ICursor
        {
            public:
                Cursor(WWindow *w);
                virtual ~Cursor();

                virtual void Enable();
                virtual void Disable();

                /// Load Data not implemented
                virtual void LoadFromData(const unsigned char *data, const unsigned char *mask, const Vector2ui &size, const Vector2ui &posCenter);
                virtual void LoadFromXpm(const char *xpm[]);

            private:
                void    CreateHiddenCursor();

                WWindow                 *_win;
                HCURSOR					_cursor;
        };
    }
}

#endif
