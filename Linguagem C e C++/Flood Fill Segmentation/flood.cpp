/************************************************************************************************************************
                                Universidade Federal de Santa Catarina (UFSC)
                                    Departamento de Computação (DEC)
                    Disciplina: Processamento Digital de Imagens e Visão Computacional
                                        Docente: Antônio Sobieranski
                                Discente: Vinícius Rodrigues Zanon - 15102833

    Objetivos: Implementação do Algoritmo Flood Fill Segmentation iterativo, sem o uso da recursividade.
    O algoritmo, implementa uma pilha de execução para obter maior controle do que está sendo de fato
    inspecionado. Possui característica de busca iterativa nas direções up, down, right e left. Além do
    mais, para cada região encontrada é atribuído uma cor randômica pertencente ao RGB, para caracterizar
    ou identificar a região destacada. A regra de similaridade é baseada na distância euclidiana entre os
    pontos RGB e limitada por um threshold passado como parâmetro.

    Obs.: Algoritmo aqui desenvolvido é uma versão adaptada do código disponibilizado pelo
    professor Antônio Sobieranski no semestre passado.

***********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>
using namespace std;


class Color {
    public:
        unsigned char m_r,m_g,m_b;
        Color(unsigned char r, unsigned char g, unsigned char b) : m_r(r), m_g(g), m_b(b) {};
};

class Point{
    public:
        int m_x, m_y;
        Point(int x, int y) : m_x(x), m_y(y) {};
};

class Flood{
    private:
        unsigned int* m_indexMap;
        unsigned char* m_data;
        unsigned int m_W, m_H;
        unsigned int m_numberOfRegions;

        // Funcao que percorre iterativamente a imagem nas direcoes (UP, DOWN, LEFT, RIGHT)
        int validatePosition(unsigned int x, unsigned int y, float threshold){
            //Movendo para cima
            if(y > 0){
                if(m_indexMap[(y-1)*m_W+x] == 0){
                    int actual = y*m_W*3+x*3;
                    int next   = (y-1)*m_W*3+x*3;

                    int r_actual = m_data[actual+0];
                    int r_next = m_data[next+0];
                    int g_actual = m_data[actual+1];
                    int g_next = m_data[next+1];
                    int b_actual = m_data[actual+2];
                    int b_next = m_data[next+2];
                    float dist = float(pow((r_actual-r_next),2) + pow((g_actual-g_next),2) + pow((b_actual-b_next),2));
                    dist = float (sqrt(dist));
                    if(dist < threshold) return 1;
                }
            }

            //Movendo para esquerda
            if(x > 0){
                if(m_indexMap[y*m_W+(x-1)] == 0){

                    int actual = y*m_W*3+x*3;
                    int next   = y*m_W*3+(x-1)*3;

                    int r_actual = m_data[actual+0];
                    int r_next = m_data[next+0];
                    int g_actual = m_data[actual+1];
                    int g_next = m_data[next+1];
                    int b_actual = m_data[actual+2];
                    int b_next = m_data[next+2];
                    float dist = float (pow((r_actual-r_next),2) + pow((b_actual-b_next),2) + pow((g_actual-g_next),2));
                    dist = sqrt(dist);
                    if(dist < threshold) return 2;
                }
            }

            // Movendo para baixo
            if(y < m_H-1){
                if(m_indexMap[(y+1)*m_W+x] == 0){
                    int actual = y*m_W*3+x*3;
                    int next   = (y+1)*m_W*3+x*3;

                    int r_actual = m_data[actual+0];
                    int r_next = m_data[next+0];
                    int g_actual = m_data[actual+1];
                    int g_next = m_data[next+1];
                    int b_actual = m_data[actual+2];
                    int b_next = m_data[next+2];
                    float dist = float (pow((r_actual-r_next),2) + pow((b_actual-b_next),2) + pow((g_actual-g_next),2));
                    dist = sqrt(dist);
                    if(dist < threshold) return 3;
                }
            }

            //Movendo para direita
            if(x < m_W-1){
                if(m_indexMap[y*m_W+x+1] == 0){
                    int actual = y*m_W*3+x*3;
                    int next   = y*m_W*3+(x+1)*3;

                    int r_actual = m_data[actual+0];
                    int r_next = m_data[next+0];
                    int g_actual = m_data[actual+1];
                    int g_next = m_data[next+1];
                    int b_actual = m_data[actual+2];
                    int b_next = m_data[next+2];
                    float dist = float (pow((r_actual-r_next),2) + pow((b_actual-b_next),2) + pow((g_actual-g_next),2));
                    dist = sqrt(dist);
                    if(dist < threshold) return 4;
                }
            }

            return 0;
        }

    public:
        //Construtor
        Flood(){
            m_data = NULL;
            m_indexMap = NULL;
            m_W = 0;
            m_H = 0;
            m_numberOfRegions = 0;
        }
        //Destrutor
        ~Flood(){
            if(m_data)
                free(m_data);

            if(m_indexMap)
                free(m_indexMap);

        };
        // Configura Imagem (tamanho e alocacao/liberacao de memoria)
        void SetData(unsigned char* data, unsigned int W, unsigned int H){
            if(m_data)
                free(m_data);

            m_data = data;
            m_W = W;
            m_H = H;
            if(m_indexMap)
                free(m_indexMap);

            m_indexMap= new unsigned int[m_W*m_H];
        }

        // Funcao que controla a pilha de execucao conforme a busca direcionada
        void ExtractPartition(unsigned int x, unsigned int y, float threshold, unsigned int index){
            vector< Point > stack;
            stack.push_back( Point(x,y) );


            unsigned int indexToMark = index;
            m_indexMap[y*m_W+x] = indexToMark;
            m_numberOfRegions++;

            while(stack.size() > 0){

                int pos = validatePosition(x,y,threshold);

                if(pos == 1){
                    m_indexMap[(y-1)*m_W+x-0] = indexToMark;
                    y--;
                    stack.push_back(Point(x,y));
                }
                if(pos == 2){
                    m_indexMap[(y-0)*m_W+x-1] = indexToMark;
                    x--;
                    stack.push_back(Point(x,y));
                }
                if(pos == 3){
                    m_indexMap[(y+1)*m_W+x+0] = indexToMark;
                    y++;
                    stack.push_back(Point(x,y));
                }
                if(pos == 4){
                    m_indexMap[(y+0)*m_W+x+1] = indexToMark;
                    x++;
                    stack.push_back(Point(x,y));
                }

                if(pos == 0){
                    x = stack.at(stack.size()-1).m_x;
                    y = stack.at(stack.size()-1).m_y;
                    stack.pop_back();
                }
            }
        }

        // Funcao que realiza o FloodFill
        unsigned char* FloodFill(){

            unsigned char* result = new unsigned char[m_W*m_H*3];
            std::vector<Color> rndcolors;
            for(unsigned int i = 0; i < m_numberOfRegions; i++){
                rndcolors.push_back(Color(rand()%256,rand()%256,rand()%256));
            }

            for(unsigned int i=0; i<m_H; i++){
                for(unsigned int j=0; j<m_W; j++){
                    unsigned int index = (i*m_W*3+j*3);
                    if(m_indexMap[index/3] > 0){
                        result[index+0] = rndcolors.at(m_indexMap[index/3]-1).m_r;
                        result[index+1] = rndcolors.at(m_indexMap[index/3]-1).m_g;
                        result[index+2] = rndcolors.at(m_indexMap[index/3]-1).m_b;
                    }
                    else{
                        result[index+0] = 255;
                        result[index+1] = 0;
                        result[index+2] = 0;
                    }
                }
            }
            return result;
        }

        // Verificacao a existencia de index
        bool isAvailable(unsigned int i, unsigned int j){
            if(m_indexMap[i*m_W+j] > 0)
                return false;

            return true;
        }

        unsigned int getNumberOfRegions() const{
            return m_numberOfRegions;
        };
};

unsigned char* readPPM(const char* filename, unsigned int &W, unsigned int &H);
bool savePPM(const char* filename, unsigned char* data, unsigned int W, unsigned int H);

int main(int argc, const char* argv[]){

    if(argc != 3){
        cout << "Erro! Entrada deveria ser <exec inputfile.pgm threshold>" << endl;
        return 1;
    }

    unsigned int W, H=W=0;
    unsigned char* data=readPPM(argv[1], W,H);
    if(data==NULL){
        cout << "Erro! Arquivo nao encontrado!" << endl;
        return 2;
    }

    Flood segmentator;
    segmentator.SetData(data, W, H);

    unsigned int k=0;
    for(unsigned int i=0; i<H; i++){
        for(unsigned int j=0; j<W; j++){
            if(segmentator.isAvailable(i,j)){
                segmentator.ExtractPartition(j,i, atoi(argv[2]), ++k);
            }
        }
    }

    unsigned char* result1 = segmentator.FloodFill();

    savePPM("FloodFill.ppm", result1, W, H);

    system("pause");

    return 0;
}
unsigned char* readPPM(const char* filename, unsigned int &W, unsigned int &H){

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
    for(unsigned int i=0; i<W*H*3; i++){
        fscanf(fp, "%d ", &data[i]);
    }
    fclose(fp);

    return data;
}
bool savePPM(const char* filename, unsigned char* data, unsigned int W, unsigned int H){

    cout << "Saving image : " << filename << endl;
    FILE* fp = fopen(filename, "w");
    if(!fp) return false;

    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d 255\n", W, H);

    for(unsigned int i=0; i<W*H*3; i++){
        fprintf(fp, "%d ", data[i]);
    }
    fclose(fp);

    return true;
}
