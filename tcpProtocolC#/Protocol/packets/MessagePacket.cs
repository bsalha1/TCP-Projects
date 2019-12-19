using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Protocol
{
    public class MessagePacket : Packet<MessagePacket>
    {
        public string Author { get; set; }
        public string Message { get; set; }
        public byte[] Packet { get; }
        
        public MessagePacket(string author, string message)
        {
            this.Author = author;
            this.Message = message;
            this.Packet = Encoding.ASCII.GetBytes(author + "\n" + message);
        }

        public bool Equals(MessagePacket obj)
        {
            return obj.Author == Author && obj.Message == Message;
        }

        public string GetASCII()
        {
            return Encoding.ASCII.GetString(Packet);
        }

        public string GetUTF8()
        {
            return Encoding.UTF8.GetString(Packet);
        }

        public byte[] GetPacket()
        {
            return Packet;
        }

        public int GetPacketLength()
        {
            return Packet.Length;
        }

        public void SendPacket(IPEndPoint endpoint)
        {
            Socket socket = new Socket(endpoint.Address.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            socket.Connect(endpoint);
            socket.Send(Packet);
            socket.Shutdown(SocketShutdown.Both);
            socket.Close();
        }

        public string toString()
        {
            return "MessagePacket: " + Author + "::" + Message;
        }
    }
}
