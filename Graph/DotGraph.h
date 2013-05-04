
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

    File Created At:        12/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_TREE_DOTGRAPH_H_
#define NC_CORE_TREE_DOTGRAPH_H_

#include <fstream>
#include "Node.h"
#include "NodeVisitor.h"
#include "../Utils/Logger.h"
#include "../Utils/IFileFormat.h"

namespace Nc
{
    namespace Graph
    {
        /// To record a graph in a dot graph file format (.dot).
        /**
            \todo implement the LoadFile method.
			\todo now that we defined a visitor for the graph, we should try to use it with the DotGraph class.

            \sa
                - BinaryTree, to see an example using the DotGraph
        */
        template<typename T, bool Subtree = false>
        class DotGraph : public Utils::IFileFormat<DotGraph<T,Subtree>, T>
        {
            public:
                DotGraph()              {}
                virtual ~DotGraph()     {}

                /** Load the given file, and fill the given content. */
                virtual void LoadFile(const Utils::FileName &file, T &content);
                /** Save the given file with the given content. */
                virtual void SaveFile(const Utils::FileName &file, const T &content);

            private:
                /** Save a node recursively */
                void SaveNodeRecursively(std::ofstream &f, const T &content);
        };

        /// Specialization of DotGraph to manage subtrees (tree which store a subtree as a data).
        /**
            \sa
                - DotGraph
        */
        template<typename T>
        class DotGraph<T,true> : public Utils::IFileFormat<DotGraph<T,true>, T>
        {
            public:
                DotGraph()              {}
                virtual ~DotGraph()     {}

                /** Load the given file, and fill the given content. */
                virtual void LoadFile(const Utils::FileName &file, T &content);
                /** Save the given file with the given content. */
                virtual void SaveFile(const Utils::FileName &file, const T &content);

            private:
                /** Save a node recursively. */
                void SaveNodeRecursively(std::ofstream &f, const T &content);
        };

        template<typename T, bool Subtree>
        void DotGraph<T,Subtree>::LoadFile(const Utils::FileName &, T &)
        {
            LOG_ERROR << "DotGraph<T,Subtree>::LoadFile: This method is not implemented" << std::endl;
        }

        template<typename T, bool Subtree>
        void DotGraph<T,Subtree>::SaveFile(const Utils::FileName &file, const T &content)
        {
            std::ofstream   f(file.c_str());
            f << "digraph \"";
            f << content.NodeTypeToString() << "\"{" << std::endl;
            SaveNodeRecursively(f, content);
            f << "}" << std::endl;
        }

        template<typename T, bool Subtree>
        void DotGraph<T,Subtree>::SaveNodeRecursively(std::ofstream &f, const T &content)
        {
            f << "\"" << &content << "\" [label=\"" << content << "\"];" << std::endl;
            unsigned int size = content.ChildCount();
            for (unsigned int i = 0; i < size; ++i)
            {
                const T *n = content.Child(i);
                if (n != NULL)
                {
                    f << "\"" << &content << "\" -> \"" << n << "\"" << std::endl;
                    SaveNodeRecursively(f, *n);
                }
            }
        }


        template<typename T>
        void DotGraph<T,true>::LoadFile(const Utils::FileName &, T &)
        {
            LOG_ERROR << "implement me" << std::endl;
        }

        template<typename T>
        void DotGraph<T,true>::SaveFile(const Utils::FileName &file, const T &content)
        {
            std::ofstream   f(file.c_str());
            f << "digraph ";
            f << content.NodeTypeToString() << "{" << std::endl;
            SaveNodeRecursively(f, content);
            f << "}" << std::endl;
        }

        template<typename T>
        void DotGraph<T,true>::SaveNodeRecursively(std::ofstream &f, const T &content)
        {
            f << "\"" << &content << "\" [label=\"" << content << "\"];" << std::endl;
            unsigned int size = content.SubTreeCount();
            for (unsigned int i = 0; i < size; ++i)
            {
                const T *n = content.SubTree(i);
                if (n != NULL)
                {
                    f << "\"" << &content << "\" -> \"" << n << "\"" << std::endl;
                    SaveNodeRecursively(f, *n);
                }
            }
            size = content.ChildCount();
            for (unsigned int i = 0; i < size; ++i)
            {
                const T *n = content.Child(i);
                if (n != NULL)
                {
                    f << "\"" << &content << "\" -> \"" << n << "\"" << std::endl;
                    SaveNodeRecursively(f, *n);
                }
            }
        }
    }
}

#endif
