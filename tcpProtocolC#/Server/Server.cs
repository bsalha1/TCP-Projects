using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using Protocol;

namespace Server
{
    class Server
    {
        private IPAddress address;
        private int port;

        public Server(IPAddress address, int port)
        {
            this.address = address;
            this.port = port;
        }

        public void Start()
        {
            IPEndPoint endpoint = new IPEndPoint(address, port);
            Socket masterSocket = new Socket(address.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            try
            {
                masterSocket.Bind(endpoint);
                masterSocket.Listen(10);

                while(true)
                {
                    Socket clientSocket = masterSocket.Accept();
                    byte[] buffer = new byte[1024];
                    string data = null;

                    while(true)
                    {
                        int numByte = clientSocket.Receive(buffer);
                        data += Encoding.ASCII.GetString(buffer, 0, numByte);
                        if (data.IndexOf("<EOF>") > -1) break;
                    }

                    Console.WriteLine("Packet: \n" + data);
                    byte[] message = Encoding.ASCII.GetBytes("Hello client");

                    clientSocket.Send(message);

                    clientSocket.Shutdown(SocketShutdown.Both);
                    clientSocket.Close();
                }
            }
            catch(Exception e)
            {
                Console.WriteLine(e.StackTrace);
            }
        }

    }
}
