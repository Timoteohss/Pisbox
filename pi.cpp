#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "cvui.h"

#define WINDOW_NAME "Sandbox PI"

// g++ -std=c++11 pi.cpp -o pisdbox `pkg-config --libs opencv`
using namespace std;

// Definição da máscara
    int mascara[3][3] = { {1,1,1}, {1,1,1}, {1,1,1} };
// END

void muda(int a, int b, int c, int d, int e, int f, int g, int h, int i) {
    mascara[0][0] = a;
    mascara[0][1] = b;
    mascara[0][2] = c;

    mascara[1][0] = d;
    mascara[1][1] = e;
    mascara[1][2] = f;

    mascara[2][0] = g;
    mascara[2][1] = h;
    mascara[2][2] = i;
}

int constrain(int src, int min, int max) {
    if(src<min) src = 0;
    if(src>max) src = max;

    return src;
}

int convolucao(int x, int y, int tamanhoMatriz, cv::Mat src) {
    vector<int> temp;
    int tLocal = 0;
    int offset = tamanhoMatriz / 2;

    for(int i = 0; i < tamanhoMatriz; i ++) {
        for(int j = 0; j < tamanhoMatriz; j ++) {
            int xLoc = x + i - offset;
            int yLoc = y + j - offset;
            int loc = xLoc + src.rows * yLoc;

            loc = constrain(loc,0,src.rows-1);

            tLocal += src.at<uchar>(cv::Point(xLoc+1,yLoc+1)) * mascara[i][j];
        }
    }

    tLocal /= pow(tamanhoMatriz,2);
    tLocal = constrain(tLocal,0,255);

    return tLocal;
}

cv::Mat filtra(cv::Mat src,int control) {
    cv::Mat tempo = cv::Mat::zeros(src.rows,src.cols,CV_8UC1);

     for(int i = 0; i < src.rows; i ++) {
        for(int j = 0; j < src.cols; j ++) {
            switch(control){
                case 1:
                    muda(1,1,1,1,1,1,1,1,1);
                    tempo.at<uchar>(cv::Point(i,j)) = convolucao(i,j,3,src);
                    break;
                case 2:
                    tempo.at<uchar>(cv::Point(i,j)) = convolucao(i,j,3,src);
                    break;
                default:
                    tempo = src;
                    break;
            }
        }
    }

     return tempo;
}





int main(int argc, const char *argv[])
{
    if (argc != 2) {
        cout << "Use apenas um argumento, o nome da imagem" << endl;
        return -1;
    }

    cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat frame = image.clone();
    cv::Mat dBuffer = frame.clone();

    //variaveis de ambiente
    int trackbarWidth = 130;
    int alt_menu = 200;
    bool mostra_menu = true;
    bool fil_media = false;
    bool fil_laplaciano = false;

    bool fil_L1 = false;
    bool fil_L2 = false;
    bool fil_L3 = false;
    bool fil_L4 = false;
    

    cv::namedWindow(WINDOW_NAME);
    cvui::init(WINDOW_NAME, 20); //Aperte "q" para sair
    

    while(true) {
        dBuffer.copyTo(frame);

        cvui::checkbox(frame,10,10,"Mostra Menu", &mostra_menu);
        if(mostra_menu) {
            cvui::window(frame, 20, 30, 160, alt_menu, "Filtros");
            cvui::beginColumn(frame, 35, 50, -1, -1, 10);
            
            cvui::space(3);
            cvui::checkbox("Media",&fil_media);
            cvui::checkbox("Laplaciano",&fil_laplaciano);
            if(fil_media) {
                dBuffer = filtra(image,1);
            } else if (fil_laplaciano) {
                if(cvui::checkbox("L1",&fil_L1)){muda(0,1,0,1,-4,1,0,1,0);}
                if(cvui::checkbox("L2",&fil_L2)){muda(1,1,1,1,-8,1,1,1,1);}
                if(cvui::checkbox("L3",&fil_L3)){muda(0,-1,0,-1,4,-1,0,-1,0);}
                if(cvui::checkbox("L4",&fil_L4)){muda(-1,-1,-1,-1,8,-1,-1,-1,-1);}
                dBuffer = filtra(image,2);
            } else {
                dBuffer = image;
            }


            cvui::endColumn();

        }





        //botao para sair
        if (cvui::button(frame, frame.cols - 70, frame.rows - 30, "&Quit")) {
            break;
        }           

        cvui::update();
        cv::imshow(WINDOW_NAME, frame);
    }

    return 0;
}

