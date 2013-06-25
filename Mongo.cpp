
#include "Mongo.h"

using namespace Nc;
using namespace Nc::Net;
using namespace mongo;

Mongo::Mongo()
    : _db(NULL)
{
}

Mongo::~Mongo()
{
    if (_db != NULL)
        delete _db;
}

bool    Mongo::Connect(const std::string &ip, const std::string &dbName)
{
    if (_db != NULL)
        throw Utils::Exception("Mongo::Connect", "The database is already connected.");

    _ip = ip;
    _dbName = dbName;
    try
    {
        _db = new DBClientConnection();
        _db->connect(_ip);
    }
    catch(const mongo::DBException &e)
    {
        LOG_ERROR << "Mongo::Connect: Failed to connect to the database: " << e.what() << std::endl;
        return false;
    }
    return true;
}

auto_ptr<mongo::DBClientCursor>     Mongo::Query(const std::string &collection, const mongo::Query &query, int nToReturn, int nToSkip, const BSONObj *fieldsToReturn, int queryOptions, int batchSize)
{
    if (_db == NULL)
        throw Utils::Exception("Mongo::Query", "The db is not connected!");
    return _db->query(_dbName + "." + collection, query, nToReturn, nToSkip, fieldsToReturn, queryOptions, batchSize);
}

void    Mongo::Insert(const string &collection, const BSONObj &obj)
{
    if (_db == NULL)
        throw Utils::Exception("Mongo::Query", "The db is not connected!");

    _db->insert(_dbName + "." + collection, obj);
    CheckErrors();
}

void    Mongo::Update(const string &collection, const mongo::Query &query, const BSONObj &obj, bool upsert, bool multi)
{
    if (_db == NULL)
        throw Utils::Exception("Mongo::Update", "The db is not connected!");

    _db->update(_dbName + "." + collection, query, obj, upsert, multi);
    CheckErrors();
}

void    Mongo::CheckErrors()
{
    // check for errors
    string err = _db->getLastError();
    if (!err.empty())
        throw Utils::Exception("Mongo::CheckErrors", err);
}
