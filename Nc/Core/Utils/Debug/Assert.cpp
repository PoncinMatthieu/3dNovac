
#include "Assert.h"

using namespace Nc;
using namespace Nc::Utils;

void AssertFailed(const std::string &expr, const std::string &file, int line, const std::string &function)
{
    CALLSTACK_INFO_ARG("At: " + file + ":" + Convert::ToString(line) + ": " + function + ": (" + expr + ")")
    CrashReporter::Abort();
}

void AssertFailedError(const std::string &error, const std::string &expr, const std::string &file, int line, const std::string &function)
{
    CALLSTACK_INFO_ARG("At: " + file + ":" + Convert::ToString(line) + ": " + function + ": (" + expr + ")");
    LOG_ERROR << "Fatal error: " << error << std::endl;
    CrashReporter::Abort();
}
