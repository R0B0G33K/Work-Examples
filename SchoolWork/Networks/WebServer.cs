using System;
using System.Text;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Networks
{
    class Request
    {
        public String method;
        public String path;
        public String version = "HTTP/1.1";
        Dictionary<String, String> headers = new Dictionary<String, String>();
        public String data;
        public String ending ="";

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
        public Request(String r)
        {
            String crlf = "\r\n";
            String crlf2 = "\r\n\r\n";
            String[] halves = r.Split(crlf2); //split up the input into lines
            String[] lines = halves[0].Split(crlf);
            String[] lines2 = halves[1].Split("\n");//split up the input into lines
            //Console.WriteLine("Halves # = "+ halves.Length);
            //Console.WriteLine("Liness # = " + lines.Length);
            String[] firstLineItems = lines[0].Split(null); //split first line
            //Console.WriteLine("first Line Items # = " + firstLineItems.Length);
            //Console.WriteLine(r);
            setMethod(firstLineItems[0]);
            setPath(firstLineItems[1]);
            version = firstLineItems[2];
            if (firstLineItems.Length > 3)
            {
                for (int j = 3; j < firstLineItems.Length; j++)
                {
                    firstLineItems[2] += " " + firstLineItems[j];
                }
            }

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

            String NewData = "";
            for (int i = 0; i < lines2.Length; i++)
            {
                if(lines2[i] != "Connection: Close")
                {
                    //Console.WriteLine("Line " + i + " of data: " + lines2[i] +" end");
                    NewData += lines2[i];
                }
            }
            //Console.WriteLine(NewData);
            setData(NewData);
            ending = "Connection: Close";
        }


        void setMethod(String method)
        {
            this.method = method;
            //set the method to GET or POST
        }

        public String getMethod()
        {
            return this.method;
        }

        void setPath(String path)
        {
            this.path = path;
            //set the path to the target request
        }

        public String getPath()
        {
            return this.path;
        }

        void setData(String data)
        {
            this.data = data;
            //add data for POST requests
        }

        public String getData()
        {
            return data;
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
            String output = methodLine + headerLines + "\r\n" + data + ending;
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

        public Response(Request request, String rootPath)
        {
            setVersion("HTTP/1.1");
            headers.Add("Server: ", "MattServer/2.0");
            headers.Add("Date: ", DateTime.Now.ToString(@"MM\/dd\/yyyy h\:mm tt"));
            try
            {
                if (request.getMethod() == "GET")
                {
                    if (request.getPath() == "/get" || request.getPath() == "index.html/get")
                    {
                        string[] readText = File.ReadAllLines(rootPath +"/index.html");
                        String contents = "";
                        foreach (string s in readText)
                        {
                            contents += s +"\r\n";
                        }
                        headers.Add("Content-Type: ", "text/html");
                        headers.Add("Content-Length: ", contents.Length.ToString());
                        data = contents + "\nConnection: Close\r\n\r\n";
                        setCode(200);
                        setMessage("OK");
                    }
                    else
                    {
                        String filepath = request.getPath();
                        filepath = filepath.Substring(0, filepath.Length - 4);
                        if (File.Exists(rootPath + filepath))
                        {
                            setCode(200);
                            setMessage("OK");
                            byte[] reqFile = File.ReadAllBytes(rootPath + filepath);
                            String reqFileString = reqFile.ToString();
                            String extension = Path.GetExtension(rootPath + filepath);
                            if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
                            {
                                switch (extension)
                                {
                                    case ".png":
                                        headers.Add("Content-Type: ", "text/png");
                                        break;
                                    case ".jpg":
                                        headers.Add("Content-Type: ", "text/jpg");
                                        break;
                                    case ".jpeg":
                                        headers.Add("Content-Type: ", "text/jpeg");
                                        break;
                                }
                            }
                            else if (extension == ".css")
                            {
                                headers.Add("Content-Type: ", "text/css");
                            }
                            else if (extension == ".html")
                            {
                                headers.Add("Content-Type: ", "text/html");
                            }
                            headers.Add("Content-Length: ", reqFileString.Length.ToString());
            
                        }
                        else
                        {
                            data = "\nConnection: Close\r\n\r\n";
                            setCode(404);
                            setMessage("INVALID PATH");
                        }
                    }
                }
                else if (request.getMethod() == "POST")
                {
                    addHeader("Content-Length: ", request.getData().Length.ToString());
                    addHeader("Content-Type: ", "text/plain");
                    data = "Data Sent: " + request.getData() + "\nConnection: Close\r\n\r\n";
                    setCode(200);
                    setMessage("OK");
                }
                else
                {
                    data = "\nConnection: Close\r\n\r\n";
                    setCode(400);
                    setMessage("BAD REQUEST");
                }
            }
            catch
            {
                data = "\nConnection: Close\r\n\r\n";
                setCode(500);
                setMessage("UNKNOWN ERROR");
            }
        }


        void setVersion(String version)
        {
            this.version = version;
            //set the path to the target request
        }

        void setMessage(String message)
        {
            this.message = message;
            //set the path to the target request
        }

        void setCode(int code)
        {
            this.code = code;
            //set the path to the target request
        }

        public void addHeader(String header, String value)
        {
            headers.Add(header, value);
            //add a header to the request
        }
        public String getHeader(String header)
        {

            //parse each string and add to headers hashmap

            return headers[header];
            //return value of specified header
            //find hash value of header in hashmap using header string

            //parse out content length header to know how much comes after the headers
            //first line, then all the headers, then blank and data
        }

        public String getData()
        {
            return data;
        }


        public override string ToString()
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
    class WebServer
    {
        public static int port = 8080;
        public static Socket listener;
        public static String rawData = null;
        public static String url = "http://localhost:"+port+ "/";
        public static int requestCount = 0;
        static String directPath = Directory.GetCurrentDirectory()+"/Website";


        public static Task<int> sendData(Socket socket, byte[] data)
        {
            return Task.Run(() =>
            {
                socket.Send(data);
                return 0;
            });
        }


        public static async Task HandleIncomingConnections()
        {
            bool runServer = true;

            // While a user hasn't visited the `shutdown` url, keep on handling requests
            while (runServer)
            {
                // Will wait here until we hear from a connection
                Console.WriteLine("Listening...");
                Socket handler = listener.Accept();

                String ip = ((IPEndPoint)(handler.RemoteEndPoint)).Address.ToString();
                byte[] bytes = new Byte[1024];

                int bytesRec = handler.Receive(bytes);

                rawData = Encoding.UTF8.GetString(bytes, 0, bytesRec);

                // Peel out the requests and response objects
                Request req = new Request(rawData);
                Response res = new Response(req, directPath);
                // Print out some info about the request
                Console.WriteLine();
                Console.WriteLine("Request #: {0}", ++requestCount);
                Console.WriteLine("Request IP: "+ ip);
                Console.WriteLine("Request made at: "+ DateTime.Now.ToString(@"MM\/dd\/yyyy h\:mm tt"));
                Console.WriteLine("Method: "+ req.getMethod());
                Console.WriteLine("Path: "+ req.getPath());
                if(req.getData() != "")
                {
                    Console.WriteLine("Data sent by requester: " + req.getData());
                }
                Console.WriteLine();

                if ((req.getMethod() == "POST") && (req.getData().ToLower() == "shutdown"))
                {
                    Console.WriteLine("Shutdown requested");
                    runServer = false;
                }


                // Write out to the response stream (asynchronously), then close it

                byte[] data = Encoding.UTF8.GetBytes(res.ToString());
                await sendData(handler, data);
                handler.Shutdown(SocketShutdown.Both);
                handler.Close();

            }
        }


        public static void Main(string[] args)
        {

            int backlog = 10;
            port = 8080;

            // bind the listening socket to the port
            IPHostEntry iPHost = Dns.GetHostEntry("localhost");
            IPAddress hostIP = iPHost.AddressList[0];
            IPEndPoint ep = new IPEndPoint(hostIP, port);
            listener = new Socket(hostIP.AddressFamily,
                         SocketType.Stream,
                         ProtocolType.Tcp);
            listener.Bind(ep);

            // start listening
            listener.Listen(backlog);
           
            // Handle requests
            Task listenTask = HandleIncomingConnections();
            listenTask.GetAwaiter().GetResult();

            // Close the listener
            listener.Close();
        }
    }
}
