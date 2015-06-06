#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define NOIR  0
#define BLANC  255
#define OK  printf("OK");

// int matrix[10][10][10] = {{{0}}};

struct Entete_Fichier fichierEntete;
struct Entete_Info_Image imageEnteteInfo;
typedef struct Pixel_RGB RGB;

void Lire_Bitmap_Header(char* nomFichierSource);
void Lire_Bitmap_RGB(char* nomFichierSource, RGB **ptrTab2DPixel);
void Ecrire_Bitmap(char* BITMAP, RGB** ptrTab2DPixel);
void Vider_Tableau_RGB(RGB** ptrTab2DPixel);
void FreeMatrice(RGB** ptrTab2DPixel);

void Convolution(RGB** ptrTab2DPixel, RGB** ptrTab2DPixelX, RGB** ptrTab2DPixelY);
//void Flou(RGB** ptrTab2DPixel, RGB** ptrTab2DPixelF)
void Seuillage(RGB** ptrTab2DPixel, int value);
void Negatif(RGB** ptrTab2DPixel);
void RemplirMatrice(int Matrice[3][3], int f);

int MatriceX[3][3] = {{0}};
int MatriceY[3][3] = {{0}};
/*
int MatriceX[3][3] ={{ 0,   0,   0},
                     { 0,   0,   0},
                     { 0,   0,   0}};

int MatriceY[3][3] ={{ 0,   0,   0},
                     { 0,   0,   0},
                     { 0,   0,   0}};
*/
int MatriceF[3][3] ={{ 1,   2,   1},
                     { 2,   4,   2},
                     { 1,   2,   1}};



#pragma pack(1)
struct Entete_Fichier{

  short Type;
  int Taille;
  int Reserve;
  int Offset;
};

struct Entete_Info_Image{
  int Taille_Entete;
  int Largeur, Hauteur;
  short Plans;
  short Bits_Par_Pixel;
  int Compression;
  int Taille_Image;
  int X_Resolution, Y_Resolution;
  int Nombre_Couleurs;
  int Couleurs_Importantes;
};
#pragma pack(pop)
#pragma pack(1)
struct Pixel_RGB {
	short R;
    short G;
    short B;
};
#pragma pack(pop)

#endif
