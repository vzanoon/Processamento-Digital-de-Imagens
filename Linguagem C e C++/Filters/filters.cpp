#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

unsigned char* readPPM(const char* filename, unsigned int &W, unsigned int &H);
bool savePPM(const char* filename, unsigned char* data, unsigned int W, unsigned int H);

//Filters Functions
unsigned char* Filter_Linear(unsigned char* data, unsigned int W, unsigned int H);
unsigned char* Filter_No_Linear_Media(unsigned char* data, unsigned int W, unsigned int H);
unsigned char* Filter_No_Linear_Gaussian(unsigned char* data, unsigned int W, unsigned int H);
//Support Functions
void FilterCreation(float GKernel[3][3]);

int main(int argc, const char* argv[])
{

    if(argc != 2)
    {
            cout << "Erro! Entrada deveria ser <exec> <inputfile pgm>" << endl;
            return 1;
    }

    unsigned int W, H=W=0;
    unsigned char *data=readPPM(argv[1], W,H);
    if(data==NULL)
    {
            cout << "Erro! Arquivo nao encontrado!" << endl;
            return 2;
    }

    unsigned char* result1;
    result1 = Filter_Linear(data, W, H);
    savePPM("FL.ppm", result1, W, H);

    unsigned char* result2;
    result2 = Filter_No_Linear_Media(data, W, H);
    savePPM("FNLM.ppm", result2, W, H);

    unsigned char* result3;
    result3 = Filter_No_Linear_Gaussian(data, W, H);
    savePPM("FNLG.ppm", result3, W, H);

    system("pause");

    return 0;
}
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
unsigned char* Filter_Linear(unsigned char* data, unsigned int W, unsigned int H)
{

    int kernel[3][3] = {4,8,4,
                        8,16,8,
                        4,8,4};

    float kernelWeight = 0.0;

    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            kernelWeight += kernel[i][j];
        }
    }

    unsigned char* result = new unsigned char[W*H*3];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W*3+j*3;
            /*Canal R*/
            result[index+0]=(float)((unsigned int) data[(i-1)*3*W+(j-1)*3]*kernel[0][0] +
                                    (unsigned int) data[(i-1)*3*W+(j+0)*3]*kernel[0][1] +
                                    (unsigned int) data[(i-1)*3*W+(j+1)*3]*kernel[0][2] +
                                    (unsigned int) data[(i+0)*3*W+(j-1)*3]*kernel[1][0] +
                                    (unsigned int) data[(i+0)*3*W+(j+0)*3]*kernel[1][1] +
                                    (unsigned int) data[(i+0)*3*W+(j+1)*3]*kernel[1][2] +
                                    (unsigned int) data[(i+1)*3*W+(j-1)*3]*kernel[2][0] +
                                    (unsigned int) data[(i+1)*3*W+(j+0)*3]*kernel[2][1] +
                                    (unsigned int) data[(i+1)*3*W+(j+1)*3]*kernel[2][2]) / kernelWeight;
            /*Canal G*/
            result[index+1]=(float)((unsigned int) data[(i-1)*3*W+(j-1)*3+1]*kernel[0][0] +
                                    (unsigned int) data[(i-1)*3*W+(j+0)*3+1]*kernel[0][1] +
                                    (unsigned int) data[(i-1)*3*W+(j+1)*3+1]*kernel[0][2] +
                                    (unsigned int) data[(i+0)*3*W+(j-1)*3+1]*kernel[1][0] +
                                    (unsigned int) data[(i+0)*3*W+(j+0)*3+1]*kernel[1][1] +
                                    (unsigned int) data[(i+0)*3*W+(j+1)*3+1]*kernel[1][2] +
                                    (unsigned int) data[(i+1)*3*W+(j-1)*3+1]*kernel[2][0] +
										(unsigned int) data[(i+1)*3*W+(j+0)*3+1]*kernel[2][1] +
                                    (unsigned int) data[(i+1)*3*W+(j+1)*3+1]*kernel[2][2]) / kernelWeight;
            /*Canal B*/
            result[index+2]=(float)((unsigned int) data[(i-1)*3*W+(j-1)*3+2]*kernel[0][0] +
                                    (unsigned int) data[(i-1)*3*W+(j+0)*3+2]*kernel[0][1] +
                                    (unsigned int) data[(i-1)*3*W+(j+1)*3+2]*kernel[0][2] +
                                    (unsigned int) data[(i+0)*3*W+(j-1)*3+2]*kernel[1][0] +
                                    (unsigned int) data[(i+0)*3*W+(j+0)*3+2]*kernel[1][1] +
                                    (unsigned int) data[(i+0)*3*W+(j+1)*3+2]*kernel[1][2] +
                                    (unsigned int) data[(i+1)*3*W+(j-1)*3+2]*kernel[2][0] +
                                    (unsigned int) data[(i+1)*3*W+(j+0)*3+2]*kernel[2][1] +
                                    (unsigned int) data[(i+1)*3*W+(j+1)*3+2]*kernel[2][2]) / kernelWeight;
        }
    }
    return result;
}
unsigned char* Filter_No_Linear_Media(unsigned char* data, unsigned int W, unsigned int H)
{
    unsigned char* result = new unsigned char[W*H*3];

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W*3+j*3;
              /*Canal R*/
            result[index+0]=(float)((unsigned int) data[(i-1)*3*W+(j-1)*3] +
                                    (unsigned int) data[(i-1)*3*W+(j+0)*3] +
                                    (unsigned int) data[(i-1)*3*W+(j+1)*3] +
                                    (unsigned int) data[(i+0)*3*W+(j-1)*3] +
                                    (unsigned int) data[(i+0)*3*W+(j+0)*3] +
                                    (unsigned int) data[(i+0)*3*W+(j+1)*3] +
                                    (unsigned int) data[(i+1)*3*W+(j-1)*3] +
                                    (unsigned int) data[(i+1)*3*W+(j+0)*3] +
                                    (unsigned int) data[(i+1)*3*W+(j+1)*3]) / 9.0;
            /*Canal G*/
            result[index+1]=(float)((unsigned int) data[(i-1)*3*W+(j-1)*3+1] +
                                    (unsigned int) data[(i-1)*3*W+(j+0)*3+1] +
                                    (unsigned int) data[(i-1)*3*W+(j+1)*3+1] +
                                    (unsigned int) data[(i+0)*3*W+(j-1)*3+1] +
                                    (unsigned int) data[(i+0)*3*W+(j+0)*3+1] +
                                    (unsigned int) data[(i+0)*3*W+(j+1)*3+1] +
                                    (unsigned int) data[(i+1)*3*W+(j-1)*3+1] +
                                    (unsigned int) data[(i+1)*3*W+(j+0)*3+1] +
                                    (unsigned int) data[(i+1)*3*W+(j+1)*3+1]) / 9.0;
            /*Canal B*/
            result[index+2]=(float)((unsigned int) data[(i-1)*3*W+(j-1)*3+2] +
                                    (unsigned int) data[(i-1)*3*W+(j+0)*3+2] +
                                    (unsigned int) data[(i-1)*3*W+(j+1)*3+2] +
                                    (unsigned int) data[(i+0)*3*W+(j-1)*3+2] +
                                    (unsigned int) data[(i+0)*3*W+(j+0)*3+2] +
                                    (unsigned int) data[(i+0)*3*W+(j+1)*3+2] +
                                    (unsigned int) data[(i+1)*3*W+(j-1)*3+2] +
                                    (unsigned int) data[(i+1)*3*W+(j+0)*3+2] +
                                    (unsigned int) data[(i+1)*3*W+(j+1)*3+2]) / 9.0;

        }
    }
    return result;
}
void FilterCreation(float GKernel[3][3])
{
    float sigma = 0.5;
    float r, s = 2.0*sigma*sigma;
    float sum = 0.0;

    // kernel 3x3
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            r = sqrt(x*x + y*y);
            GKernel[x+1][y+1] = (exp(-(r*r)/s))/(M_PI*s);
            sum += GKernel[x+1][y+1];
        }
    }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            GKernel[i][j] /= sum;

    return;
}
unsigned char* Filter_No_Linear_Gaussian(unsigned char* data, unsigned int W, unsigned int H)
{
    unsigned char* result = new unsigned char[W*H*3];

     float GKernel[3][3];
     FilterCreation(GKernel);

     for (int i = 0; i < 3; i++) {
         for (int j = 0; j < 3; j++)
             cout << GKernel[i][j] << "\t";
         cout << endl;
     }

    for(unsigned int i=1; i<H-1; i++)
    {
        for(unsigned int j=1; j<W-1; j++)
        {
            unsigned int index = i*W*3+j*3;
              /*Canal R*/
            result[index+0]=(float)((unsigned int) data[(i-1)*3*W+(j-1)*3]*GKernel[0][0] +
                                    (unsigned int) data[(i-1)*3*W+(j+0)*3]*GKernel[0][1] +
                                    (unsigned int) data[(i-1)*3*W+(j+1)*3]*GKernel[0][2] +
                                    (unsigned int) data[(i+0)*3*W+(j-1)*3]*GKernel[1][0] +
                                    (unsigned int) data[(i+0)*3*W+(j+0)*3]*GKernel[1][1] +
                                    (unsigned int) data[(i+0)*3*W+(j+1)*3]*GKernel[1][2] +
                                    (unsigned int) data[(i+1)*3*W+(j-1)*3]*GKernel[2][0] +
                                    (unsigned int) data[(i+1)*3*W+(j+0)*3]*GKernel[2][1] +
                                    (unsigned int) data[(i+1)*3*W+(j+1)*3]*GKernel[2][2]);
             /*Canal G*/
            result[index+1]=(float)((unsigned int) data[(i-1)*3*W+(j-1)*3+1]*GKernel[0][0] +
                                    (unsigned int) data[(i-1)*3*W+(j+0)*3+1]*GKernel[0][1] +
                                    (unsigned int) data[(i-1)*3*W+(j+1)*3+1]*GKernel[0][2] +
                                    (unsigned int) data[(i+0)*3*W+(j-1)*3+1]*GKernel[1][0] +
                                    (unsigned int) data[(i+0)*3*W+(j+0)*3+1]*GKernel[1][1] +
                                    (unsigned int) data[(i+0)*3*W+(j+1)*3+1]*GKernel[1][2] +
                                    (unsigned int) data[(i+1)*3*W+(j-1)*3+1]*GKernel[2][0] +
                                    (unsigned int) data[(i+1)*3*W+(j+0)*3+1]*GKernel[2][1] +
                                    (unsigned int) data[(i+1)*3*W+(j+1)*3+1]*GKernel[2][2]);
            /*Canal B*/
            result[index+2]=(float)((unsigned int) data[(i-1)*3*W+(j-1)*3+2]*GKernel[0][0] +
                                    (unsigned int) data[(i-1)*3*W+(j+0)*3+2]*GKernel[0][1] +
                                    (unsigned int) data[(i-1)*3*W+(j+1)*3+2]*GKernel[0][2] +
                                    (unsigned int) data[(i+0)*3*W+(j-1)*3+2]*GKernel[1][0] +
                                    (unsigned int) data[(i+0)*3*W+(j+0)*3+2]*GKernel[1][1] +
                                    (unsigned int) data[(i+0)*3*W+(j+1)*3+2]*GKernel[1][2] +
                                    (unsigned int) data[(i+1)*3*W+(j-1)*3+2]*GKernel[2][0] +
                                    (unsigned int) data[(i+1)*3*W+(j+0)*3+2]*GKernel[2][1] +
                                    (unsigned int) data[(i+1)*3*W+(j+1)*3+2]*GKernel[2][2]);
        }
    }

    return result;
}
