#include <iostream>
#include <map>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "cvui.h"

#define WINDOW_NAME "Sandbox PI"

using namespace std;


vector<int> getHist(cv::Mat src) {
	vector<int> temp;
	
	for(int i = 0; i < src.cols; i++) {
		for(int j = 0; j < src.rows; j ++) {
			temp.push_back((int)src.at<uchar>(cv::Point(i,j)));
		}
	}
	return temp;
}

vector<int> findVale(vector<int> thr) {
	vector<int> temp;
	
	for(int i = 1; i < thr.size() - 1; i++) {
		if(thr[i-1] > thr[i] && thr[i+1] > thr[i]) {
			temp.push_back(thr[i]);
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

    cv::Mat image = cv::imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat frame = image.clone();
    cv::Mat dBuffer = frame.clone();

    cv::namedWindow(WINDOW_NAME);
    cvui::init(WINDOW_NAME, 20); //Aperte "q" para sair
    

    while(true) {
        dBuffer.copyTo(frame);
       

        //botao para sair
        if (cvui::button(frame, frame.cols - 70, frame.rows - 30, "&Quit")) {
            break;
        }
	    if (cvui::button(frame, frame.cols -140, frame.rows -30, "Hist")) {
		vector<int> thr = findVale(getHist(image));
		sort(thr.begin(),thr.end());
		cout << thr[thr.size()] << endl;
		dBuffer = image.clone() > thr[thr.size()];	
        }

            

        cvui::update();
        cv::imshow(WINDOW_NAME, frame);
    }

    return 0;
}

