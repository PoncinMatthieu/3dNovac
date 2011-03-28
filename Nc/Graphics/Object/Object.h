
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

                    Implementation de la classe "Object"
                Classe de base pour definir un objet graphique

-----------------------------------------------------------------------------*/


#ifndef NOVAC_GRAPHIC_OBJECT_H_
#define NOVAC_GRAPHIC_OBJECT_H_

#include "../Define.h"
#include "../Material/Material.h"

namespace Nc
{
    namespace Graphic
    {
        class LGRAPHICS Object : public System::Object
        {
            public:
                Object();
                Object(const TMatrix &m);
                virtual ~Object();

                /**
                    To update an object with the running time (in second)
                    To redefine in your own object
                */
                virtual inline void     Update(float)           {}

                /**
                    To Render the object
                    To redefine in your own object
                */
                virtual void            Render(ISceneGraph *scene) = 0;

                // set the display states
                inline void             DisplayState(bool state)            {_displayState = state;}
                virtual inline bool     DisplayState() const                {return _displayState;}
				inline void             DisplayStateBox(bool state)         {_displayStateBox = state;}
                virtual inline bool     DisplayStateBox() const             {return _displayStateBox;}

                /** The matrix transformation of the object (basic transformation: rotate, translate, scale) */
                TMatrix                 Matrix;                             // for performance, the matrix object is public

            protected:
                bool            _displayState;                              ///< if true, draw the object
                bool            _displayStateBox;                           ///< if true, draw the box of the object
        };
    }
}

#endif // Object_H_INCLUDED
