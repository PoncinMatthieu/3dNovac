
#ifndef TUTO_NC_CORE_SERVER_CLIENT_H_
#define TUTO_NC_CORE_SERVER_CLIENT_H_

#include <Nc/Core/Network/SocketTcp.h>

#define BUFF_SIZE	2048

class Client : public Nc::Network::SocketTcp, public Nc::Utils::NonCopyable
{
private:
  typedef std::list<std::string>	ListPendingRequest;

public:
  Client()	{}
  ~Client()	{LOG << "Connection closed" << std::endl; Close();}

  /** \return true if the client has something to write */
  inline bool   HasPendingRequest()			{return (!_pendingRequests.empty());}

  /** Push a new request to send */
  inline void	PushRequest(const std::string &r)	{_pendingRequests.push_back(r);}

  /** Read the data from the socket, and parse the incoming data */
  void Read()
  {
    int r = Nc::Network::SocketTcp::Read(_buffer.data, BUFF_SIZE);

    if (r <= 0)
      Close();
    else
      {
	_buffer.data[r] = '\0';
	LOG << _buffer << std::endl;
      }
  }

  /** Write data into the socket */
  void Write()
  {
    for (ListPendingRequest::iterator it = _pendingRequests.begin(); !_pendingRequests.empty(); it = _pendingRequests.erase(it))
      Nc::Network::SocketTcp::Write(reinterpret_cast<const unsigned char*>(it->c_str()), it->size());
  }


private:
  Nc::Math::Array<unsigned char, BUFF_SIZE + 1>	_buffer;		///< buffer for reading
  ListPendingRequest			_pendingRequests;	///< streams to send
};

typedef std::list<Client*>	ListClient;

#endif
