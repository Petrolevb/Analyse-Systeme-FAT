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
        
        if(deletedFile)
        {
            longName = (
                buffer[10] == 0x00 && buffer[11] == 0x00 && buffer[12] == 0x00 &&
                buffer[26] == 0x00 && buffer[27] == 0x00);
            newFile.setDeleted();
        }

        int numberLongEntry = 0;
        if(longName)
        {
            numberLongEntry = buffer[0] ^ LAST_LONG_ENTRY;
            if(deletedFile) numberLongEntry = 1;
        }

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
                { 
                    if(buffer[28] != 0x00) 
                    {
                        tmp += buffer[28]; 
                        if(buffer[30] != 0x00) 
                            tmp += buffer[30]; 
                    }
                }

                numberLongEntry--;
                name = tmp + name;
                if(numberLongEntry == 0) break; 
                if(!ifs.read(reinterpret_cast<char *>(&buffer[0]), SIZE_ENTRY))
                {
                    cerr << "Error reading file " << endl;
                    ifs.close();
                    return 2;
                }

                // if this is a deleted file, we have to check another time if is it still
                // a long name entry, or the short one
                if(buffer[10] == 0x00 && buffer[11] == 0x00 && buffer[12] == 0x00 &&
                    buffer[26] == 0x00 && buffer[27] == 0x00)
                        numberLongEntry++;
        }


        if (longName)
        {
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
        }
        int i = 0;
        string tmp = "";
        if(deletedFile) 
            i++;
        for(; i < 8; i++) tmp += buffer[i];
        newFile.setShortName(tmp);
         
        tmp = "";
        for(; i < 12; i++) tmp += buffer[i];
        newFile.setExtension(tmp);

        long firstCluster = 0x00000000;
        firstCluster |= buffer[26];
        firstCluster <<= 8;

        firstCluster |= buffer[27];
        firstCluster <<= 8;
        
        if(buffer[20] != 0x00 && buffer[21] != 0x00)
        {
            firstCluster |= buffer[20];
            firstCluster <<= 8;

            firstCluster |= buffer[21];
            firstCluster <<= 8;
        }

        newFile.addCluster(firstCluster);
        filesRed.push_back(newFile);
        cout << newFile.toString() << endl;
        cout << "\t" << hex << "First cluster : " << firstCluster << dec << endl;
    } // Loop of binary reading

    ifs.close();

    cout << "Total file : " << filesRed.size() << endl;
    return 0;
}

