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
    Coordonnee c={80,150};
    Pixel rouge = {0, 0, 255};
    Pixel black = {0, 0, 0};
//    monImage.Colorie(c, 60, 60, black);
//    monImage.Detoure(0, 50, 100, 0, rouge);
//    monImage.Negatif();
    monImage.RetourneHorizontal();
    monImage.RetourneVertical();
    monImage.NiveauGris();
    monImage.Tourne90Droite();

    monImage.Sauvegarde("testpower.bmp");
    WinExec("mspaint.exe testpower.bmp" , 1) ;
    cin.get();
    return a.exec();

}
