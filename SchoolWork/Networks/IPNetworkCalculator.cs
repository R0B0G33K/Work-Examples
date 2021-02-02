using System;

namespace Networks
{ 
    class ipaddr
    {
        int address;
        int netmask;

        public ipaddr(int addr) { }

        public ipaddr(String cidr)
        {
            String[] parts = cidr.Split("/");
            this.setMask(Int32.Parse(parts[1]));
            this.setAddress(parts[0]);
        }

        public int getAddress()
        {
            return address;
        }

        public int getNetMask()
        {
            return netmask;
        }

        public void printNetInfo()
        {
            int netaddr = this.address & this.netmask;
            Console.WriteLine("Network address: " + ipToString(netaddr));
            int bcast = netaddr + ~this.netmask;
            Console.WriteLine("Broadcast address: " + ipToString(bcast));
            Console.WriteLine("Netmask: " + ipToString(this.netmask));
            int usable = bcast - netaddr - 2;
            Console.WriteLine();
            Console.WriteLine("Would you like to list the " + usable +" remaining addresses? (Y or N)");
            while (true)
            {
                String input = Console.ReadLine();
                if (input.ToUpper() == "Y" || input.ToUpper() == "YES")
                {
                    listAddresses(netaddr, bcast);
                    break;
                }
                else if (input.ToUpper() == "N" || input.ToUpper() == "NO")
                {
                    break;
                }
                else
                {
                    Console.WriteLine("ERROR understanding request");
                }
            }
        }

        private static String ipToString(int ip)
        {
            String sip = "";
            for (int i = 3; i >= 0; i--)
            {
                int octet = (ip >> (i * 8)) & 0xff;
                sip += octet + ".";
            }
            String removeLastChar = sip.Remove(sip.Length - 1, 1);
            return removeLastChar;
        }

        private void listAddresses(int netaddr, int bcast)
        {
            for(int i = netaddr+1; i < bcast; i++)
            {
                Console.WriteLine(ipToString(i));
            }
        }

        private void setAddress(String ip)
        {
            String[] octets = ip.Split(".");
            for(int i = 0; i<4; i++)
            {
                int o = Int32.Parse(octets[i]);
                this.address = this.address << 8;
                this.address += o;
            }
        }

        private void setMask(int nm)
        {
            for (int i = 0; i < nm; i++)
            {
                this.netmask = this.netmask << 1;
                this.netmask = this.netmask + 1;
            }
            this.netmask = this.netmask << (32 - nm);
            this.printHex(this.netmask);
        }


        private void printHex(int i)
        {
            Console.WriteLine(String.Format("0x{0:X}", i));
        }

    }
  
    class IPNetworkCalculator
    {

        static void Main(string[] args)
        {
            Console.WriteLine("Enter a cidr address: ");
            String input = Console.ReadLine();
            ipaddr test = new ipaddr(input);
            Console.WriteLine();
            test.printNetInfo();
        }
    }
}
