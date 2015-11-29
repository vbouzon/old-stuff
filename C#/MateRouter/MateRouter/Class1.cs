using System;
using System.Collections;
using System.Net.Sockets;
using System.Net;
using System.Text;

namespace MateRouter
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class Class1
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			Random rnd = new Random();
			DATABASE db = new DATABASE();
			Socket serv;
			serv = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			IPHostEntry host_local = Dns.Resolve(Dns.GetHostName());
			IPAddress ip_local = host_local.AddressList[0];
			IPEndPoint iep_local = new IPEndPoint(ip_local, 10001);
			serv.Bind(iep_local);
			serv.Listen(20);
			while (true)
			{
				try
				{
					byte[] RecvByte = new byte[2048];
					Int32 nb_RecvByte;
					Socket s_demandeur = serv.Accept();
					Console.WriteLine("------------------------------------Liste---------------------------------------");
					foreach (object o in db.ips) Console.WriteLine((string)o);
					Console.WriteLine("--------------------------------------------------------------------------------");
					Console.Write("[+]Une personne ");
					nb_RecvByte = s_demandeur.Receive(RecvByte, 0, 2048, SocketFlags.None);
					string demande = System.Text.Encoding.ASCII.GetString(RecvByte, 0, nb_RecvByte);
					string[] demande_parse = demande.Split(new Char[] { ' ', '\r', '\n' });
					string commande = demande_parse[0];
					switch (commande)
					{
						case "HELLO":
							Console.Write("qui veut que je l'ajoute dans ma liste \n");
							if (db.Add(((IPEndPoint)s_demandeur.RemoteEndPoint).Address.ToString()))
							{
								s_demandeur.Send(Encoding.ASCII.GetBytes("OK\r\n"));
							}
							else s_demandeur.Send(Encoding.ASCII.GetBytes("NO\r\n"));
							s_demandeur.Close();
							break;

						case "CHAT":
							if (db.ips.Contains(((IPEndPoint)s_demandeur.RemoteEndPoint).Address.ToString()))
							{
								int n;
								n = rnd.Next(db.ips.Count - 1);
								s_demandeur.Send(Encoding.ASCII.GetBytes((string)db.ips[n]));
							}
							else s_demandeur.Send(Encoding.ASCII.GetBytes("NO\r\n"));
							s_demandeur.Close();
							break;

						case "QUIT":
							Console.Write("qui veut que je le supprime dans ma liste \n");
							if (db.Delete(((IPEndPoint)s_demandeur.RemoteEndPoint).Address.ToString()))
							{
								s_demandeur.Send(Encoding.ASCII.GetBytes("OK\r\n"));
							}
							else s_demandeur.Send(Encoding.ASCII.GetBytes("NO\r\n"));
							s_demandeur.Close();
							break;

						default:
							Console.Write("\n");
							s_demandeur.Close();
							break;
					}
				}
				catch (Exception e)
				{
					Console.WriteLine("\n------------------------------------EXCEPTION GEREE-----------------------------" + e + "\n--------------------------------------------------------------------------------");
				}
			}

		}
	}
	class DATABASE
	{
		public ArrayList ips;
		public ArrayList state;
		public DATABASE()
		{
			ips = new ArrayList();
			state = new ArrayList();
		}

		public bool Add(string ip)
		{
			//if (!(ips.Contains(ip)))
			//{
			ips.Add(ip);
			state.Add(true);
			return true;
			//}
			//return false;
		}
		public void Change(int n)
		{
			if ((bool)state[n]) state[0] = false;
			else state[n] = true;
		}
		public bool Delete(string ip)
		{
			if (ips.Contains(ip))
			{
				int n = ips.BinarySearch(ip);
				ips.Remove(ip);
				state.RemoveAt(n);
				return true;
			}
			return false;
		}

	}
}
