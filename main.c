#include "header.h"

int main()
{

    int i, value;
    char nomBitmap[20];
    char input[20];

    RGB **ptrTab2DPixel  = NULL;
    RGB **ptrTab2DPixelX = NULL;
    RGB **ptrTab2DPixelY = NULL;
    RGB **ptrTab2DPixelF = NULL;

    system("dir /b *.bmp");
    printf("\n\n");

    printf("Nom du fichier bitmap : ");
    scanf("%s", nomBitmap);
    strcat(nomBitmap,".bmp");

    Lire_Bitmap_Header(nomBitmap);

    ptrTab2DPixel = (RGB**)malloc(imageEnteteInfo.Hauteur * sizeof(RGB *));
    for (i = 0; i < imageEnteteInfo.Hauteur; ++i)
        ptrTab2DPixel[i] = (RGB*)malloc(imageEnteteInfo.Largeur * sizeof(RGB));

    ptrTab2DPixelX = (RGB**)malloc(imageEnteteInfo.Hauteur * sizeof(RGB *));
    for (i = 0; i < imageEnteteInfo.Hauteur; ++i)
        ptrTab2DPixelX[i] = (RGB*)malloc(imageEnteteInfo.Largeur * sizeof(RGB));

    ptrTab2DPixelY = (RGB**)malloc(imageEnteteInfo.Hauteur * sizeof(RGB *));
    for (i = 0; i < imageEnteteInfo.Hauteur; ++i)
        ptrTab2DPixelY[i] = (RGB*)malloc(imageEnteteInfo.Largeur * sizeof(RGB));

    ptrTab2DPixelF = (RGB**)malloc(imageEnteteInfo.Hauteur * sizeof(RGB *));
    for (i = 0; i < imageEnteteInfo.Hauteur; ++i)
        ptrTab2DPixelF[i] = (RGB*)malloc(imageEnteteInfo.Largeur * sizeof(RGB));

    Vider_Tableau_RGB(ptrTab2DPixel);
    Vider_Tableau_RGB(ptrTab2DPixelX);
    Vider_Tableau_RGB(ptrTab2DPixelY);

    Lire_Bitmap_RGB(nomBitmap, ptrTab2DPixel);

    jmpstart:
        printf("Remplir manuellement les matrices de convolution ? (y/n) : \n");
        scanf("%s", input);
        printf("\n\n");
        if(strcmp(input, "y") == 0){
            RemplirMatrice(MatriceX, 1);
            RemplirMatrice(MatriceY, 2);
        }
        else if(strcmp(input, "n") == 0){
            goto jmpf;
        }
        else if(strcmp(input, "y") != 0 && strcmp(input, "n") != 0){
            printf("Repondre par y ou n !\n\n");
            goto jmpstart;
        }

    jmpf:
        printf("Appliquer un flou gaussien ? (y/n) : \n");
        scanf("%s", input);
        printf("\n\n");
        if(strcmp(input, "y") == 0){
            Flou(ptrTab2DPixel, ptrTab2DPixelF);
        }
        else if(strcmp(input, "n") == 0){
            goto jmp;
        }
        else if(strcmp(input, "y") != 0 && strcmp(input, "n") != 0){
            printf("Repondre par y ou n !\n\n");
            goto jmpf;
        }

    jmp:
        printf("Appliquer un seuillage ? (y/n) : \n");
        scanf("%s", input);
        printf("\n\n");
        if(strcmp(input, "y") == 0){
            printf("\nValeur du seuillage (entre 0 et 255):\n");
            scanf("%d", &value);
            if(value < 0 || value > 255){
                printf("La valeur doit être comprise entre 0 et 255")
                goto jmp:
            }
            printf("\n\n");
            Seuillage(ptrTab2DPixel, value);
        }
        else if(strcmp(input, "n") == 0){
            goto jmp2;
        }
        else if(strcmp(input, "y") != 0 && strcmp(input, "n") != 0){
            printf("Repondre par y ou n !\n\n");
            goto jmp;
        }

    jmp2:
        printf("Convoluer l'image ? (y/n) : \n");
        scanf("%s", input);
        printf("\n\n");
        if(strcmp(input, "y") == 0){
            Convolution(ptrTab2DPixel, ptrTab2DPixelX, ptrTab2DPixelY);
        }
        else if(strcmp(input, "n") == 0){
            goto jmp3;
        }
        else if(strcmp(input, "y") != 0 && strcmp(input, "n") != 0){
            printf("Repondre par y ou n !\n\n");
            goto jmp2;
        }

    jmp3:
        printf("Appliquer un filtre negatif ? (y/n) : \n");
        scanf("%s", input);
        printf("\n\n");
        if(strcmp(input, "y") == 0){
            Negatif(ptrTab2DPixel);
        }
        else if(strcmp(input, "n") == 0){
            goto jmp4;
        }
        else if(strcmp(input, "y") != 0 && strcmp(input, "n") != 0){
            printf("Repondre par y ou n !\n\n");
            goto jmp3;
        }

    jmp4:
        printf("Nom du fichier de sortie ? : ");
        scanf("%s", nomBitmap);
        strcat(nomBitmap,".bmp");
        Ecrire_Bitmap(nomBitmap, ptrTab2DPixel);

        FreeMatrice(ptrTab2DPixel);
        FreeMatrice(ptrTab2DPixelX);
        FreeMatrice(ptrTab2DPixelY);

        system(nomBitmap);
}

void Lire_Bitmap_Header(char* nomFichierSource)
{
    FILE *ptrFichierEntree = fopen(nomFichierSource, "rb");

    fread(&fichierEntete, sizeof(fichierEntete), 1, ptrFichierEntree);
    fread(&imageEnteteInfo, sizeof(imageEnteteInfo), 1, ptrFichierEntree);

    if (fichierEntete.Type != 0x4D42)
    {
        printf("\nFormat non pris en charge, il faut un .bmp\n");
        fclose(ptrFichierEntree);
        system("pause");
        exit(0);
    }
    else
    {
        printf("************************************************************\n\n");
        printf("Fichier ouvert avec succes : %s\n\n", nomFichierSource);
        printf("************************************************************\n\n");
    }

    fclose(ptrFichierEntree);
}

void Lire_Bitmap_RGB(char* nomFichierSource, RGB **ptrTab2DPixel)
{
    int i, j;
    FILE *ptrFichierEntree = fopen(nomFichierSource, "rb");

    fseek(ptrFichierEntree, fichierEntete.Offset, SEEK_SET);
    for (i = 0; i < imageEnteteInfo.Hauteur; i++)
    {
        for (j = 0; j < imageEnteteInfo.Largeur; j++)
        {
            fread(&ptrTab2DPixel[i][j].R, 1, 1, ptrFichierEntree);
            fread(&ptrTab2DPixel[i][j].G, 1, 1, ptrFichierEntree);
            fread(&ptrTab2DPixel[i][j].B, 1, 1, ptrFichierEntree);
        }
    }
    fclose(ptrFichierEntree);
}

void Ecrire_Bitmap(char* BITMAP, RGB** ptrTab2DPixel)
{
    int i, j;
    FILE *ptrFichierSortie = fopen(BITMAP, "wb");

    fwrite(&fichierEntete, sizeof(fichierEntete), 1, ptrFichierSortie);
    fwrite(&imageEnteteInfo, sizeof(imageEnteteInfo), 1, ptrFichierSortie);

    for (i = 0; i < imageEnteteInfo.Hauteur; ++i)
    {

        for (j = 0; j < imageEnteteInfo.Largeur; ++j)
        {
            if (j < imageEnteteInfo.Largeur)
            {
                fwrite(&ptrTab2DPixel[i][j].R, 1, 1, ptrFichierSortie);
                fwrite(&ptrTab2DPixel[i][j].G, 1, 1, ptrFichierSortie);
                fwrite(&ptrTab2DPixel[i][j].B, 1, 1, ptrFichierSortie);
            }

        }
    }
    fclose(ptrFichierSortie);
}

void Seuillage(RGB** ptrTab2DPixel, int value)
{
    int i, j;

    for (i = 0; i < imageEnteteInfo.Hauteur; ++i)
    {
        for (j = 0; j < imageEnteteInfo.Largeur; ++j)
        {
            if (((ptrTab2DPixel[i][j].R + ptrTab2DPixel[i][j].G + ptrTab2DPixel[i][j].B) / 3) >= value)
            {
                ptrTab2DPixel[i][j].R = NOIR;
                ptrTab2DPixel[i][j].G = NOIR;
                ptrTab2DPixel[i][j].B = NOIR;
            }
            else
            {
                ptrTab2DPixel[i][j].R = BLANC;
                ptrTab2DPixel[i][j].G = BLANC;
                ptrTab2DPixel[i][j].B = BLANC;
            }
        }
    }
}

void Negatif(RGB** ptrTab2DPixel)
{
    int i, j;
    for (i = 0; i < imageEnteteInfo.Hauteur; i++)
    {
        for (j = 0; j < imageEnteteInfo.Largeur; j++)
        {
            ptrTab2DPixel[i][j].R = 255 - ptrTab2DPixel[i][j].R;
            ptrTab2DPixel[i][j].G = 255 - ptrTab2DPixel[i][j].G;
            ptrTab2DPixel[i][j].B = 255 - ptrTab2DPixel[i][j].B;
        }
    }
}

void Convolution(RGB** ptrTab2DPixel, RGB** ptrTab2DPixelX, RGB** ptrTab2DPixelY)
{
    int i, j, x, y;
    double sumX1 = 0.0, sumY1 = 0.0;
    double sumTotX = 0.0, sumTotY = 0.0;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            sumX1 += fabs(MatriceX[i][j]);
            sumY1 += fabs(MatriceY[i][j]);
        }
    }

    sumTotX = sumX1 ;
    sumTotY = sumY1 ;

    system("cls");

    for (i = 1; i < imageEnteteInfo.Hauteur - 1; i++)
    {
        for (j = 1; j < imageEnteteInfo.Largeur - 1; j++)
        {

                ptrTab2DPixelX[i][j].R =(int)floor(
                ptrTab2DPixel[i + 1][j - 1].R         *MatriceX[0][0] / sumTotX
                + ptrTab2DPixel[i + 1][j].R             *MatriceX[0][1] / sumTotX
                + ptrTab2DPixel[i + 1][j + 1].R         *MatriceX[0][2] / sumTotX
                + ptrTab2DPixel[i][j - 1].R             *MatriceX[1][0] / sumTotX
                + ptrTab2DPixel[i][j].R                 *MatriceX[1][1] / sumTotX
                + ptrTab2DPixel[i][j + 1].R             *MatriceX[1][2] / sumTotX
                + ptrTab2DPixel[i - 1][j - 1].R         *MatriceX[2][0] / sumTotX
                + ptrTab2DPixel[i - 1][j].R             *MatriceX[2][1] / sumTotX
                + ptrTab2DPixel[i - 1][j + 1].R         *MatriceX[2][2] / sumTotX);



            ptrTab2DPixelX[i][j].G =(int)floor(
                ptrTab2DPixel[i + 1][j - 1].G         *MatriceX[0][0] / sumTotX
                + ptrTab2DPixel[i + 1][j].G             *MatriceX[0][1] / sumTotX
                + ptrTab2DPixel[i + 1][j + 1].G         *MatriceX[0][2] / sumTotX
                + ptrTab2DPixel[i][j - 1].G             *MatriceX[1][0] / sumTotX
                + ptrTab2DPixel[i][j].G                 *MatriceX[1][1] / sumTotX
                + ptrTab2DPixel[i][j + 1].G             *MatriceX[1][2] / sumTotX
                + ptrTab2DPixel[i - 1][j - 1].G         *MatriceX[2][0] / sumTotX
                + ptrTab2DPixel[i - 1][j].G             *MatriceX[2][1] / sumTotX
                + ptrTab2DPixel[i - 1][j + 1].G         *MatriceX[2][2] / sumTotX);

            ptrTab2DPixelX[i][j].B =(int)floor(
                ptrTab2DPixel[i + 1][j - 1].B         *MatriceX[0][0] / sumTotX
                + ptrTab2DPixel[i + 1][j].B             *MatriceX[0][1] / sumTotX
                + ptrTab2DPixel[i + 1][j + 1].B         *MatriceX[0][2] / sumTotX
                + ptrTab2DPixel[i][j - 1].B             *MatriceX[1][0] / sumTotX
                + ptrTab2DPixel[i][j].B                 *MatriceX[1][1] / sumTotX
                + ptrTab2DPixel[i][j + 1].B             *MatriceX[1][2] / sumTotX
                + ptrTab2DPixel[i - 1][j - 1].B         *MatriceX[2][0] / sumTotX
                + ptrTab2DPixel[i - 1][j].B             *MatriceX[2][1] / sumTotX
                + ptrTab2DPixel[i - 1][j + 1].B         *MatriceX[2][2] / sumTotX);
        }
    }

    for (i = 1; i < imageEnteteInfo.Hauteur - 1; i++)
    {
        for (j = 1; j < imageEnteteInfo.Largeur - 1; j++)
        {

            ptrTab2DPixelY[i][j].R =(int)floor(
                ptrTab2DPixel[i + 1][j - 1].R         *MatriceY[0][0] / sumTotY
                + ptrTab2DPixel[i + 1][j].R             *MatriceY[0][1] / sumTotY
                + ptrTab2DPixel[i + 1][j + 1].R         *MatriceY[0][2] / sumTotY
                + ptrTab2DPixel[i][j - 1].R             *MatriceY[1][0] / sumTotY
                + ptrTab2DPixel[i][j].R                 *MatriceY[1][1] / sumTotY
                + ptrTab2DPixel[i][j + 1].R             *MatriceY[1][2] / sumTotY
                + ptrTab2DPixel[i - 1][j - 1].R         *MatriceY[2][0] / sumTotY
                + ptrTab2DPixel[i - 1][j].R             *MatriceY[2][1] / sumTotY
                + ptrTab2DPixel[i - 1][j + 1].R         *MatriceY[2][2] / sumTotY);

            ptrTab2DPixelY[i][j].G =(int)floor(
                ptrTab2DPixel[i + 1][j - 1].G         *MatriceY[0][0] / sumTotY
                + ptrTab2DPixel[i + 1][j].G             *MatriceY[0][1] / sumTotY
                + ptrTab2DPixel[i + 1][j + 1].G         *MatriceY[0][2] / sumTotY
                + ptrTab2DPixel[i][j - 1].G             *MatriceY[1][0] / sumTotY
                + ptrTab2DPixel[i][j].G                 *MatriceY[1][1] / sumTotY
                + ptrTab2DPixel[i][j + 1].G             *MatriceY[1][2] / sumTotY
                + ptrTab2DPixel[i - 1][j - 1].G         *MatriceY[2][0] / sumTotY
                + ptrTab2DPixel[i - 1][j].G             *MatriceY[2][1] / sumTotY
                + ptrTab2DPixel[i - 1][j + 1].G         *MatriceY[2][2] / sumTotY);

            ptrTab2DPixelY[i][j].B =(int)floor(
                ptrTab2DPixel[i + 1][j - 1].B         *MatriceY[0][0] / sumTotY
                + ptrTab2DPixel[i + 1][j].B             *MatriceY[0][1] / sumTotY
                + ptrTab2DPixel[i + 1][j + 1].B         *MatriceY[0][2] / sumTotY
                + ptrTab2DPixel[i][j - 1].B             *MatriceY[1][0] / sumTotY
                + ptrTab2DPixel[i][j].B                 *MatriceY[1][1] / sumTotY
                + ptrTab2DPixel[i][j + 1].B             *MatriceY[1][2] / sumTotY
                + ptrTab2DPixel[i - 1][j - 1].B         *MatriceY[2][0] / sumTotY
                + ptrTab2DPixel[i - 1][j].B             *MatriceY[2][1] / sumTotY
                + ptrTab2DPixel[i - 1][j + 1].B         *MatriceY[2][2] / sumTotY);
        }
    }


    for (x = 0; x < imageEnteteInfo.Hauteur; x++)
    {
        for (y = 0; y < imageEnteteInfo.Largeur; y++)
        {
            ptrTab2DPixel[x][y].R = (int)floor(sqrt((ptrTab2DPixelX[x][y].R)*(ptrTab2DPixelX[x][y].R) + (ptrTab2DPixelY[x][y].R)*(ptrTab2DPixelY[x][y].R)));
            ptrTab2DPixel[x][y].G = (int)floor(sqrt((ptrTab2DPixelX[x][y].G)*(ptrTab2DPixelX[x][y].G) + (ptrTab2DPixelY[x][y].G)*(ptrTab2DPixelY[x][y].G)));
            ptrTab2DPixel[x][y].B = (int)floor(sqrt((ptrTab2DPixelX[x][y].B)*(ptrTab2DPixelX[x][y].B) + (ptrTab2DPixelY[x][y].B)*(ptrTab2DPixelY[x][y].B)));
        }
    }
}

void Flou(RGB** ptrTab2DPixel, RGB** ptrTab2DPixelF){
    int i, j, x, y;
    double sumF1 = 0.0;
    double sumTotF = 0.0;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            sumF1 += fabs(MatriceF[i][j]);
        }
    }

    sumTotF = sumF1;

    for (i = 1; i < imageEnteteInfo.Hauteur - 1; i++)
    {
        for (j = 1; j < imageEnteteInfo.Largeur - 1; j++)
        {

            ptrTab2DPixelF[i][j].R =(int)floor(
                ptrTab2DPixel[i + 1][j - 1].R         *MatriceF[0][0] / sumTotF
                + ptrTab2DPixel[i + 1][j].R             *MatriceF[0][1] / sumTotF
                + ptrTab2DPixel[i + 1][j + 1].R         *MatriceF[0][2] / sumTotF
                + ptrTab2DPixel[i][j - 1].R             *MatriceF[1][0] / sumTotF
                + ptrTab2DPixel[i][j].R                 *MatriceF[1][1] / sumTotF
                + ptrTab2DPixel[i][j + 1].R             *MatriceF[1][2] / sumTotF
                + ptrTab2DPixel[i - 1][j - 1].R         *MatriceF[2][0] / sumTotF
                + ptrTab2DPixel[i - 1][j].R             *MatriceF[2][1] / sumTotF
                + ptrTab2DPixel[i - 1][j + 1].R         *MatriceF[2][2] / sumTotF);

            ptrTab2DPixelF[i][j].G =(int)floor(
                ptrTab2DPixel[i + 1][j - 1].G         *MatriceF[0][0] / sumTotF
                + ptrTab2DPixel[i + 1][j].G             *MatriceF[0][1] / sumTotF
                + ptrTab2DPixel[i + 1][j + 1].G         *MatriceF[0][2] / sumTotF
                + ptrTab2DPixel[i][j - 1].G             *MatriceF[1][0] / sumTotF
                + ptrTab2DPixel[i][j].G                 *MatriceF[1][1] / sumTotF
                + ptrTab2DPixel[i][j + 1].G             *MatriceF[1][2] / sumTotF
                + ptrTab2DPixel[i - 1][j - 1].G         *MatriceF[2][0] / sumTotF
                + ptrTab2DPixel[i - 1][j].G             *MatriceF[2][1] / sumTotF
                + ptrTab2DPixel[i - 1][j + 1].G         *MatriceF[2][2] / sumTotF);

            ptrTab2DPixelF[i][j].B =(int)floor(
                ptrTab2DPixel[i + 1][j - 1].B         *MatriceF[0][0] / sumTotF
                + ptrTab2DPixel[i + 1][j].B             *MatriceF[0][1] / sumTotF
                + ptrTab2DPixel[i + 1][j + 1].B         *MatriceF[0][2] / sumTotF
                + ptrTab2DPixel[i][j - 1].B             *MatriceF[1][0] / sumTotF
                + ptrTab2DPixel[i][j].B                 *MatriceF[1][1] / sumTotF
                + ptrTab2DPixel[i][j + 1].B             *MatriceF[1][2] / sumTotF
                + ptrTab2DPixel[i - 1][j - 1].B         *MatriceF[2][0] / sumTotF
                + ptrTab2DPixel[i - 1][j].B             *MatriceF[2][1] / sumTotF
                + ptrTab2DPixel[i - 1][j + 1].B         *MatriceF[2][2] / sumTotF);
        }
    }

    for (x = 0; x < imageEnteteInfo.Hauteur; x++)
    {
        for (y = 0; y < imageEnteteInfo.Largeur; y++)
        {
            ptrTab2DPixel[x][y].R = (int)floor(ptrTab2DPixelF[x][y].R);
            ptrTab2DPixel[x][y].G = (int)floor(ptrTab2DPixelF[x][y].G);
            ptrTab2DPixel[x][y].B = (int)floor(ptrTab2DPixelF[x][y].B);
        }
    }
}


void Vider_Tableau_RGB(RGB** ptrTab2DPixel)
{
    int i, j;

    for (i = 0; i < imageEnteteInfo.Hauteur; i++)
    {
        for (j = 0; j < imageEnteteInfo.Largeur; j++)
        {
            ptrTab2DPixel[i][j].R = BLANC;
            ptrTab2DPixel[i][j].G = BLANC;
            ptrTab2DPixel[i][j].B = BLANC;
        }
    }
}

void FreeMatrice(RGB** ptrTab2DPixel)
{
    int i;

    for (i = 0; i < imageEnteteInfo.Hauteur; i++)
        free(ptrTab2DPixel[i]);
    free(ptrTab2DPixel);
}

void RemplirMatrice(int Matrice[3][3], int f)
{
    int i = 0, j = 0, valeur , k = 0, l = 0;
/*
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            MatriceX[i][j] = 0;
            MatriceY[i][j] = 0;
        }
    }
*/
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if(f == 1)
                printf("Matrice X\n\n");
            else
                printf("Matrice Y\n\n");
            for (k = 0; k < 3; k++)
            {
                for (l = 0; l < 3; l++)
                {

                    printf("[%d]     ", Matrice[k][l]);
                    if(l == 2)
                        printf("\n");
                }
            }
            printf("\nValeur de <%d,%d>: ", i+1, j+1);

            scanf("%d", &valeur);
            Matrice[i][j] = valeur;

            if( i!= 2 || j != 2)
                system("cls");
        }
    }
    system("cls");
    for (k = 0; k < 3; k++)
    {
        for (l = 0; l < 3; l++)
        {
            printf("[%d]     ", Matrice[k][l]);
            if(l == 2)
                printf("\n");
        }
    }
    system("cls");
}
