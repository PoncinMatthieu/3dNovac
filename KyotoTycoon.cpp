
#include <ktremotedb.h>
#include "KyotoTycoon.h"

using namespace Nc;
using namespace Nc::Net;

KyotoTycoon::KyotoTycoon()
    : _db(NULL)
{
}

KyotoTycoon::~KyotoTycoon()
{
    if (_db != NULL)
        Disconnect();
}

bool    KyotoTycoon::Connect(const std::string &ip)
{
    if (_db != NULL)
        throw Utils::Exception("KyotoTycoon::Set", "The database is already connected.");

    _db = new kyototycoon::RemoteDB();
    if (!_db->open(ip))
    {
        LOG_ERROR << "KyotoTycoon::Set: Failed to connect to the database: " << _db->error().name() << std::endl;
        delete _db;
        _db = NULL;
        return false;
    }
    return true;
}

bool    KyotoTycoon::Disconnect()
{
    if (_db == NULL)
        throw Utils::Exception("KyotoTycoon::Disconnect", "The db is not connected!");

    if (!_db->close())
    {
        LOG_ERROR << "KyotoTycoon::Disconnect: " << _db->error().name() << std::endl;
        delete _db;
        _db = NULL;
        return false;
    }
    delete _db;
    _db = NULL;
    return true;
}

bool    KyotoTycoon::Set(const std::string &key, const std::string &value)
{
    if (_db == NULL)
        throw Utils::Exception("KyotoTycoon::Set", "The db is not connected!");

    if (!_db->set(key, value))
    {
        LOG_ERROR << "KyotoTycoon::Set: " << _db->error().name() << std::endl;
        return false;
    }
    return true;
}

bool    KyotoTycoon::Get(const std::string &key, std::string &value)
{
    if (_db == NULL)
        throw Utils::Exception("KyotoTycoon::Get", "The db is not connected!");

    if (!_db->get(key, &value))
    {
        LOG_ERROR << "KyotoTycoon::Get: " << _db->error().name() << std::endl;
        return false;
    }
    return true;
}

void    KyotoTycoon::GetAll(std::list<std::pair<std::string, std::string> > &listRecords)
{
    if (_db == NULL)
        throw Utils::Exception("KyotoTycoon::Set", "The db is not connected!");

    // traverse records
    kyototycoon::RemoteDB::Cursor *cur = _db->cursor();
    cur->jump();
    std::string ckey, cvalue;
    while (cur->get(&ckey, &cvalue, NULL, true))
        listRecords.push_back(std::pair<std::string, std::string>(ckey, cvalue));
    delete cur;
}
