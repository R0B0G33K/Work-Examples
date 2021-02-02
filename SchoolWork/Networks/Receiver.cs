using System;
using System.Text;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Security.Cryptography;
using System.Threading;
using System.Threading.Tasks;

namespace Networks
{
    class rdt
    {
        HashAlgorithm hash = MD5.Create();
        String serverHostname;
        int serverPort;
        IPAddress ipAddress;
        UdpClient udpClient;
        int seqNum = 0;
        String checksum;
        IPEndPoint remoteIpEndPoint = new IPEndPoint(IPAddress.Any, 0);

        public rdt(String server, int port)
        {
            serverHostname = new String(server);
            IPHostEntry iPHost = Dns.GetHostEntry(serverHostname);
            ipAddress = iPHost.AddressList[0];
            serverPort = port;
        }

        public void rdt_send(String data)
        {
            byte[] packet = make_pkt(data, data);
            udp_send(packet);
            Console.WriteLine("Data sent...");
            var task = Task.Run(() => udp_recv());
            if (task.Wait(TimeSpan.FromMilliseconds(100)))
            {
                String ACK = task.Result;
                String[] Split = ACK.Split("|");
                if (Split[1] != checksum)
                {
                    //data corruption
                    Console.WriteLine("Data corrupted when sent, resending...");
                    rdt_send(data);
                }
                else if (Split[2] != seqNum.ToString())
                {
                    //sequence mismatch
                    Console.WriteLine("Receiver Mismatch Sequence, resending...");
                    rdt_send(data);
                }
                else
                {
                    Console.WriteLine(Split[0]);
                    if (seqNum == 0)
                    {
                        seqNum = 1;
                    }
                    else
                    {
                        seqNum = 0;
                    }
                    Console.WriteLine("Acknowledgment Recived, data sent successfully.");
                }
            }
            else
            {
                Console.WriteLine("Timeout, Data lost, resending...");
                rdt_send(data);
            }
        }

        public void rdt_recv()
        {
            String rawRecv = udp_recv();
            String[] Split = rawRecv.Split("|");
            if (Split[2] != seqNum.ToString())
            {
                //out of order sequence number
                Console.WriteLine("(DUPLICATE)");
                if (seqNum == 0)
                {
                    seqNum = 1;
                }
                else
                {
                    seqNum = 0;
                }
            }
            Console.WriteLine("Recived data from sender");
            Console.WriteLine("Data is: " + Split[0]);
            String ACK = "Recived: " + Split[0];
            byte[] packet = make_pkt(ACK, Split[0]);
            udp_send(packet);
            Console.WriteLine("Sent Acknowledgement.");
            if (seqNum == 0)
            {
                seqNum = 1;
            }
            else
            {
                seqNum = 0;
            }
        }

        private String hashMaker(String tobeHashed)
        {
            byte[] inputBytes = System.Text.Encoding.ASCII.GetBytes(tobeHashed);
            byte[] hashBytes = hash.ComputeHash(inputBytes);
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < hashBytes.Length; i++)
            {
                sb.Append(hashBytes[i].ToString("X2"));
            }
            return sb.ToString();
        }

        private byte[] make_pkt(String data, String checksumData)
        {
            checksum = hashMaker(checksumData);
            String fullData = data + "|" + checksum + "|" + seqNum.ToString();
            byte[] annotherOneBytesTheDust = Encoding.ASCII.GetBytes(fullData);
            return annotherOneBytesTheDust;
        }

        private String udp_recv()
        {
            udpClient = new UdpClient(serverPort);
            //bloc untill recive
            Byte[] receiveBytes = udpClient.Receive(ref remoteIpEndPoint);
            String returnData = Encoding.ASCII.GetString(receiveBytes);
            udpClient.Close();
            return returnData;
        }

        private void udp_send(byte[] packet)
        {
            udpClient = new UdpClient();
            udpClient.EnableBroadcast = true;
            udpClient.Send(packet, packet.Length, new IPEndPoint(IPAddress.Broadcast, serverPort));
            udpClient.Close();
        }
    }


    class Reciver
    {
        static void Main(string[] args)
        {
            rdt r = new rdt("127.0.0.1", 8080);
            while (true)
            {
                r.rdt_recv();
            }
        }
    }
}
