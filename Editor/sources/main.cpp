
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

    File Created At:        05/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <QApplication>

#include "Interface/MainWindow.h"
#include "Engines/QGraphicEngine.h"

using namespace std;
using namespace Nc;
using namespace Nc::Editor;

int main(int ac, char **av)
{
    // init XLib support for concurrent thread support
    if (XInitThreads() == 0)
        throw Utils::Exception("main", "Can't init the X threads. your system does not support threads.");

    QApplication app(ac, av);
    QApplication::setStyle("Oxygen");

    try
      {
	Interface::MainWindow win;
	win.show();
	app.exec();
    }
    catch (const std::exception &e)
    {
        LOG_ERROR << "ERROR: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
