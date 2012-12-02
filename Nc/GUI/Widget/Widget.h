
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

#ifndef NC_GUI_WIDGET_WIDGET_H_
#define NC_GUI_WIDGET_WIDGET_H_

#include <Nc/Core/Engine/Handler.h>
#include "../Define.h"

namespace Nc
{
    namespace GUI
    {
        /// Base class to define a widget.
        /**
            A widget can receive window events and interact concequently.
            A widget use it's parents widget to calculate it's relative positions.
            And The relative position is computed with the defined Alignment for X position and Y position.

            \todo Think about a whole new event system for the widgets, a widget should be able to send a particular event to a parametized target.
        */
        class LGUI  Widget : public Graphic::Object, public Engine::Handler
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_INVOKABLE(Graphic::Object, System::Object, System::Object, Nc::GUI::Widget);

            public:
                Widget(const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0, 0));
                Widget(const Widget &w);
                Widget &operator = (const Widget &w);
                virtual ~Widget();

                virtual ISceneNode      *Clone() const                          {return new Widget(*this);}

                // update et affichage du widget
                /** Manage the window event of the widget. */
                void                    ManageWindowEvent(const Nc::System::Event &event);

                /** \return true if the state of the widget has changed after the last rendering pass. */
                inline bool             StateChanged() const                    {return _stateChanged;}
                /** Notify to the widget that it's state has changed. The Update method will be called at the next render pass. */
                inline void             ChangeState()                           {_stateChanged = true;}

                /** Set the enable statement of the widget. */
                virtual void            Enable(bool status);
                /** Enable the widget. */
                virtual void            Enable();
                /** Disable the widget. */
                virtual void            Disable();
                /** \return the enable statement. */
                virtual bool            Enabled() const                     {return Object::Enabled();}

                /** Set the inhibit statement of the widget. */
                virtual void            Inhibit(bool status);
                /** inhibit the widget. */
                virtual void            Inhibit();
                /** Uninhibit the widget. */
                virtual void            Uninhibit();
                /** \return the inhibit statement. */
                virtual bool            Inhibited() const                   {return _inhibit;}
                /**
                    \return the inhibit statement (if false, draw it but for expample fill in grey a disabled button).
                    Recursif, return false if a parent is set to false.
                */
                virtual bool            InhibitedRecursif() const;

                /** \return the size of the widget. */
                inline const Vector2i   &Size() const                       {return _size;}
                /** Set the size of the widget. */
                void                    Size(const Vector2i &size);
                /** Set the resizable statement. */
                void                    Resizable(bool state)               {_resizable = state;}
                /** \return the resizable statement. */
                bool                    Resizable() const                   {return _resizable;}
                /** Notify to the widget that it has been resized. The Resize method will be called at the next render pass. */
                virtual void            Resized();

                /** Return the position of the widget. */
                inline const Vector2i   &Pos() const                        {return _pos;}
                /** Set the position of the widget. */
                void                    Pos(const Vector2i &pos);
                /** \return the relative position of the widget (including the alignment of the widget and it's padding's parent). */
                void                    RelativePos(Vector2i &pos) const;
                /** \return the absolute position of the widget (recursively includes the alignment of the widget and it's padding's parent). */
                void                    AbsolutePos(Vector2i &pos) const;
                /** Notify a changement of position to the widget. The Repos method will be called at the next render pass. */
                virtual void            Reposed();

                /** \return the percent size property. */
                inline const Vector2f   &Percent() const                    {return _percent;}
                /** Set the percent size. If the percent value is different of null, the widget will be resized with a proportion of his parent. */
                void                    Percent(const Vector2f &percent);

                /** Set the focus statement. */
                void                    Focus(bool state);
                /** \return the focus statement. */
                bool                    Focus() const                               {return _focus;}
                /** Set the generateHandleAtEnterFocus statement. If it's true, the widget will generate an event when the widget will entered in focus. */
                void                    GenerateHandleAtEnterFocus(bool state);

                /** Set the alignment settings. */
                inline void                 Alignment(const AlignmentMask &mask)    {_alignment = mask;}
                /** \return the alignment settings. */
                inline const AlignmentMask  &Alignment() const                      {return _alignment;}

                /** Set the margin of the widget. */
                void                Margin(const BoxEdges &margin);
                /** Set the horizontal margin value. */
                void                MarginH(int m);
                /** Set the vertical margin value. */
                void                MarginV(int m);
                /** \return the margin left. */
                void                MarginLeft(int m);
                /** \return the margin right. */
                void                MarginRight(int m);
                /** \return the margin top. */
                void                MarginTop(int m);
                /** \return the margin bottom. */
                void                MarginBottom(int m);
                /** \return MarginLeft() + MarginRight() */
                unsigned int        MarginH() const;
                /** \return MarginTop() + MarginBottom() */
                unsigned int        MarginV() const;
                /** \return the margin left. */
                unsigned int        MarginLeft() const;
                /** \return the margin right. */
                unsigned int        MarginRight() const;
                /** \return the margin top. */
                unsigned int        MarginTop() const;
                /** \return the margin bottom. */
                unsigned int        MarginBottom() const;

                /** Set the padding of the widget. */
                void                Padding(const BoxEdges &padding);
                /** Set the horizontal padding value. */
                void                PaddingH(int p);
                /** Set the vertical padding value. */
                void                PaddingV(int p);
                /** \return the padding left. */
                void                PaddingLeft(int p);
                /** \return the padding right. */
                void                PaddingRight(int p);
                /** \return the padding top. */
                void                PaddingTop(int p);
                /** \return the padding bottom. */
                void                PaddingBottom(int p);
                /** \return PaddingLeft() + PaddingRight() */
                unsigned int        PaddingH() const;
                /** \return PaddingTop() + PaddingBottom() */
                unsigned int        PaddingV() const;
                /** \return the padding left, the result correspond to the padding property plus the size of the widget look edge. */
                unsigned int        PaddingLeft() const;
                /** \return the padding right, the result correspond to the padding property plus the size of the widget look edge. */
                unsigned int        PaddingRight() const;
                /** \return the padding top, the result correspond to the padding property plus the size of the widget look edge. */
                unsigned int        PaddingTop() const;
                /** \return the padding bottom, the result correspond to the padding property plus the size of the widget look edge. */
                unsigned int        PaddingBottom() const;

                /** Set the use stencil statement.
                    \warning this statement should be modified only before rendering...otherwise you should use a mutex to protect it's access.
                 */
                void                UseStencil(bool state);
                /** Get the use stencil statement */
                inline bool         UseStencil() const                          {return _useStencil;}

                /**
                    Set the look object of the widget and delete the previous look.
                    If \p look is null, the widget will not be decored.
                    \warning the look will be deleted by the widget, a look object should be used with one and only one widget.
                */
                void					UseLook(GUI::ILook *look = NULL);

                /** Remove the given widget. */
                void                    RemoveWidget(Widget *w);

            protected:
                /** called when the widget gain the focus. */
                virtual void            EnterFocus()                            {}
                /** called when the widget loose the focus. */
                virtual void            LeaveFocus()                            {}

                /**
                    Add a composed widget.
                    The composed widget are not deleted by the widget itself, so they should be deleted by the class which added those childs.
                */
                void                    AddComposedWidget(Widget *widget);
                /**
                    Remove the widget from the list of composed widget.
                    The composed widget are not deleted by the widget itself, so they should be deleted by the class which added those childs.
                */
                void                    RemoveComposedWidget(Widget *widget);

                /** Check the state and update the matrix before rendering. */
                virtual void            RenderBegin(Graphic::SceneGraph *scene);
                /** draw Debug GUI and update the matrix before rendering. */
                virtual void            RenderEnd(Graphic::SceneGraph *scene);
                /**
                    Set the stencil buffer if activated.
                    Here the clipping part is rectangular, so we simply use the scissor test insteed of using the stencil test.
                */
                virtual void            RenderChildsBegin(Graphic::SceneGraph *scene);
                /** Unset the stencil buffer if activated. */
                virtual void            RenderChildsEnd(Graphic::SceneGraph *scene);
                /** Draw the widget. */
                virtual void            Draw(Graphic::SceneGraph *scene);

                /**
                    Can be redefine to manage the specific placement of childs in a widget (like in a WindowBox or a Layout).
                    \return a vector to translate the childs when we call the RelativePos method.
                 */
                virtual void            PosChild(const Widget *, Vector2i &) const          {}
                /**
                    Can be redefine to manage the specific placement of childs in a widget (like in a WindowBox or a Layout).
                    \return the size that the child should use to set up it's percent size when we call the Resized method.
                 */
                virtual void            SizeChild(const Widget *, Vector2i &size) const;

                /** Check the focus statement of the childs with the given event, compute the focus test. */
                void                    CheckFocus(const Nc::System::Event &event);

                // handler d'evenement de la fenetre
                /** Handler of Mouse motion event. */
                virtual void            MouseMotionEvent(const Nc::System::Event&)     {}
                /** Handler of Mouse button event. */
                virtual void            MouseButtonEvent(const Nc::System::Event&)     {}
                /** Handler of Keyboard event. */
                virtual void            KeyboardEvent(const Nc::System::Event&)        {}

                /** Set the changed state statement to true for all child widgets. */
                void                    ChangeChildsStateRecursive();

                /** Update the widget geometry. Called when the state of the widget has changed (when property _stateChanged == true) */
                virtual void            UpdateState();
                /** Resize the widget. Called when the widget or a parent has been resized. */
                virtual void            Resize();
                /** Repos the widget. Called when the widget or a parent has changed of position. */
                virtual void            Repos()         {}

            private:
                /** Init the widget with the basics information. */
                void                    Init(const Vector2i &size, const AlignmentMask &alignment);

                /** Copy the widget attributes. */
                void                    Copy(const Widget &w);

                /** Verify if the widget or it's childs needs to be updated. */
                void                    CheckState();

            protected:
                Widget                  *_owner;                    ///< widget's owner... widgets can have a owner, this owner is charged to manage the widget like if it was a child but this widget does not appear in the child tree. This allow the creation of composed widget.
                ListPWidget             _composedWidget;            ///< list of composed widget... those widgets are managed like childs but are owned by the current widget and can't be modified outside of the class.

                AlignmentMask           _alignment;                 ///< The alignment of the widget relative to the parent.
                Vector2i                _size;                      ///< Size of the widget.
                Vector2i                _pos;                       ///< Position if the widget (relative to the parent and the alignment mask).

                Widget                  *_childFocused;             ///< The child which has the focus.
                bool                    _focus;                     ///< Mark if the widget has the focus.

                bool                    _inhibit;                   ///< eg: if a button or a parent of the button is set false, the button don't exec the handler.
                bool                    _stateChanged;              ///< if true, the widget will be update before to be rendered.
                bool                    _resized;                   ///< if true, the method resize will be called before the widget render itself and the size of the widget will be compute if possible.
                bool                    _reposed;                   ///< if true, the method repos will be called before the widget render itself.
                bool                    _generateHandleAtEnterFocus;///< if true, genere an handle when we enter in focus.
                bool                    _resizable;                 ///< if false, the widget will not be resized.
                Vector2f                _percent;                   ///< if the percent is different of null, then the size will be calculated in function of the parent size (if no parent then use the window size).

                bool                    _useStencil;                ///< if true, use the stencil buffer to be sure that the childs will not be drawn outside of the widget.

                ILook                   *_widgetLook;               ///< look used to render the look of the widget.

            private:
                BoxEdges                _margin;                    ///< Used to space out widgets, mergin correspond of the space outside the widget in which the childs are spaced out. To access the property you should call the methods "MarginLeft / MarginRight / MarginTop / MarginBottom"
                BoxEdges                _padding;                   ///< Used to space out widgets, padding correspond of the space inside the widget in which the childs are spaced out. To access the property you should call the methods "PaddingLeft / PaddingRight / PaddingTop / PaddingBottom"

                template<typename VisitorType, bool IsConst, typename ReturnType>
                friend class Visitor::WidgetVisitor;

                /** \todo to delete after adding an InvokationMethode on visitors */
                friend struct Visitor::CheckFocus;
        };
    }
}

#endif

