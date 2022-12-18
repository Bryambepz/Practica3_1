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
Mat gray;
Mat imgLab;

#define compab_mask_inc(ptr, shift)                                          \
    {                                                                        \
        value |= ((unsigned int)(cntr - *ptr) & 0x80000000) >> (31 - shift); \
        ptr++;                                                               \
    }

int *lbp(const int *data, int rows, int columns)
{
    const int
        *pos0 = data,
        *pos1 = pos0 + 1,
        *pos2 = pos1 + 1,
        *pos3 = pos2 + columns,
        *pos4 = pos3 + columns,
        *pos5 = pos4 - 1,
        *pos6 = pos5 - 1,
        *pos7 = pos6 - columns,
        *center = pos7 + 1;

    int cntr;
    unsigned int value;
    int *result = (int *)malloc(256 * sizeof(int));

    memset(result, 0, 256 * sizeof(int));

    for (int r = 0; r < rows - 2; r++)
    {
        for (int c = 0; c < columns - 2; c++)
        {
            value = 0;
            cntr = *center - 1;
            compab_mask_inc(pos0, 0);
            compab_mask_inc(pos1, 1);
            compab_mask_inc(pos2, 2);
            compab_mask_inc(pos3, 3);
            compab_mask_inc(pos4, 4);
            compab_mask_inc(pos5, 5);
            compab_mask_inc(pos6, 6);
            compab_mask_inc(pos7, 7);
            center++;
            result[value]++;
        }

        pos0 += 2;
        pos1 += 2;
        pos2 += 2;
        pos3 += 2;
        pos4 += 2;
        pos5 += 2;
        pos6 += 2;
        pos7 += 2;
        center += 2;
    }
    return result;
}

vector<int> histogramLBP(Mat img)
{
    cvtColor(img, gray, COLOR_BGR2GRAY);

    int rows = img.rows;
    int cols = img.cols;

    int *pixeles = (int *)malloc(rows * cols * sizeof(int));
    int k = 0;
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            *(pixeles + k) = (int)img.at<uchar>(i, j);
            k++;
        }
    }

    vector<int> histo;
    int *histoP = lbp(pixeles, rows, cols);
    for (int i = 0; i < 256; i++)
        histo.push_back(histoP[i]);

    free(pixeles);

    return histo;
}

void convertirColor()
{
    cv::cvtColor(imgLab, imgLab, COLOR_BGR2Lab);
}

int main()
{
    String path = "IMGS";
    String salida;

    ofstream contenido2("./histograma.csv", ios::out);
    for (int i = 0; i < 256; i++)
    {
        contenido2 << to_string(i) << ",";
    }
    contenido2 << "salida" << endl;
    contenido2.close();

    for(const fs::directory_entry& dir_entry : fs::directory_iterator(path)){
        salida = dir_entry.path().string().substr(5,dir_entry.path().string().length()-1);
        
        for(const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(dir_entry.path().string())){
            // cout << dir_entry.path().string() << endl;
            
            img = imread(dir_entry.path().string(), IMREAD_COLOR);
            namedWindow("cieLab", WINDOW_AUTOSIZE);
            imgLab = img.clone();
            convertirColor();
            imshow("cieLab", imgLab);
            waitKey(0);
            destroyAllWindows();
            cout << "\n" << img.size() << endl;
            vector<int> histogram = histogramLBP(img);

            ifstream contenido("./histograma.csv", ios::in);
            String linea;
            String con = "";
            while (getline(contenido, linea))
            {
                con = con + linea + "\n";
                // cout << con ;
            }
            // cout << salida << endl;

            ofstream contenido2("./histograma.csv", ios::out);
            contenido2 << con;
            int c = 0;
            for (int i = 0; i < histogram.size(); i++)
            {
                contenido2 << histogram[i] << ",";
                c++;
                // cout << i << "---," << histogram[i]<<"||||";
            }
            contenido2 << salida << endl;
            contenido2.close();
        }
    }

    // img = imread("textura2.jpg", IMREAD_COLOR);
    // cout << img.size() << endl;
    // vector<int> histogram = histogramLBP(img);

    // cout << endl;


    return 0;
}