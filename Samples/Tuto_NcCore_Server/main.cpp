
#include <Nc/Core/Network/Select.h>
#include "Client.h"

using namespace Nc;

void InitSelector(ListClient &clients, Network::Select &select, Network::SocketTcp &socketListen)
{
  select.Clear();
  select.SetForReading(socketListen);
  for (ListClient::iterator it = clients.begin(); it != clients.end(); ++it)
    {
      select.SetForReading(**it);
      if ((*it)->HasPendingRequest())
	select.SetForWriting(**it);
    }
}

void AcceptNewConnection(ListClient &clients, Network::SocketTcp &socketListen)
{
  Client *newClient = new Client();
  Network::Ip         newIp;

  if (!socketListen.Accept(*newClient, newIp) || !newClient->IsValid())
    {
      delete newClient;
      throw Utils::Exception("ServerMBI", "Problem in accept");
    }

  LOG << "New connection from: " << newIp.ToString() << std::endl;
  clients.push_back(newClient);

  // send welcom message
  newClient->PushRequest("Welcome");
}

void ProcessNetwork(ListClient &clients, Network::Select &select, Network::SocketTcp &socketListen, float timeout)
{
  // initialize the selector
  InitSelector(clients, select, socketListen);
  if (select.Wait(timeout) > 0)
    {
      // accept new connection
      if (select.IsReadyForReading(socketListen))
	AcceptNewConnection(clients, socketListen);
      // for each clients
      for (ListClient::iterator it = clients.begin(); it != clients.end();)
	{
	  // read incoming data
	  if (select.IsReadyForReading(**it))
	    (*it)->Read();
	  // write data
	  if ((*it)->IsValid() && select.IsReadyForWriting(**it))
	    (*it)->Write();
	  // delete disconnected clients
	  if (!(*it)->IsValid())
	    {
	      delete *it;
	      it = clients.erase(it);
	    }
	  else
	    ++it;
	}
    }
}

int main()
{
  float			timeout = 0.5;		// timeout used by the select
  unsigned int		listeningPort = 4242;	// the port used to accept new connection
  Network::Select	select;			// select used to communicate with the clients
  Network::SocketTcp	socketListen;		// the socket used to accept new connections
  ListClient		clients;		// a list of clients
  int			value = 0;
  Utils::Clock		clock;

  // create the listing socket
  socketListen.Create();
  socketListen.Listen(listeningPort);

  // infinit while
  while (1)
    {
      // exec the read/write methods with the select
      ProcessNetwork(clients, select, socketListen, timeout);

      // every seconds, send to the all clients a incremental value for exemple
      if (clock.ElapsedTime() >= 1)
	{
	  for (ListClient::iterator it = clients.begin(); it != clients.end(); ++it)
	    (*it)->PushRequest(Utils::Convert::ToString(value++));
	  clock.Reset();
	}
    }
  return 0;
}
