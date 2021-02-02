using System;
using System.Text;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;

namespace Networks
{
    class Request
    {
        public String method;
        public String path;
        public String version = "HTTP/1.1";
        Dictionary<String, String> headers = new Dictionary<String, String>();
        String data;

        /*
        *    GET /get HTTP/1.1
        *    Host: httpbin.org
        *    Accept: \*\/*
        *    User-Agent: myprogram
        */

        public Request(String m, String p, String d, String h)
        {
            //Console.WriteLine(p);
            setMethod(m);
            setPath(p);
            setData(d);
            addHeader("Host: ", h);
            addHeader("Accept: ", "*/*");
            addHeader("User-Agent: ", "myprogram");
            if (data != null)
            {
                addHeader("Content-Length: ", data.Length.ToString());
                addHeader("Content-Type: ", "text/plain");
            }
        }

        void setMethod(String method)
        {
            this.method = method;
            //set the method to GET or POST
        }

        void setPath(String path)
        {
            this.path = path;
            //set the path to the target request
        }

        void setData(String data)
        {
            this.data = data;
            //add data for POST requests
        }

        void addHeader(String header, String value)
        {
            headers.Add(header, value);
            //add a header to the request
        }

        public override string ToString()
        {
            String methodLine = method + " " + path + " " + version + "\r\n";
            //Console.WriteLine(methodLine);
            String headerLines = "";
            foreach(KeyValuePair<String, String> kvp in headers)
            {
                headerLines += kvp.Key;
                headerLines += kvp.Value;
                headerLines += "\r\n";
            }
            String output = methodLine + headerLines + "\r\n" + data;
            return output;
            // print request formatted like seen in class
        }
        
    }
    class Response
    {
        public String version;
        public int code;
        public String message;
        Dictionary<String, String> headers = new Dictionary<String, String>();
        String data;

        public Response(String r)
        {
            String crlf = "\r\n";
            String crlf2 = "\r\n\r\n";
            String[] halves = r.Split(crlf2); //split up the input into lines
            String[] lines = halves[0].Split(crlf); //split up the input into lines
            //Console.WriteLine("Halves # = "+ halves.Length);
            //Console.WriteLine("Liness # = " + lines.Length);
            String[] firstLineItems = lines[0].Split(null); //split first line
            //Console.WriteLine("first Line Items # = " + firstLineItems.Length);
            //Console.WriteLine(r);
            version = firstLineItems[0];
            code = Int32.Parse(firstLineItems[1]);
            if (firstLineItems.Length > 3)
            {
                for (int j = 3; j < firstLineItems.Length; j++)
                {
                    firstLineItems[2] += " " + firstLineItems[j];
                }
            }
            message = firstLineItems[2];

            for (int i = 1; i < lines.Length; i++)
            {
                String[] tokens = lines[i].Split(null); //split up each line into header and headerdata
                if (tokens.Length > 2)
                {
                    for (int j = 2; j < tokens.Length; j++)
                    {
                        tokens[1] += " " + tokens[j];
                    }
                }
                headers.Add(tokens[0], tokens[1]);
            }
            data = halves[1];
        }

        String getHeader(String header)
        {

            //parse each string and add to headers hashmap

            return headers[header];
            //return value of specified header
            //find hash value of header in hashmap using header string

            //parse out content length header to know how much comes after the headers
            //first line, then all the headers, then blank and data
        }

        String getData(String d)
        {
            return data;
        }

        public String toString()
        {
            String output = version + " " + code + " " + message + "\r\n";
            foreach (KeyValuePair<String, String> kvp in headers)
            {
                output += kvp.Key + "" + kvp.Value + "\r\n";
            }
            output += "\r\n";
            output += data;

            return output;
            //print response as seen in class
        }
    }
    class HTTPClient
    {
        private static Socket ConnectSocket(string server, int port)
        {
            Socket s = null;
            IPHostEntry hostEntry = null;

            // Get host related information.
            hostEntry = Dns.GetHostEntry(server);

            // Loop through the AddressList to obtain the supported AddressFamily. This is to avoid
            // an exception that occurs when the host IP Address is not compatible with the address family
            // (typical in the IPv6 case).
            foreach (IPAddress address in hostEntry.AddressList)
            {
                IPEndPoint ipe = new IPEndPoint(address, port);
                Socket tempSocket =
                    new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

                tempSocket.Connect(ipe);

                if (tempSocket.Connected)
                {
                    s = tempSocket;
                    break;
                }
                else
                {
                    continue;
                }
            }
            return s;
        }

        static void Main(string[] args)
        {
            
            String serverAddress = null;
            String method = "";
            String path = "";
            String data = null;
            int port = 80;


            Console.WriteLine("Please enter the web address: ");
            try
            {
                String input = Console.ReadLine();
                String div = "/";
                String[] parts = input.Split(div);
                serverAddress = parts[0];
                if (parts.Length > 1)
                {
                    for (int i = 1; i < parts.Length; i++)
                    {
                        path += "/" + parts[i];
                    }
                }
                //Console.WriteLine(path);
            }
            catch
            {
                Console.WriteLine("IO error trying to read web address!");
                return;
            }

            Console.WriteLine("Please enter the port: ");
            try
            {
                port = Int32.Parse(Console.ReadLine());
      
            }
            catch
            {
                Console.WriteLine("IO error trying to read port!");
                return;
            }

            Console.WriteLine("GET or POST?");
            try
            {
                method = Console.ReadLine();
            }
            catch
            {
                Console.WriteLine("IO error trying to read server IP address!");
                return;
            }

            if (method.ToLower().Equals("post"))
            {
                Console.WriteLine("Enter data to send: ");
                try
                {
                    data = Console.ReadLine();
                }
                catch
                {
                    Console.WriteLine("IO error trying to read server IP address!");
                    return;
                }
            }

            Request req = new Request(method.ToUpper(), path + "/" + method.ToLower(), data, serverAddress);

            String package = req.ToString() + "\nConnection: Close\r\n\r\n";
            Console.WriteLine(package);
            Byte[] toSendOut = Encoding.ASCII.GetBytes(package);
            Byte[] bytesRecieved = new Byte[256];
            String answer = "";

            using (Socket client = ConnectSocket(serverAddress, port))
            {
                if(client == null)
                {
                    Console.WriteLine("Connection failed");
                    return;
                }

                client.Send(toSendOut, toSendOut.Length, 0);

                //sent da
               // Console.WriteLine("Recieving Message");
                int bytes = 0;
                do
                {
                    bytes = client.Receive(bytesRecieved, bytesRecieved.Length, 0);
                    answer = answer + Encoding.ASCII.GetString(bytesRecieved, 0, bytes);
                }
                while (bytes > 0);
            }
           // Console.WriteLine("Message Recieved");
           // Console.WriteLine(answer);
            Response res = new Response(answer);
            Console.WriteLine(res.toString());
        }
    }
}
