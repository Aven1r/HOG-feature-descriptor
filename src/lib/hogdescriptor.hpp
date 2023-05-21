#ifndef HOGDESCRIPTOR_H
#define HOGDESCRIPTOR_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <memory>
#include <vector>
#include <functional>
#include <math.h>

class HOGDescriptor {
public:
    HOGDescriptor();
    HOGDescriptor(const size_t blockSize, const size_t cellSize, 
        const size_t stride, const size_t binNumber, const size_t gradType);
    ~HOGDescriptor();

    // Copy constructor
    HOGDescriptor(const HOGDescriptor &lhs);

    // Assignment operator
    HOGDescriptor& operator=(const HOGDescriptor &lhs);

public:
    
    static const size_t GRADIENT_SIGNED = 360;
    static const size_t GRADIENT_UNSIGNED = 180;

    /**
     * @brief Method for computing HOG features
     * 
     * @param image: input image
     * @return std::vector<float> - HOG feature vector for the image.
     */
    std::vector<float> computeHOG(cv::Mat image);


private:
    /**
     * @brief Function to compute the gradient and orientation of each pixel in the input image
     * 
     * @param image: input image
     * @param gradient:  output magnitude of the gradient at each pixel
     * @param orientation output orientation of the gradient at each pixel
     */
    void computeGradient(cv::Mat image, cv::Mat& gradient, cv::Mat& orientation);

    /**
     * @brief Compute the HOG feature vectors for each cell in the image.
     * 
     * @param gradient: gradient matrix computed by computeGradient()
     * @param orientation:  orientation matrix computed by computeGradient()
     * @param histograms:  output vector of HOG feature vectors for each cell
     */
    void computeCellHistograms(cv::Mat gradient, cv::Mat orientation, std::vector<std::vector<float>>& histograms);

    /**
     * @brief Function to normalize the HOG feature vectors for each block of cells in the image
     * 
     * @param block: vector of histograms representing the cells within a block
     */
    void normalizeBlock(std::vector<float>& block);

private:
    // variables
    int blockSize_;
    int cellSize_;
    int binNumber_;
    int binWidth_;
    int stride_;
    int gradType_;
    // Norm_function

    cv::Mat imageGradient_;
    cv::Mat imageOrientation_;
};

#endif //HOGDESCRIPTOR_H