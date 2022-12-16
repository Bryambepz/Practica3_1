#include <cstring>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

#include <opencv2/core.hpp>

// procesamiento y manipulacion de imagenes
#include <opencv2/imgproc.hpp>

// ventana y GUI
#include <opencv2/highgui.hpp>

// codecs y diferentes formatos de imagenes
#include <opencv2/imgcodecs.hpp>

// lectura de video
#include <opencv2/video.hpp>

// escritura de video
#include <opencv2/videoio.hpp>

// espacio de nombres de OpenCV
#include <experimental/filesystem>
using namespace cv;
using namespace std;
namespace fs = std::experimental::filesystem;

Mat img;
Mat imgTr;
Mat copia;
bool bandera;
int cordenadaInicio;
int cordenadaFinal;
int umbralExt;
int umbralInt;
int contBlancos = 0;
int contNegros = 0;
int cont=0;
double momentsHu[7];

void pintarPoligono()
{
    for (int i = 0; i < copia.rows-1; i++)
    {
        for (int j = 0; j < copia.cols; j++)
        {
            int pixel = copia.at<uchar>(i,j);
            int pixel2 = copia.at<uchar>(i,j+1);
            if(pixel != pixel2){
                // cout << pixel2 << endl;
                umbralInt = pixel2;
                break;
            }else{
                umbralExt = pixel;
            }
        }
        
    }
    
}

void blancos(){
    for (int i = 0; i < imgTr.rows; i++)
    {
        for (int j = 0; j < imgTr.cols; j++)
        {
            int pixel = imgTr.at<uchar>(i,j);
            if(pixel == 0){
                contNegros +=1;
            }else{
                contBlancos +=1;
            }
        }
    }
}

int main()
{
    namedWindow("fotoTr", WINDOW_AUTOSIZE);
    String entrenamiento = "corpus/TRAIN";
    String path[1700];
    String salida;

    ofstream contenido2("./csv_train.csv", ios::out);
    contenido2 << "" << "momento1," << "momento2," << "momento3," << "momento4,"
            << "momento5," << "momento6," << "momento7," << "distancia," << "poligono"<< endl;
    contenido2.close();

    for(const fs::directory_entry& dir_entry : fs::directory_iterator(entrenamiento)){
        salida = dir_entry.path().string().substr(13,dir_entry.path().string().length()-1);

        for(const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(dir_entry.path().string())){
            img = imread(dir_entry.path().string(), IMREAD_COLOR);

            //ii        
            cv::cvtColor(img, img, COLOR_BGR2GRAY);
            copia = img.clone();
            pintarPoligono();
            if(umbralInt > umbralExt){
                cv::threshold(copia, imgTr, umbralExt, 255, THRESH_BINARY_INV);
            }else{
                cv::threshold(copia, imgTr, umbralInt, 255, THRESH_BINARY);
            }
            blancos();
            if(contBlancos > contNegros){
                cv::bitwise_not(imgTr, imgTr);
            }
            Moments moment = moments(imgTr, true);
            HuMoments(moment, momentsHu);

            ifstream contenido("./csv_train.csv", ios::in);
            String linea;
            String con = "";
            while (getline(contenido, linea))
            {
                con = con + linea + "\n";
            }

            ofstream contenido2("./csv_train.csv", ios::out);
            contenido2 << con;
            for (int i = 0; i < 7; i++)
            {
                contenido2 << momentsHu[i] << ",";
                cout << momentsHu[i] << ",";
            }

            double d = 0.0;
            for (int i = 0; i < 7; i++)
            {
                d+=pow(momentsHu[i],2);
            }
            d = sqrt(d);
            contenido2 << d << ",";
            cout << d << ",";
            contenido2 << salida << endl;
            cout << salida << endl;
            contenido2.close();

            cv::imshow("fotoTr", imgTr);
            destroyAllWindows();    
        }
    }


    


    return 0;
}