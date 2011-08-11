#ifndef NC_EDITOR_INTERFACE_LOGGER_H
#define NC_EDITOR_INTERFACE_LOGGER_H

#include <QTextEdit>
#include "../Define.h"
#include <Nc/Core/Utils/Logger.h>

namespace Nc
{
    namespace Editor
    {
        class Logger : public Utils::Logger
        {
        public:
            Logger(QTextEdit *text);

            void LogIntoTextEdit(bool state)    {_mutex.Lock(); _logIntoTextEdit = state; _mutex.Unlock();}

        protected:
            virtual void Write(const std::string msg, bool flush);

        private:
            bool        _logIntoTextEdit;
            QTextEdit   *_text;
        };
    }
}

#endif
