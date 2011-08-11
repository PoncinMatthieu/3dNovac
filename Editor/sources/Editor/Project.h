
/*-----------------------------------------------------------------------------

	3dNovac Editor
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

    File Created At:        20/02/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef EDITOR_PROJECT_H_
#define EDITOR_PROJECT_H_

#include "../Define.h"
#include "../Engines/QManager.h"

namespace Nc
{
  namespace Editor
  {
    class EditorManager;

    class Project : public Nc::Utils::NonCopyable
    {
    public:
      Project(const std::string &name, EditorManager *editorManager);
      Project(const Utils::FileName &file, EditorManager *editorManager);
      virtual ~Project();

      void    ClearProject();
      void    LoadDefaultProject();   // load a new default project

      inline const std::string    &LibraryPath()          {return _libraryPath;}

    private:
      void    LoadProject();

      std::string             _libraryPath;

      std::string             _name;
      Utils::FileName	      _file;
      EditorManager	      *_editorManager;
      bool                    _loaded;
    };
  }
}

#endif
