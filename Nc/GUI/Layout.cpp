
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

#include "Layout.h"

using namespace Nc;
using namespace Nc::GUI;

Layout::Layout(Pattern p, Corner x, Corner y)
    :   Widget(x, y),
        _pattern(p)
{
}

void Layout::SetRatio(Widget *child, float ratio)
{
    _mapRatio[child] = ratio;
}

void Layout::PosChild(const Widget *child, Vector2i &v) const
{
    int childCount = Childs().size();
    int childNumber = 0;
    for (ContainerType::const_iterator it = Childs().begin(); it != Childs().end(); ++it, ++childNumber)
        if (child == *it)
            break;

    Vector2i reelSize;
    GetReelSize(reelSize);

    switch (_pattern)
    {
        case Horizontal:
        {
            // if the ratio map is empty, no need to complexify it
            if (_mapRatio.empty())
            {
                int inc = reelSize[0] / childCount;
                if (child->GetCorner(0) == Right)
                    childNumber = childNumber - childCount + 1;
                if (child->GetCorner(0) == Center)
                    v[0] = v[0] - (reelSize[0] / 2) + (inc / 2);
                v[0] += (inc * childNumber);
            }
            else // otherwise, we need to create a size table to store childs size
            {
                Array<int> inc(childCount);
                int i = 0;
                int totalSize = 0;
                for (ContainerType::const_iterator it = Childs().begin(); it != Childs().end(); ++it, ++i)
                {
                    MapRatio::const_iterator itRatio = _mapRatio.find((Widget*)*it);
                    if (itRatio != _mapRatio.end())
                    {
                        inc[i] = ((float)reelSize[0] * itRatio->second) / 100;
                        totalSize += inc[i];
                    }
                    else
                        inc[i] = -1;
                }
                int leftInc = (childCount == _mapRatio.size()) ? 0 : ((reelSize[0] - totalSize) / (childCount - _mapRatio.size()));

                if (child->GetCorner(0) == Right)
                {
                    for (i = childNumber + 1; i < childCount; ++i)
                    {
                        if (inc[i] == -1)
                            inc[i] = leftInc;
                        v[0] -= inc[i];
                    }
                }
                else
                {
                    for (i = 0; i < childNumber; ++i)
                    {
                        if (inc[i] == -1)
                            inc[i] = leftInc;
                        v[0] += inc[i];
                    }
                    if (child->GetCorner(0) == Center)
                    {
                        if (inc[i] == -1)
                            inc[i] = leftInc;
                        v[0] = (v[0] - (reelSize[0] / 2)) + (inc[i] / 2);
                    }
                }
            }
        }
        break;


        case Vertical:
        {
            // if the ratio map is empty, no need to complexify it
            if (_mapRatio.empty())
            {
                int inc = reelSize[1] / childCount;
                if (child->GetCorner(1) == Bottom)
                    childNumber = childNumber - childCount + 1;
                if (child->GetCorner(1) == Center)
                    v[1] = v[1] + (reelSize[1] / 2) - (inc / 2);
                v[1] -= (inc * childNumber);
            }
            else // otherwise, we need to create a size table to store childs size
            {
                Array<int> inc(childCount);
                int i = 0;
                int totalSize = 0;
                for (ContainerType::const_iterator it = Childs().begin(); it != Childs().end(); ++it, ++i)
                {
                    MapRatio::const_iterator itRatio = _mapRatio.find((Widget*)*it);
                    if (itRatio != _mapRatio.end())
                    {
                        inc[i] = ((float)reelSize[1] * itRatio->second) / 100;
                        totalSize += inc[i];
                    }
                    else
                        inc[i] = -1;
                }
                int leftInc = (childCount == _mapRatio.size()) ? 0 : ((reelSize[1] - totalSize) / (childCount - _mapRatio.size()));

                if (child->GetCorner(1) == Bottom)
                {
                    for (i = childNumber + 1; i < childCount; ++i)
                    {
                        if (inc[i] == -1)
                            inc[i] = leftInc;
                        v[1] += inc[i];
                    }
                }
                else
                {
                    for (i = 0; i < childNumber; ++i)
                    {
                        if (inc[i] == -1)
                            inc[i] = leftInc;
                        v[1] -= inc[i];
                    }
                    if (child->GetCorner(1) == Center)
                    {
                        if (inc[i] == -1)
                            inc[i] = leftInc;
                        v[1] = (v[1] + (reelSize[1] / 2)) - (inc[i] / 2);
                    }
                }
            }
        }
        break;

/*
        case Grid:
            LOG << "The Layout grid is not implemented" << std::endl;
            break;
*/
    }
}

void Layout::SizeChild(const Widget *child, Vector2i &size) const
{
    GetReelSize(size);

    switch (_pattern)
    {
        case Horizontal:
        {
            if (_mapRatio.empty())
                size[0] /= Childs().size();
            else
            {
                MapRatio::const_iterator itRatio = _mapRatio.find((Widget*)child);
                if (itRatio != _mapRatio.end())
                    size[0] = (size[0] * itRatio->second) / 100;
                else
                {
                    int totalSize = 0;
                    for (itRatio = _mapRatio.begin(); itRatio != _mapRatio.end(); ++itRatio)
                        totalSize += ((float)size[0] * itRatio->second) / 100;
                    int leftInc = (size[0] - totalSize) / (_childs.size() - _mapRatio.size());
                    size[0] = (size[0] - totalSize) - ((_childs.size() - _mapRatio.size() - 1) * leftInc);
                }
            }
        }
        break;


        case Vertical:
        {
            if (_mapRatio.empty())
                size[1] /= Childs().size();
            else
            {
                MapRatio::const_iterator itRatio = _mapRatio.find((Widget*)child);
                if (itRatio != _mapRatio.end())
                    size[1] = (size[1] * itRatio->second) / 100;
                else
                {
                    int totalSize = 0;
                    for (itRatio = _mapRatio.begin(); itRatio != _mapRatio.end(); ++itRatio)
                        totalSize += ((float)size[1] * itRatio->second) / 100;
                    int leftInc = (size[1] - totalSize) / (_childs.size() - _mapRatio.size());
                    size[1] = (size[1] - totalSize) - ((_childs.size() - _mapRatio.size() - 1) * leftInc);
                }
            }
        }
        break;

/*
        case Grid:
            LOG << "The Layout grid is not implemented" << std::endl;
            break;
*/
    }
}

