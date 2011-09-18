
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
            A widget use it's parents widget to calculate it's relative positions.
            And The relative position is computed with the defined Corner for X position and Y position
        */
        class LGUI  Widget : public Graphic::Object, public Engine::Handler
        {
            public:
                NC_UTILS_DEFINE_PARENT_CLASS(Graphic::Object);
                NC_UTILS_DEFINE_VISITABLE(System::Object);

            public:
                Widget(const Vector2i &pos = Vector2i(0, 0), const Vector2i &size = Vector2i(10, 10), Corner x = Top, Corner y = Left);
                Widget(const char *className, const Vector2i &pos = Vector2i(0, 0), const Vector2i &size = Vector2i(10, 10), Corner x = Top, Corner y = Left);
                Widget(const Widget &w);
                Widget &operator = (const Widget &w);
                virtual ~Widget();

                static const char   *ClassName()                                {return "Widget";}
                virtual ISceneNode  *Clone() const                              {return new Widget(*this);}

                // update et affichage du widget
                /** Manage the window event of the widget */
                void ManageWindowEvent(const Nc::System::Event &event);
                /** Render the widget */
                virtual void Render(Graphic::SceneGraph *scene);

                /** \return true if the state of the widget has changed after the last rendering pass */
                inline bool             StateChanged() const                    {return _stateChanged;}
                /** Set to true the changed statement */
                inline void             ChangeState()                           {_stateChanged = true;}

                /** Set the enable statement of the widget */
                virtual void            Enable(bool status)                {_enabled = status; ChangeChildsStateRecursive();}
                /** Enable the widget */
                virtual void            Enable()                           {_enabled = true; ChangeChildsStateRecursive();}
                /** Disable the widget */
                virtual void            Disable()                          {_enabled = false; ChangeChildsStateRecursive();}
                /** \return the enable statement */
                virtual bool            Enabled() const                    {return _enabled;}

                /** Set the inhibit statement of the widget */
                virtual void            Inhibit(bool status)              {_inhibit = status; ChangeChildsStateRecursive();}
                /** inhibit the widget */
                virtual void            Inhibit()                         {_inhibit = true; ChangeChildsStateRecursive();}
                /** Uninhibit the widget */
                virtual void            Unnihibit()                       {_inhibit = false; ChangeChildsStateRecursive();}
                /** \return the inhibit statement */
                virtual bool            Inhibited() const                 {return _inhibit;}
                /**
                    \return the inhibit statement (if false, draw it but for expample fill in grey a disabled button).
                    Recursif, return false if a parent is set to false
                */
                virtual bool            InhibitedRecursif() const;

                /** Resize and reposition the widget with their parent size and position */
                void                    Resized();

                inline const Vector2f   &Percent() const                        {return _percent;}
                /** Set the percent size. If the percent value is different of null, the widget will be resized with a proportion of his parent */
                void                    Percent(const Vector2f &percent);

                /** Return the position of the widget */
                inline const Vector2i   &Pos() const                            {return _pos;}
                /** Set the position */
                inline void             Pos(const Vector2f &pos)                {_pos = pos; _stateChanged = true;}
                /** Return the reel position of the widget (including the relative position Corner) */
                virtual void            GetReelPos(Vector2f &pos) const;
                /** Return the reel recursive position of the widget (including the relative position Corner and the parents) */
                void                    GetReelPosRecursif(Vector2f &pos) const;

                /** Set the size of the widget */
                inline virtual void     Size(const Vector2f &size)              {_size = size; _stateChanged = true;}
                /** Return the size of the widget */
                inline const Vector2i   &Size() const                           {return _size;}
                /** Return the reel size of the widget (including some specifique modification) */
                virtual void            GetReelSize(Vector2f &size) const;

                /** Set the focus statement */
                void                    Focus(bool state);
                /** Return the focus statement */
                bool                    Focus() const                           {return _focus;}
                /** Set the generateHandleAtEnterFocus statement. If it's true, the widget will generate an event when the widget will entered in focus */
                inline void             GenerateHandleAtEnterFocus(bool state)  {_generateHandleAtEnterFocus = state; _stateChanged = true;}

                /** Set the corner relative position */
                inline void             SetCorner(Corner x, Corner y)           {_corner[0] = x; _corner[1] = y; _stateChanged = true;}
                /** Return the corner relative position */
                inline Corner           GetCorner(unsigned int i)               {if (i < 2) return _corner[i]; throw Utils::Exception("Widget", "Overflow in function GetCorner");}
                /** Set the margin inside of the widget for childs */
                inline void             Margin(const Vector2i &margin)          {_margin = margin; _stateChanged = true;}
                /** Set the X margin value */
                inline void             MarginX(int x)                          {_margin.Data[0] = x; _stateChanged = true;}
                /** Set the Y margin value */
                inline void             MarginY(int y)                          {_margin.Data[1] = y; _stateChanged = true;}

                /** Set the draw edge statement */
                inline void             DrawEdge(bool state)                    {_drawEdge = state; _stateChanged = true;}
                /** Set the edge color */
                inline void             EdgeColor(const Color &color)           {_edgeColor = color; _stateChanged = true;}

            protected:
                /** Draw the widget */
                virtual void            Draw(Graphic::SceneGraph *scene);

                /**
                    Return a vector to translate the childs when we call the GetReelPos method. <br/>
                    Could be redefinen to manage the specific placement of childs in a widget (like in a WindowBox)
                 */
                virtual void            TranslateChild(const Corner[2], Vector2f &) const   {}

                /** Check the focus statement of the childs with the given event, compute the focus test */
                void                    CheckFocus(const Nc::System::Event &event);

                // handler d'evenement de la fenetre
                /** Handler of Mouse motion event */
                virtual void            MouseMotionEvent(const Nc::System::Event&)     {}
                /** Handler of Mouse button event */
                virtual void            MouseButtonEvent(const Nc::System::Event&)     {}
                /** Handler of Keyboard event */
                virtual void            KeyboardEvent(const Nc::System::Event&)        {}

                /** Set the changed state statement to true for all child widgets */
                void                    ChangeChildsStateRecursive();

                /** Update the widget geometry */
                virtual void            Update();

            private:
                /** Init the widget with the basics information */
                void                    Init(const Vector2i &pos, const Vector2i &size, Corner x, Corner y);

                /** Copy the widget attributes */
                void                    Copy(const Widget &w);

                /** Verify if the widget or it's childs needs to be updated */
                void                    CheckState();

            protected:
                Widget                  *_childFocused;             ///< The child witch has the focus
                bool                    _focus;                     ///< Mark if the widget has the focus
                Vector2i                _size;                      ///< Size of the widget
                Vector2i                _pos;                       ///< Position if the widget (relative to the parent and the Corner)
                Corner                  _corner[2];                 ///< The Corner[0]=Top||Botom and Corner[1]=Left||Right

                bool                    _inhibit;                   ///< eg: if a button or a parent of the button is set false, the button don't exec the handler
                bool                    _stateChanged;              ///< if true, the widget will be update before to be rendered
                bool                    _drawEdge;                  ///< if true, the edge will be rendered
                bool                    _generateHandleAtEnterFocus;///< if true, genere an handle when we enter in focus
                Color                   _edgeColor;                 ///< edge color
                Vector2i                _margin;                    ///< margin inside of the widget
                Vector2f                _percent;                   ///< if the percent is different of null, then the size will be calculated in function of the parent size (if no parent then use the window size).

            private:
                /** Draw the edge box */
                virtual void DrawEdgeBox(Graphic::SceneGraph *scene);
        };
    }
}

#endif

