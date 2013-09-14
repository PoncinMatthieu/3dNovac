
#include <Nc/Core/Network/Select.h>
#include <Nc/Core/Network/SocketTcp.h>

#define BUFF_SIZE	2048

using namespace Nc;

int main()
{
  unsigned char	        buf[BUFF_SIZE + 1];
  float			        timeout = 0;
  unsigned int		    port = 4242;
  Network::Ip		    ip("127.0.0.1");
  Network::SocketTcp	socket;
  Network::Select	    select;

  // connection
  socket.Create();
  if (!socket.Connect(ip, port))
    throw Utils::Exception("Can't connect to the given ip");

  // read on the socket until a deconnection
  while (socket.IsValid())
    {
      // clear the select and set the socket for reading accept
      // those things needs to be done at each loop before a Wait
      select.Clear();
      select.SetForReading(socket);

      // Wait for incoming data
      if (select.Wait(timeout) > 0)
	{
	  // if the socket is ready for read, then read
	  if (select.IsReadyForReading(socket))
	    {
	      int r = socket.Read(buf, BUFF_SIZE);

	      // disconnected ?
	      if (r <= 0)
		socket.Close();
	      else
		{
		  // print the incoming data
		  buf[r] = '\0';
		  LOG << buf << std::endl;
		}
	    }
	}
    }
  return 0;
}
