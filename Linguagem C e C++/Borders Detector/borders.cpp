/************************************************************************************************************************
                                Universidade Federal de Santa Catarina (UFSC)
                                    Departamento de Computação (DEC)
                    Disciplina: Processamento Digital de Imagens e Visão Computacional
                                        Docente: Antônio Sobieranski
                                Discente: Vinícius Rodrigues Zanon - 15102833

    Objetivos: Implementação de Detectores de Borda. O detector de bordas consiste em fazer a operação de
    convolução simples. Neste programa são utilizados os operadores de Roberts (2 - kernel 3x3), Sobel
    (2 - kernel 2x2) e Robinson (8 - kernel 3x3).

***********************************************************************************************************************/


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

unsigned char* readPGM(const char* filename, unsigned int &W, unsigned int &H);
bool savePGM(const char* filename, unsigned char* data, unsigned int W, unsigned int H);

//Kernel para Sobel
int kernelSobelx[3][3] = {1,2,1,
                          0,0,0,
                         -1,-2,-1};

int kernelSobely[3][3] = {1,0,-1,
                          2,0,-2,
                          1,0,-1};
//Kernel para Roberts
int kernelRobertsx[3][3] = {0,0,0,
                            0,1,0,
                            0,0,-1};

int kernelRobertsy[3][3] = {0,0,0,
                            0,0,1,
                            0,-1,0};
//Kernel para Robinson
int kernelRobinsonx[3][3] = {1,2,1,
                             0,0,0,
                             -1,-2,-1};

int kernelRobinsony[3][3] = {1,0,-1,
                             2,0,-2,
                             1,0,-1};

int kernelRobinsonDP[3][3] = {2,1,0,
                              1,0,-1,
                              0,-1,-2};

int kernelRobinsonDS[3][3] = {0,-1,-2,
                              1,0,-1,
                              2,1,0};
//Convolution Sobel
int* Sconvolvex(unsigned char* data, int* impulse, unsigned int W, unsigned int H);
int* Sconvolvey(unsigned char* data, int* impulse, unsigned int W, unsigned int H);
//Convolution Roberts
int* Rconvolvex(unsigned char* data, int* impulse, unsigned int W, unsigned int H);
int* Rconvolvey(unsigned char* data, int* impulse, unsigned int W, unsigned int H);
//Convolution Robinson
int* Robsconvolvex(unsigned char* data, int* impulse, unsigned int W, unsigned int H);
int* Robsconvolvey(unsigned char* data, int* impulse, unsigned int W, unsigned int H);
int* RobsconvolveDP(unsigned char* data, int* impulse, unsigned int W, unsigned int H);
int* RobsconvolveDS(unsigned char* data, int* impulse, unsigned int W, unsigned int H);
int* RobsconvolvexI(unsigned char* data, int* impulse, unsigned int W, unsigned int H);
int* RobsconvolveyI(unsigned char* data, int* impulse, unsigned int W, unsigned int H);
int* RobsconvolveDPI(unsigned char* data, int* impulse, unsigned int W, unsigned int H);
int* RobsconvolveDSI(unsigned char* data, int* impulse, unsigned int W, unsigned int H);

unsigned char* filter_sobel(int* result1, int* result2, unsigned int W, unsigned int H);
unsigned char* filter_roberts(int* result4, int* result5, unsigned int W, unsigned int H);
unsigned char* filter_robinson(int* result7, int* result8, int* result9, int* result10, int* result11, int* result12, int* result13, int* result14, unsigned int W, unsigned int H);

int main(int argc, const char* argv[]){
    if(argc != 2)
    {
            cout << "Error !  Input should be <exec> <inputfile pgm>" << endl;
            return 1;
    }

    unsigned int W, H=W=0;
    unsigned char *data=readPGM(argv[1], W,H);
    int *impulse = new int[W*H];

    if(data==NULL)
    {
            cout << "Error !  File not found" << endl;
            return 2;
    }


    //SOBEL
    int* result1 = Sconvolvex(data, impulse, W, H);
    int* result2 = Sconvolvey(data, impulse, W, H);
    unsigned char* result3 = filter_sobel(result1,result2, W, H);
    savePGM("Sobel.pgm", result3, W, H);

    //ROBERTS
    int* result4 = Rconvolvex(data, impulse, W, H);
    int* result5 = Rconvolvey(data, impulse, W, H);
    unsigned char* result6 = filter_roberts(result4,result5, W, H);
    savePGM("Roberts.pgm", result6, W, H);

    //ROBINSON
    int* result7 = Robsconvolvex(data,impulse,W,H);
    int* result8 = Robsconvolvey(data,impulse,W,H);
    int* result9 = RobsconvolveDP(data,impulse,W,H);
    int* result10 = RobsconvolveDS(data,impulse,W,H);
    int* result11 = RobsconvolvexI(data,impulse,W,H);
    int* result12 = RobsconvolveyI(data,impulse,W,H);
    int* result13 = RobsconvolveDPI(data,impulse,W,H);
    int* result14 = RobsconvolveDSI(data,impulse,W,H);
    unsigned char* result15 = filter_robinson(result7, result8, result9, result10, result11, result12, result13, result14, W,H);
    savePGM("Robinson.pgm", result15, W, H);

    system("pause");
    return 0;
}
unsigned char* readPGM(const char* filename, unsigned int &W, unsigned int &H){
    cout << "Reading image : " << filename << endl;
    char type[2];

    FILE* fp = fopen(filename, "r");
    if(!fp) return NULL;

    fscanf(fp, "%c", &type[0]);
    fscanf(fp, "%c", &type[1]);
    cout << "Type : " << type << endl;

    W=H=0;
    fscanf(fp, "%d %d 255", &W, &H);
    cout << "Image Dimensions: W: " << W << ", H: " << H << endl;

    unsigned char* data;
    data = new unsigned char[W*H];
    for(unsigned int i=0; i<W*H; i++)
    {
        fscanf(fp, "%d ", &data[i]);
    }
    fclose(fp);

    return data;
}
bool savePGM(const char* filename, unsigned char* data, unsigned int W, unsigned int H){
    cout << "Saving image : " << filename << endl;
    FILE* fp = fopen(filename, "w");
    if(!fp) return false;

    fprintf(fp, "P2\n");
    fprintf(fp, "%d %d 255\n", W, H);

    for(unsigned int i=0; i<W*H; i++)
    {
        fprintf(fp, "%d ", data[i]);
    }
    fclose(fp);

    return true;
}
int* Sconvolvex(unsigned char* data, int* impulse, unsigned int W, unsigned int H){
    int* result = new int[W*H];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;

            result[index]=((int) data[(i-1)*W+(j-1)]*kernelSobelx[0][0] +
                            (int) data[(i-1)*W+(j+0)]*kernelSobelx[0][1] +
                            (int) data[(i-1)*W+(j+1)]*kernelSobelx[0][2] +

                            (int) data[(i)*W+(j-1)]*kernelSobelx[1][0] +
                            (int) data[(i)*W+(j+0)]*kernelSobelx[1][1] +
                            (int) data[(i)*W+(j+1)]*kernelSobelx[1][2] +

                            (int) data[(i+1)*W+(j-1)]*kernelSobelx[2][0] +
                            (int) data[(i+1)*W+(j+0)]*kernelSobelx[2][1] +
                            (int) data[(i+1)*W+(j+1)]*kernelSobelx[2][2]);

           //result[index] = fabs(impulse[index]) <= 0 ? 0 : fabs(impulse[index]) > 255 ? 255 : fabs(impulse[index]);
        }
    }
    return result;
}
int* Sconvolvey(unsigned char* data, int* impulse, unsigned int W, unsigned int H){
    int* result = new int[W*H];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;

            result[index]=((int) data[(i-1)*W+(j-1)]*kernelSobely[0][0] +
                            (int) data[(i-1)*W+(j+0)]*kernelSobely[0][1] +
                            (int) data[(i-1)*W+(j+1)]*kernelSobely[0][2] +

                            (int) data[(i)*W+(j-1)]*kernelSobely[1][0] +
                            (int) data[(i)*W+(j+0)]*kernelSobely[1][1] +
                            (int) data[(i)*W+(j+1)]*kernelSobely[1][2] +

                            (int) data[(i+1)*W+(j-1)]*kernelSobely[2][0] +
                            (int) data[(i+1)*W+(j+0)]*kernelSobely[2][1] +
                            (int) data[(i+1)*W+(j+1)]*kernelSobely[2][2]);

           //result[index] = fabs(impulse[index]) <= 0 ? 0 : fabs(impulse[index]) > 255 ? 255 : fabs(impulse[index]);
        }
    }
    return result;
}
unsigned char* filter_sobel(int* result1, int* result2, unsigned int W, unsigned int H){

    unsigned char* result = new unsigned char[W*H];
    float aux = 0.0;
    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;
            aux = (float) sqrt(pow((result1[index]),2) + pow((result2[index]),2));
            result[index] = (aux <= 0 ? 0 : (aux > 255 ? 255 : aux));
        }
    }

    return result;

}
int* Rconvolvex(unsigned char* data, int* impulse, unsigned int W, unsigned int H){
    int* result = new int[W*H];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;

            result[index]=((int) data[(i-1)*W+(j-1)]*kernelRobertsx[0][0] +
                            (int) data[(i-1)*W+(j+0)]*kernelRobertsx[0][1] +
                            (int) data[(i-1)*W+(j+1)]*kernelRobertsx[0][2] +

                            (int) data[(i)*W+(j-1)]*kernelRobertsx[1][0] +
                            (int) data[(i)*W+(j+0)]*kernelRobertsx[1][1] +
                            (int) data[(i)*W+(j+1)]*kernelRobertsx[1][2] +

                            (int) data[(i+1)*W+(j-1)]*kernelRobertsx[2][0] +
                            (int) data[(i+1)*W+(j+0)]*kernelRobertsx[2][1] +
                            (int) data[(i+1)*W+(j+1)]*kernelRobertsx[2][2]);
        }
    }

    return result;
}
int* Rconvolvey(unsigned char* data, int* impulse, unsigned int W, unsigned int H){
    int* result = new int[W*H];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;

            result[index]=((int) data[(i-1)*W+(j-1)]*kernelRobertsy[0][0] +
                            (int) data[(i-1)*W+(j+0)]*kernelRobertsy[0][1] +
                            (int) data[(i-1)*W+(j+1)]*kernelRobertsy[0][2] +

                            (int) data[(i)*W+(j-1)]*kernelRobertsy[1][0] +
                            (int) data[(i)*W+(j+0)]*kernelRobertsy[1][1] +
                            (int) data[(i)*W+(j+1)]*kernelRobertsy[1][2] +

                            (int) data[(i+1)*W+(j-1)]*kernelRobertsy[2][0] +
                            (int) data[(i+1)*W+(j+0)]*kernelRobertsy[2][1] +
                            (int) data[(i+1)*W+(j+1)]*kernelRobertsy[2][2]);

        }
    }

    return result;
}
unsigned char* filter_roberts(int* result4, int* result5, unsigned int W, unsigned int H){

    unsigned char* result = new unsigned char[W*H];
    float aux = 0.0;

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;
                aux = (float) sqrt(pow((result4[index]),2) + pow((result5[index]),2));
                result[index] = (aux <= 0 ? 0 : (aux > 255 ? 255 : aux));

        }
    }

    return result;
}
int* Robsconvolvex(unsigned char* data, int* impulse, unsigned int W, unsigned int H){
    int* result = new int[W*H];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;

            result[index]=((int) data[(i-1)*W+(j-1)]*kernelRobinsonx[0][0] +
                            (int) data[(i-1)*W+(j+0)]*kernelRobinsonx[0][1] +
                            (int) data[(i-1)*W+(j+1)]*kernelRobinsonx[0][2] +

                            (int) data[(i)*W+(j-1)]*kernelRobinsonx[1][0] +
                            (int) data[(i)*W+(j+0)]*kernelRobinsonx[1][1] +
                            (int) data[(i)*W+(j+1)]*kernelRobinsonx[1][2] +

                            (int) data[(i+1)*W+(j-1)]*kernelRobinsonx[2][0] +
                            (int) data[(i+1)*W+(j+0)]*kernelRobinsonx[2][1] +
                            (int) data[(i+1)*W+(j+1)]*kernelRobinsonx[2][2]);

        }
    }

    return result;
}
int* RobsconvolvexI(unsigned char* data, int* impulse, unsigned int W, unsigned int H){
    int* result = new int[W*H];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;

            result[index]=((int) data[(i-1)*W+(j-1)]*(-1)*kernelRobinsonx[0][0] +
                            (int) data[(i-1)*W+(j+0)]*(-1)*kernelRobinsonx[0][1] +
                            (int) data[(i-1)*W+(j+1)]*(-1)*kernelRobinsonx[0][2] +

                            (int) data[(i)*W+(j-1)]*(-1)*kernelRobinsonx[1][0] +
                            (int) data[(i)*W+(j+0)]*(-1)*kernelRobinsonx[1][1] +
                            (int) data[(i)*W+(j+1)]*(-1)*kernelRobinsonx[1][2] +

                            (int) data[(i+1)*W+(j-1)]*(-1)*kernelRobinsonx[2][0] +
                            (int) data[(i+1)*W+(j+0)]*(-1)*kernelRobinsonx[2][1] +
                            (int) data[(i+1)*W+(j+1)]*(-1)*kernelRobinsonx[2][2]);

        }
    }

    return result;
}
int* Robsconvolvey(unsigned char* data, int* impulse, unsigned int W, unsigned int H){
int* result = new int[W*H];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;

             result[index]=((int) data[(i-1)*W+(j-1)]*kernelRobinsony[0][0] +
                            (int) data[(i-1)*W+(j+0)]*kernelRobinsony[0][1] +
                            (int) data[(i-1)*W+(j+1)]*kernelRobinsony[0][2] +

                            (int) data[(i)*W+(j-1)]*kernelRobinsony[1][0] +
                            (int) data[(i)*W+(j+0)]*kernelRobinsony[1][1] +
                            (int) data[(i)*W+(j+1)]*kernelRobinsony[1][2] +

                            (int) data[(i+1)*W+(j-1)]*kernelRobinsony[2][0] +
                            (int) data[(i+1)*W+(j+0)]*kernelRobinsony[2][1] +
                            (int) data[(i+1)*W+(j+1)]*kernelRobinsony[2][2]);

        }
    }

    return result;
}
int* RobsconvolveyI(unsigned char* data, int* impulse, unsigned int W, unsigned int H){
int* result = new int[W*H];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;

             result[index]=((int) data[(i-1)*W+(j-1)]*(-1)*kernelRobinsony[0][0] +
                            (int) data[(i-1)*W+(j+0)]*(-1)*kernelRobinsony[0][1] +
                            (int) data[(i-1)*W+(j+1)]*(-1)*kernelRobinsony[0][2] +

                            (int) data[(i)*W+(j-1)]*(-1)*kernelRobinsony[1][0] +
                            (int) data[(i)*W+(j+0)]*(-1)*kernelRobinsony[1][1] +
                            (int) data[(i)*W+(j+1)]*(-1)*kernelRobinsony[1][2] +

                            (int) data[(i+1)*W+(j-1)]*(-1)*kernelRobinsony[2][0] +
                            (int) data[(i+1)*W+(j+0)]*(-1)*kernelRobinsony[2][1] +
                            (int) data[(i+1)*W+(j+1)]*(-1)*kernelRobinsony[2][2]);

        }
    }

    return result;
}
int* RobsconvolveDP(unsigned char* data, int* impulse, unsigned int W, unsigned int H){

   int* result = new int[W*H];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;

             result[index]=((int) data[(i-1)*W+(j-1)]*kernelRobinsonDP[0][0] +
                            (int) data[(i-1)*W+(j+0)]*kernelRobinsonDP[0][1] +
                            (int) data[(i-1)*W+(j+1)]*kernelRobinsonDP[0][2] +

                            (int) data[(i)*W+(j-1)]*kernelRobinsonDP[1][0] +
                            (int) data[(i)*W+(j+0)]*kernelRobinsonDP[1][1] +
                            (int) data[(i)*W+(j+1)]*kernelRobinsonDP[1][2] +

                            (int) data[(i+1)*W+(j-1)]*kernelRobinsonDP[2][0] +
                            (int) data[(i+1)*W+(j+0)]*kernelRobinsonDP[2][1] +
                            (int) data[(i+1)*W+(j+1)]*kernelRobinsonDP[2][2]);
        }
    }

    return result;
}
int* RobsconvolveDPI(unsigned char* data, int* impulse, unsigned int W, unsigned int H){

    int* result = new int[W*H];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;

             result[index]=((int) data[(i-1)*W+(j-1)]*(-1)*kernelRobinsonDP[0][0] +
                            (int) data[(i-1)*W+(j+0)]*(-1)*kernelRobinsonDP[0][1] +
                            (int) data[(i-1)*W+(j+1)]*(-1)*kernelRobinsonDP[0][2] +

                            (int) data[(i)*W+(j-1)]*(-1)*kernelRobinsonDP[1][0] +
                            (int) data[(i)*W+(j+0)]*(-1)*kernelRobinsonDP[1][1] +
                            (int) data[(i)*W+(j+1)]*(-1)*kernelRobinsonDP[1][2] +

                            (int) data[(i+1)*W+(j-1)]*(-1)*kernelRobinsonDP[2][0] +
                            (int) data[(i+1)*W+(j+0)]*(-1)*kernelRobinsonDP[2][1] +
                            (int) data[(i+1)*W+(j+1)]*(-1)*kernelRobinsonDP[2][2]);
        }
    }

    return result;
}
int* RobsconvolveDS(unsigned char* data, int* impulse, unsigned int W, unsigned int H){
   int* result = new int[W*H];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;

             result[index]=((int) data[(i-1)*W+(j-1)]*kernelRobinsonDS[0][0] +
                            (int) data[(i-1)*W+(j+0)]*kernelRobinsonDS[0][1] +
                            (int) data[(i-1)*W+(j+1)]*kernelRobinsonDS[0][2] +

                            (int) data[(i)*W+(j-1)]*kernelRobinsonDS[1][0] +
                            (int) data[(i)*W+(j+0)]*kernelRobinsonDS[1][1] +
                            (int) data[(i)*W+(j+1)]*kernelRobinsonDS[1][2] +

                            (int) data[(i+1)*W+(j-1)]*kernelRobinsonDS[2][0] +
                            (int) data[(i+1)*W+(j+0)]*kernelRobinsonDS[2][1] +
                            (int) data[(i+1)*W+(j+1)]*kernelRobinsonDS[2][2]);

        }
    }

    return result;
}
int* RobsconvolveDSI(unsigned char* data, int* impulse, unsigned int W, unsigned int H){
    int* result = new int[W*H];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;

             result[index]=((int) data[(i-1)*W+(j-1)]*(-1)*kernelRobinsonDS[0][0] +
                            (int) data[(i-1)*W+(j+0)]*(-1)*kernelRobinsonDS[0][1] +
                            (int) data[(i-1)*W+(j+1)]*(-1)*kernelRobinsonDS[0][2] +

                            (int) data[(i)*W+(j-1)]*(-1)*kernelRobinsonDS[1][0] +
                            (int) data[(i)*W+(j+0)]*(-1)*kernelRobinsonDS[1][1] +
                            (int) data[(i)*W+(j+1)]*(-1)*kernelRobinsonDS[1][2] +

                            (int) data[(i+1)*W+(j-1)]*(-1)*kernelRobinsonDS[2][0] +
                            (int) data[(i+1)*W+(j+0)]*(-1)*kernelRobinsonDS[2][1] +
                            (int) data[(i+1)*W+(j+1)]*(-1)*kernelRobinsonDS[2][2]);

        }
    }

    return result;
}
unsigned char* filter_robinson(int* result7, int* result8, int* result9, int* result10, int* result11, int* result12, int* result13, int* result14, unsigned int W, unsigned int H){

unsigned char* result = new unsigned char[W*H];
float aux = 0.0;

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W+j;
                aux = (float) (sqrt(pow((result7[index]),2) + pow((result8[index]),2)+ pow((result9[index]),2) + pow((result10[index]),2) + pow((result11[index]),2) + pow((result12[index]),2) + pow((result13[index]),2)+ pow((result14[index]),2)));
                result[index] = (aux <= 0 ? 0 : (aux > 255 ? 255 : aux));

        }
    }

    return result;
}
