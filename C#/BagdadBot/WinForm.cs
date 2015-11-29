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

namespace BagdadBot
{
   /// <summary> 
   /// Description Résumé de WinForm. 
   /// </summary> 
   public class WinForm : System.Windows.Forms.Form
   {
      /// <summary> 
      /// Variable requise par le concepteur. 
      /// </summary> 
      ArrayList identifylist = new ArrayList();
      bool stop;
      private System.ComponentModel.Container components = null;
      private System.Windows.Forms.Button Connect;
      private System.Windows.Forms.RichTextBox consol;
      private System.Net.Sockets.Socket s;
      private System.Windows.Forms.Button disconnect;
      private System.Windows.Forms.Button scrollingdown;
      private System.Windows.Forms.ListBox identifyliste;
      private System.Windows.Forms.Label labelidentify;
      private System.Windows.Forms.Button scrollingup;
      private System.Windows.Forms.Label console;
      private System.Windows.Forms.GroupBox options;
      private System.Windows.Forms.CheckBox voiceauto;
      private System.Windows.Forms.CheckBox nodeopbot;
      private System.Windows.Forms.Button reopbot;
      private System.Windows.Forms.CheckBox debug;
      private System.Windows.Forms.CheckBox split;
      private System.Windows.Forms.Button desidentifybutton;

      public WinForm()
      {
         // 
         // Requis pour la gestion du concepteur Windows Form 
         // 
         InitializeComponent();

         // 
         // TODO: Ajouter tout le code du constructeur après l'appel de InitializeComponent 
         // 
      }

      /// <summary> 
      /// Nettoyage des ressources utilisées. 
      /// </summary> 
      protected override void Dispose(bool disposing)
      {
         if (disposing)
         {
            if (components != null)
            {
               components.Dispose();
            }
         }
         base.Dispose(disposing);
      }

      #region Code généré par le concepteur Windows Form 
      /// <summary> 
      /// Méthode requise pour la gestion du concepteur - ne pas modifier 
      /// le contenu de cette méthode avec l'éditeur de code. 
      /// </summary> 
      private void InitializeComponent()
      {
         System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(WinForm));
         this.Connect = new System.Windows.Forms.Button();
         this.disconnect = new System.Windows.Forms.Button();
         this.consol = new System.Windows.Forms.RichTextBox();
         this.scrollingdown = new System.Windows.Forms.Button();
         this.identifyliste = new System.Windows.Forms.ListBox();
         this.labelidentify = new System.Windows.Forms.Label();
         this.scrollingup = new System.Windows.Forms.Button();
         this.console = new System.Windows.Forms.Label();
         this.options = new System.Windows.Forms.GroupBox();
         this.split = new System.Windows.Forms.CheckBox();
         this.debug = new System.Windows.Forms.CheckBox();
         this.nodeopbot = new System.Windows.Forms.CheckBox();
         this.voiceauto = new System.Windows.Forms.CheckBox();
         this.reopbot = new System.Windows.Forms.Button();
         this.desidentifybutton = new System.Windows.Forms.Button();
         this.options.SuspendLayout();
         this.SuspendLayout();
         // 
         // Connect 
         // 
         this.Connect.Location = new System.Drawing.Point(8, 264);
         this.Connect.Name = "Connect";
         this.Connect.Size = new System.Drawing.Size(56, 24);
         this.Connect.TabIndex = 0;
         this.Connect.Text = "Connect";
         this.Connect.Click += new System.EventHandler(this.Connect_Click);
         // 
         // disconnect 
         // 
         this.disconnect.Location = new System.Drawing.Point(64, 264);
         this.disconnect.Name = "disconnect";
         this.disconnect.Size = new System.Drawing.Size(72, 24);
         this.disconnect.TabIndex = 2;
         this.disconnect.Text = "Disconnect";
         this.disconnect.Click += new System.EventHandler(this.disconnect_Click);
         // 
         // consol 
         // 
         this.consol.Location = new System.Drawing.Point(8, 24);
         this.consol.Name = "consol";
         this.consol.Size = new System.Drawing.Size(184, 240);
         this.consol.TabIndex = 4;
         this.consol.Text = "";
         // 
         // scrollingdown 
         // 
         this.scrollingdown.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
         this.scrollingdown.Location = new System.Drawing.Point(192, 144);
         this.scrollingdown.Name = "scrollingdown";
         this.scrollingdown.Size = new System.Drawing.Size(24, 120);
         this.scrollingdown.TabIndex = 5;
         this.scrollingdown.Text = "vvvvvvvvvvvvvvvvvv";
         this.scrollingdown.Click += new System.EventHandler(this.scrollingdown_Click);
         // 
         // identifyliste 
         // 
         this.identifyliste.Location = new System.Drawing.Point(256, 24);
         this.identifyliste.Name = "identifyliste";
         this.identifyliste.Size = new System.Drawing.Size(104, 238);
         this.identifyliste.TabIndex = 6;
         // 
         // labelidentify 
         // 
         this.labelidentify.Font = new System.Drawing.Font("Palatino Linotype", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
         this.labelidentify.ForeColor = System.Drawing.Color.DeepSkyBlue;
         this.labelidentify.Location = new System.Drawing.Point(256, 8);
         this.labelidentify.Name = "labelidentify";
         this.labelidentify.Size = new System.Drawing.Size(96, 16);
         this.labelidentify.TabIndex = 7;
         this.labelidentify.Text = "Liste d\'identifiée:";
         // 
         // scrollingup 
         // 
         this.scrollingup.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
         this.scrollingup.Location = new System.Drawing.Point(192, 24);
         this.scrollingup.Name = "scrollingup";
         this.scrollingup.Size = new System.Drawing.Size(24, 120);
         this.scrollingup.TabIndex = 8;
         this.scrollingup.Text = "^^^^^^^^^^^^^^^^";
         this.scrollingup.Click += new System.EventHandler(this.scrollingup_Click);
         // 
         // console 
         // 
         this.console.Font = new System.Drawing.Font("Palatino Linotype", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
         this.console.ForeColor = System.Drawing.Color.DeepSkyBlue;
         this.console.Location = new System.Drawing.Point(8, 8);
         this.console.Name = "console";
         this.console.Size = new System.Drawing.Size(48, 16);
         this.console.TabIndex = 9;
         this.console.Text = "Console:";
         // 
         // options 
         // 
         this.options.Controls.Add(this.split);
         this.options.Controls.Add(this.debug);
         this.options.Controls.Add(this.nodeopbot);
         this.options.Controls.Add(this.voiceauto);
         this.options.Location = new System.Drawing.Point(376, 16);
         this.options.Name = "options";
         this.options.Size = new System.Drawing.Size(160, 248);
         this.options.TabIndex = 10;
         this.options.TabStop = false;
         this.options.Text = "Options";
         // 
         // split 
         // 
         this.split.Location = new System.Drawing.Point(8, 96);
         this.split.Name = "split";
         this.split.Size = new System.Drawing.Size(144, 16);
         this.split.TabIndex = 3;
         this.split.Text = "Mode Splitter";
         // 
         // debug 
         // 
         this.debug.Location = new System.Drawing.Point(8, 72);
         this.debug.Name = "debug";
         this.debug.Size = new System.Drawing.Size(144, 16);
         this.debug.TabIndex = 2;
         this.debug.Text = "Mode Debug";
         // 
         // nodeopbot 
         // 
         this.nodeopbot.Checked = true;
         this.nodeopbot.CheckState = System.Windows.Forms.CheckState.Checked;
         this.nodeopbot.Location = new System.Drawing.Point(8, 48);
         this.nodeopbot.Name = "nodeopbot";
         this.nodeopbot.Size = new System.Drawing.Size(144, 16);
         this.nodeopbot.TabIndex = 1;
         this.nodeopbot.Text = "Impossible deop bot";
         // 
         // voiceauto 
         // 
         this.voiceauto.Checked = true;
         this.voiceauto.CheckState = System.Windows.Forms.CheckState.Checked;
         this.voiceauto.Location = new System.Drawing.Point(8, 24);
         this.voiceauto.Name = "voiceauto";
         this.voiceauto.Size = new System.Drawing.Size(144, 16);
         this.voiceauto.TabIndex = 0;
         this.voiceauto.Text = "Voice Automatique";
         // 
         // reopbot 
         // 
         this.reopbot.Location = new System.Drawing.Point(136, 264);
         this.reopbot.Name = "reopbot";
         this.reopbot.Size = new System.Drawing.Size(80, 24);
         this.reopbot.TabIndex = 11;
         this.reopbot.Text = "Opbot";
         this.reopbot.Click += new System.EventHandler(this.reopbot_Click);
         // 
         // desidentifybutton 
         // 
         this.desidentifybutton.Location = new System.Drawing.Point(256, 264);
         this.desidentifybutton.Name = "desidentifybutton";
         this.desidentifybutton.Size = new System.Drawing.Size(104, 24);
         this.desidentifybutton.TabIndex = 12;
         this.desidentifybutton.Text = "Desidentifier";
         this.desidentifybutton.Click += new System.EventHandler(this.desidentifybutton_Click);
         // 
         // WinForm 
         // 
         this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
         this.ClientSize = new System.Drawing.Size(544, 302);
         this.Controls.Add(this.desidentifybutton);
         this.Controls.Add(this.reopbot);
         this.Controls.Add(this.options);
         this.Controls.Add(this.console);
         this.Controls.Add(this.scrollingup);
         this.Controls.Add(this.labelidentify);
         this.Controls.Add(this.identifyliste);
         this.Controls.Add(this.scrollingdown);
         this.Controls.Add(this.consol);
         this.Controls.Add(this.disconnect);
         this.Controls.Add(this.Connect);
         this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
         this.Name = "WinForm";
         this.Text = "Bot Bagdad";
         this.Closed += new System.EventHandler(this.disconnect_Click);
         this.options.ResumeLayout(false);
         this.ResumeLayout(false);
      }
      #endregion

      /// <summary> 
      /// Le point d'entrée principal de l'application. 
      /// </summary> 
      [STAThread]
      static void Main()
      {
         Application.Run(new WinForm());
      }

      private void Connect_Click(object sender, System.EventArgs e)
      {

         s = new System.Net.Sockets.Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
         IPHostEntry ipHostInfo = Dns.Resolve("127.0.0.1");
         IPAddress ipadress = ipHostInfo.AddressList[0];
         IPEndPoint ipe = new IPEndPoint(ipadress, 6667);
         s.Connect(ipe);
         Thread th = new Thread(new ThreadStart(Connection));
         th.Start();
      }

      public void Connection()
      {
         byte[] RecvBytes = new byte[5000];
         stop = true;
         string texte = null;
         string[] cop = null;
         string nick = ("NICK Bagdad\nUSER irak 0 0 Sadam\r\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(nick));
         string chan = ("JOIN #chan\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(chan));
         string ident = ("PRIVMSG nickserv :identify pass\r\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(ident));
         string op = ("cs op #chan\r\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(op));

         // boucle infinie pour la reception des donnes du serveur
         while (stop != false && s.Connected == true)
         {
            Int32 bytes = s.Receive(RecvBytes, RecvBytes.Length, SocketFlags.None);
            texte = System.Text.Encoding.ASCII.GetString(RecvBytes, 0, bytes);
            if (debug.Checked == true) consol.Text += texte;
            cop = texte.Split(new Char[] { ':', '!', '@', ' ', '\r', '\n' });
            if (split.Checked == true)
            {
               for (int i = 0; i < cop.Length; i++)
               {
                  consol.Text += "Arg[" + i + "]==>" + cop[i].ToString() + "\n";
               }
            }

            if (cop.Length >= 5)
            {
               string action = cop[4].ToString();
               if (action == "PART" || action == "QUIT")
               {
                  desidentify(cop[1].ToString(), false);
               }
               if (action == "NICK")
               {
                  if (isidentify(cop[1].ToString()) == true)
                  {
                     desidentify(cop[1].ToString(), false);
                     identify(cop[6].ToString());
                  }
               }
               if (voiceauto.Checked == true)
               {
                  if (action == "JOIN")
                  {
                     givevoice(cop[1].ToString());
                  }
               }


               if (nodeopbot.Checked == true)
               {
                  if (action == "MODE" && (cop[6].ToString()) == "-o" && (cop[7].ToString()) == "Bagdad")
                  {
                     opbot();
                  }
               }
            }

            if (cop.Length == 11 && cop[7].ToString() == "")
            {
               string cmd = cop[8].ToString();

               if (cmd == "secure")
               {
                  identify(cop[1].ToString());
               }
               if (cmd == "unsecure")
               {
                  desidentify(cop[1].ToString(), true);
               }
               if (cmd == "op" && isidentify(cop[1].ToString()) == true)
               {
                  giveop(cop[1].ToString());
               }
               if (cmd == "deop" && isidentify(cop[1].ToString()) == true)
               {
                  takeop(cop[1].ToString());
               }
               if (cmd == "voice" && isidentify(cop[1].ToString()) == true)
               {
                  givevoice(cop[1].ToString());
               }
               if (cmd == "devoice" && isidentify(cop[1].ToString()) == true)
               {
                  takevoice(cop[1].ToString());
               }
               if (cmd == "cmd" && isidentify(cop[1].ToString()) == true)
               {
                  envoicmd(cop[1].ToString());
               }

            }
            if (cop.Length >= 12 && cop[7].ToString() == "")
            {
               string cmd = cop[8].ToString();

               if (cmd == "secure")
               {
                  int i = 9;
                  for (int nbarg = cop.Length - 11; nbarg > 0; nbarg--)
                  {
                     identify(cop[i].ToString());
                     i++;
                  }
               }
               if (cmd == "unsecure")
               {
                  int i = 9;
                  for (int nbarg = cop.Length - 11; nbarg > 0; nbarg--)
                  {
                     desidentify(cop[i].ToString(), true);
                     i++;
                  }
               }
               if (cmd == "op" && isidentify(cop[1].ToString()) == true)
               {
                  int i = 9;
                  for (int nbarg = cop.Length - 11; nbarg > 0; nbarg--)
                  {
                     giveop(cop[i].ToString());
                     i++;
                  }
               }
               if (cmd == "deop" && isidentify(cop[1].ToString()) == true)
               {
                  int i = 9;
                  for (int nbarg = cop.Length - 11; nbarg > 0; nbarg--)
                  {
                     takeop(cop[i].ToString());
                     i++;
                  }
               }
               if (cmd == "voice" && isidentify(cop[1].ToString()) == true)
               {
                  int i = 9;
                  for (int nbarg = cop.Length - 11; nbarg > 0; nbarg--)
                  {
                     givevoice(cop[i].ToString());
                     i++;
                  }
               }
               if (cmd == "devoice" && isidentify(cop[1].ToString()) == true)
               {
                  int i = 9;
                  for (int nbarg = cop.Length - 11; nbarg > 0; nbarg--)
                  {
                     takevoice(cop[i].ToString());
                     i++;
                  }
               }
               if (cmd == "kick" && isidentify(cop[1].ToString()) == true)
               {
                  int i = 9;
                  for (int nbarg = cop.Length - 11; nbarg > 0; nbarg--)
                  {
                     if ((cop[i].ToString()) == "Bagdad")
                     {
                        kick(cop[1].ToString());
                     }
                     else
                     {
                        kick(cop[i].ToString());
                        i++;
                     }
                  }
               }
               if (cmd == "kb" && isidentify(cop[1].ToString()) == true)
               {
                  int i = 9;
                  for (int nbarg = cop.Length - 11; nbarg > 0; nbarg--)
                  {
                     if ((cop[i].ToString()) == "Bagdad")
                     {
                        ban(cop[1].ToString());
                        kick(cop[1].ToString());
                        i++;
                     }
                     else
                     {
                        ban(cop[i].ToString());
                        kick(cop[i].ToString());
                        i++;
                     }
                  }
               }

               if (cmd == "ban" && isidentify(cop[1].ToString()) == true && cop.Length == 14)
               {
                  ban(cop[9].ToString() + "!" + cop[10].ToString() + "@" + cop[11].ToString());
               }
               if (cmd == "unban" && isidentify(cop[1].ToString()) == true && cop.Length == 14)
               {
                  unban(cop[9].ToString() + "!" + cop[10].ToString() + "@" + cop[11].ToString());
               }
               if (cmd == "notice" && isidentify(cop[1].ToString()) == true)
               {
                  int i = 10;
                  string message = "";
                  for (int nbarg = cop.Length - 11; nbarg > 0; nbarg--)
                  {
                     if (nbarg == 1)
                     {
                        message += cop[i].ToString();
                     }
                     else
                     {
                        message += cop[i].ToString() + " ";
                     }
                     i++;
                  }
                  notice(cop[9].ToString(), message);
               }

               if (cmd == "googlecom")
               {
                  int i = 9;
                  string recherche = "";
                  for (int nbarg = cop.Length - 11; nbarg > 0; nbarg--)
                  {
                     if (nbarg == 1)
                     {
                        recherche += cop[i].ToString();
                     }
                     else
                     {
                        recherche += cop[i].ToString() + "+";
                     }
                     i++;
                  }
                  googlecom(recherche);
               }


               if (cmd == "googlefr")
               {
                  int i = 9;
                  string recherche = "";
                  for (int nbarg = cop.Length - 11; nbarg > 0; nbarg--)
                  {
                     if (nbarg == 1)
                     {
                        recherche += cop[i].ToString();
                     }
                     else
                     {
                        recherche += cop[i].ToString() + "+";
                     }
                     i++;
                  }
                  googlefr(recherche);
               }
               if (cmd == "google")
               {
                  int i = 9;
                  string recherche = "";
                  for (int nbarg = cop.Length - 11; nbarg > 0; nbarg--)
                  {
                     if (nbarg == 1)
                     {
                        recherche += cop[i].ToString();
                     }
                     else
                     {
                        recherche += cop[i].ToString() + "+";
                     }
                     i++;
                  }
                  googlefr(recherche);
                  googlecom(recherche);
               }
            }

         }

         s.Shutdown(SocketShutdown.Both);
         s.Close();


      }

      public void identify(string pseudo)
      {
         if (isidentify(pseudo) == false)
         {
            identifylist.Add(pseudo);
            consol.Text += pseudo + " a été identifié.\n";
            privmsg(pseudo, "Vous étes désormais identifié");
            refreshidentifyliste();
         }
         else privmsg(pseudo, "Vous étes déja identifié");
      }
      public bool isidentify(string pseudo)
      {
         if (identifylist.Contains(pseudo)) return true;
         return false;
      }
      public void desidentify(string pseudo, bool warning)
      {
         if (isidentify(pseudo) == true)
         {
            identifylist.Remove(pseudo);
            if (warning == true) privmsg(pseudo, "Vous n'êtes désormais plus identifié");
            consol.Text += pseudo + " a été désidentifié.\n";
            refreshidentifyliste();
         }
      }
      public void giveop(string pseudo)
      {
         string op = ("MODE #chan +o " + pseudo + "\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(op));
      }
      public void givevoice(string pseudo)
      {
         string voice = ("MODE #chan +v " + pseudo + "\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(voice));
      }
      public void takeop(string pseudo)
      {
         string deop = ("MODE #chan -o " + pseudo + "\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(deop));
      }
      public void takevoice(string pseudo)
      {
         string devoice = ("MODE #chan -v " + pseudo + "\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(devoice));
      }
      public void quit(string raison)
      {
         string quit = ("QUIT :" + raison + "\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(quit));
      }

      public void privmsg(string destinataire, string message)
      {
         string privmsg = ("PRIVMSG " + destinataire + " :" + message + "\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(privmsg));
      }
      public void notice(string destinataire, string message)
      {
         string notice = ("NOTICE " + destinataire + " :" + message + "\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(notice));
      }
      public void kick(string pseudo)
      {
         string kick = ("KICK #chan " + pseudo + "\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(kick));
      }
      public void ban(string masque)
      {
         string ban = ("MODE #chan +b " + masque + "\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(ban));
      }
      public void unban(string masque)
      {
         string unban = ("MODE #chan -b " + masque + "\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(unban));
      }
      private void envoicmd(string pseudo)
      {
         privmsg(pseudo, "BOT Bagdad v1.5 cree pour La TECH DATAS FOUNDATION PAR b0l0k");
         privmsg(pseudo, "Ce qui est entre () est faculatif");
         privmsg(pseudo, "Ce qui est entre <> est obligatoire !!!");
         privmsg(pseudo, "!op (personne(s)) Donner le mode operateur");
         privmsg(pseudo, "!deop (personne(s))Enlever le mode operateur");
         privmsg(pseudo, "!voice (personne(s))Donner le mode voice");
         privmsg(pseudo, "!devoice (personne(s))Enlever le mode voice");
         privmsg(pseudo, "!kick <personne(s)> Kicker une ou plusieurs personnes");
         privmsg(pseudo, "!ban <pseudo!user@domain> Bannir un certain masque");
         privmsg(pseudo, "!kb <personne(s)> Kicker et Bannir une ou plusieurs personnes");
         privmsg(pseudo, "!unban <pseudo!user@domain> Debannir un certain masque");
         privmsg(pseudo, "!notice <Destinataire> <Notice> Faire une notice");
         privmsg(pseudo, "!secure(personne(s))Identifie au pres de moi");
         privmsg(pseudo, "!unsecure(personne(s))Desidentifie au pres de moi");
         privmsg(pseudo, "!googlefr <recherche> Trouvez la premiere occurence sur google Langue:fr");
         privmsg(pseudo, "!googlecom <recherche> Trouvez la premiere occurence sur google");
         privmsg(pseudo, "!google <recherche> Combine les deux commandes precedentes");
      }
      private void googlefr(string recherche)
      {
         consol.Text += "Recherche de " + recherche + "\n";
         Socket google = new System.Net.Sockets.Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
         IPHostEntry ipHostInfo = Dns.Resolve("google.fr");
         IPAddress ipadress = ipHostInfo.AddressList[0];
         IPEndPoint ipe = new IPEndPoint(ipadress, 80);
         google.Connect(ipe);
         string search = ("GET ./search?hl=fr&ie=UTF-8&q=" + recherche + "&meta=lr%3Dlang_fr&btnI=yes HTTP/1.1\n\n");
         google.Send(System.Text.Encoding.ASCII.GetBytes(search));
         byte[] reponse = new byte[5000];
         Int32 bytes = google.Receive(reponse, reponse.Length, SocketFlags.None);
         string sreponse = System.Text.Encoding.ASCII.GetString(reponse, 0, bytes);
         string[] spreponse = sreponse.Split(new Char[] { '\n' });
         if ((spreponse[1].ToString()).StartsWith("Location") == true)
         {
            privmsg("#chan", ("En Francais " + spreponse[1].ToString()));
         }
         else privmsg("#chan", "Il n'y pas de reponse sur google.com");
      }
      private void googlecom(string recherche)
      {
         consol.Text += "Recherche de " + recherche + "\n";
         if (recherche == "tdf")
         {
            privmsg("#chan", "En Francais Location: http://www.tdf-group.com");
         }
         else
         {
            Socket google = new System.Net.Sockets.Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPHostEntry ipHostInfo = Dns.Resolve("google.com");
            IPAddress ipadress = ipHostInfo.AddressList[0];
            IPEndPoint ipe = new IPEndPoint(ipadress, 80);
            google.Connect(ipe);
            string search = ("GET ./search?hl=fr&ie=UTF-8&q=" + recherche + "&btnI=yes HTTP/1.1\n\n");
            google.Send(System.Text.Encoding.ASCII.GetBytes(search));
            byte[] reponse = new byte[5000];
            Int32 bytes = google.Receive(reponse, reponse.Length, SocketFlags.None);
            string sreponse = System.Text.Encoding.ASCII.GetString(reponse, 0, bytes);
            string[] spreponse = sreponse.Split(new Char[] { '\n' });
            if ((spreponse[1].ToString()).StartsWith("Location") == true)
            {
               privmsg("#chan", "Dans le monde " + spreponse[1].ToString());
            }
            else privmsg("#chan", "Il n'y pas de reponse sur google.com");
         }
      }

      private void disconnect_Click(object sender, System.EventArgs e)
      {
         if (stop != false)
         {
            quit("[TDF] Bot TECH DATAS FOUNDATION http://www.tdf-group.com");
            stop = false;
         }
      }

      private void refreshidentifyliste()
      {

         identifyliste.Items.Clear();
         foreach (object o in identifylist)
            identifyliste.Items.Add(o.ToString());
      }

      private void scrollingdown_Click(object sender, System.EventArgs e)
      {
         consol.Focus();
         consol.SelectionStart = consol.TextLength;
      }

      private void scrollingup_Click(object sender, System.EventArgs e)
      {
         consol.Focus();
         consol.SelectionStart = 0;
      }

      private void opbot()
      {
         string op = ("cs op #chan\r\n");
         s.Send(System.Text.Encoding.ASCII.GetBytes(op));
      }

      private void reopbot_Click(object sender, System.EventArgs e)
      {
         opbot();
      }

      private void desidentifybutton_Click(object sender, System.EventArgs e)
      {
         string pseudo = identifyliste.SelectedItem.ToString();
         desidentify(pseudo, true);
      }

   }
}

