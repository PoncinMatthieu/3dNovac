
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        16/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_TREE_OCTREE_H_
#define NC_CORE_TREE_OCTREE_H_

#include "../Math/Math.h"
#include "NNodePolitic.h"
#include "Node.h"

namespace Nc
{
    namespace Graph
    {
		/// Politic used to define an octree with 8 childs and with space management (center and size)
        template<typename T, typename NodeType, class Allocator>
        struct OctreeNodePolitic : public NNodePolitic<std::list<T>, NodeType, 8, true, Allocator>
        {
            public:
                typedef NNodePolitic<std::list<T>, NodeType, 8, true, Allocator>    NodePolitic;

            public:
                OctreeNodePolitic()												: NodePolitic()                 {}
                OctreeNodePolitic(NodeType *parent)								: NodePolitic(parent)           {}
                ~OctreeNodePolitic()                                    {}

            protected:
                inline void     Init(const Math::Vector3f &center, unsigned int size)           {_center = center; _size = size;}

                Math::Vector3f  GetPositionFromIndex(unsigned int index);

            protected:
                Math::Vector3f      _center;
                unsigned int        _size;
        };

        template<typename T, typename NodeType, class Allocator>
        Math::Vector3f      OctreeNodePolitic<T,NodeType,Allocator>::GetPositionFromIndex(unsigned int index)
        {
            unsigned int sizeBy2 = _size / 2;
            switch (index)
            {
                case 0:     return Math::Vector3f(_center.data[0] - sizeBy2, _center.data[1] - sizeBy2, _center.data[2] - sizeBy2);
                case 1:     return Math::Vector3f(_center.data[0] + sizeBy2, _center.data[1] - sizeBy2, _center.data[2] - sizeBy2);
                case 2:     return Math::Vector3f(_center.data[0] - sizeBy2, _center.data[1] + sizeBy2, _center.data[2] - sizeBy2);
                case 3:     return Math::Vector3f(_center.data[0] + sizeBy2, _center.data[1] + sizeBy2, _center.data[2] - sizeBy2);
                case 4:     return Math::Vector3f(_center.data[0] - sizeBy2, _center.data[1] - sizeBy2, _center.data[2] + sizeBy2);
                case 5:     return Math::Vector3f(_center.data[0] + sizeBy2, _center.data[1] - sizeBy2, _center.data[2] + sizeBy2);
                case 6:     return Math::Vector3f(_center.data[0] - sizeBy2, _center.data[1] + sizeBy2, _center.data[2] + sizeBy2);
                case 7:     return Math::Vector3f(_center.data[0] + sizeBy2, _center.data[1] + sizeBy2, _center.data[2] + sizeBy2);
                default:    return Math::Vector3f::Null;
            }
        }

    }
}

#endif
