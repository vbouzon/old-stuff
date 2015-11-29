using System; 
using System.Drawing; 
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms; 
using System.Data; 
using System.Net.Sockets; 
using System.Text; 
using System.IO; 
using System.Net; 
using System.Threading; 


namespace P2p
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button listen_demande;
		private System.Windows.Forms.GroupBox groupBox_demande;
		private System.Windows.Forms.TextBox textBox_port_demande;
		private System.Windows.Forms.Label label_port_demande;
		private System.Windows.Forms.RichTextBox consol_demande;
		private System.Windows.Forms.Label label_console_demande;
		private System.Windows.Forms.GroupBox groupBox_pere;
		private System.Windows.Forms.Label label_port_pere;
		private System.Windows.Forms.TextBox textBox_port_pere;
		private System.Windows.Forms.RichTextBox consol_pere;
		private System.Windows.Forms.Label label_console_pere;
		private System.Windows.Forms.GroupBox groupBox_adoption;
		private System.Windows.Forms.Button connect_adoption;
		private System.Windows.Forms.Label label_console_adoption;
		private System.Windows.Forms.TextBox textBox_port_adoption;
		bool deja_fils = false;
		Socket s_fils;
		Socket s_pere;
		ArrayList liste = new ArrayList();
		private System.Windows.Forms.TextBox textBox_ip_adoption;
		private System.Windows.Forms.RichTextBox consol_adoption;
		private System.Windows.Forms.Label label_adoption;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.listen_demande = new System.Windows.Forms.Button();
			this.groupBox_demande = new System.Windows.Forms.GroupBox();
			this.label_port_demande = new System.Windows.Forms.Label();
			this.textBox_port_demande = new System.Windows.Forms.TextBox();
			this.consol_demande = new System.Windows.Forms.RichTextBox();
			this.label_console_demande = new System.Windows.Forms.Label();
			this.groupBox_pere = new System.Windows.Forms.GroupBox();
			this.label_port_pere = new System.Windows.Forms.Label();
			this.textBox_port_pere = new System.Windows.Forms.TextBox();
			this.consol_pere = new System.Windows.Forms.RichTextBox();
			this.label_console_pere = new System.Windows.Forms.Label();
			this.groupBox_adoption = new System.Windows.Forms.GroupBox();
			this.textBox_port_adoption = new System.Windows.Forms.TextBox();
			this.label_adoption = new System.Windows.Forms.Label();
			this.connect_adoption = new System.Windows.Forms.Button();
			this.textBox_ip_adoption = new System.Windows.Forms.TextBox();
			this.consol_adoption = new System.Windows.Forms.RichTextBox();
			this.label_console_adoption = new System.Windows.Forms.Label();
			this.groupBox_demande.SuspendLayout();
			this.groupBox_pere.SuspendLayout();
			this.groupBox_adoption.SuspendLayout();
			this.SuspendLayout();
			// 
			// listen_demande
			// 
			this.listen_demande.Location = new System.Drawing.Point(8, 328);
			this.listen_demande.Name = "listen_demande";
			this.listen_demande.Size = new System.Drawing.Size(192, 24);
			this.listen_demande.TabIndex = 5;
			this.listen_demande.Text = "Lancez serveur demande !";
			this.listen_demande.Click += new System.EventHandler(this.listen_demande_Click);
			// 
			// groupBox_demande
			// 
			this.groupBox_demande.Controls.Add(this.label_port_demande);
			this.groupBox_demande.Controls.Add(this.listen_demande);
			this.groupBox_demande.Controls.Add(this.textBox_port_demande);
			this.groupBox_demande.Controls.Add(this.consol_demande);
			this.groupBox_demande.Controls.Add(this.label_console_demande);
			this.groupBox_demande.Location = new System.Drawing.Point(8, 0);
			this.groupBox_demande.Name = "groupBox_demande";
			this.groupBox_demande.Size = new System.Drawing.Size(208, 360);
			this.groupBox_demande.TabIndex = 1;
			this.groupBox_demande.TabStop = false;
			this.groupBox_demande.Text = "Serveur des demandes :";
			// 
			// label_port_demande
			// 
			this.label_port_demande.Location = new System.Drawing.Point(8, 16);
			this.label_port_demande.Name = "label_port_demande";
			this.label_port_demande.Size = new System.Drawing.Size(176, 16);
			this.label_port_demande.TabIndex = 3;
			this.label_port_demande.Text = "Port pour serveur dermande:";
			// 
			// textBox_port_demande
			// 
			this.textBox_port_demande.Location = new System.Drawing.Point(8, 32);
			this.textBox_port_demande.Name = "textBox_port_demande";
			this.textBox_port_demande.TabIndex = 4;
			this.textBox_port_demande.Text = "";
			// 
			// consol_demande
			// 
			this.consol_demande.Location = new System.Drawing.Point(8, 72);
			this.consol_demande.Name = "consol_demande";
			this.consol_demande.ReadOnly = true;
			this.consol_demande.Size = new System.Drawing.Size(192, 248);
			this.consol_demande.TabIndex = 6;
			this.consol_demande.TabStop = false;
			this.consol_demande.Text = "";
			// 
			// label_console_demande
			// 
			this.label_console_demande.Location = new System.Drawing.Point(8, 56);
			this.label_console_demande.Name = "label_console_demande";
			this.label_console_demande.Size = new System.Drawing.Size(128, 16);
			this.label_console_demande.TabIndex = 2;
			this.label_console_demande.Text = "Console des demandes:";
			// 
			// groupBox_pere
			// 
			this.groupBox_pere.Controls.Add(this.label_port_pere);
			this.groupBox_pere.Controls.Add(this.textBox_port_pere);
			this.groupBox_pere.Controls.Add(this.consol_pere);
			this.groupBox_pere.Controls.Add(this.label_console_pere);
			this.groupBox_pere.ForeColor = System.Drawing.SystemColors.ControlText;
			this.groupBox_pere.Location = new System.Drawing.Point(224, 0);
			this.groupBox_pere.Name = "groupBox_pere";
			this.groupBox_pere.Size = new System.Drawing.Size(208, 360);
			this.groupBox_pere.TabIndex = 2;
			this.groupBox_pere.TabStop = false;
			this.groupBox_pere.Text = "Pere :";
			// 
			// label_port_pere
			// 
			this.label_port_pere.Location = new System.Drawing.Point(8, 16);
			this.label_port_pere.Name = "label_port_pere";
			this.label_port_pere.Size = new System.Drawing.Size(176, 16);
			this.label_port_pere.TabIndex = 3;
			this.label_port_pere.Text = "Port pour le pere:";
			// 
			// textBox_port_pere
			// 
			this.textBox_port_pere.Location = new System.Drawing.Point(8, 32);
			this.textBox_port_pere.Name = "textBox_port_pere";
			this.textBox_port_pere.TabIndex = 2;
			this.textBox_port_pere.Text = "";
			// 
			// consol_pere
			// 
			this.consol_pere.Location = new System.Drawing.Point(8, 72);
			this.consol_pere.Name = "consol_pere";
			this.consol_pere.ReadOnly = true;
			this.consol_pere.Size = new System.Drawing.Size(192, 280);
			this.consol_pere.TabIndex = 7;
			this.consol_pere.TabStop = false;
			this.consol_pere.Text = "";
			// 
			// label_console_pere
			// 
			this.label_console_pere.Location = new System.Drawing.Point(8, 56);
			this.label_console_pere.Name = "label_console_pere";
			this.label_console_pere.Size = new System.Drawing.Size(152, 16);
			this.label_console_pere.TabIndex = 2;
			this.label_console_pere.Text = "Console liaison pere <-> fils:";
			// 
			// groupBox_adoption
			// 
			this.groupBox_adoption.Controls.Add(this.textBox_port_adoption);
			this.groupBox_adoption.Controls.Add(this.label_adoption);
			this.groupBox_adoption.Controls.Add(this.connect_adoption);
			this.groupBox_adoption.Controls.Add(this.textBox_ip_adoption);
			this.groupBox_adoption.Controls.Add(this.consol_adoption);
			this.groupBox_adoption.Controls.Add(this.label_console_adoption);
			this.groupBox_adoption.Location = new System.Drawing.Point(440, 0);
			this.groupBox_adoption.Name = "groupBox_adoption";
			this.groupBox_adoption.Size = new System.Drawing.Size(208, 360);
			this.groupBox_adoption.TabIndex = 4;
			this.groupBox_adoption.TabStop = false;
			this.groupBox_adoption.Text = "Client d\'adoption:";
			// 
			// textBox_port_adoption
			// 
			this.textBox_port_adoption.Location = new System.Drawing.Point(120, 32);
			this.textBox_port_adoption.Name = "textBox_port_adoption";
			this.textBox_port_adoption.Size = new System.Drawing.Size(80, 20);
			this.textBox_port_adoption.TabIndex = 1;
			this.textBox_port_adoption.Text = "";
			// 
			// label_adoption
			// 
			this.label_adoption.Location = new System.Drawing.Point(8, 16);
			this.label_adoption.Name = "label_adoption";
			this.label_adoption.Size = new System.Drawing.Size(152, 16);
			this.label_adoption.TabIndex = 3;
			this.label_adoption.Text = "IP:Port pour accueil du pere:";
			// 
			// connect_adoption
			// 
			this.connect_adoption.Location = new System.Drawing.Point(8, 328);
			this.connect_adoption.Name = "connect_adoption";
			this.connect_adoption.Size = new System.Drawing.Size(192, 24);
			this.connect_adoption.TabIndex = 3;
			this.connect_adoption.Text = "Allez a une acceuil d\'adoption !";
			this.connect_adoption.Click += new System.EventHandler(this.connect_adoption_Click);
			// 
			// textBox_ip_adoption
			// 
			this.textBox_ip_adoption.Location = new System.Drawing.Point(8, 32);
			this.textBox_ip_adoption.Name = "textBox_ip_adoption";
			this.textBox_ip_adoption.TabIndex = 0;
			this.textBox_ip_adoption.Text = "";
			// 
			// consol_adoption
			// 
			this.consol_adoption.Location = new System.Drawing.Point(8, 72);
			this.consol_adoption.Name = "consol_adoption";
			this.consol_adoption.ReadOnly = true;
			this.consol_adoption.Size = new System.Drawing.Size(192, 248);
			this.consol_adoption.TabIndex = 8;
			this.consol_adoption.TabStop = false;
			this.consol_adoption.Text = "";
			// 
			// label_console_adoption
			// 
			this.label_console_adoption.Location = new System.Drawing.Point(8, 56);
			this.label_console_adoption.Name = "label_console_adoption";
			this.label_console_adoption.Size = new System.Drawing.Size(128, 16);
			this.label_console_adoption.TabIndex = 2;
			this.label_console_adoption.Text = "Console de l\'adoption:";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(656, 366);
			this.Controls.Add(this.groupBox_pere);
			this.Controls.Add(this.groupBox_demande);
			this.Controls.Add(this.groupBox_adoption);
			this.Name = "Form1";
			this.Text = "EnjoyP2p";
			this.groupBox_demande.ResumeLayout(false);
			this.groupBox_pere.ResumeLayout(false);
			this.groupBox_adoption.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}


		#region Serveur demande
		private void listen_demande_Click(object sender, System.EventArgs e)
		{
			Thread th_sv_demande = new Thread(new ThreadStart(sv_demande));
			th_sv_demande.Start();
			listen_demande.Enabled = false;
		}

		public void sv_demande()
		{
			byte[] RecvByte = new byte[2048];
			Int32 nb_RecvByte;
			Socket sv_demande = new Socket(AddressFamily.InterNetwork,SocketType.Stream,ProtocolType.Tcp);
			IPHostEntry host_local = Dns.Resolve(Dns.GetHostName());
			IPAddress ip_local = host_local.AddressList[0];
			IPEndPoint iep_local = new IPEndPoint(ip_local,Convert.ToInt32(textBox_port_demande.Text));
			sv_demande.Bind(iep_local);
			sv_demande.Listen(5);
			while(true)
			{
				Socket s_demandeur = sv_demande.Accept();
				consol_demande.Text += "[+]Une personne ";
				nb_RecvByte = s_demandeur.Receive(RecvByte,0,2048,SocketFlags.None);
				string demande = Encoding.ASCII.GetString(RecvByte,0,nb_RecvByte);
				string[] demande_parse = demande.Split(new Char[] {' ','\r','\n'});
				string commande = demande_parse[0];
				switch (commande)
				{
					case "HELO":
						consol_demande.Text += "qui veut que je l'adopte.\r\n";
						if (deja_fils == true)
						{
							s_demandeur.Send(Encoding.ASCII.GetBytes("SRY " + liste[liste.Count - 1] + "\r\n"));
							s_demandeur.Close();
							consol_demande.Text += "Non,j'ai déjà un fils.\r\n";
						}
						else
						{
							s_demandeur.Send(Encoding.ASCII.GetBytes("OK\r\n"));
							consol_demande.Text += "Je veux bien (Port:" + Convert.ToInt32(demande_parse[1]) + ")\r\n";
							bool connec= connect_to_fils(IPAddress.Parse(((IPEndPoint)s_demandeur.RemoteEndPoint).Address.ToString()),Convert.ToInt32(demande_parse[1]));
							if(connec)
							{
								
								s_demandeur.Send(Encoding.ASCII.GetBytes("GOOD\r\n"));
								consol_demande.Text += "Et c'est fait !\r\n";
								
								deja_fils = true;
								connect_adoption.Enabled = false;
								
								liste.Add(IPAddress.Parse(((IPEndPoint)s_demandeur.RemoteEndPoint).Address.ToString()));
								Thread rcv_fils = new Thread(new ThreadStart(receive_to_fils));
								rcv_fils.Start();
								s_demandeur.Close();
								if (s_pere != null)
								{
									if (s_pere.Connected)
									{
										try
										{
											s_pere.Send(Encoding.ASCII.GetBytes("ADD " + IPAddress.Parse(((IPEndPoint)s_demandeur.RemoteEndPoint).Address.ToString()) + "\r\n"));
										}
										catch(InvalidCastException e) 
										{
											consol_pere.Text += e;
										}
									}
								}

							}
							else
							{
								consol_demande.Text += "Mais ces ports ne sont pas ouverts !\r\n";
								s_demandeur.Send(Encoding.ASCII.GetBytes("BAD\r\n"));
								s_demandeur.Close();
							}
						}
						break;
				}
			}
			
}
		#endregion

		#region Gestion Fils
		public bool connect_to_fils(IPAddress ip_fils,int port_fils)
		{
			s_fils = new Socket(AddressFamily.InterNetwork,SocketType.Stream,ProtocolType.Tcp);
			IPEndPoint iep_fils = new IPEndPoint(ip_fils,port_fils);
			try {s_fils.Connect(iep_fils);} catch (Exception e){}
			if (s_fils.Connected)return true;
			else s_fils.Close();
			return false;
		}

		public void receive_to_fils()
		{
			byte[] RecvByte = new byte[2048];
			Int32 nb_RecvByte;
			consol_pere.Text += "En attente de message du fils ...\r\n";
			while ((nb_RecvByte = s_fils.Receive(RecvByte,0,2048,SocketFlags.None)) != 0 && s_fils.Connected)
			{
				string info = Encoding.ASCII.GetString(RecvByte,0,nb_RecvByte);
				consol_pere.Text += info;
                if (s_pere.Connected)s_pere.Send(RecvByte,0,nb_RecvByte,SocketFlags.None);
			}
			consol_pere.Text += "Fils perdu !\r\n";
			//Routine d auto reconnection sur le petit fils
		}
		#endregion

		#region Adoption
		private void connect_adoption_Click(object sender, System.EventArgs e)
		{
			Thread adoption_form = new Thread(new ThreadStart(adoption_with_form));
			adoption_form.Start();
		}			
		private void adoption_with_form()
		{
			adoption_to_demande(textBox_ip_adoption.Text,Convert.ToInt32(textBox_port_adoption.Text));
		}
		public void adoption_to_demande(string ip_demande,int port_demande)
		{
			byte[] RecvByte = new byte[2048];
			Int32 nb_RecvByte;
			Socket s_adoption = new Socket(AddressFamily.InterNetwork,SocketType.Stream,ProtocolType.Tcp);
			IPHostEntry host_adoption = Dns.Resolve(ip_demande);
			IPAddress ip_adoption = host_adoption.AddressList[0];
			IPEndPoint iep_adoption = new IPEndPoint(ip_adoption,port_demande);
			s_adoption.Connect(iep_adoption);
			consol_adoption.Text += "Tentative de connexion .....\r\n";
			
			if (s_adoption.Connected)
			{
				Thread th_accept_pere = new	Thread(new ThreadStart(accept_for_pere));
				th_accept_pere.Start();

				consol_adoption.Text += "Connexion établie....\r\nDemande d'adoption...\r\n";
				s_adoption.Send(Encoding.ASCII.GetBytes("HELO " + textBox_port_pere.Text + "\r\n"));
				nb_RecvByte= s_adoption.Receive(RecvByte,0,2048,SocketFlags.None);
				string reponse = Encoding.ASCII.GetString(RecvByte,0,nb_RecvByte);
				string[] reponse_parse = reponse.Split(new Char[] {' ','\r','\n'});
				string verdict = reponse_parse[0];
				switch(verdict)
				{
					case "OK":
						consol_adoption.Text += "Il veut bien\r\nAccepte mon pere...\r\n";
						nb_RecvByte= s_adoption.Receive(RecvByte,0,2048,SocketFlags.None);
						string result = Encoding.ASCII.GetString(RecvByte,0,nb_RecvByte);
						string[] result_parse = result.Split(new Char[] {' ','\r','\n'});
						if (result_parse[0] == "GOOD")
						{
							consol_adoption.Text += "PERE ACCEPTEE !!!!!!\r\n";
							s_adoption.Close();
						}
						else
						{
							consol_adoption.Text += "Forward tes ports !!!!!!\r\n";
							th_accept_pere.Abort();
							s_adoption.Close();
						}
					break;
					case "SRY":
						th_accept_pere.Abort();
						consol_adoption.Text += "Il a déjà un fils.\r\nRedirectionne vers " + reponse_parse[1];
						s_adoption.Close();
						adoption_to_demande(reponse_parse[1],56);//56 c est au hasard apres le port du serveur de demande sera fixe
					break;
					default:
						s_adoption.Close();
					break;
				}
			}
		}
		#endregion
		
		#region Gestion pere
		public void accept_for_pere()
		{
			Socket s_accept = new Socket(AddressFamily.InterNetwork,SocketType.Stream,ProtocolType.Tcp);
			IPHostEntry host_local = Dns.Resolve(Dns.GetHostName());
			IPAddress ip_local = host_local.AddressList[0];
			IPEndPoint iep_local = new IPEndPoint(ip_local,Convert.ToInt32(textBox_port_pere.Text));
			s_accept.Bind(iep_local);
			s_accept.Listen(1);
			s_pere = s_accept.Accept();
		}
		#endregion



	}
}
