#include <QCoreApplication>
#include <iostream>
#include <stdio.h>
using namespace std;
#include "snimage.h"
#include <windows.h>

int main(int argc, char *argv[])
{
    SNImage monImage;

    QCoreApplication a(argc, argv);
    monImage.Chargement("power.bmp");
    cout << monImage.Largeur() << endl;
    cout << monImage.Hauteur() << endl;
    Coordonnee c={50,50};
    Pixel rouge = {0, 0, 255};
    //monImage.Colorie(c, 50, 60, rouge);


    monImage.Sauvegarde("testpower.bmp");
    WinExec("mspaint.exe testpower.bmp" , 1) ;
    cin.get();
    return a.exec();

}
