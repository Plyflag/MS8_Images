#include <QCoreApplication>
#include <iostream>
#include <stdio.h>
using namespace std;
#include "snimage.h"
#include <windows.h>

int main(int argc, char *argv[])
{
    SNImage monImage;
    Coordonnee debut={0,0};
    Coordonnee fin={100,201};

    QCoreApplication a(argc, argv);
   // monImage.Chargement("power.bmp");
    monImage.Chargement("konoha.bmp");
    cout << monImage.Largeur() << endl;
    cout << monImage.Hauteur() << endl;
    //Coordonnee c={80,150};
    //Pixel rouge = {0, 0, 255};
    Pixel couleur = {255, 255, 0};
//    monImage.Colorie(c, 60, 60, black);
//    monImage.Detoure(0, 50, 100, 0, rouge);
    //    monImage.Negatif();
        monImage.RetourneHorizontal();
      //  monImage.RetourneVertical();
        monImage.NiveauGris();
   // monImage.Tourne90Droite();
   // monImage.DessineDiagonaleComplexe(debut,fin,12,couleur);
    monImage.PixelToChar();
    monImage.Histogramme();

    monImage.Sauvegarde("testkonoha.bmp");
    WinExec("mspaint.exe konoha.bmp" , 1) ;
    cin.get();
    return a.exec();

}
