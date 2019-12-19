using System;
using System.Net;
using System.Text;

namespace Protocol
{
    interface Packet<T>
    {
        public string GetASCII();

        public string GetUTF8();

        public byte[] GetPacket();

        public int GetPacketLength();

        public void SendPacket(IPEndPoint endpoint);


        public bool Equals(T obj);

        public string toString();
    }
}
