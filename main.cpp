#include<iostream>
#include<fstream>
#include<string>

using namespace std;

#define SIZE_ENTRY 32

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cout << "Usage : " << argv[0] << " file" << endl;
        return 1;
    }
    cout << "Opening " << argv[1] << endl;
    fstream ifs (argv[1], ios::in | ios::binary);
    if(ifs.fail())
    {
        cerr << "Error opening file " << argv[1] << endl;
        return 2;
    }
    // movement to interresting position for test purpose 
    ifs.seekg(0x1f200);

    char buffer[SIZE_ENTRY];
    while(ifs.tellg() < 0x1f5f0)
    {
        if(!ifs.read(buffer, SIZE_ENTRY))
        {
            cerr << "Error reading file " << endl;
            ifs.close();
            return 2;
        }

        cout << "Data at this point " << hex << ifs.tellg() << " : ";
        bool deletedFile = buffer[0] == 0xffffffe5 ;
        if(deletedFile) cout << " [deleted file] > ";
        else            cout << "                > ";
        bool longName = buffer[0] == 0x41 || buffer[0] == 0x42;

        if (longName)
        {
            cout << "long name"; 
        }
        else
        {
            int i = 0;
            if(deletedFile) { cout << "."; i++; }
            for(; i < 8; i++) cout << buffer[i];
            cout << "." ;
            for(; i < 12; i++) cout << buffer[i];
        }
        cout << endl;

    }
    ifs.close();
    return 0;
}
