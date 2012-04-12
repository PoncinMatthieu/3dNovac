
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
        /// Base class to define a widget
        /**
            A widget can receive window events and interact concequently.   <br/>
            A widget use it's parents widget to calculate it's relative positions.
            And The relative position is computed with the defined Corner for X position and Y position
        */
        class LGUI  Widget : public Graphic::Object, public Engine::Handler
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_INVOKABLE(Graphic::Object, System::Object, System::Object, Nc::GUI::Widget);

            private:
                class Look
                {
                    public:
                        Look(const std::string &name);
                        Look(const Look &l);
                        Look &operator = (const Look &l);
                        ~Look();

                        void    Draw(Graphic::SceneGraph *scene);
                        void    Update(const Vector2i &size);

                        bool                    activated;

                    private:
                        void    Copy(const Look &l);

                    private:
                        Graphic::Sprite         *_spriteLeftEdge;
                        Graphic::Sprite         *_spriteTopEdge;
                        Graphic::Sprite         *_spriteRightEdge;
                        Graphic::Sprite         *_spriteBottomEdge;
                        Graphic::Sprite         *_spriteLeftTop;
                        Graphic::Sprite         *_spriteRightTop;
                        Graphic::Sprite         *_spriteLeftBottom;
                        Graphic::Sprite         *_spriteRightBottom;
                        Graphic::Sprite         *_spriteMiddle;
                };

            public:
                Widget(Corner x = Left, Corner y = Top, const Vector2i &pos = Vector2i(0, 0), const Vector2i &size = Vector2i(0, 0));
                Widget(const Widget &w);
                Widget &operator = (const Widget &w);
                virtual ~Widget();

                virtual ISceneNode  *Clone() const                              {return new Widget(*this);}

                // update et affichage du widget
                /** Manage the window event of the widget */
                void                    ManageWindowEvent(const Nc::System::Event &event);

                /** \return true if the state of the widget has changed after the last rendering pass */
                inline bool             StateChanged() const                    {return _stateChanged;}
                /** Set to true the changed statement */
                inline void             ChangeState()                           {_stateChanged = true;}

                /** Set the enable statement of the widget */
                virtual void            Enable(bool status)                {Object::Enable(status); ChangeChildsStateRecursive();}
                /** Enable the widget */
                virtual void            Enable()                           {Object::Enable(); ChangeChildsStateRecursive();}
                /** Disable the widget */
                virtual void            Disable()                          {Object::Disable(); ChangeChildsStateRecursive();}
                /** \return the enable statement */
                virtual bool            Enabled() const                    {return Object::Enabled();}

                /** Set the inhibit statement of the widget */
                virtual void            Inhibit(bool status)              {_inhibit = status; ChangeChildsStateRecursive();}
                /** inhibit the widget */
                virtual void            Inhibit()                         {_inhibit = true; ChangeChildsStateRecursive();}
                /** Uninhibit the widget */
                virtual void            Uninhibit()                       {_inhibit = false; ChangeChildsStateRecursive();}
                /** \return the inhibit statement */
                virtual bool            Inhibited() const                 {return _inhibit;}
                /**
                    \return the inhibit statement (if false, draw it but for expample fill in grey a disabled button).
                    Recursif, return false if a parent is set to false
                */
                virtual bool            InhibitedRecursif() const;

                /** Set the resizable statement */
                void                    Resizable(bool state)           {_resizable = state;}
                /** \return the resizable statement */
                bool                    Resizable() const               {return _resizable;}
                /** Resize and reposition the widget with their parent size and position */
                void                    Resized();

                /** \return the percent size property */
                inline const Vector2f   &Percent() const                        {return _percent;}
                /** Set the percent size. If the percent value is different of null, the widget will be resized with a proportion of his parent */
                void                    Percent(const Vector2f &percent);

                /** Return the position of the widget */
                inline const Vector2i   &Pos() const                            {return _pos;}
                /** Set the position */
                inline void             Pos(const Vector2i &pos)                {_pos = pos; _stateChanged = true;}
                /** \return the reel position of the widget (including the relative position Corner) */
                virtual void            GetReelPos(Vector2i &pos) const;
                /** \return the reel recursive position of the widget (including the relative position Corner and the parents) */
                void                    GetReelPosRecursif(Vector2i &pos) const;

                /** Set the size of the widget */
                inline void             Size(const Vector2i &size)              {_size = size; _stateChanged = true;}
                /** \return the size of the widget */
                inline const Vector2i   &Size() const                           {return _size;}
                /** \return the reel size of the widget (including some specifique modification) */
                virtual void            GetReelSize(Vector2i &size) const;

                /** Set the focus statement */
                void                    Focus(bool state);
                /** \return the focus statement */
                bool                    Focus() const                           {return _focus;}
                /** Set the generateHandleAtEnterFocus statement. If it's true, the widget will generate an event when the widget will entered in focus */
                inline void             GenerateHandleAtEnterFocus(bool state)  {_generateHandleAtEnterFocus = state; _stateChanged = true;}

                /** Set the corner relative position */
                inline void             SetCorner(Corner x, Corner y)           {_corner[0] = x; _corner[1] = y; _stateChanged = true;}
                /** \return the corner relative position */
                inline Corner           GetCorner(unsigned int i) const         {if (i < 2) return _corner[i]; throw Utils::Exception("Widget", "Overflow in function GetCorner");}
                /** Set the margin inside of the widget for childs */
                inline void             Margin(const Vector2i &margin)          {_margin = margin; _stateChanged = true;}
                /** Set the X margin value */
                inline void             MarginX(int x)                          {_margin[0] = x; _stateChanged = true;}
                /** Set the Y margin value */
                inline void             MarginY(int y)                          {_margin[1] = y; _stateChanged = true;}

                /** Set the use stencil statement.
                    \warning this statement should be modified only before rendering...otherwise you should use a mutex to protect it's access.
                 */
                inline void             UseStencil(bool state)                  {_useStencil = state; _stateChanged = true;}
                /** Get the use stencil statement */
                inline bool             UseStencil() const                      {return _useStencil;}

                /** Set the look name of the widget.
                    If \p name is empty, the widget will be decorate with the default widget sprite name.
                */
                virtual void            UseLook(const std::string &name = "");

                /** Remove the given widget */
                void                    RemoveWidget(Widget *w);

            protected:
                /**
                    Add a composed widget.
                    The composed widget are not deleted by the widget itself, so they should be deleted by the class wich added those childs
                */
                void                    AddComposedWidget(Widget *widget);
                /**
                    Remove the widget from the list of composed widget.
                    The composed widget are not deleted by the widget itself, so they should be deleted by the class wich added those childs
                */
                void                    RemoveComposedWidget(Widget *widget);

                /** Check the state and update the matrix before rendering */
                virtual void            RenderBegin(Graphic::SceneGraph *scene);
                /** draw Debug GUI and update the matrix before rendering */
                virtual void            RenderEnd(Graphic::SceneGraph *scene);
                /**
                    Set the stencil buffer if activated.
                    Here the clipping part is rectangular, so we simply use the scissor test insteed of using the stencil test.
                */
                virtual void            RenderChildsBegin(Graphic::SceneGraph *scene);
                /** Unset the stencil buffer if activated */
                virtual void            RenderChildsEnd(Graphic::SceneGraph *scene);
                /** Draw the widget */
                virtual void            Draw(Graphic::SceneGraph *scene);

                /**
                    Can be redefine to manage the specific placement of childs in a widget (like in a WindowBox or a Layout)
                    \return a vector to translate the childs when we call the GetReelPos method.
                 */
                virtual void            PosChild(const Widget *, Vector2i &) const          {}
                /**
                    Can be redefine to manage the specific placement of childs in a widget (like in a WindowBox or a Layout)
                    \return the size that the child should use to set up it's percent size when we call the Resized method.
                 */
                virtual void            SizeChild(const Widget *, Vector2i &size) const     {GetReelSize(size);}

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
                Widget                  *_owner;                    ///< widget's owner... widgets can have a owner, this owner is charged to manage the widget like if it was a child but this widget does not appear in the child tree. This allow the creation of composed widget
                ListPWidget             _composedWidget;            ///< list of composed widget... those widgets are managed like childs but are owned by the current widget and can't be modified outside of the class.

                Widget                  *_childFocused;             ///< The child wich has the focus
                bool                    _focus;                     ///< Mark if the widget has the focus
                Vector2i                _size;                      ///< Size of the widget
                Vector2i                _pos;                       ///< Position if the widget (relative to the parent and the Corner)
                Corner                  _corner[2];                 ///< The Corner[0]=Top||Botom and Corner[1]=Left||Right

                bool                    _inhibit;                   ///< eg: if a button or a parent of the button is set false, the button don't exec the handler
                bool                    _stateChanged;              ///< if true, the widget will be update before to be rendered
                bool                    _generateHandleAtEnterFocus;///< if true, genere an handle when we enter in focus
                bool                    _resizable;                 ///< if false, the widget will not be resized
                Vector2i                _margin;                    ///< margin inside of the widget
                Vector2f                _percent;                   ///< if the percent is different of null, then the size will be calculated in function of the parent size (if no parent then use the window size).

                bool                    _useStencil;                ///< if true, use the stencil buffer to be sure that the childs will not be drawn outside of the widget.

            private:
                Look                    *_widgetLook;

                template<typename VisitorType, bool IsConst, typename ReturnType>
                friend class WidgetVisitor;
        };
    }
}

#endif

