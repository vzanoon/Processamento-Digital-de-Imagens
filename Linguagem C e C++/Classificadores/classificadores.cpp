#include "classificadores.h"

using namespace std;

int main(int argc, const char* argv[])
{

    if(argc != 2)
    {
        cout << "Error !  Input should be <exec> <inputfile pgm>" << endl;
        return 1;
    }

    unsigned int W, H=W=0;
    unsigned char *data=readPPM(argv[1], W,H);

    if(data==NULL)
    {
        cout << "Error !  File not found" << endl;
        return 2;
    }

    //TODAS OS ALGORITMOS CLASSIFICADORES
    unsigned char* result1 = L1norm(data, W, H, 45, Color(199,84,99));
    savePPM("3_L1_EYE_PDR.ppm", result1, W, H);
    unsigned char* result2 = L2norm(data, W, H, 30, Color(199,84,99));
    savePPM("3_L2_EYE_PDR.ppm", result2, W, H);
    unsigned char* result3 = K_means(data, W, H, 8, 5);
    savePPM("3_KNN_EYE_PDR.ppm", result3, W, H);
    unsigned char* result4 = Mahalanobis(data, W, H, 0.0899);
    savePPM("3_MAH_EYE_PDR.ppm", result4, W, H);

    return 0;
}
