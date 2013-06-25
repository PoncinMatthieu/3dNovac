
#ifndef NC_NET_MONGO_H_
#define NC_NET_MONGO_H_

#include <mongo/client/dbclient.h>
#undef LOG // force redefinition of the macro overwrite by mongodb headers

#include <Nc/Core/Utils/Logger.h>
#include <Nc/Core/Math/Math.h>
#include "Define.h"

namespace Nc
{
    namespace Net
    {
        class Mongo
        {
            public:
                Mongo();
                ~Mongo();

                /**
                    Attempt a connection to the database.
                    \return true if the connection succeed.
                 */
                bool    Connect(const std::string &ip, const std::string &dbName);

                /** Attempt a query. */
                std::auto_ptr<mongo::DBClientCursor>     Query(const std::string &collection, const mongo::Query &query, int nToReturn=0, int nToSkip=0, const mongo::BSONObj *fieldsToReturn=0, int queryOptions=0, int batchSize=0);

                /** Insert data. */
                void    Insert(const std::string &collection, const mongo::BSONObj &obj);
                /** Update data. */
                void    Update(const std::string &collection, const mongo::Query &query, const mongo::BSONObj &obj, bool upsert=false, bool multi=false);

            private:
                void    CheckErrors();

            private:
                std::string                 _ip;
                std::string                 _dbName;
                mongo::DBClientConnection   *_db;
        };
    }
}

#endif
