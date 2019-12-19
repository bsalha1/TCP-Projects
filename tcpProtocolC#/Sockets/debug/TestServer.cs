﻿using System;
using System.Net;

namespace Sockets
{
    class TestServer
    {
        static void Main(string[] args)
        {
            IPHostEntry entry = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress addr = entry.AddressList[0];
            new Server(addr, 8282);
        }
    }
}
