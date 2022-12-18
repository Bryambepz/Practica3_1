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
                umbralInt = pixel2;
                // std::cout <<  pixel2 << endl;
                break;
            }else{
                // cout << pixel[0] << " - " << pixel[1] << " - " << pixel[2] << endl;
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
    // img = imread("corpus/TRAIN/Pentagon/Pentagon_0660479e-2a98-11ea-8123-8363a7ec19e6.png", IMREAD_COLOR);

    namedWindow("fotoTr", WINDOW_AUTOSIZE);
    namedWindow("fotoGray", WINDOW_AUTOSIZE);
    namedWindow("fotoOrg", WINDOW_AUTOSIZE);
    String entrenamiento = "corpus/TEST";
    String path[1700];
    String salida;

    // copia = img.clone();
    // Laplacian(imhC, imhC ,-1,CV_16S);
    // cv::cvtColor(img, img, COLOR_BGR2GRAY,1);
    // pintarPoligono();
    // cout << "bla " << umbralInt << " -- " << " ng " << contNegros << endl; 
    // cv::imshow("fotoTr", img);
    // cv::imshow("f", copia);
    // waitKey(0);
    // destroyAllWindows();    
    ofstream contenido2("./csv_test.csv", ios::out);
    contenido2 << "" << "momento1," << "momento2," << "momento3," << "momento4,"
            << "momento5," << "momento6," << "momento7," << "poligono"<< endl;
    contenido2.close();

    for(const fs::directory_entry& dir_entry : fs::directory_iterator(entrenamiento)){
        salida = dir_entry.path().string().substr(12,dir_entry.path().string().length()-1);

        for(const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(dir_entry.path().string())){
            cout << dir_entry.path().string() << endl;
            img = imread(dir_entry.path().string(), IMREAD_COLOR);
            
            //ii        
            Mat org = img.clone();
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

            ifstream contenido("./csv_test.csv", ios::in);
            String linea;
            String con = "";
            while (getline(contenido, linea))
            {
                con = con + linea + "\n";
            }

            ofstream contenido2("./csv_test.csv", ios::out);
            contenido2 << con;
            for (int i = 0; i < 7; i++)
            {
                contenido2 << momentsHu[i] << ",";
                cout << momentsHu[i] << ",";
            }

            // double d = 0.0;
            // for (int i = 0; i < 7; i++)
            // {
            //     d+=pow(momentsHu[i],2);
            // }
            // d = sqrt(d);
            // contenido2 << d << ",";
            // cout << d << ",";
            contenido2 << salida << endl;
            cout << salida << endl;
            contenido2.close();

            cv::imshow("fotoGray", img);
            cv::imshow("fotoTr", imgTr);
            cv::imshow("fotoOrg", org);
            waitKey(0);
            destroyAllWindows();    
        }
    }


    


    return 0;
}