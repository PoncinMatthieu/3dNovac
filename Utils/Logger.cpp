
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

#include <fstream>

#include "Logger.h"
#include "Convert.h"
#include "Exception.h"

#if defined(_DEBUG) && defined(SYSTEM_WINDOWS)
    #include <Windows.h>
#endif

using namespace std;
using namespace Nc;
using namespace Nc::Utils;

int Logger::_status = 0;

Logger::Logger()
{
    _filename = FileName("out.log");
    _loggingfunction = NULL;
    _writeTime = false;
    _newLine = true;
}

Logger::~Logger()
{
    if (_file.is_open())
        CloseFile();
}

Logger &Logger::Log(int status)
{
    _status = status;
    return Instance();
}

Logger	&Logger::Log(const char* file, int line, int status)
{
    _status = status;
    Logger &instance = Instance();
    instance.Write(file + std::string(" line:") + Convert::ToString(line) + std::string(":\t"), false);
    return instance;
}

#ifdef SYSTEM_WINDOWS
Logger &Logger::operator << (std::ostream  &(APIENTRY *f)(std::ostream &))
{
    std::ostringstream oss;

    f(oss);
    Write(oss.str(), true);
    return *this;
}

Logger &Logger::operator << (std::ostream  &(__cdecl *f)(std::ostream &))
{
    std::ostringstream oss;

    f(oss);
    Write(oss.str(), true);
    return *this;
}
#else
Logger &Logger::operator << (std::ostream &(*f)(std::ostream &))
{
    std::ostringstream oss;

    f(oss);
    Write(oss.str(), true);
    return *this;
}
#endif

void Logger::CheckFile()
{
    if (!_file.is_open())
    {
        _file.open(_filename.c_str());
        if (!_file)
            throw Exception("Logger", "Can't open the file '" + _filename + "' for the logger class");
    }
}

void Logger::Write(const std::string &Msg, bool flush)
{

    if (_newLine && !Msg.empty())
    {
        _newLine = false;
        if (_writeTime)
        {
            time_t rawtime;
            struct tm *timeinfo;
            char buffer[100];
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            if (timeinfo != NULL)
            {
                strftime(buffer, 100, "[%H:%M:%S - %d/%m/%Y] ", timeinfo);
                Write(buffer, false);
            }
        }
    }

    System::Locker l(&_mutex);

  // log or debug log
    if (_status == 0 || _status == 2)
    {
        #if defined(_DEBUG) && defined(SYSTEM_WINDOWS)
            OutputDebugStringA(Msg.c_str());
        #endif
        if (_loggingfunction != NULL)
            _loggingfunction(Msg, flush);
        CheckFile();
        clog << Msg;
        _file << Msg;
        if (flush)
        {
            clog << std::flush;
            _file.flush();
        }
    }
  // error
    else if (_status == 1)
    {
        #if defined(_DEBUG) && defined(SYSTEM_WINDOWS)
            OutputDebugStringA(Msg.c_str());
        #endif
        if (_loggingfunction != NULL)
            _loggingfunction(Msg, flush);
        CheckFile();
        cerr << Msg;
        _file << Msg;
        if (flush)
        {
            cerr << std::flush;
            _file.flush();
        }
    }

    if (flush)
        _newLine = true;
}
