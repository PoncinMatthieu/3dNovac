
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

    File Created At:        17/09/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_VISITORS_H_
#define NC_GUI_VISITORS_H_

#include <Nc/Graphics/Scene/SceneNodeVisitor.h>
#include "Define.h"
#include "WindowBox.h"
namespace Nc
{
    namespace GUI
    {
        namespace Visitor
        {
            struct ResizeAll : public Graphic::SceneNodeVisitor<ResizeAll>
            {
                ResizeAll()
                    : Graphic::SceneNodeVisitor<ResizeAll>(Utils::Metaprog::Seq<Widget>::Type(),
                                                           Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type())
                    {}

                void VisitNode(Widget &w)       {w.Resized();}
            };

            struct IsInhibited : public Graphic::SceneNodeVisitor<IsInhibited, true>
            {
                IsInhibited()
                    : Graphic::SceneNodeVisitor<IsInhibited, true>( Utils::Metaprog::Seq<Widget>::Type(),
                                                                    Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type(), Graph::VisitParents),
                      result(false)
                    {}

                void VisitNode(const Widget &w)         {if (w.Inhibited()) result = true;}

                bool    result;
            };

            struct CheckFocus : public Graphic::SceneNodeVisitor<CheckFocus>
            {
                CheckFocus(const Nc::System::Event &e, Vector2i mouseP)
                    : Graphic::SceneNodeVisitor<CheckFocus>(Utils::Metaprog::Seq<Widget>::Type(),
                                                            Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type()),
                      event(e), mousePos(mouseP), childFocused(NULL)
                    {}

                void VisitNode(Widget &w)
                {
                    if (!w.EnabledRecursif() || w.InhibitedRecursif())
                        return;

                    Vector2f    pos;
                    Vector2f    size;

                    w.GetReelPosRecursif(pos);
                    w.GetReelSize(size);
                    #ifdef _DEBUG_GUI_FOCUS
                    LOG_DEBUG << "Widget: " << w << std::endl;
                    LOG_DEBUG << "ReelPos   = " << pos << std::endl;
                    LOG_DEBUG << "ReelSize  = " << size << std::endl;
                    LOG_DEBUG << "Mouse = " << mousePos << std::endl;
                    #endif
                    if (Math::InRect(pos, size, mousePos))
                    {
                        childFocused = &w;
                        childFocused->Focus(true);
                        #ifdef _DEBUG_GUI_FOCUS
                        LOG_DEBUG << "OK" << std::endl;
                        #endif
                    }
                    #ifdef _DEBUG_GUI_FOCUS
                    LOG << std::endl;
                    #endif
                }

                const Nc::System::Event     &event;
                const Vector2i              &mousePos;
                Widget                      *childFocused;
            };

            struct GetParentWidget : public Graphic::SceneNodeVisitor<GetParentWidget, true>
            {
                GetParentWidget(const Widget *w)
                    : Graphic::SceneNodeVisitor<GetParentWidget, true>( Utils::Metaprog::Seq<Widget>::Type(),
                                                                        Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type(), Graph::VisitParents),
                      widget(w), parent(NULL)
                    {}

                void VisitNode(const Widget &w)         {if (widget != &w && parent == NULL) parent = &w;}

                const Widget    *widget;
                const Widget    *parent;
            };

            struct ChangeStates : public Graphic::SceneNodeVisitor<ChangeStates>
            {
                ChangeStates()
                    : Graphic::SceneNodeVisitor<ChangeStates>(Utils::Metaprog::Seq<Widget>::Type(),
                                                              Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type())
                    {}

                void VisitNode(Widget &w)       {w.ChangeState();}
            };


/*
struct CheckFocusFonctor
{
    CheckFocusFonctor(const Event &e, Vector2i mouseP)
        : event(e), mousePos(mouseP), childFocused(NULL) {}

    bool operator () (ISceneNode *node)
    {
        Widget *w = node->AsWithoutThrow<Widget>();
        if (w != NULL)
        {
            if (!w->EnabledRecursif() || w->InhibitedRecursif())
                return true;

            Vector2f    pos;
            Vector2f    size;

            w->GetReelPosRecursif(pos);
            w->GetReelSize(size);
            #ifdef _DEBUG_GUI_FOCUS
            LOG_DEBUG << "Widget: " << *w << std::endl;
            LOG_DEBUG << "ReelPos   = " << pos << std::endl;
            LOG_DEBUG << "ReelSize  = " << size << std::endl;
            LOG_DEBUG << "Mouse = " << mousePos << std::endl;
            #endif
            if (Math::InRect(pos, size, mousePos))
            {
                childFocused = w;
                childFocused->Focus(true);
                #ifdef _DEBUG_GUI_FOCUS
                LOG_DEBUG << "OK" << std::endl;
                #endif
            }
            #ifdef _DEBUG_GUI_FOCUS
            LOG << std::endl;
            #endif
            return false;
        }
        return true;
    }

    const Event     &event;
    const Vector2i  &mousePos;
    Widget          *childFocused;
};

struct GetFirstFindWidgetFonctor
{
    GetFirstFindWidgetFonctor(const Widget *c)
        : widget(NULL), current(c)    {}

    bool operator () (const ISceneNode *node)
    {
        if (node != current)
        {
            const Widget *w = node->AsWithoutThrow<Widget>();
            if (w != NULL)
                widget = w;
        }
        return (widget == NULL);
    }

    const Widget *widget;
    const Widget *current;
};

struct ResizedFonctor
{
    bool operator () (ISceneNode *node)
    {
        Widget *n = node->AsWithoutThrow<Widget>();
        if (n != NULL)
        {
            float sParent;
            if (n->Percent().Data[0] != 0 || n->Percent().Data[1] != 0)
            {
                GetFirstFindWidgetFonctor f(n);
                n->ForEachParents<false>(f); // get back the first widget parent
                Vector2i newSize = n->Size();
                if (n->Percent().Data[0] != 0)
                {
                    sParent = (f.widget != NULL) ? f.widget->Size()[0] : Window::Width();
                    newSize[0] = ((float)(n->Percent().Data[0] * sParent) / 100.0);
                }
                if (n->Percent().Data[1] != 0)
                {
                    sParent = (f.widget != NULL) ? f.widget->Size()[1] : Window::Height();
                    newSize[1] = ((float)(n->Percent().Data[1] * sParent) / 100.0);
                }
                n->Size(newSize);
            }
            n->ChangeState();
        }
        return true;
    }
};


struct ChangeChildsStateRecursiveFonctor
{
    bool operator () (ISceneNode *node)
    {
        Widget *p = node->AsWithoutThrow<Widget>();
        if (p != NULL)
            p->ChangeState();
        return true;
    }
};
*/
        }
    }
}

#endif
