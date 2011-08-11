
#include "Logger.h"

using namespace Nc;
using namespace Nc::Editor;

Logger::Logger(QTextEdit *text)
    : Utils::Logger(), _logIntoTextEdit(true), _text(text)
{
}

void Logger::Write(const std::string msg, bool flush)
{
    Utils::Logger::Write(msg, flush);

    _mutex.Lock();
    if (_logIntoTextEdit)
    {
        static QString str;

        str += msg.c_str();
        if (flush)
        {
            if (str.size() > 0 && str[str.size() - 1] == '\n')
                str.remove(str.size() - 1, 1);
            _text->append(str);
            QTextCursor c =  _text->textCursor();
            c.movePosition(QTextCursor::End);
            _text->setTextCursor(c);
            str.clear();
        }
    }
    _mutex.Unlock();
}
