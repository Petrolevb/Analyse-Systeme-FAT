using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace Analyse_d_un_systeme_FAT
{
    class Program
    {
        static void Main(string[] args)
        {
            byte[] test = Encoding.Default.GetBytes(Int32.Parse("0002", System.Globalization.NumberStyles.HexNumber).ToString());
            Console.WriteLine(Encoding.Default.GetString(test));
            test = Program.inversionOctet(test);
            Console.WriteLine(Encoding.Default.GetString(test));

            if (args.Length < 1) { Console.WriteLine("Pas de fichier en entrée"); return; }
            #region Recuperation du fichier dans lb
            BinaryReader br = new BinaryReader(new FileStream(args[0], FileMode.Open ,FileAccess.Read, FileShare.Read));
            List<byte> lb = new List<byte>();
            lb.Add(br.ReadByte());
            br.Close();
            #endregion
            
            int positionCurseur = 0; // initialisation de toutes les boucles
            #region Traitement secteur de boot de FAT
            //Prise de 512 Octets
            if (lb.Count < 512) { Console.WriteLine("Problème de taille de fichier"); return; }
            byte[] instructionSaut = new byte[3];
            
            for (int i = positionCurseur; i < 3; positionCurseur++, i++)
                instructionSaut[i] = lb[positionCurseur];

            byte[] nomSE = new byte[8];
            for (int i = 0; i < 8; i++, positionCurseur++)
                nomSE[i] = lb[positionCurseur];
            
            #endregion

        }

        static byte[] inversionOctet(byte[] nombre)
        {
            byte[] retour = new byte[nombre.Length];
            for (int i = 0; i < nombre.Length; i++)
                retour[i] = nombre[nombre.Length-1-i];
            return retour;
        }
    }
}
