
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

    File Created At:        02/07/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_ITEXTFORMATER_H_
#define NC_GRAPHICS_CORE_ITEXTFORMATER_H_

#include "../Define.h"

namespace Nc
{
    namespace Graphic
    {
        namespace Core
        {
            /// Interface used to format text.
            /**
                <div class="title">Definning you own formater:</div>
                To create your own text formater, you should redefine the following methods:
                    - Clone: To be able to create a new copy of your formater.
                    - InitDrawable: To create and initialize the drawables used to render the text.
                    - ComputeDrawables: To update the drawables used to render the text.

                To be able to render structured rich text documents, you can define your own text formater by inheriting from ITextFormater.
                We use the boolean _needUpdate to know if we should recompute the drawables with the size,
                this boolean is obviously important for perfomance issue, and should be updated in your own formater.

                \sa
                    - PlainTextFormater, Correspond to the default text formater allowing the creation of stylised and aligned text.
            */
            class ITextFormater
            {
                public:
                    ITextFormater() : _needUpdate(true) {}
                    virtual ~ITextFormater() {}

                    /** \return a copy of the ITextFormater. */
                    virtual ITextFormater   *Clone() const = 0;

                    /** Notify to the text formater that the rendered text has changed. */
                    inline void     TextChanged()           {_needUpdate = true;}
                    /** \return true if the drawables must be recompute. */
                    inline bool     NeedUpdate()            {return _needUpdate;}

                    /** Init drawables array, to avoid to recreate the drawables after each modification of the text. */
                    virtual void    InitDrawables(DrawableArray &drawableArray) = 0;

                    /**
                        Create every Drawables for the given \p text and add them to the given \p drawableArray.
                        \return the size of the text.
                    */
                    virtual void    ComputeDrawables(Vector2f &textSize, DrawableArray &drawableArray, const Utils::Unicode::UTF32 &text) = 0;

                protected:
                    bool        _needUpdate;      ///< true if the drawables must be recompute since the last computation.
            };
        }
    }
}

#endif
