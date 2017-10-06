#include <iostream>
#include <map>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "cvui.h"

#define WINDOW_NAME "Sandbox PI"


vector<int> inicia() {
    vector<int> temp;
    for(int i = 0; i < 256; i++) { temp.push_back(0); }
    return temp;
}
vector<double> iniciaD() {
    vector<double> temp;
    for(int i = 0; i < 256; i++) { temp.push_back(0.0); }
    return temp;
}

cv::Mat eqHist(cv::Mat src, vector<int> hist) {
      
            double nk = 0.0;

            for(int i = 0; i < 256; i++) {
                nk+=hist.at(i);
                // cout << "Posicao " << i << ": " << hist.at(i) << endl;
            }

            cout << "nk:" <<nk << endl;
    

            vector<double> prk = iniciaD();
            for(int i = 0; i < 256; i++) {
                if(hist.at(i) != 0){
                    prk.at(i) = (double)hist.at(i)/nk;
                } else {
                    prk.at(i) = 0.0;
                } 
                // cout << "Prk Posicao " << i << ": " << prk.at(i) << endl;
            }


            vector<double> freq = iniciaD();

            for(int i = 0; i < 256; i++) {
                if(i-1 >= 0){
                    freq.at(i) = prk.at(i) + prk.at(i-1);
                } else {
                    freq.at(i) = prk.at(i);
                }
                // cout << "Freq Posicao " << i << ": " << freq.at(i) << endl;
            }

            vector<double> eq = iniciaD();
            double max = 0,min = 256;
            for(int i = 0; i < 256; i++) {
                eq.at(i) = 255*prk.at(i);
                if(max < eq.at(i)) { max = eq.at(i); }
                if(min > eq.at(i)) { min = eq.at(i); }
            }
            for(int i = 0; i < 256; i++) {
                eq.at(i) = ceil((eq.at(i) - min) * (255/(max-min)));
                // cout << "EQ Posicao " << i << ": " << eq.at(i) << endl;
            }

}




vector<int> fazHist(cv::Mat image) {
    vector<int> eq = inicia();

    for(int i = 0; i < 256; i++) { eq.push_back(0); }
	
	for(int i = 0; i < image.cols - 1 ; i++) {
		for(int j = 0; j < image.rows - 1; j ++) {
			eq.at(image.at<uchar>(cv::Point(i,j)))++;
		}
	}
	return eq;
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
            
            dBuffer = eqHist(image,hist);
        }

            

        cvui::update();
        cv::imshow(WINDOW_NAME, frame);
    }

    return 0;
}

