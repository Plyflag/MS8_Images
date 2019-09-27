
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
    if(dimensionMax)
     { for(int h=0;h<dimensionMax;h++) delete []image[h];
     delete []image;
     }
}

void SNImage::Sauvegarde(string nomFichier)
{
    ofstream sortie; int i, j; char k, z=0, n;
    sortie.open(nomFichier.c_str(), fstream::binary);
    k=(4-(largeur*3)%4)%4;
    sortie.write((char*)&sign, 2*sizeof(char));
    sortie.write((char*)&tailleFichier, sizeof(long));
    sortie.write((char*)&zero,sizeof(long));
    sortie.write((char*)&offset,sizeof(long));
    sortie.write((char*)&tailleTete,sizeof(long));
    sortie.write((char*)&largeur,sizeof(long));
    sortie.write((char*)&hauteur,sizeof(long));
    sortie.write((char*)&format,sizeof(long));
    sortie.write((char*)&zero,sizeof(long));
    sortie.write((char*)&tailleImage,sizeof(long));
    sortie.write((char*)&resoLarg,sizeof(long));
    sortie.write((char*)&resoHaut,sizeof(long));
    sortie.write((char*)&zero,sizeof(long));
    sortie.write((char*)&zero,sizeof(long));
    for(i = hauteur - 1; i>=0; i--)
    {for(j=0; j < largeur; j++)
        {
          sortie.write((char*)&image[i][j].bleu, sizeof(char));
          sortie.write((char*)&image[i][j].vert, sizeof(char));
          sortie.write((char*)&image[i][j].rouge, sizeof(char));
        }
        for(int n=0; n<k; n++)
            sortie.write((char*)&z, sizeof(char));
    }
    sortie.close();
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

void SNImage::Colorie(Coordonnee coord,int l,int h,Pixel couleur)
{ if((coord.ligne+h<=hauteur)&&(coord.colonne+l<=largeur))
 for(int i=coord.ligne;i<coord.ligne+h;i++) for(int j=coord.colonne;j<coord.colonne+l;j++)
 {
     image[i][j].bleu=couleur.bleu;
     image[i][j].vert=couleur.vert;
     image[i][j].rouge=couleur.rouge;
 }
}

void SNImage::Detoure(int largeurGauche,int largeurDroite,int largeurHaut,int largeurBas,Pixel couleur)
{
    Coordonnee coor;
    coor.ligne=0;coor.colonne=0;
    Colorie(coor,largeurGauche,hauteur,couleur);
    Colorie(coor,largeur,largeurHaut,couleur);
    coor.ligne=0;coor.colonne=largeur-largeurDroite;
    Colorie(coor,largeurDroite,hauteur,couleur);
    coor.ligne=hauteur-largeurBas;coor.colonne=0;
    Colorie(coor,largeur,largeurBas,couleur);
}


void SNImage::Recadre(Coordonnee coord,int l,int h)
{ if((coord.ligne+h<=hauteur)&&(coord.colonne+l<=largeur))
 { for(int i=0;i<h;i++)
 for(int j=0;j<l;j++)
 { image[i][j]=image[i+coord.ligne][j+coord.colonne];
 }
 largeur=l;hauteur=h;
 int k=(4-(largeur*3)%4)%4;
 tailleImage=(largeur+k)*(hauteur)*3;
 tailleFichier=tailleImage+offset;
 }
}

Coordonnee SNImage::RechercheZone(int l,int h,Pixel couleur)
{ Coordonnee coord={-1,-1};
 int i,j,m,n;
 bool zoneTrouve;
 for(int i=0;i<hauteur;i++)
 { for(int j=0;j<largeur;j++)
 { if(image[i][j].bleu==couleur.bleu && image[i][j].vert==couleur.vert && image[i][j].rouge==couleur.rouge)
 { zoneTrouve=true;
 for(int m=0;m<h;m++) for(int n=0;n<l;n++)
 { if(image[i+m][j+n].bleu!=couleur.bleu || image[i+m][j+n].vert!=couleur.vert || image[i+m][j+n].rouge!=couleur.rouge)
 zoneTrouve=false;
 }
 }
 if(zoneTrouve)
 { coord.ligne=i; coord.colonne=j;
 i=hauteur; j=largeur;
 }
 }
 } return coord;
}

void SNImage::DessineCroix(Coordonnee coord,int taille,int epaisseur,Pixel couleur)
{
    Coordonnee debut;
    debut.ligne=coord.ligne-taille/2;debut.colonne=coord.colonne-epaisseur/2;
    if(debut.ligne<0) debut.ligne=0;
    if(debut.colonne<0) debut.colonne=0;
    Colorie(debut,epaisseur,taille,couleur);
    debut.ligne=coord.ligne-epaisseur/2;debut.colonne=coord.colonne-taille/2;
    if(debut.ligne<0) debut.ligne=0;
    if(debut.colonne<0) debut.colonne=0;
    Colorie(debut,taille,epaisseur,couleur);
}

void SNImage::DessineCarre(Coordonnee coord,int taille,int epaisseur,Pixel couleur)
{
    Coordonnee debut=coord;
    debut.ligne-=epaisseur/2; debut.colonne-=epaisseur/2;
    Colorie(debut,epaisseur,taille,couleur);//f ou e
    Colorie(debut,taille,epaisseur,couleur);//a
    debut.colonne+=taille;
    Colorie(debut,epaisseur,taille,couleur);//b ou c
    debut=coord;
    debut.ligne-=epaisseur/2; debut.colonne-=epaisseur/2;
    debut.ligne+=taille;
    Colorie(debut,taille+epaisseur,epaisseur,couleur);/*g ou d*/
}

void SNImage::ChangeCouleurSegment(Coordonnee coord,int taille,int epaisseur,Pixel couleur,char segment)//segment='a' 'b' 'c' 'd' 'e' 'f'
{ Coordonnee debut;
 switch (segment)
 { case 'a': debut=coord; debut.ligne-=epaisseur/2; debut.colonne-=epaisseur/2; Colorie(debut,taille/2+epaisseur,epaisseur,couleur); break;
 case 'b': debut=coord; debut.ligne-=epaisseur/2; debut.colonne-=epaisseur/2; debut.colonne+=taille/2; Colorie(debut,epaisseur,taille/2+epaisseur,couleur);
 break;
 case 'f': debut=coord; debut.ligne-=epaisseur/2; debut.colonne-=epaisseur/2; Colorie(debut,epaisseur,taille/2+epaisseur,couleur); break;
 case 'g': debut=coord; debut.ligne-=epaisseur/2; debut.colonne-=epaisseur/2; debut.ligne+=taille/2; Colorie(debut,taille/2+epaisseur,epaisseur,couleur); break;
 case 'c': debut=coord; debut.ligne-=epaisseur/2-taille/2; debut.colonne-=epaisseur/2; debut.colonne+=taille/2;
 Colorie(debut,epaisseur,taille/2+epaisseur,couleur); break;
 case 'e': debut=coord; debut.ligne-=epaisseur/2-taille/2; debut.colonne-=epaisseur/2; Colorie(debut,epaisseur,taille/2+epaisseur,couleur); break;
 case 'd': debut=coord; debut.ligne-=epaisseur/2-taille/2; debut.colonne-=epaisseur/2; debut.ligne+=taille/2; Colorie(debut,taille/2+epaisseur,epaisseur,couleur);
 break;
 }
}

void SNImage::Dessine7Segments(Coordonnee coord,int taille,int epaisseur,Pixel couleur,string message)
{ int ligneDepart=coord.ligne;
 for(int i=0;i<message.length();i++)
 { coord.ligne=ligneDepart;
 switch (message.c_str()[i])
 { case '0': case 'O':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'a');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'b');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'c');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'d');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'e');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'f'); break;
 case '1': case 'I':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'b');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'c'); break;
 case '2':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'a');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'b');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'d');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'e');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'g'); break;
 case '3':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'a');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'b');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'c');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'d');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'g'); break;
 case '4':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'b');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'c');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'f');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'g'); break;
 case '5': case 's': case 'S':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'a');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'c');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'d');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'f');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'g'); break;
 case '6':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'a');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'c');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'d');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'e');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'f');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'g'); break;
 case '7':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'a');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'b');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'c'); break;
 case '8': case 'B':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'a');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'b');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'c');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'d');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'e');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'f');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'g'); break;
 case '9':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'a');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'b');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'c');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'d');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'f');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'g'); break;
 case 'p': case 'P':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'a');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'b');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'e');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'f');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'g'); break;
 case 'o':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'c');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'d');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'e');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'g'); break;
 case 'i':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'c'); break;
 case 'n':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'c');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'e');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'g'); break;
 case 't':
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'d');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'e');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'f');
 ChangeCouleurSegment(coord,taille,epaisseur,couleur,'g'); break;
 }
 coord.colonne+=2*(taille/2);
 coord.ligne=ligneDepart;
 }
}

void SNImage::Negatif()
{
    for(unsigned int i = 0; i < hauteur; i++)
    {
        for(unsigned int j = 0; j < largeur; j++)
        {
            image[i][j].bleu = 255 - image[i][j].bleu;
            image[i][j].vert = 255 - image[i][j].vert;
            image[i][j].rouge = 255 - image[i][j].rouge;
        }
    }
}

void SNImage::RetourneHorizontal()
{
    for(unsigned int i = 0; i < hauteur; i++)
    {
        for(unsigned int j = 0; j < largeur/2; j++)
        {
            Pixel t = image[i][j];
            image[i][j] = image[i][largeur - 1 - j];
            image[i][largeur-j] = t;
        }
    }

}

void SNImage::RetourneVertical()
{
    for(unsigned int i = 0; i < hauteur/2; i++)
    {
        for(unsigned int j = 0; j < largeur; j++)
        {
            Pixel t = image[i][j];
            image[i][j] = image[hauteur - 1 - i][j];
            image[hauteur-i][j] = t;
        }
    }

}

void SNImage::NiveauGris()
{
    for(unsigned int i = 0; i < hauteur; i++)
    {
        for(unsigned int j = 0; j < largeur; j++)
        {
           int ass;
           ass = (image[i][j].bleu + image[i][j].vert + image[i][j].rouge);
           image[i][j].bleu = ass / 3;
           image[i][j].vert = ass / 3 ;
           image[i][j].rouge = ass / 3;
        }
    }
}

void SNImage::Tourne90Droite()
{
    unsigned long temp;
    for(unsigned int i = 0; i < hauteur; i++)
    {
        for (unsigned int j = 0; j < largeur; j++)
        {
            image2[j][hauteur - 1 - i] = image[i][j];

            image = image2;
            delete image;
            temp = hauteur;
            hauteur = largeur;
            largeur = temp;

        }
    }
}
