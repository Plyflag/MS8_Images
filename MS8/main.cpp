#include <QCoreApplication>
#include <iostream>
#include <stdio.h>
using namespace std;
#include "snimage.h"
#include <winbase.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cout << "hello" << endl;
    cin.get();
    WinExec("mspaint.exe Resultats\\Resultat.bmp" , 1) ;
    return a.exec();
}
