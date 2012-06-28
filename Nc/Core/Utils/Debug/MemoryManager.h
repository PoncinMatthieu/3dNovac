
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

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_MEMORY_MANAGER_H_
#define NC_CORE_UTILS_MEMORY_MANAGER_H_

#include <map>
#include <stack>
#include <string>
#include <fstream>
#include "../FileName.h"
#include "../../System/API/API.h"

namespace Nc
{
    namespace Utils
    {
        /// Check the allocated memory
        /**
            MemoryManager is a singleton class who provide a check of all memory allocated.

            Log the memory allocations in the file DebugMemory.log

            To Use this class, you have just to include `OverloadAlloc.h` in your file.

            \todo recode this memory manager to be thread safe.
			\deprecated this memory manager is deprecated. It needs to be recoded.
            \warning `OverloadAlloc.h` need to be not included in a .h and included after all include.
        */
        class MemoryManager
        {
            public:
                static MemoryManager& Instance();                      // Renvoie la reference de l'instance du singleton

                // allocation de memoire, renvoie le pointeur de l'objet aloué
                // lorsque aType est a "true" on aloue un tableau
                void* Allocate(std::size_t aSize, FileName aFile, int aLine, bool aType);

                // libere la memoire precedament aloué
                // lorsque aType est a "true" on aloue un tableau
                void Free(void* aPtr, bool aType);

                // Permet de sauvegarder les infos de la destruction suivante, ceci car on ne peut pas passer de parametre lors d'une destruction
                void CurrentFree(Utils::FileName aFile, int aLine);


            private:
                MemoryManager();   // constructeur et destructeur en privé (singleton)
                ~MemoryManager();

                /// Define infos of each allocated object for the MemoryManager class
                struct DataMemory
                {
                    std::size_t         Size;
                    Utils::FileName     File;
                    int                 Line;
                    bool                Type;
                };
                typedef std::map<void*, DataMemory>     MapDataMemory;  ///< Definition of a map of DataMemory

                MapDataMemory               _mapMemory;             // contient toutes les infos des allocations de memoires
                std::stack<DataMemory>      _stackCurrentFree;      // contient toutes les infos des prochainne suppression
                std::ofstream               _logFile;               // fichier de log
                System::Mutex               _mutex;                 // protection des list, map, fichier de log

                static MemoryManager       *_instance;
        };
    }
}

#endif // DMEMORYMANAGEMENT_H_INCLUDED
