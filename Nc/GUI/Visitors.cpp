
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

    File Created At:        27/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Visitors.h"

using namespace Nc;
using namespace Nc::GUI;
using namespace Nc::GUI::Visitor;

ResizedAll::ResizedAll()
	: WidgetVisitor<ResizedAll>(Utils::Metaprog::Seq<Widget>::Type(),
                                            Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type())
{
}

void ResizedAll::VisitNode(Widget &w)
{
	w.Resized();
}

ReposedAll::ReposedAll()
	: WidgetVisitor<ReposedAll>(Utils::Metaprog::Seq<Widget>::Type(),
											Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type())
{
}

void ReposedAll::VisitNode(Widget &w)
{
	w.Reposed();
}

IsInhibited::IsInhibited()
	: WidgetVisitor<IsInhibited, true>( Utils::Metaprog::Seq<Widget>::Type(),
													Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type(), Graph::VisitParents),
    result(false)
{
}

void IsInhibited::VisitNode(const Widget &w)
{
	if (w.Inhibited())
		result = true;
}

CheckFocus::CheckFocus(const Nc::System::Event &e, const Vector2i &mouseP)
    : WidgetVisitor<CheckFocus>(Utils::Metaprog::Seq<Widget>::Type(),
                                Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type()),
        event(e), mousePos(mouseP), childFocused(NULL)
{}

void CheckFocus::VisitNode(Widget &w)
{
    if (!w.EnabledRecursif() || w.InhibitedRecursif())
        return;

    Vector2i    pos;
    Vector2i    size;

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

GetParentWidget::GetParentWidget(const Widget *w)
    : WidgetVisitor<GetParentWidget, true>( Utils::Metaprog::Seq<Widget, SceneGraph>::Type(),
                                                        Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type(), Graph::VisitParents),
        widget(w), parent(NULL), parentSceneGraph(NULL)
{}

void GetParentWidget::VisitNode(const Widget &w)
{
    if (widget != &w && parent == NULL)
        parent = &w;
}

void GetParentWidget::VisitNode(const SceneGraph &s)
{
    if (parentSceneGraph == NULL)
        parentSceneGraph = &s;
}

ChangeStates::ChangeStates()
    : WidgetVisitor<ChangeStates>(Utils::Metaprog::Seq<Widget>::Type(),
                                                Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type())
{}

void ChangeStates::VisitNode(Widget &w)
{
	w.ChangeState();
}
