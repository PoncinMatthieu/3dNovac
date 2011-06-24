
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

    File Created At:        04/05/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WINDOWBOX_H_
#define NC_GUI_WINDOWBOX_H_

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// To manage a window
        class LGUI  WindowBox : public Widget
        {
            public:
                WindowBox(const std::string &title, const Vector2f &pos, const Vector2f &size, Corner x = Top, Corner y = Left, Widget *parent = NULL);
                WindowBox(const char *className, const std::string &title, const Vector2f &pos, const Vector2f &size, Corner x = Top, Corner y = Left, Widget *parent = NULL);
                WindowBox(const WindowBox &w);
                WindowBox &operator = (const WindowBox &w);
                virtual ~WindowBox();

                static const char   *ClassName()                {return "WindowBox";}
                virtual ISceneNode  *Clone() const              {return new WindowBox(*this);}
                virtual void        ToString(std::ostream &os) const;

                /** Change the title statement */
                inline void     DrawTitleState(bool state)                      {_drawTitle = state; _stateChanged = true;}
                /** Change the title of the window */
                void            ChangeTitle(const std::string &title, const std::string &ttf);
                /** Set the title height */
                inline int      TitleHeight() const                             {return _titleHeight;}

                /** Set tje colors of the window */
                inline void     SetColor(const Color &cInside, const Color &cTitle1, const Color &cTitle2, const Color &cBorder)
                                                                            {_color = cInside; _titleColor1 = cTitle1; _titleColor2 = cTitle2; _edgeColor = cBorder; _stateChanged = false;}
                /** Set the filling color */
                inline void     FillColor(const Color &color)               {_color = color; _stateChanged = false;}

                /** Return the reel size */
                virtual void    GetReelSize(Vector2f &size) const;

            protected:
                WindowBox(const std::string &title, const std::string &ttf = "arial");
                WindowBox(const char *className, const std::string &title, const std::string &ttf = "arial");

                /** Update the geometry of the window */
                virtual void    Update();
                /** Render the window */
                virtual void    Draw(Graphic::SceneGraph *scene);
                /** Translate the childs of the window */
                virtual void    TranslateChild(const Corner corner[2], Vector2f &v) const;

            private:
                /** Initialize the widget */
                void            Init(const std::string &title, const std::string &ttf);

                /** Copy the widget properties */
                void            Copy(const WindowBox &w);

            protected:
                Color               _color;             ///< the filling color
                //bool                _dragable;

                Graphic::String     *_title;            ///< the title of the window
                Color               _titleColor1;       ///< right color of the window
                Color               _titleColor2;       ///< left color of the window
                int                 _titleHeight;       ///< height of the title
                bool                _drawTitle;         ///< Mark if we need to draw the title

            private:
                unsigned int        _indexDrawable;
        };
    }
}

#endif

