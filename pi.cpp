#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "cvui.h"

#define WINDOW_NAME "Sandbox PI"


// g++ -std=c++11 pi.cpp -o pisdbox `pkg-config --libs opencv`

using namespace std;

cv::Mat alargaContraste(cv::Mat src, double alpha_a, int beta_b) {
    cv::Mat temp = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

    for( int y = 0; y < src.cols; y++ ) { 
        for( int x = 0; x < src.rows; x++ ) {
                temp.at<uchar>(cv::Point(x,y)) = cv::saturate_cast<uchar>( alpha_a * (src.at<uchar>(cv::Point(x,y)) + beta_b));
        }
    }

    return temp;
}

cv::Mat negativa(cv::Mat src) {
    cv::Mat temp = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

    for( int y = 0; y <= src.cols - 1; y++ ) { 
        for( int x = 0; x <= src.rows - 1; x++ ) {
                temp.at<uchar>(cv::Point(x,y)) = 254  - src.at<uchar>(cv::Point(x,y));         
        }
    }

    return temp;
}

cv::Mat logaritmica(cv::Mat src, double contrast) {
    cv::Mat temp = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

    for( int y = 0; y <= src.cols - 1; y++ ) { 
        for( int x = 0; x <= src.rows - 1; x++ ) {
            temp.at<uchar>(cv::Point(x,y)) = contrast * log(1 + src.at<uchar>(cv::Point(x,y)));
        }
    }
    return temp;
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        cout << "Use apenas um argumento, o nome da imagem" << endl;
        return -1;
    }

    int limite = 128;

    cv::Mat image_col = cv::imread(argv[1]);
    cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    // cv::Mat image_bw = image > limite;
    cv::Mat frame = image.clone();
    cv::Mat dBuffer = frame.clone();

    //variaveis de ambiente
    int trackbarWidth = 130;
    bool mostra_menu = false;
    bool the_esc = false;
    bool ala_esc = false;
    bool log_esc = false;
    bool neq_esc = false;
    int alt_menu = 130;
    double cont_log = 3;

    double alpha_a = 1.0;
    int beta_b = 50.0;
    

    cv::namedWindow(WINDOW_NAME);
    cvui::init(WINDOW_NAME, 20); //Aperte "q" para sair
    

    while(true) {
        dBuffer.copyTo(frame);

        cvui::checkbox(frame,10,10,"Mostra Menu", &mostra_menu);
        if(mostra_menu) {
            cvui::window(frame, 20, 30, 160, alt_menu, "Ajuste");
            cvui::beginColumn(frame, 35, 50, -1, -1, 10);
            bool modificado = false;

            if(!the_esc && !ala_esc && !neq_esc && !log_esc) { alt_menu = 230; }
            
            cvui::space(3);
            cvui::checkbox("Threshhold",&the_esc);
            if(the_esc){
                alt_menu = 280;
                cvui::space(2);
                cvui::text("Threshold");
                if (cvui::trackbar(trackbarWidth, &limite, (int)0, (int)254, 2)) {
                        modificado = true;
                }
            }
            cvui::space(3);
            cvui::checkbox("Alargamento",&ala_esc);
            if(ala_esc){
                alt_menu = 370;
                cvui::space(2);
                cvui::text("Alpha");
                if (cvui::trackbar(trackbarWidth, &alpha_a, (double)0, (double)3, 2.0d)) {
                        modificado = true;
                }
                cvui::space(1);
                cvui::text("Beta");
                if (cvui::trackbar(trackbarWidth, &beta_b, (int)0, (int)100, 2)) {
                        modificado = true;
                }
            }

            cvui::space(3);
            cvui::checkbox("Negativo",&neq_esc);
            if(neq_esc){
                alt_menu = 230;
                if (cvui::button("Negativo")) {
                    dBuffer = negativa(image.clone());
                }
                cvui::space(3);
            }

            cvui::space(3);
            cvui::checkbox("Logaritmica",&log_esc);
            if(log_esc){
                alt_menu = 230;
                if (cvui::trackbar(trackbarWidth, &cont_log, (double)0, (double)100, 2.0d)) {
                        modificado = true;
                }
                cvui::space(3);
            }


            if(modificado) {
                if(the_esc) {
                    dBuffer = image.clone() > limite;
                }
                else if(ala_esc) {
                    dBuffer = alargaContraste(image.clone(),alpha_a,beta_b);
                }
                else if(log_esc) {
                    dBuffer = logaritmica(image.clone(),cont_log);
                }
            }

            if(cvui::button("Reiniciar")) {
                dBuffer = image.clone();
                limite = 128;
                alpha_a = 1.0;
                beta_b = 50;
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

