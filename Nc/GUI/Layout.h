
/*-----------------------------------------------------------------------------

	3dNovac GUI
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

    File Created At:        30/11/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_LAYOUT_H_
#define NC_GUI_LAYOUT_H_

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        ///
        /**
            \todo Implemente the grid layout, with the possibility to set up a given widget to a given case
        */
        class LGUI Layout : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::Layout);

                enum Pattern
                {
                    Horizontal,
                    Vertical
                    //Grid
                };

                typedef std::map<Widget*, float>        MapRatio;

            public:
                Layout(Pattern p, Corner x = Top, Corner y = Left);

                virtual ISceneNode  *Clone() const                              {return new Layout(*this);}

                /** \return the current pattern of the layout */
                inline Pattern      GetPattern()                                {return _pattern;}
                /** Set the pattern of the layout */
                inline void         SetPattern(Pattern p)                       {_pattern = p;}

                /** Set the given ratio for the given child */
                void                SetRatio(Widget *child, float ratio);

            protected:
                /** \return the translation information to position childs */
                virtual void        PosChild(const Widget *child, Vector2i &v) const;
                /** \return the size that the child should use to set up it's percent size */
                virtual void        SizeChild(const Widget *, Vector2i &size) const;

            private:
                Pattern         _pattern;
                MapRatio        _mapRatio;
        };
    }
}

#endif
