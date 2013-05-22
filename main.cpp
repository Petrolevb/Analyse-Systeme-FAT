#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"fichier.hpp"

using namespace std;

#define SIZE_ENTRY 32

#define DELETED_FILE 0xe5

#define LONG_NAME 0x0F
#define LAST_LONG_ENTRY 0x40

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cout << "Usage : " << argv[0] << " file" << endl;
        return 1;
    }
    vector<Fichier> filesRed;
    cout << "Opening " << argv[1] << endl;
    fstream ifs (argv[1], ios::in | ios::binary);
    if(ifs.fail())
    {
        cerr << "Error opening file " << argv[1] << endl;
        return 2;
    }
    // movement to interresting position for test purpose 
    ifs.seekg(0x1f200);

    unsigned char buffer[SIZE_ENTRY];
    while(ifs.tellg() < 0x1f5f0)
    {
        Fichier newFile;
        if(!ifs.read(reinterpret_cast<char *>(&buffer[0]), SIZE_ENTRY))
        {
            cerr << "Error reading file " << endl;
            ifs.close();
            return 2;
        }

        cout << "Data at this point " << hex << ifs.tellg() << dec << " : ";
        bool deletedFile = buffer[0] == DELETED_FILE ;
        bool longName = buffer[11] == LONG_NAME;
        int numberLongEntry = 0;
        if(!deletedFile && longName)
            numberLongEntry = buffer[0] ^ LAST_LONG_ENTRY;

        // if there is some entry after that still contains the name
        string name = "";
        while(numberLongEntry >= 1)
        {
                string tmp = "";
                bool endName = false;
                for(int i = 1; i < 10; i += 2)
                    if(buffer[i] == 0x00 && buffer[i+1] == 0x00) 
                    { endName = true ; break;}
                    else tmp += buffer[i];

                if(!endName) // we continue only if we didn't already finished the name 
                for(int i = 14 ; i < 25; i+=2)
                    if(buffer[i] == 0x00 && buffer[i+1] == 0x00) 
                    { endName = true; break; }
                    else tmp += buffer[i];
                if(!endName) // same, we continue if we didn't end
                { tmp += buffer[28]; tmp += buffer[30]; }

                numberLongEntry--;
                name = tmp + name;
                if(numberLongEntry == 0) break; 
                if(!ifs.read(reinterpret_cast<char *>(&buffer[0]), SIZE_ENTRY))
                {
                    cerr << "Error reading file " << endl;
                    ifs.close();
                    return 2;
                }
        }


        if(deletedFile) cout << " [deleted file] > ";
        else            cout << "                > ";

        if (longName)
        {
            cout << "long : " << name ;
            newFile.setLongName(name);
            /*
            * We now reach the end of the "long name entries" part
            * So we can continue with the next entry, 
            * That will be the "short entry" of that file
            */
            if(!ifs.read(reinterpret_cast<char *>(&buffer[0]), SIZE_ENTRY))
            {
                cerr << "Error reading file " << endl;
                ifs.close();
                return 2;
            }
            cout << endl << "\t";
        }
        int i = 0;
        string tmp = "";
        if(deletedFile) { cout << "."; i++; }
        for(; i < 8; i++) tmp += buffer[i];
        newFile.setShortName(tmp);
         
        cout << tmp << "." ;
        tmp = "";
        for(; i < 12; i++) tmp += buffer[i];
        newFile.setExtension(tmp);
        cout << tmp;
        
        cout << endl << endl;
        filesRed.push_back(newFile);
    } // Loop of binary reading

    ifs.close();

    cout << "Total file : " << filesRed.size() << endl;
    return 0;
}
