#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "cvui.h"

#define WINDOW_NAME "Sandbox PI"


// g++ -std=c++11 pi.cpp -o pisdbox `pkg-config --libs opencv`

using namespace std;

void rotulacao(cv::Mat image);

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        cout << "Use apenas um argumento, o nome da imagem" << endl;
        return -1;
    }

    //variaveis de ambiente
    int trackbarWidth = 130;
    int limite = 127;

    cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat image_bw = image > limite;
    cv::Mat frame = image_bw.clone();
    cv::Mat dBuffer = frame.clone();
    cv::Mat bw_final;

    

    cv::namedWindow(WINDOW_NAME);
    cvui::init(WINDOW_NAME, 20); //Aperte "q" para sair
    

    while(true) {
        dBuffer.copyTo(frame);
       

        //botao para sair
        if (cvui::button(frame, frame.cols - 70, frame.rows - 30, "&Quit")) {
            break;
        }

        cvui::window(frame, 20, 20, 160, 130, "Ajuste");
		cvui::beginColumn(frame, 35, 50, -1, -1, 10);
            static int valor = 127;
            bool modificado = false;

            if (cvui::trackbar(trackbarWidth, &valor, (int)0, (int)254, 2)) {
                    modificado = true;
            }
            
            if(modificado){
                dBuffer = image > valor;
            }

            cvui::space(5);

            if (cvui::button(frame , 70, 110 , "Gerar")) {
                rotulacao(dBuffer);
            }

        cvui::endColumn();



            

        cvui::update();
        cv::imshow(WINDOW_NAME, frame);
    }

    return 0;
}


cv::Mat pintaVizinhos(cv::Mat image, cv::Point ponto, cv::Mat novaimg, int r, int b, int g) {

    image.at<uchar>(ponto) = 0;
    novaimg.at<cv::Vec3b>(ponto)[0] = r;
    novaimg.at<cv::Vec3b>(ponto)[1] = g;
    novaimg.at<cv::Vec3b>(ponto)[2] = b;
    
    if((int)image.at<uchar>(cv::Point(ponto.x + 1,ponto.y)) != 0 && novaimg.at<cv::Vec3b>(cv::Point(ponto.x + 1,ponto.y))[0] == 0 && ponto.x + 1 < image.cols -1) {
        // cout << "indo para baixo" << endl;
        novaimg = pintaVizinhos(image, cv::Point(ponto.x + 1,ponto.y),novaimg,r,g,b);
    }

    if((int)image.at<uchar>(cv::Point(ponto.x,ponto.y + 1)) != 0 && novaimg.at<cv::Vec3b>(cv::Point(ponto.x,ponto.y + 1))[0] == 0 && ponto.y + 1 < image.rows -1) {
        // cout << "indo para direita" << endl;
        novaimg = pintaVizinhos(image, cv::Point(ponto.x,ponto.y+1),novaimg,r,g,b);
    }

    if((int)image.at<uchar>(cv::Point(ponto.x,ponto.y - 1)) != 0 && novaimg.at<cv::Vec3b>(cv::Point(ponto.x,ponto.y - 1))[0] == 0 && ponto.y - 1 > 0) {
        // cout << "indo para esquerda" << endl;
        novaimg = pintaVizinhos(image, cv::Point(ponto.x,ponto.y-1),novaimg,r,g,b);
    }

    if((int)image.at<uchar>(cv::Point(ponto.x - 1,ponto.y)) != 0 && novaimg.at<cv::Vec3b>(cv::Point(ponto.x - 1,ponto.y))[0] == 0 && ponto.x - 1 > 0) {
        // cout << "indo para cima" << endl;
        novaimg = pintaVizinhos(image, cv::Point(ponto.x - 1,ponto.y),novaimg,r,g,b);
    }

    return novaimg;
}


void rotulacao(cv::Mat image) {
cv::Mat copia = image.clone();
cv::Mat novaimg = cv::Mat::zeros(image.rows, image.cols, CV_8UC3);
cv::namedWindow("kek");
int contador = 1;
srand (time(NULL));



     for (int y = 0; y <= copia.rows - 1 ; y++) {
        for (int x = 0; x <= copia.cols - 1 ; x++) {
            if((int)copia.at<uchar>(cv::Point(x,y)) != 0 && 
            novaimg.at<cv::Vec3b>(cv::Point(x,y))[0] == 0 &&
            novaimg.at<cv::Vec3b>(cv::Point(x,y))[1] == 0 &&
            novaimg.at<cv::Vec3b>(cv::Point(x,y))[2] == 0) {

                int r = rand() % 255 + 1;
                int b = rand() % 255 + 1;
                int g = rand() % 255 + 1;
                contador++;
                //cout << contador << endl;
                
                novaimg = pintaVizinhos(copia, cv::Point(x,y), novaimg, r, b, g);
                cv::imshow("kek",novaimg);
                //cv::waitKey(0);
            }
        }
    }

    cout << contador << " grupos diferentes encontrados" << endl;   

}

