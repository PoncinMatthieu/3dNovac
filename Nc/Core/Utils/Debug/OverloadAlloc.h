
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

#ifdef _DEBUG_MEMORY

    #ifndef NC_CORE_UTILS_OVERLOAD_ALLOC_DYNAMIC_H_
    #define NC_CORE_UTILS_OVERLOAD_ALLOC_DYNAMIC_H_

        #include "MemoryManager.h"

        // surcharge de l'operateur new, celui ci fait appel a la fonction d'allocation de "MemoryManager"
        inline void* operator new(std::size_t aSize, const char* aFile, int aLine)
        {
            return MemoryManager::Instance().Allocate(aSize, aFile, aLine, false);
        }

        // surcharge de l'operateur new[], celui ci fait appel a la fonction d'allocation de "MemoryManager"
        inline void* operator new[](std::size_t aSize, const char* aFile, int aLine)
        {
            return MemoryManager::Instance().Allocate(aSize, aFile, aLine, true);
        }

        // surcharge de l'operteur delete
        inline void operator delete(void* aPtr)
        {
            MemoryManager::Instance().Free(aPtr, false);
        }

        // surcharge de l'operateur delete appelé lors d'une destruction foireuse
        inline void operator delete(void* aPtr, const char* aFile, int aLine)
        {
            MemoryManager::Instance().CurrentFree(aFile, aLine);
            MemoryManager::Instance().Free(aPtr, false);
        }

        // surcherga de l'operateur delete[]
        inline void operator delete[](void* aPtr)
        {
            MemoryManager::Instance().Free(aPtr, true);
        }

        // surcharge de l'operateur delete[] appelé lors d'une destruction foireuse
        inline void operator delete[](void* aPtr, const char* aFile, int aLine)
        {
            MemoryManager::Instance().CurrentFree(aFile, aLine);
            MemoryManager::Instance().Free(aPtr, true);
        }

    #endif

    // declaration des macros permettant l'appel des surcharges sans aucune modification du code (a cause du nom de fichier et de la ligne passé en parametres)
    #ifndef new
        #define new new(__FILE__,__LINE__)
        #define delete MemoryManager::Instance().CurrentFree(__FILE__,__LINE__),delete
    #endif

#endif // _DEBUG
