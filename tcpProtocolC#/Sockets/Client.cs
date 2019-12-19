using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Sockets
{
    class Client
    {
        private IPAddress address;
        private int port;

        public Client(IPAddress address, int port)
        {
            this.address = address;
            this.port = port;
        }

        public void Start()
        {
            try
            {
                IPEndPoint endpoint = new IPEndPoint(address, port);
                Socket socket = new Socket(address.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

                socket.Connect(endpoint);

                Console.WriteLine("Connected to " + socket.RemoteEndPoint.ToString());

                byte[] message = Encoding.ASCII.GetBytes("Test Client<EOF>");
                int bytesSent = socket.Send(message);

                byte[] messageReceived = new byte[1024];

                int byteRecv = socket.Receive(messageReceived);
                Console.WriteLine("Message from server: " + Encoding.ASCII.GetString(messageReceived, 0, byteRecv));

                socket.Shutdown(SocketShutdown.Both);
                socket.Close();
            }
            catch(Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }
    }
}
