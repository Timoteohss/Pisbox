#include <iostream>
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
                break;
            }

        cvui::endColumn();



            

        cvui::update();
        cv::imshow(WINDOW_NAME, frame);
    }

    return 0;
}

void rotulacao(cv::Mat image) {

}