#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "cvui.h"

#define WINDOW_NAME "Sandbox PI"


// g++ -std=c++11 pi.cpp -o pisdbox `pkg-config --libs opencv`

using namespace std;

cv::Mat func_adicao(cv::Mat src1, cv::Mat src2) {
    cv::Mat temp = src1.clone();

    for (int y = 0; y <= src1.rows - 1 ; y++) {
        for (int x = 0; x <= src2.cols - 1 ; x++) {
            temp.at<cv::Vec3b>(cv::Point(x,y))[0] = (src1.at<cv::Vec3b>(cv::Point(x,y))[0] + src2.at<cv::Vec3b>(cv::Point(x,y))[0]) / 2;
            temp.at<cv::Vec3b>(cv::Point(x,y))[1] = (src1.at<cv::Vec3b>(cv::Point(x,y))[1] + src2.at<cv::Vec3b>(cv::Point(x,y))[1]) / 2;
            temp.at<cv::Vec3b>(cv::Point(x,y))[2] = (src1.at<cv::Vec3b>(cv::Point(x,y))[2] + src2.at<cv::Vec3b>(cv::Point(x,y))[2]) / 2;
        }
    }

    return temp;
}

cv::Mat func_subtracao(cv::Mat src1, cv::Mat src2) {
    cv::Mat temp = src1.clone();

    for (int y = 0; y <= src1.rows - 1 ; y++) {
        for (int x = 0; x <= src2.cols - 1 ; x++) {
            temp.at<cv::Vec3b>(cv::Point(x,y))[0] = abs( src1.at<cv::Vec3b>(cv::Point(x,y))[0] - src2.at<cv::Vec3b>(cv::Point(x,y))[0] );
            temp.at<cv::Vec3b>(cv::Point(x,y))[1] = abs( src1.at<cv::Vec3b>(cv::Point(x,y))[1] - src2.at<cv::Vec3b>(cv::Point(x,y))[1] );
            temp.at<cv::Vec3b>(cv::Point(x,y))[2] = abs( src1.at<cv::Vec3b>(cv::Point(x,y))[2] - src2.at<cv::Vec3b>(cv::Point(x,y))[2] );
        }
    }

    return temp;
}

cv::Mat rotacao(cv::Mat src1, double angle) {
    cv::Mat temp = cv::Mat::zeros(src1.rows + src1.cols, src1.cols + src1.rows, CV_8UC3);

    for (int y = 0; y <= src1.rows - 1 ; y++) {
        for (int x = 0; x <= src1.cols - 1 ; x++) {
            int nnx = x + floor(src1.cols / 2);
            int nny = y + floor(src1.rows / 2);
            int nx = nnx * cos(angle) - nny * sin(angle);
            int ny = nnx * cos(angle) + nny * sin(angle);

            temp.at<cv::Vec3b>(cv::Point(nnx,nny))[0] = src1.at<cv::Vec3b>(cv::Point(nx,ny))[0];
            temp.at<cv::Vec3b>(cv::Point(nnx,nny))[1] = src1.at<cv::Vec3b>(cv::Point(nx,ny))[1];
            temp.at<cv::Vec3b>(cv::Point(nnx,nny))[2] = src1.at<cv::Vec3b>(cv::Point(nx,ny))[2];
        }
    }
    
    return temp;
}


int main(int argc, const char *argv[])
{
    if (argc != 3) {
        cout << "Use dois argumentos, os nomes das imagens" << endl;
        return -1;
    }

    cv::Mat image = cv::imread(argv[1]);
    cv::Mat frame = image.clone();
    cv::Mat dBuffer = frame.clone();

    cv::Mat image2 = cv::imread(argv[2]);
    cv::Mat frame2 = image2.clone();
    cv::Mat dst;


    //variaveis de ambiente
    int trackbarWidth = 130;
    double angle = 15.0;
    bool escondeMenu = false;
    bool adicao = false;
    bool subtracao = false;
    bool inverter = false;


    cv::namedWindow(WINDOW_NAME);
   
    cvui::init(WINDOW_NAME, 20); //Aperte "q" para sair
    
    cv::moveWindow(WINDOW_NAME, 10, 20);

  

    while(true) {
        dBuffer.copyTo(frame);
        
        cvui::checkbox(frame,10,10,"Mostrar Menu", &escondeMenu);
        if(escondeMenu){
            cvui::window(frame, 20, 50, 180, 240, "Controle");
            cvui::beginColumn(frame, 35, 80, -1, -1, 10);

            
                
            if(cvui::checkbox("Adicao", &adicao)){
                subtracao = false;
            }
            if(cvui::checkbox("Subtracao", &subtracao)){
                adicao = false;
                cvui::checkbox("Inverter", &inverter);
            }

            cvui::trackbar(trackbarWidth, &angle, (double)0., (double)90., 2, "%3.02Lf");
        
            if(cvui::button("Girar")) {
                cv::Point2f src_center(dBuffer.cols/2.0F, dBuffer.rows/2.0F);
                cv::Mat rot_mat = cv::getRotationMatrix2D(src_center, angle, 1.0);
                cv::warpAffine(dBuffer, dst, rot_mat, dBuffer.size());
                cv::imshow("kek",dst);
            }
        

            if(adicao && !subtracao) {
                dBuffer = func_adicao(image2,image);
            }
            else if (!adicao && subtracao){
                if(inverter)
                    dBuffer = func_subtracao(image2,image);
                else
                    dBuffer =  func_subtracao(image,image2);
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

