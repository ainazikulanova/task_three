#include <iostream>
#include <thread>
#include <windows.h>
#include<string>
#include <fstream>
#include<stdio.h>

using namespace std;

HANDLE sem = CreateSemaphore(NULL, 1, 1, L"xx");
HANDLE sem1 = CreateSemaphore(NULL, 1, 1, L"sem1");
HANDLE sem2 = CreateSemaphore(NULL, 1, 1, L"sem2");
int user = 0;

void readF() {

    string path = "myFile.txt";
    ifstream fin;
    fin.open(path);

    while (true) {
        if (fin.is_open()) {
            if (user == 1) {
                WaitForSingleObject(sem1, INFINITE);
            }
            else {
                WaitForSingleObject(sem2, INFINITE);
            }

            string str;
            while (!fin.eof()) {
                str = "";
                getline(fin, str);
                cout << str << endl;
            }
            fin.close();
        }
    }
}

void writeF() {

    string path = "myFile.txt";
    ofstream fout;

    while (true) {
        string text;
        cin >> text;

        fout.open(path, ofstream::app);
        if (fout.is_open())
        {
            getline(cin, text);
            cout << "enter text: " << text << endl;
            fout.close();
            if (user == 1)
                ReleaseSemaphore(sem2, 1, NULL);
            else
                ReleaseSemaphore(sem1, 1, NULL);
        }

    }
}

int main(int args, char* argv[])
{
    user = atoi(argv[0]);
    thread writerThread(writeF);
    thread readerThread(readF);
    writerThread.join();
    readerThread.join();

}
