
#ifndef NC_NET_KYOTOTYCOON_H_
#define NC_NET_KYOTOTYCOON_H_

#include "Define.h"

namespace kyototycoon
{
    class RemoteDB;
}

namespace Nc
{
    namespace Net
    {
        class KyotoTycoon
        {
            public:
                KyotoTycoon();
                ~KyotoTycoon();

                /**
                    Attempt a connection to the database.
                    \return true if the connection succeed.
                 */
                bool    Connect(const std::string &ip);

                /**
                    Attempt a disconnection to the database.
                    \return true if the disconnection succeed without errors.
                 */
                bool    Disconnect();

                /**
                    Set a record in the database.
                    \return true if the record was save properly.
                */
                bool    Set(const std::string &key, const std::string &value);

                /** Get a record from the given key. */
                bool    Get(const std::string &key, std::string &value);

                /** Get all records in a list of key/value pair. */
                void    GetAll(std::list<std::pair<std::string, std::string> > &listRecords);

            private:
                kyototycoon::RemoteDB   *_db;
        };
    }
}

#endif
