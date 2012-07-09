
/*-----------------------------------------------------------------------------

	3dNovac Core
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

    File Created At:        10/03/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_GRAPH_NODEVISITOR_H_
#define NC_CORE_GRAPH_NODEVISITOR_H_

#include "NNodePolitic.h"
#include "BinaryNode.h"

namespace Nc
{
    namespace Graph
    {
        /// Visitor base class used to visit a Node.
        template<class VisitorType, typename ReturnType = void, typename Base = Priv::INodeBasePolitic>
        class NodeVisitor : public AbstractNodeVisitor<VisitorType, ReturnType, Base>
        {
            public:
                template<typename ToVisitList, typename VisitableList>
                NodeVisitor(const ToVisitList &toVisitList, const VisitableList &visitableList, VisitTarget visitTarget = VisitChilds, bool postVisits = true, bool direction = true)
                    : AbstractNodeVisitor<VisitorType, ReturnType, Base>(toVisitList, visitableList, visitTarget, postVisits), _direction(direction)
                {}

                template<typename VisitedList>
                NodeVisitor(const VisitedList &visitedList, VisitTarget visitTarget = VisitChilds, bool postVisits = true, bool direction = true)
                    : AbstractNodeVisitor<VisitorType, ReturnType, Base>(visitedList, visitedList, visitTarget, postVisits), _direction(direction)
                {}

                template<typename T, class NodeType, bool Graph, class Allocator>
                void InvokeChilds(NNodePolitic<T,NodeType,0,Graph,Allocator> &n)
                {
                    if (_direction)
                    {
                        for (typename std::list<NodeType*>::iterator it = n.Childs().begin(); it != n.Childs().end(); ++it)
                            (*this)(**it);
                    }
                    else
                    {
                        for (typename std::list<NodeType*>::reverse_iterator it = n.Childs().rbegin(); it != n.Childs().rend(); ++it)
                            (*this)(**it);
                    }
                }

                template<typename T, class NodeType, unsigned int NbChilds, bool Graph, class Allocator>
                void InvokeChilds(NNodePolitic<T,NodeType,NbChilds,Graph,Allocator> &n)
                {
                    if (_direction)
                    {
                        for (unsigned int i = 0; i < NbChilds; ++i)
                        {
                            if (n.Childs().data[i] != NULL)
                                (*this)(n.Childs().data[i]);
                        }
                    }
                    else
                    {
                        for (unsigned int i = NbChilds - 1; i >= 0; --i)
                        {
                            if (n.Childs().data[i] != NULL)
                                (*this)(*n.Childs().data[i]);
                        }
                    }
                }

                template<typename T, class NodeType, class Allocator>
                void InvokeChilds(BinaryNodePolitic<T, NodeType, Allocator> &n)
                {
                    if (_direction)
                    {
                        if (n.Left() != NULL)
                            (*this)(*n.Left());
                        if (n.Right() != NULL)
                            (*this)(*n.Right());
                    }
                    else
                    {
                        if (n.Right() != NULL)
                            (*this)(*n.Right());
                        if (n.Left() != NULL)
                            (*this)(*n.Left());
                    }
                }

            private:
                bool        _direction;
        };
    }
}

#endif
