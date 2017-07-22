//#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <fstream>
#include <conio.h>

using namespace std;

class Wave {

public:
    Wave(char * filename);
    ~Wave();
    void play(bool async=true);
    bool isok();

private:
    char * buffer;
    bool ok;
    HINSTANCE HInstance;
};

Wave::Wave(char * filename)
{
    ok = false;
    //buffer = 0;
    HInstance = GetModuleHandle(0);

    ifstream infile(filename, ios::binary);

    if (!infile)
    {
         std::cout << "Wave::file error: "<< filename << std::endl;
        return;
    }

    infile.seekg (0, ios::end);   // get length of file
    int length = infile.tellg();
    buffer = new char[length];    // allocate memory
    infile.seekg (0, ios::beg);   // position to start of file
    infile.read (buffer,length);  // read entire file

    infile.close();
    ok = true;
}

Wave::~Wave()
{
    PlaySound(NULL, 0, 0); // STOP ANY PLAYING SOUND
    delete [] buffer;      // before deleting buffer.
}
void Wave::play(bool async)
{
    if (!ok)
        return;

    if (async)
        PlaySoundA(buffer, HInstance, SND_MEMORY | SND_ASYNC);
    else
        PlaySoundA(buffer, HInstance, SND_MEMORY);
}

bool Wave::isok()
{
    return ok;
}

/*
int main(int argc, char *argv[]) {

    std::cout << "Trying to play sound ...\n";

    Wave one("sound1.WAV");
    Wave two("sound2.WAV");
    Wave three("sound3.WAV");

    one.play(1);
    two.play(1);
    three.play(1);

    std::cout << "press key to exit";
    getch();

    return 0;
}
*/