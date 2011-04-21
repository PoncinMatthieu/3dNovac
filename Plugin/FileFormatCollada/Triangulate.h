
/*-----------------------------------------------------------------------------

	3dNovac PluginFileFormatCollada
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

    File Created At:        10/04/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_PLUGINFILEFORMATCOLLADA_TRIANGULATE_H_
#define NC_PLUGINFILEFORMATCOLLADA_TRIANGULATE_H_

#include <dae.h>
#include <dom/domCOLLADA.h>

namespace Nc
{
    class Triangulate
    {
        public:
            Triangulate(DAE *dae);
            void Process();

        private:
            void            CreateTrianglesFromPolygons(domMesh *thisMesh, domPolygons *thisPolygons);
            void            CreateTrianglesFromPolylist(domMesh *thisMesh, domPolylist *thisPolylist);
            unsigned int    getMaxOffset(domInputLocalOffset_Array &input_array);

            DAE             *_dae;
    };
}

#endif
