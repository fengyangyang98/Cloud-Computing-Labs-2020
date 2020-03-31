#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, const char **argv)
{
    fstream file(argv[1]);
    long long int line_num = atoi(argv[2]);
    long long int coefficient = line_num << atoi(argv[3]);
    fstream fileout(argv[4],ios::app);

    if (!file.is_open())
    {
        cout << "file open failed!.\n";
        return 0;
    }
    string tmp[line_num];
    for (int i = 0; i < line_num; i++)
    {
        getline(file, tmp[i]);
    }

    for (long long int i = 0; i < coefficient; i++)
    {
        fileout << tmp[i % line_num] ;
        if (i != coefficient-1) {
            fileout <<'\n';
        }
    }
    fileout.close();
    return 0;
}