//
// Created by Andy Mina on 3/7/23.
//

// 3rd party
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

// stl
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>

using namespace std;

/**
 * Generates a 1D matrix of the specified size with random numbers between [0, 256).
 *
 * @param size - the specified size
 * @returns a 1D vector representing a Matrix
*/
vector<uchar> generateMatrix(size_t size) {
    vector<uchar> res;
    for (int i = 0; i < size; i++)
        res.push_back(rand() % 256);
    return res;
}

/**
 * Applies the Sobel convolution operator to the input.
*/
void applySobel(cv::InputArray input, cv::OutputArray output) {
    cv::GaussianBlur(input, output, cv::Size(3, 3), 0, 0);

    cv::Mat x_nrg, y_nrg;
    cv::Sobel(output, x_nrg, CV_16S, 1, 0);
    cv::Sobel(output, y_nrg, CV_16S, 0, 1);

    // convert back to CV_8U depth and merge
    cv::convertScaleAbs(x_nrg, x_nrg);
    cv::convertScaleAbs(y_nrg, y_nrg);
    cv::addWeighted(x_nrg, 0.5, y_nrg, 0.5, 0, output);
}

int main() {
    // define constants
    const int MATRIX_ROWS = 6;
    const int MATRIX_COLS = 6;
    // seed rng
    srand(time(0));

    // create the matrix and it's sobel
    vector<uchar> matrix_data = generateMatrix(MATRIX_ROWS * MATRIX_COLS);
    cv::Mat input(MATRIX_ROWS, MATRIX_COLS, CV_8U, matrix_data.data());
    cv::Mat output;
    applySobel(input, output);

    cout << "MATRIX\n";
    cout << input << "\n\n";
    cout << "SOBEL\n";
    cout << output << "\n\n";
}
