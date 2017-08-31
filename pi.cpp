#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "cvui.h"

#define WINDOW_NAME "Sandbox PI"


// g++ -std=c++11 pi.cpp -o pisdbox `pkg-config --libs opencv`

using namespace std;


int main(int argc, const char *argv[])
{
    if (argc != 2) {
        cout << "Use apenas um argumento, o nome da imagem" << endl;
        return -1;
    }

    cv::Mat image = cv::imread(argv[1]);
    cv::Mat frame = image.clone();
    cv::Mat dBuffer = frame.clone();

    //variaveis de controle aqui
    cv::Mat ctrl = cv::Mat(400,300,CV_8UC3,cvScalar(0.));
    cv::Mat ctrl_f = ctrl.clone();
    cv::Mat ctrl_db = ctrl_f.clone();

    //variaveis de ambiente
    int trackbarWidth = 130;
    

    cv::namedWindow(WINDOW_NAME);
    cv::namedWindow("Controles");
    cvui::init(WINDOW_NAME, 20); //Aperte "q" para sair
    cvui::init("Controles", 20);

    while(true) {
        dBuffer.copyTo(frame);
        ctrl_db.copyTo(ctrl_f);

        //botao para sair
        if (cvui::button(ctrl_f, ctrl_f.cols - 70, ctrl_f.rows - 30, "&Quit")) {
            break;
        }


        cvui::window(ctrl_f, 10, 10, 200, 290, "Controle 1");


        cvui::beginColumn(ctrl_f, 40, 40,-1,-1,10);



        cvui::endColumn();







        

        cvui::update();
        cv::imshow(WINDOW_NAME, frame);
        cv::imshow("Controles", ctrl_f);
    }

    return 0;
}


/*

if (argc != 2) {
        cout << "Use apenas um argumento, o nome da imagem" << endl;
        return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    Mat vizinho_maior = Mat::zeros(image.cols * razao, image.rows * razao, CV_8UC3);


    if(!image.data) {
        cout << "Nao consegui abrir a imagem." << endl;
        return -1;
    }
    
    namedWindow("Original" , WINDOW_AUTOSIZE);
    imshow("Original", image);

    // vizinho_maior
    for (int y = 0; y <= image.rows - 1; y++) {
        for (int x = 0; x <= image.cols - 1; x++) {
            Vec3b intensity = image.at<Vec3b>(y,x);
            cout << "Em : " << x << ":" << y << "temos" << intensity << endl;
            vizinho_maior.at<Vec3b>( y * razao , x * razao ) = image.at<Vec3b>( y , x );
            vizinho_maior.at<Vec3b>( y * razao , x * razao + 1) = image.at<Vec3b>( y , x + 1 );
            vizinho_maior.at<Vec3b>( y * razao + 1 , x * razao ) = image.at<Vec3b>( y + 1 , x );
            vizinho_maior.at<Vec3b>( y * razao + 1 , x * razao + 1) = image.at<Vec3b>( y + 1 , x + 1 );
        
                              
        }
    }

    namedWindow("2x vizinho_maior", WINDOW_AUTOSIZE);
    imshow("2x vizinho_maior",vizinho_maior);

    Mat bicubico_maior = Mat::zeros(image.cols * razao, image.rows * razao, CV_8UC3);

     // bicubico_maior
    for (int y = 0; y <= image.rows - 1; y++) {
        for (int x = 0; x <= image.cols - 1; x++) {
        bicubico_maior.at<Vec3b>( y * razao, x * razao ) = image.at<Vec3b>( y , x );
        }
    }
    int conta = 0;
     for (int y = 0; y <= bicubico_maior.rows - 1; y++) {
        for (int x = 0; x <= bicubico_maior.cols - 1; x++) {
            if(bicubico_maior.at<Vec3b>(x , y)[0] == 0 && x % 2 == 0) {
                bicubico_maior.at<Vec3b>(x , y)[0] = (bicubico_maior.at<Vec3b>(x, y + 1)[0] + bicubico_maior.at<Vec3b>(x, y - 1)[0]) / 2;
                bicubico_maior.at<Vec3b>(x , y)[1] = (bicubico_maior.at<Vec3b>(x, y + 1)[1] + bicubico_maior.at<Vec3b>(x, y - 1)[1]) / 2;
                bicubico_maior.at<Vec3b>(x , y)[2] = (bicubico_maior.at<Vec3b>(x, y + 1)[2] + bicubico_maior.at<Vec3b>(x, y - 1)[2]) / 2;
            
            }
            
            else if (bicubico_maior.at<Vec3b>(x , y)[0] == 0 && x % 2 != 0) {
                if(conta %  2 == 0) {
                    bicubico_maior.at<Vec3b>(x , y)[0] = (bicubico_maior.at<Vec3b>(x + 1, y)[0] + bicubico_maior.at<Vec3b>(x + 1, y)[0]) / 2;
                    bicubico_maior.at<Vec3b>(x , y)[1] = (bicubico_maior.at<Vec3b>(x + 1, y)[1] + bicubico_maior.at<Vec3b>(x + 1, y)[1]) / 2;
                    bicubico_maior.at<Vec3b>(x , y)[2] = (bicubico_maior.at<Vec3b>(x + 1, y)[2] + bicubico_maior.at<Vec3b>(x + 1, y)[2]) / 2;
                } else if(conta % 2 != 0) {
                    cout << x << " " << y << ":" << conta << endl;
                    bicubico_maior.at<Vec3b>(x , y)[0] = (bicubico_maior.at<Vec3b>(x - 1, y - 1)[0] + bicubico_maior.at<Vec3b>(x - 1, y + 1)[0] + bicubico_maior.at<Vec3b>(x + 1, y - 1)[0] + bicubico_maior.at<Vec3b>(x + 1, y + 1)[0]) /4;
                    bicubico_maior.at<Vec3b>(x , y)[1] = (bicubico_maior.at<Vec3b>(x - 1, y - 1)[1] + bicubico_maior.at<Vec3b>(x - 1, y + 1)[1] + bicubico_maior.at<Vec3b>(x + 1, y - 1)[1] + bicubico_maior.at<Vec3b>(x + 1, y + 1)[1]) /4;
                    bicubico_maior.at<Vec3b>(x , y)[2] = (bicubico_maior.at<Vec3b>(x - 1, y - 1)[2] + bicubico_maior.at<Vec3b>(x - 1, y + 1)[2] + bicubico_maior.at<Vec3b>(x + 1, y - 1)[2] + bicubico_maior.at<Vec3b>(x + 1, y + 1)[2]) /4;
                }
                conta++;
            }
        }
    }
    namedWindow("2x bicubico_maior", WINDOW_AUTOSIZE);
    imshow("2x bicubico_maior",bicubico_maior);     
    

    waitKey(0);
    return 0;

    */