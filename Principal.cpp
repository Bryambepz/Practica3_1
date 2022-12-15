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
using namespace cv;
using namespace std;
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
void pintarPoligono()
{
    for (int i = 0; i < imgTr.rows; i++)
    {
        for (int j = 0; j < imgTr.cols; j++)
        {
            int pixel = imgTr.at<uchar>(i,j);
            if (pixel == 0){
                contNegros = contNegros + 1;
                // umbralExt = pixel;

                // ifstream contenido("./pixeles.txt", ios::in);
                // String linea;
                // String con = "";
                // while (getline(contenido, linea))
                // {
                //     con = con + linea + "\n";
                // }

                // ofstream contenido2("./pixeles.txt", ios::out);
                // contenido2 << con << pixel << endl;
                // contenido2.close();

            }else{
                contBlancos = contBlancos + 1;
            }
        }
        
    }
    
}

int main(int argc, char *argv[])
{
    img = imread("corpus/TRAIN/Star/Star_074f6b98-2a94-11ea-8123-8363a7ec19e6.png");
    img = imread("corpus/TRAIN/Circle/Circle_0579363e-2a92-11ea-8123-8363a7ec19e6.png");
    namedWindow("foto", WINDOW_AUTOSIZE);
    namedWindow("fotoTr", WINDOW_AUTOSIZE);
    namedWindow("copia", WINDOW_AUTOSIZE);

    cvtColor(img, img, COLOR_BGR2GRAY);
    copia = img.clone();
    cv::Laplacian(img, img, -1, CV_16S);
    cout << "int = " << umbralInt << " extg = " << umbralExt << endl;
    cv::threshold(copia, imgTr, 70, 255, THRESH_BINARY_INV);
    pintarPoligono();
    if(contBlancos > contNegros){
        cv::bitwise_not(imgTr, imgTr);
    }

    cv::imshow("foto", img);
    cv::imshow("fotoTr", imgTr);
    cv::imshow("copia", copia);

    waitKey(0);
    destroyAllWindows();

    return 0;
}