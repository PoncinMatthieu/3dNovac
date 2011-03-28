
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
}

Logger::~Logger()
{
    if (_file.is_open())
        CloseFile();
}

Logger &Logger::Log(int status)
{
    _status = status;
    if (_instance == NULL)
        _instance = new Logger();
    return *_instance;
}

Logger	&Logger::Log(const char* aFile, int aLine, int status)
{
    _status = status;
    if (_instance == NULL)
        _instance = new Logger();
    _instance->Write(aFile + std::string(" line:") + Convert::ToString(aLine) + std::string(":\t"), false);
    return *_instance;
}

#ifdef SYSTEM_WINDOWS
Logger &Logger::operator << (std::ostream  &(APIENTRY *f)(std::ostream &))
{
    std::ostringstream oss;

    f(oss);
    _mutex.Lock();
    Write(oss.str(), true);
    _mutex.Unlock();
    return *this;
}

Logger &Logger::operator << (std::ostream  &(__cdecl *f)(std::ostream &))
{
    std::ostringstream oss;

    f(oss);
    _mutex.Lock();
    Write(oss.str(), true);
    _mutex.Unlock();
    return *this;
}
#else
Logger &Logger::operator << (std::ostream &(*f)(std::ostream &))
{
    std::ostringstream oss;

    f(oss);
    _mutex.Lock();
    Write(oss.str(), true);
    _mutex.Unlock();
    return *this;
}
#endif

void Logger::CheckFile()
{
    if (!_file.is_open())
    {
        _file.open(_filename.Fullname().c_str());
        if (!_file)
            throw Exception("Logger", "Can't open the file '" + _filename.Fullname() + "' for the logger class");
    }
}

void Logger::Write(const string Msg, bool flush)
{
  // log
    if (_status == 0)
    {
        #if defined(_DEBUG) && defined(SYSTEM_WINDOWS)
            OutputDebugStringA(Msg.c_str());
        #endif
        if (_loggingfunction != NULL)
            _loggingfunction(Msg, flush);
        clog << Msg;
        if (flush)
            clog << std::flush;
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
  // debug
    else if (_status == 2)
    {
        #if defined(_DEBUG) && defined(SYSTEM_WINDOWS)
            OutputDebugStringA(Msg.c_str());
        #endif
        if (_loggingfunction != NULL)
            _loggingfunction(Msg, flush);
        CheckFile();
        cout << Msg;
        _file << Msg;
        if (flush)
        {
            cout << std::flush;
            _file.flush();
        }
    }
}
