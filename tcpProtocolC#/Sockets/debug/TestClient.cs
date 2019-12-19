using System;
using System.Collections.Generic;
using System.Net;
using System.Text;

namespace Sockets
{
    class TestClient
    {
        public static void Main(string[] args)
        {
            IPHostEntry entry = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress addr = entry.AddressList[0];
            new Client(addr, 8282);
        }
    }
}
