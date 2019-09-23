#include "snimage.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace std;


SNImage::SNImage()
{
    sign=0;
    largeur=0;
    hauteur=0;
    tailleFichier=0;
    offset=0;
    tailleImage=0;
    tailleTete=0;
    format=0;
    resoLarg=0;
    resoHaut=0;
    dimensionMax=0;
}

SNImage::~SNImage()
{
    if (dimensionMax != 0)
        ~SNImage();
}

void SNImage::Chargement(string nomFichier)
{
    ifstream entree;
    long i,j;
    char k;
    long h;
    unsigned long dimensionMaxPrecedente=dimensionMax;
    entree.open(nomFichier.c_str(),fstream::binary);
    //lecture entête image:
    entree.read((char*)&sign,2*sizeof(char));//signature (BM)=424D
    entree.read((char*)&tailleFichier,sizeof(long));
    entree.read((char*)&zero,sizeof(long));
    entree.read((char*)&offset,sizeof(long));
    entree.read((char*)&tailleTete,sizeof(long));
    entree.read((char*)&largeur,sizeof(long));
    entree.read((char*)&hauteur,sizeof(long));
    entree.read((char*)&format,sizeof(long));
    entree.read((char*)&zero,sizeof(long));
    entree.read((char*)&tailleImage,sizeof(long));
    entree.read((char*)&resoLarg,sizeof(long));
    entree.read((char*)&resoHaut,sizeof(long));
    entree.read((char*)&zero,sizeof(long));
    entree.read((char*)&zero,sizeof(long));
    //calcul de k
    k=(4-(largeur*3)%4)%4;
    //remplissage tableau 2 dimensions
    i=hauteur-1;j=0;//i indice des lignes
    if(sign!=0x4D42) cout<<"Vous devez choisir un bitmap .bmp";
    else if(format!=0x180001) cout<<"Vous devez choisir un bitmap 24 bits";
    else
    { //libération éventuelle de la mémoire occupée par l'ancienne image
        if(dimensionMaxPrecedente)
        {
            for(h=0;h<dimensionMaxPrecedente;h++) delete []image[h];
                delete []image;
        }
        if(hauteur>largeur)
            dimensionMax=hauteur;
        else dimensionMax=largeur;
        //réservation de la mémoire
        image=new Pixel*[dimensionMax];//i
        for(h=0;h<dimensionMax;h++) image[h]=new Pixel[dimensionMax];//j
        while(!entree.eof())
        {
            entree.read((char*)&image[i][j].bleu,sizeof(char));
            entree.read((char*)&image[i][j].vert,sizeof(char));
            entree.read((char*)&image[i][j].rouge,sizeof(char));
            j++;
            if(j==largeur)
            { entree.seekg(k,ios_base::cur);
              j=0;
              i--;
            }
        }
    }
 entree.close();
}
