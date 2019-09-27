#ifndef SNIMAGE_H
#define SNIMAGE_H
#include <iostream>
#include <stdio.h>
#include "snimage.h"
using namespace std;

typedef struct {
    unsigned char bleu, vert, rouge;
} Pixel;

typedef struct {
    int ligne, colonne;
} Coordonnee;

class SNImage
{
private: unsigned long dimensionMax;
unsigned long tailleFichier,offset;
unsigned long tailleImage,tailleTete,zero,format,resoLarg,resoHaut;
void ChangeCouleurSegment(Coordonnee coord,int taille,int epaisseur,Pixel couleur,char segment);
//segment='a' 'b' 'c' 'd' 'e' 'f';
protected:
Pixel **image;
Pixel **image2;
unsigned short sign;
unsigned long largeur,hauteur;
public: SNImage();
~SNImage();
inline unsigned short Signature(){return sign;}
inline unsigned long Largeur(){return largeur;}
inline unsigned long Hauteur(){return hauteur;}
inline unsigned long TailleFichier(){return tailleFichier;}
inline unsigned long Offset(){return offset;}
inline unsigned long TailleImage(){return tailleImage;}
inline unsigned long TailleEntete(){return tailleTete;}
inline unsigned long Format(){return format;}
inline unsigned long ResolutionLargeur(){return resoLarg;}
inline unsigned long ResolutionHauteur(){return resoHaut;}
void Chargement(string nomFichier);
void Sauvegarde(string nomFichier);
void Tourne90Droite();
void Tourne90Gauche();
void Tourne180();
void Negatif();
void RetourneHorizontal();
void RetourneVertical();
void NiveauGris();
void Eclaircit(int niveau);
void Assombrit(int niveau);
void SeuilleNoirBlanc(int niveau);
void Colorie(Coordonnee coord,int l,int h,Pixel couleur);
void Detoure(int largeurGauche,int largeurDroite,int largeurHaut,int largeurBas,Pixel couleur);
void Recadre(Coordonnee coord,int l,int h);
Coordonnee RechercheZone(int l,int h,Pixel couleur);
void DessineCroix(Coordonnee coord,int taille,int epaisseur,Pixel couleur);
void DessineCarre(Coordonnee coord,int taille,int epaisseur,Pixel couleur);
void Dessine7Segments(Coordonnee coord,int taille,int epaisseur,Pixel couleur,string message);
};

#endif // SNIMAGE_H
