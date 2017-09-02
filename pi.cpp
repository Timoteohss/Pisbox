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
                rotulacao(dBuffer);
            }

        cvui::endColumn();



            

        cvui::update();
        cv::imshow(WINDOW_NAME, frame);
    }

    return 0;
}

class rot_ulo {
    public:
        cv::Scalar getCor() { return new_cor; }
        rot_ulo(string name);

    private:
        int qtd_rots = 1;
        string nome;
        cv::Scalar new_cor;
        void setN_cor();
};

rot_ulo::rot_ulo(string name) { nome = name; rot_ulo::setN_cor(); }
        
void rot_ulo::setN_cor() {
    cv::RNG rng(12345);
    cv::Scalar new_cor = cv::Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));
}

void rotulacao(cv::Mat image) {
    vector<rot_ulo> v_rotulos;
    cout << image.cols << ":" << image.rows << endl;
    cv::Mat img_rotulada = cv::Mat::zeros(image.rows, image.cols, CV_8UC3);
    cout << img_rotulada.cols << ":" << img_rotulada.rows << endl;
   
    int r ,t, qtd = 0;

     for (int y = 0; y <= image.rows  ; y++) {
        for (int x = 0; x <= image.cols  ; x++) {
            //cout << (int)image.at<uchar>(y,x) << endl;
            if ( x - 1 > 0 ) {
                r = (int)image.at<uchar>(y    ,x - 1);
            } else {
                r = -1;
            }

            if ( y - 1 > 0 ) {
                t = (int)image.at<uchar>(y - 1 ,x   );
            } else {
                t = -1;
            }
            
            //Se p = 0 então verifica o próximo pixel;
            //Se p = 1, examina r e t
            if((int)image.at<uchar>(y,x) != 0) {
                if(r == 0 && t == 0) {
                    qtd++;
                    rot_ulo novoo(to_string(qtd));
                    v_rotulos.push_back(novoo);

                    //cout << v_rotulos.size() << endl;
                    img_rotulada.at<cv::Scalar>(y,x) = cv::Scalar(255,255,127);
                } 
            }
        }
    }


     cv::namedWindow("nova_img");
     imshow("nova_img",img_rotulada);
}

