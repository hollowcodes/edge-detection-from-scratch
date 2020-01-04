
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/plot.hpp>

#include <math.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define kernelSize 3

float** graysScale(cv::Mat img);
float** convolution(float** grayScaleImage, int row, int col, const float sobelFiler[kernelSize][kernelSize]);
float** mergeEdges(float** edgesX, float** edgesY, int rows, int cols);
float pythagoras(float a, float b);
void printMatrix(float** matrix, int rows, int cols);
void plotMatrix(float** matrix, int rows, int cols);
void saveMatrix(float** matrix, int rows, int cols);


int main() {
    // sobel filter for x-direction
    const float kernelX[3][3] = {{1.0, 2.0, 1.0},
                                {0.0, 0.0, 0.0}, 
                                {-1.0, -2.0, -1.0}};
                          
    // sobel filter for y-direction
    const float kernelY[3][3] = {{1.0, 0.0, -1.0},
                                {2.0, 0.0, -2.0}, 
                                {1.0, 0.0, -1.0}};

    clock_t start;
    double duration;
    start = clock();

    // loading image over OpenCv
    cv::Mat img = cv::imread("images/house.png", cv::IMREAD_COLOR);
    
    // converting OpenCv Image to gray scale float matrix
    float** graysScaleImage = graysScale(img);

    img.rows -= 2;
    img.cols -= 2;

    float** edgesX = convolution(graysScaleImage, img.rows, img.cols, kernelX);
    float** edgesY = convolution(graysScaleImage, img.rows, img.cols, kernelY);
    float** totalEdges = mergeEdges(edgesX, edgesY, img.rows, img.cols);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << "took: " << duration << endl;

    saveMatrix(totalEdges, img.rows, img.cols);

    // finish
    cv::waitKey(0);
    return 0;
}

// converts OpenCv Image into a gray scale matrix format
float** graysScale(cv::Mat img) {
    float** grayScaleImage;
    grayScaleImage = new float*[img.rows];

    for(int i=0; i<img.rows; i++) {
        grayScaleImage[i] = new float[img.cols];

        for(int j=0; j<img.cols; j++) {
            float redValue = (float) img.at<cv::Vec3b>(i,j)[0];
            grayScaleImage[i][j] = redValue / 255.0;
        }
    }

    return grayScaleImage;
}

// makes convolution over the image with sobel filer (padding 0)
float** convolution(float** matrix, int rows, int cols, const float kernel[kernelSize][kernelSize]) {
    float** output;

    output = new float*[rows];
    for(int k_i=0; k_i<rows; k_i++) {

        output[k_i] = new float[cols];
        for(int k_j=0; k_j<cols; k_j++) {

            //float scalar[kernelSize][kernelSize];
            float sum = 0;
            for(int i=0; i<kernelSize; i++) {        

                for(int j=0; j<kernelSize; j++) {
                    sum += matrix[i + k_i][j + k_j] * kernel[i][j];
                }
            }
            output[k_i][k_j] = sum;
        }
    }

    return output;
}

// merges x-axis edges and y-axis edges
float** mergeEdges(float** edgesX, float** edgesY, int rows, int cols) {
    float** output;

    output = new float*[rows];
    for(int i=0; i<rows; i++) {

        output[i] = new float[cols];
        for(int j=0; j<cols; j++) {
            output[i][j] = pythagoras((float) edgesX[i][j], (float) edgesY[i][j]);
        }
    }

    return output;
}

// pythagorean theorem
float pythagoras(float a, float b) {
    return sqrt(pow(a, 2.0) + pow(b, 2.0));
}

// prints matrix
void printMatrix(float** matrix, int rows, int cols) {
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            cout << matrix[i][j] << " , ";
        }
        cout << endl;
    }
}

// plots matrix
void plotMatrix(float** matrix, int rows, int cols) {
    cv::Mat convMat = cv::Mat(rows, cols, CV_8UC1, matrix);
    cv::imshow("Display window", convMat);
}

// saves matrix in list format: [[], [], [], ..., []]
void saveMatrix(float** matrix, int rows, int cols) {
    ofstream file;
    file.open("result_files/result_matrix.txt");

    string content = "[";
    for(int i=0; i<rows; i++) {
        content = string(content) + "[";

        for(int j=0; j<cols; j++) {
            content = string(content) + to_string(matrix[i][j]);
            if(j != cols - 1) {
                content = string(content) + ", ";
            }
        }
        content = string(content) + "]";
        if(i != rows - 1) {
            content = string(content) + ", ";
        }
    }
    content = string(content) + "]";

    file << content;
    file.close();
}

