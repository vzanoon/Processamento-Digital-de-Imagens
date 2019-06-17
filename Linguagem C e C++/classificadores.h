#ifndef CLASSIFICADORES_H
#define CLASSIFICADORES_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;
#define N 230

unsigned char* readPPM(const char* filename, unsigned int &W, unsigned int &H);
bool savePPM(const char* filename, unsigned char* data, unsigned int W, unsigned int H);

class Color
{
public:
    unsigned int m_r, m_g, m_b;
    Color() {};
    Color(unsigned int r, unsigned int g, unsigned int b) : m_r(r), m_g(g), m_b(b) {};
};

int cmpfunc (const void * a, const void * b);

//similarity functions T1
unsigned char* L1norm(unsigned char* data, unsigned int W, unsigned int H, unsigned int th, Color ref);
unsigned char* L2norm(unsigned char* data, unsigned int W, unsigned int H, unsigned int th, Color ref);
unsigned char* Mahalanobis(unsigned char* data, unsigned W, unsigned int H, float th);
unsigned char* K_means(unsigned char* data, unsigned int W, unsigned int H, unsigned int th, Color ref, int qtd_viz);

unsigned char* readPPM(const char* filename, unsigned int &W, unsigned int &H)
{
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
    data = new unsigned char[W*H*3];
    for(unsigned int i=0; i<W*H*3; i++)
    {
        fscanf(fp, "%d ", &data[i]);
    }
    fclose(fp);

    return data;
}

bool savePPM(const char* filename, unsigned char* data, unsigned int W, unsigned int H)
{
    cout << "Saving image : " << filename << endl;
    FILE* fp = fopen(filename, "w");
    if(!fp) return false;

    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d 255\n", W, H);

    for(unsigned int i=0; i<W*H*3; i++)
    {
        fprintf(fp, "%d ", data[i]);
    }
    fclose(fp);

    return true;
}

unsigned char* L1norm(unsigned char* data, unsigned int W, unsigned int H, unsigned int th, Color ref)
{   // Distância de Manhattan
    unsigned char* result = new unsigned char[W*H*3];

    for(unsigned int i=0; i<H; i++)
    {
        for(unsigned int j=0; j<W; j++)
        {
            unsigned int index = i*W*3+j*3;
            unsigned int r1 = (unsigned int) data[index+0];
            unsigned int g1 = (unsigned int) data[index+1];
            unsigned int b1 = (unsigned int) data[index+2];

            float de = abs(r1-ref.m_r)+ abs(g1-ref.m_g)+ abs(b1-ref.m_b);

            if(de<th)
            {
                result[index+0] = 0;
                result[index+1] = 0;
                result[index+2] = 0;
            }
            else
            {
                result[index+0] = 255;
                result[index+1] = 255;
                result[index+2] = 255;
            }
        }
    }
    return result;
}

unsigned char* L2norm(unsigned char* data, unsigned int W, unsigned int H, unsigned int th, Color ref)
{   // Distância Euclidiana
    unsigned char* result = new unsigned char[W*H*3];

    for(unsigned int i=0; i<H; i++)
    {
        for(unsigned int j=0; j<W; j++)
        {
            unsigned int index = i*W*3+j*3;
            unsigned int r1 = (unsigned int) data[index+0];
            unsigned int g1 = (unsigned int) data[index+1];
            unsigned int b1 = (unsigned int) data[index+2];

            float de = (r1-ref.m_r)*(r1-ref.m_r)+
                    (g1-ref.m_g)*(g1-ref.m_g)+
                    (b1-ref.m_b)*(b1-ref.m_b);
            de = sqrt(de);

            if(de<th)
            {
                result[index+0] = 0;
                result[index+1] = 0;
                result[index+2] = 0;
            }
            else
            {
                result[index+0] = 255;
                result[index+1] = 255;
                result[index+2] = 255;
            }
        }
    }
    return result;
}

unsigned char* Mahalanobis(unsigned char* data, unsigned int W, unsigned int H, float th)
{   // Distância de Mahalanobis

    float amostra[N][3]={
{193,116,105},
{207,125,113},
{184,82,93},
{182,73,83},
{186,68,82},
{176,68,75},
{181,70,79},
{195,97,93},
{191,85,95},
{189,89,104},
{181,78,90},
{177,74,95},
{163,66,81},
{162,69,84},
{163,64,82},
{148,60,82},
{148,60,82},
{150,68,81},
{136,61,82},
{141,68,89},
{143,71,95},
{146,76,104},
{128,73,104},
{126,71,101},
{147,71,97},
{146,62,87},
{155,69,85},
{163,75,95},
{159,58,76},
{155,57,81},
{162,62,83},
{167,64,83},
{175,75,88},
{174,76,85},
{202,99,106},
{195,94,97},
{191,90,96},
{176,63,72},
{192,67,84},
{185,68,79},
{189,68,77},
{202,85,97},
{188,68,84},
{196,70,90},
{187,57,81},
{205,77,96},
{205,78,96},
{205,77,97},
{194,65,86},
{202,58,91},
{190,68,86},
{193,72,89},
{200,80,96},
{200,81,97},
{207,84,99},
{203,79,97},
{193,76,98},
{180,67,88},
{181,73,84},
{185,73,92},
{190,78,99},
{197,90,106},
{200,93,112},
{202,96,116},
{181,81,107},
{180,84,111},
{170,83,112},
{163,88,113},
{164,90,114},
{161,88,111},
{168,84,111},
{189,87,111},
{198,94,114},
{188,77,95},
{186,60,89},
{194,74,95},
{211,84,102},
{205,83,98},
{226,99,110},
{227,97,108},
{217,99,101},
{234,108,113},
{223,101,102},
{218,99,102},
{233,114,122},
{231,105,108},
{220,106,106},
{218,104,102},
{222,112,106},
{236,126,120},
{230,122,116},
{223,111,108},
{218,107,102},
{217,105,100},
{216,95,96},
{211,85,91},
{209,78,90},
{202,79,81},
{194,67,76},
{209,83,91},
{193,75,76},
{208,107,104},
{221,102,103},
{217,92,99},
{235,96,107},
{251,108,115},
{250,98,119},
{255,93,113},
{255,129,143},
{255,135,142},
{252,164,163},
{247,152,153},
{249,136,143},
{254,98,121},
{254,87,110},
{253,81,104},
{255,98,116},
{254,115,131},
{253,118,134},
{244,149,150},
{253,144,149},
{252,110,128},
{240,92,118},
{254,99,129},
{249,72,106},
{253,57,87},
{248,47,83},
{254,43,86},
{251,56,90},
{248,32,79},
{249,39,77},
{254,41,85},
{254,55,92},
{247,34,76},
{253,55,91},
{234,65,84},
{248,98,110},
{236,100,110},
{217,101,107},
{205,93,101},
{192,91,96},
{184,83,96},
{177,74,91},
{172,68,83},
{178,73,89},
{171,66,87},
{177,69,89},
{171,72,90},
{178,82,94},
{176,80,92},
{174,66,87},
{171,73,83},
{178,72,95},
{180,72,97},
{175,66,88},
{170,71,91},
{174,78,89},
{172,75,91},
{165,71,89},
{156,68,82},
{160,77,92},
{156,72,88},
{167,80,95},
{164,80,95},
{161,73,90},
{152,72,87},
{155,75,90},
{148,70,88},
{142,68,94},
{139,76,97},
{131,67,90},
{127,67,92},
{127,69,92},
{133,75,96},
{132,74,98},
{140,85,107},
{138,78,97},
{130,75,94},
{131,73,95},
{126,73,98},
{135,77,96},
{147,88,104},
{159,96,108},
{161,90,101},
{175,102,109},
{173,94,103},
{178,97,104},
{185,98,107},
{191,106,111},
{195,112,116},
{203,114,120},
{195,106,112},
{201,112,118},
{199,109,111},
{202,110,113},
{200,106,107},
{210,107,113},
{219,112,113},
{224,114,116},
{226,114,113},
{236,121,126},
{238,128,129},
{243,131,130},
{225,110,108},
{234,117,114},
{232,109,113},
{226,106,105},
{228,114,116},
{225,122,115},
{235,142,137},
{224,123,123},
{213,114,109},
{237,131,126},
{239,126,116},
{252,134,125},
{255,148,129},
{250,34,86},
{255,101,128},
{250,39,80},
{254,29,81},
{249,39,77},
{250,40,77},
{254,29,81},
{247,46,81},
{248,47,83},
{255,44,88},
{243,46,82},
{247,46,81},
{249,44,81},
{252,43,82}};

    for(int i = 0; i<N; i++){
        for(int j = 0; j<3; j++){
            amostra[i][j] =  amostra[i][j];
        }
    }

    /*for(int i = 0; i<N; i++){
        for(int j = 0; j<3; j++){
            cout <<"["<< amostra[i][j]<<"] ";
        }
        cout<<endl;
    }*/


    float sum[3] = {0,0,0};
    float media[3] = {0,0,0};
    float cov[3][3] = {0};

    //cout <<"Somatorio: "<<endl;
    for(int j = 0; j < 3; j++){
        for (int i = 0; i < N; i++){
            sum[j] += amostra[i][j];
        }
      //  cout <<sum[j]<<" ";
    }

    //cout <<"\nMedia: "<<endl;
    for(int i = 0; i < 3; i++){
        media[i] = (float) (sum[i]/N);
        //cout <<media[i]<<" ";
    }
    //system("pause");

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            for (int k=0; k<N; k++){
                cov[i][j] += (float) ((amostra[k][i] - media[i])*(amostra[k][j] - media[j]));
            }
        }
    }
    /*
    cout<<"\nMatriz Covariancia"<<endl;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            cout<<"["<<cov[i][j]<<"] ";
        }
        cout<<endl;
    }*/

    float i_cov[3][3] = {0};
    /*Calculo da Adjunta*/
    i_cov[0][0] = (cov[1][1]*cov[2][2] - cov[1][2]*cov[2][1]); i_cov[0][1] = (-1)*(cov[0][1]*cov[2][2] - cov[0][2]*cov[2][1]); i_cov[0][2] = (cov[0][1]*cov[1][2] - cov[0][2]*cov[1][1]);
    i_cov[1][0] = i_cov[0][1]; i_cov[1][1] = (cov[0][0]*cov[2][2] - cov[0][2]*cov[2][0]) ; i_cov[1][2] = (-1)*(cov[0][0]*cov[1][2] - cov[0][2]*cov[1][0]);
    i_cov[2][0] = i_cov[0][2]; i_cov[2][1] = i_cov[1][2] ; i_cov[2][2] = (cov[0][0]*cov[1][1] - cov[0][1]*cov[1][0]);


    /*
    cout<<"\nMatriz Inversa de C s/ divisao pelo Determinante"<<endl;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            cout<<"["<<i_cov[i][j]<<"] ";
        }
        cout<<endl;
    }*/

    /*Calculo do Determinante*/
    float a,b,c,d,e,f,g,h,i;
    float det_C = 0;

    a = cov[0][0]; b = cov[0][1]; c = cov[0][2];
    d = cov[1][0]; e = cov[1][1]; f = cov[1][2];
    g = cov[2][0]; h = cov[2][1]; i = cov[2][2];

    det_C = (a*e*i + b*f*g + c*d*h) - (b*d*i + a*f*h + c*e*g);

    //cout<<"\nDeterminante de C: "<<endl;
    //cout <<det_C<<endl;

    for(int i=0; i<3;i++){
        for(int j =0; j<3 ;j++){
             i_cov[i][j] = i_cov[i][j]/det_C;
        }
    }
    /*cout<<"\nMatriz Inversa de C"<<endl;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            cout<<"["<<i_cov[i][j]<<"] ";
        }
        cout<<endl;
    }
     cout<<"\nMEDIA"<<endl;
     */
       for(int j=0; j<3; j++){
            media[j] = (unsigned int) media[j];
            cout<<"["<<media[j]<<"] ";
        }

    system("pause");

    unsigned char* result = new unsigned char[W*H*3];

    for(unsigned int i=0; i<H; i++)
    {
        for(unsigned int j=0; j<W; j++)
        {
            unsigned int index = i*W*3+j*3;
            unsigned int r1 = (unsigned int) data[index+0];
            unsigned int g1 = (unsigned int) data[index+1];
            unsigned int b1 = (unsigned int) data[index+2];
            float dm = 0;
            for (int k = 0; k < 3; k++){
                float aux = (r1-media[0])*i_cov[k][0]+
                            (g1-media[1])*i_cov[k][1]+
                            (b1-media[2])*i_cov[k][2];
                if(k == 0){
                    dm += aux*(r1-media[0]);
                }
                 if(k == 1){
                    dm += aux*(g1-media[1]);
                }
                 if(k == 2){
                    dm += aux*(b1-media[2]);
                }

            }

            dm = sqrt(dm);
            //cout<<"Distancia de Mahalanobis: "<<endl;
            //cout<<dm<<endl;

            if(dm<th)
            {
                result[index+0] = 0;
                result[index+1] = 0;
                result[index+2] = 0;
            }
            else
            {
                result[index+0] = 255;
                result[index+1] = 255;
                result[index+2] = 255;
            }
        }
    }
    return result;


}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

unsigned char* K_means(unsigned char* data, unsigned int W, unsigned int H, unsigned int th, int qtd_viz)
{   // Distância dos k-vizinhos mais proximos

     float amostra[N][3]={
{193,116,105},
{207,125,113},
{184,82,93},
{182,73,83},
{186,68,82},
{176,68,75},
{181,70,79},
{195,97,93},
{191,85,95},
{189,89,104},
{181,78,90},
{177,74,95},
{163,66,81},
{162,69,84},
{163,64,82},
{148,60,82},
{148,60,82},
{150,68,81},
{136,61,82},
{141,68,89},
{143,71,95},
{146,76,104},
{128,73,104},
{126,71,101},
{147,71,97},
{146,62,87},
{155,69,85},
{163,75,95},
{159,58,76},
{155,57,81},
{162,62,83},
{167,64,83},
{175,75,88},
{174,76,85},
{202,99,106},
{195,94,97},
{191,90,96},
{176,63,72},
{192,67,84},
{185,68,79},
{189,68,77},
{202,85,97},
{188,68,84},
{196,70,90},
{187,57,81},
{205,77,96},
{205,78,96},
{205,77,97},
{194,65,86},
{202,58,91},
{190,68,86},
{193,72,89},
{200,80,96},
{200,81,97},
{207,84,99},
{203,79,97},
{193,76,98},
{180,67,88},
{181,73,84},
{185,73,92},
{190,78,99},
{197,90,106},
{200,93,112},
{202,96,116},
{181,81,107},
{180,84,111},
{170,83,112},
{163,88,113},
{164,90,114},
{161,88,111},
{168,84,111},
{189,87,111},
{198,94,114},
{188,77,95},
{186,60,89},
{194,74,95},
{211,84,102},
{205,83,98},
{226,99,110},
{227,97,108},
{217,99,101},
{234,108,113},
{223,101,102},
{218,99,102},
{233,114,122},
{231,105,108},
{220,106,106},
{218,104,102},
{222,112,106},
{236,126,120},
{230,122,116},
{223,111,108},
{218,107,102},
{217,105,100},
{216,95,96},
{211,85,91},
{209,78,90},
{202,79,81},
{194,67,76},
{209,83,91},
{193,75,76},
{208,107,104},
{221,102,103},
{217,92,99},
{235,96,107},
{251,108,115},
{250,98,119},
{255,93,113},
{255,129,143},
{255,135,142},
{252,164,163},
{247,152,153},
{249,136,143},
{254,98,121},
{254,87,110},
{253,81,104},
{255,98,116},
{254,115,131},
{253,118,134},
{244,149,150},
{253,144,149},
{252,110,128},
{240,92,118},
{254,99,129},
{249,72,106},
{253,57,87},
{248,47,83},
{254,43,86},
{251,56,90},
{248,32,79},
{249,39,77},
{254,41,85},
{254,55,92},
{247,34,76},
{253,55,91},
{234,65,84},
{248,98,110},
{236,100,110},
{217,101,107},
{205,93,101},
{192,91,96},
{184,83,96},
{177,74,91},
{172,68,83},
{178,73,89},
{171,66,87},
{177,69,89},
{171,72,90},
{178,82,94},
{176,80,92},
{174,66,87},
{171,73,83},
{178,72,95},
{180,72,97},
{175,66,88},
{170,71,91},
{174,78,89},
{172,75,91},
{165,71,89},
{156,68,82},
{160,77,92},
{156,72,88},
{167,80,95},
{164,80,95},
{161,73,90},
{152,72,87},
{155,75,90},
{148,70,88},
{142,68,94},
{139,76,97},
{131,67,90},
{127,67,92},
{127,69,92},
{133,75,96},
{132,74,98},
{140,85,107},
{138,78,97},
{130,75,94},
{131,73,95},
{126,73,98},
{135,77,96},
{147,88,104},
{159,96,108},
{161,90,101},
{175,102,109},
{173,94,103},
{178,97,104},
{185,98,107},
{191,106,111},
{195,112,116},
{203,114,120},
{195,106,112},
{201,112,118},
{199,109,111},
{202,110,113},
{200,106,107},
{210,107,113},
{219,112,113},
{224,114,116},
{226,114,113},
{236,121,126},
{238,128,129},
{243,131,130},
{225,110,108},
{234,117,114},
{232,109,113},
{226,106,105},
{228,114,116},
{225,122,115},
{235,142,137},
{224,123,123},
{213,114,109},
{237,131,126},
{239,126,116},
{252,134,125},
{255,148,129},
{250,34,86},
{255,101,128},
{250,39,80},
{254,29,81},
{249,39,77},
{250,40,77},
{254,29,81},
{247,46,81},
{248,47,83},
{255,44,88},
{243,46,82},
{247,46,81},
{249,44,81},
{252,43,82}};

    unsigned char* result = new unsigned char[W*H*3];
    float dkmin[N];

    for(unsigned int i=0; i<H; i++)
    {
        for(unsigned int j=0; j<W; j++)
        {
            unsigned int index = i*W*3+j*3;
            unsigned int r1 = (unsigned int) data[index+0];
            unsigned int g1 = (unsigned int) data[index+1];
            unsigned int b1 = (unsigned int) data[index+2];

            for(unsigned int k=0; k<N;k++){
                dkmin[k] = sqrt((r1-amostra[k][0])*(r1-amostra[k][0])+
                                (g1-amostra[k][1])*(g1-amostra[k][1])+
                                (b1-amostra[k][2])*(b1-amostra[k][2]));
            }

            qsort(dkmin, N, sizeof(float), cmpfunc);

            float dist = 0.0;

            for(unsigned int l = 0; l < qtd_viz; l++){
                dist+= (float) (dkmin[l]/qtd_viz);
            }

            if(dist<th)
            {
                result[index+0] = 0;
                result[index+1] = 0;
                result[index+2] = 0;
            }
            else
            {
                result[index+0] = 255;
                result[index+1] = 255;
                result[index+2] = 255;
            }

        }
    }

    return result;
}

#endif // CLASSIFICADORES_H
