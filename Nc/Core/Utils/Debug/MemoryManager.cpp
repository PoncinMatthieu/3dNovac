
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

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "MemoryManager.h"

using namespace std;
using namespace Nc::Utils;

MemoryManager *MemoryManager::_instance = NULL;

MemoryManager& MemoryManager::Instance()
{
    if (_instance == NULL)
        _instance = new MemoryManager();
    return *_instance;
}

MemoryManager::MemoryManager() : _logFile("DebugMemory.log", ios::out | ios::trunc), _mutex()
{
    if(!_logFile)
        throw Utils::Exception("MemoryManager", "Can't open the memory log file `DebugMemory.log`");
    _logFile << "------------------- Memory allocation log --------------------" <<"\n"<<"\n";
}

MemoryManager::~MemoryManager()
{
// si la map est vide, tout c'est bien deroulé
    if(_mapMemory.empty())
        _logFile << "\n" << "No memory leek ! You are a 1337 :)" << std::endl;
// sinon, on va enregistrer dans le log les infos utile et liberer la mémoire
    else
    {
        _logFile << "\n" << "WARNING, There are a memory leek !" << "\n" << endl;
        for (MapDataMemory::iterator It = _mapMemory.begin(); It != _mapMemory.end(); ++It)
        {
            _logFile << "MEMORY LEEK \t 0x" << It->first << " \t " << It->second.Size << "octets\t"
                     << It->second.File.Filename() << "\tligne : " << It->second.Line << endl;
        }
    }
}

void* MemoryManager::Allocate(std::size_t aSize, FileName aFile, int aLine, bool aType)
{
// allocation dynamique et recuperation de la valeur du pointeur
    void* Ptr = malloc(aSize);

    DataMemory NewData;
    NewData.Size = aSize;
    NewData.File = aFile;
    NewData.Line = aLine;
    NewData.Type = aType;

// enregistrement des donnes dans la map memory avec pour clé la valeur du pointeur vers l'objet
    _mutex.Lock();
    _mapMemory[Ptr] = NewData;
    _mutex.Unlock();

// enregistrement dans le log
#ifdef _DEBUG_MEMORY_CONSOLE
    LOG <<"Allocation \t 0x" << Ptr << " \t " << aSize << "octets\t" << aFile.Filename() << "\tligne : " << aLine << endl;
#endif
    _logFile << "Allocation \t 0x" << Ptr << " \t " << aSize << "octets\t" << aFile.Filename() << "\tligne : " << aLine << endl;

// retourne le pointeur vers l'objet aloué
    return Ptr;

}

void MemoryManager::Free(void* Ptr, bool aType)
{
// Recherche de l'adresse dans les Mémoire alloués
//    _mutex.Lock();
    MapDataMemory::iterator It = _mapMemory.find(Ptr);
//    _mutex.Unlock();

// Si le bloc n'a pas été alloué
    if (It == _mapMemory.end())
    {
        // cela arive lorsque le fichier de surcharge des new et delete n'est pas ajouté dans un fichier
        // ou l'on effectue une allocation, car le delete reste surchargé et pas le new
        free(Ptr);
        return;
    }

// Si le type d'allocation ne correspond pas, on génère une exeption
    else if (It->second.Type != aType)
    {
        //throw BadDelete(); //TODO : exception
        _logFile << "Bad Delete" << endl;
#ifdef _DEBUG_MEMORY_CONSOLE
        LOG << "Bad Delete" << endl;
#endif
    }

// enfin, enregistrement dans le log et destruction de l'objet dans la map et la pile
#ifdef _DEBUG_MEMORY_CONSOLE
    LOG << "Desallocation \t 0x" << Ptr << " \t " << It->second.Size << "octets\t"
        << _stackCurrentFree.top().File.Filename() << "\tligne : " << _stackCurrentFree.top().Line << endl;
#endif
    if (_stackCurrentFree.size() > 0)
    {
        _logFile << "Desallocation \t 0x" << Ptr << " \t " << It->second.Size << "octets\t"
                 << _stackCurrentFree.top().File.Filename() << "\tligne : " << _stackCurrentFree.top().Line << endl;
        _stackCurrentFree.pop();
    }

    _mutex.Lock();
    _mapMemory.erase(It);
    _mutex.Unlock();
    free(Ptr);// Libération de la mémoire
}

void MemoryManager::CurrentFree(FileName aFile, int aLine)
{
    DataMemory Data;
    Data.File = aFile;
    Data.Line = aLine;

    _mutex.Lock();
    _stackCurrentFree.push(Data);
    _mutex.Unlock();
}



