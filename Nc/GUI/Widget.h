
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

    File Created At:        02/06/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WIDGET_H_
#define NC_GUI_WIDGET_H_

#include <Nc/Core/Engine/Handler.h>
#include "Define.h"

namespace Nc
{
    namespace GUI
    {
        /// enum to define a relative position of a Widget
        enum Corner { Top, Bottom, Left, Right, Center };

        /// Base class to define a widget
        /**
            A widget can receive window events and interact concequently.   <br/>
            A widget has a list of child and a widget parent. Childs are displaying in the relative position of their father.
        */
        class LGUI  Widget : public Graphic::Object2d, public Engine::Handler
        {
            public:
                Widget(const Vector2i &pos = Vector2i(0, 0), const Vector2i &size = Vector2i(10, 10), Corner x = Top, Corner y = Left, Widget *parent = NULL);
                virtual ~Widget();

                Widget(const Widget &w);
                Widget &operator = (const Widget &w);
                virtual void    Copy(const Widget &w); // recopie tous les attribus du widget

                /** Copy the widget */
                virtual Widget* Clone() const                                   {return new Widget(*this);}

                // update et affichage du widget
                /** Manage the window event of the widget */
                void ManageWindowEvent(const Nc::System::Event &event);
                /** Render the widget */
                virtual void Render(Graphic::ISceneGraph *scene);

                // accessors
                /** Return the parent of the widget */
                inline Widget           *Parent() const                         {return _parent;}
                /** Return the list of childs */
                const std::list<Widget*> &ListChild() const                     {return _listChild;}
                /** Add a Child to the widget */
                void                    AddChild(Widget *child);
                /** Delete the childs of the widget */
                void                    DeleteChilds();

                /**
                    return the enable statement (if false draw it but for expemple fill in grey a disabled button). <br/>
                    Recursif, return false if a parent is set to false
                */
                virtual bool            Enable() const                          {bool e = _enable; if (e && _parent != NULL) e = (e && _parent->Enable());  return e;}
                /** Set the enable statement */
                virtual void            Enable(bool state)                      {_enable = state; _stateChange = false;}
                /** Set the display statement */
                virtual void            DisplayState(bool state)                {_displayState = state; _stateChange = true;}
                /** Return the display statement */
                virtual bool            DisplayState()                          {return _displayState;}

                /** Resize and reposition the widget with their parent size and position */
                void Resized();
                /** Set the percent size. If the percent value is different of null, the widget will be resized with a proportion of his parent */
                void                    Percent(Vector2f percent);

                /** Return the position of the widget */
                inline const Vector2i   &Pos() const                            {return _pos;}
                /** Set the position */
                inline void             Pos(const Vector2f &pos)                {_pos = pos; _stateChange = true;}
                /** Return the reel position of the widget (including the relative position Corner) */
                virtual Vector2f        GetReelPos() const;
                /** Return the reel recursive position of the widget (including the relative position Corner and the parents) */
                Vector2f                GetReelPosRecursif() const;

                /** Set the size of the widget */
                inline virtual void     Size(const Vector2f &size)              {_size = size; _stateChange = true;}
                /** Return the size of the widget */
                inline const Vector2i   &Size() const                           {return _size;}
                /** Return the reel size of the widget (including some specifique modification) */
                virtual Vector2f        GetReelSize() const;

                /** Set the focus statement */
                void                    Focus(bool state);
                /** Return the focus statement */
                bool                    Focus() const                           {return _focus;}
                /** Set the generateHandleAtEnterFocus statement. If it's true, the widget will generate an event when the widget will entered in focus */
                inline void             GenerateHandleAtEnterFocus(bool state)  {_generateHandleAtEnterFocus = state; _stateChange = true;}

                /** Set the corner relative position */
                inline void             SetCorner(Corner x, Corner y)           {_corner[0] = x; _corner[1] = y; _stateChange = true;}
                /** Return the corner relative position */
                inline Corner           GetCorner(unsigned int i)               {if (i < 2) return _corner[i]; throw Utils::Exception("Widget", "Overflow in function GetCorner");}
                /** Set the margin inside of the widget for childs */
                inline void             Margin(const Vector2i &margin)          {_margin = margin; _stateChange = true;}
                /** Set the X margin value */
                inline void             MarginX(int x)                          {_margin.Data[0] = x; _stateChange = true;}
                /** Set the Y margin value */
                inline void             MarginY(int y)                          {_margin.Data[1] = y; _stateChange = true;}

                /** Set the draw edge statement */
                inline void             DrawEdge(bool state)                    {_drawEdge = state; _stateChange = true;}
                /** Set the edge color */
                inline void             EdgeColor(const Color &color)           {_edgeColor = color; _stateChange = true;}

            protected:
                /** Draw the widget */
                virtual void Draw(Graphic::ISceneGraph *scene);
                /** Update the widget */
                virtual void Update();

                /**
                    Return a vector to translate the childs when we call the GetReelPos method. <br/>
                    Could be redefinen to manage the specific placement of childs in a widget (like in a WindowBox)
                 */
                virtual Vector2f            TranslateChild(const Corner[2]) const {return Vector2f();}

                /**
                    Fill a data, used to get data from specific widget and return these data in an event
                    (like the button witch send an event with the data of the childs of the same parent)
                */
                virtual void                GetData(std::string &data)          {data = "";}
                /**
                    Fill a list of data, used to get data from specific widget and return these data in an event
                    (like the button witch send an event with the data of the childs of the same parent)
                */
                void                        GetParentChildData(std::list<std::string> &listData);

                /** check the focus statement with the given event, compute the focus test */
                void CheckFocus(const Nc::System::Event &event);
                /** Verify if the widget or it's childs needs to be updated */
                void CheckState();

                // handler d'evenement de la fenetre
                /** Handler of Mouse motion event */
                virtual void MouseMotionEvent(const Nc::System::Event&)     {}
                /** Handler of Mouse button event */
                virtual void MouseButtonEvent(const Nc::System::Event&)     {}
                /** Handler of Keyboard event */
                virtual void KeyboardEvent(const Nc::System::Event&)        {}

                Widget                  *_parent;                   ///< The parent of the child null if there is not
                std::list<Widget*>      _listChild;                 ///< The list of childs
                Widget                  *_childFocused;             ///< The child witch have the focus
                bool                    _focus;                     ///< Mark if the widget has the focus
                Vector2i                _size;                      ///< Size of the widget
                Vector2i                _pos;                       ///< Position if the widget (relative to the parent and the Corner)
                Corner                  _corner[2];                 ///< The Corner[0]=Top||Botom and Corner[1]=Left||Right

                bool                    _enable;                    ///< eg: if a button or a parent of the button is set false, the button don't exec the handler
                bool                    _stateChange;               ///< if true, the widget will be update before to be rendered
                bool                    _drawEdge;                  ///< if true, the edge will be rendered
                bool                    _generateHandleAtEnterFocus;///< if true, genere an handle when we enter in focus
                Color                   _edgeColor;                 ///< edge color
                Vector2i                _margin;                    ///< margin inside of the widget

                Vector2f                _percent;                   ///< if the percent is different of null, then the size will be calculated in function of the parent size (if no parent then use the window size).

                Graphic::Material<Graphic::BasicVertexType::Colored2d>                      *_materialColored2d;        ///< the material used to draw the edge

            private:
                /** Draw the edge box */
                virtual void DrawEdgeBox(Graphic::ISceneGraph *scene);

                Graphic::Drawable<Graphic::BasicVertexType::Colored2d, false>     _drawableEdgeBox;     ///< the drawable used to draw the edge
        };
    }
}

#endif

