#include <iostream>
#include <tuple>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "cvui.h"

#define WINDOW_NAME "Sandbox PI"


// g++ -std=c++11 pi.cpp -o pisdbox `pkg-config --libs opencv`

using namespace std;


vector<tuple<int,int>> fazHist(cv::Mat image) {
	vector<tuple<int,int>> hist;
	int contador = 0;
	
	for(int i = 0; i < image.cols - 1 ; i++) {
		for(int j = 0; j < image.rows - 1; j ++) {
			auto temp = make_tuple(contador,image.at<uchar>(cv::Point(i,j)));
			contador++;
			hist.push_back(temp);
		}
	}
	return hist;
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        cout << "Use apenas um argumento, o nome da imagem" << endl;
        return -1;
    }

    cv::Mat image = cv::imread(argv[1]);
    cv::Mat frame = image.clone();
    cv::Mat dBuffer = frame.clone();

    //variaveis de ambiente
    int trackbarWidth = 130;
    

    cv::namedWindow(WINDOW_NAME);
    cvui::init(WINDOW_NAME, 20); //Aperte "q" para sair
    

    while(true) {
        dBuffer.copyTo(frame);
       

        //botao para sair
        if (cvui::button(frame, frame.cols - 70, frame.rows - 30, "&Quit")) {
            break;
        }
	if (cvui::button(frame, frame.cols -140, frame.rows -30, "Hist")) {
		auto hist = fazHist(image);
	}

            

        cvui::update();
        cv::imshow(WINDOW_NAME, frame);
    }

    return 0;
}

