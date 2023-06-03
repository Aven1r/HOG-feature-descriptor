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
    /**
     * @brief Default constructor for the HOGDescriptor class.
     */
    HOGDescriptor();
    HOGDescriptor(const size_t blockSize, const size_t cellSize, 
        const size_t stride, const size_t binNumber, const size_t gradType); /*! @brief Constructor for the HOGDescriptor class with all the parameters.*/
    HOGDescriptor(const size_t blockSize, const size_t cellSize); /*! @brief Constructor for the HOGDescriptor class with parameters for the block size and cell size.*/
    
    /**
     * @brief Destroy the HOGDescriptor object
     */
    ~HOGDescriptor();

    /**
     * @brief Assignment operator for the HOGDescriptor class
     * 
     * @param rhs HOGDescriptor object to be copied
     * @return HOGDescriptor& 
     */
    HOGDescriptor& operator=(const HOGDescriptor &rhs);

public:
    static const size_t GRADIENT_SIGNED = 360; /* 360 degree spread of histogram channels */
    static const size_t GRADIENT_UNSIGNED = 180; /* 180 degree spread of histogram channels */

    /**
     * @brief Method for computing HOG features
     * 
     * @param image: input image
     * @return std::vector<float> - HOG feature vector for the image.
     */
    void computeHOG(cv::Mat& image);

    /**
     * @brief Method for getting the HOG feature vector
     * 
     * @return std::vector<float> 
     */
    std::vector<float> getHOGFeatureVector();

    /**
     * @brief Method for getting the visualizations of the cells HOG 
     * 
     * @param hogFeatureVector 
     * @param scale 
     */
    void visualizeHOG(float scale, bool imposed);

    /**
     * @brief Get the Cell Histograms object
     * 
     * @return std::vector<std::vector<std::vector<float>>> 
     */
    std::vector<std::vector<std::vector<float>>> getCellHistograms();

    /**
     * @brief Method for getting the visualization of the cell HOG
     * 
     * @param x Number of the cell in the x direction
     * @param y Number of the cell in the y direction
     */
    void visualizeHOGCell(int x, int y);

private:
    /**
     * @brief Function to compute the magnitude and orientation of each pixel in the input image
     * 
     * @param image: input image
     */
    void computeGradientFeatures(cv::Mat& image);

    /**
     * @brief Compute the HOG feature vectors for each cell in the image.
     * 
     * @param gradient: gradient matrix computed by computeGradient()
     * @param orientation:  orientation matrix computed by computeGradient()
     * @param histograms:  output vector of HOG feature vectors for each cell
     * @return 3d vector of cell histograms
     */
    std::vector<std::vector<std::vector<float>>> computeCellHistograms(cv::Mat magnitude, cv::Mat orientation, std::vector<std::vector<std::vector<float>>>& cell_histograms);

    /**
     * @brief Method to compute the histogram for the given cell
     * 
     * @param cellMagnitude cell magnitude matrix
     * @param cellOrientation cell orientation matrix
     */
    std::vector<float> cellHistogram(const cv::Mat& cellMagnitude, const cv::Mat& cellOrientation);

    /**
     * @brief Function to normalize the HOG feature vectors for each block of cells in the image
     * 
     * @param block: vector of histograms representing the cells within a block
     */
    void normalizeBlockHistogram(std::vector<float>& block_histogram);

    /**
     * @brief Method to calculate the HOG feature vector
     * 
     * @param cell_histograms 3d vector of cell histograms
     * @return Final vector
     */
    const std::vector<float> calculateHOGVector(const std::vector<std::vector<std::vector<float>>>& cell_histograms);



private:
    // variables
    int blockSize_;
    int cellSize_;
    int binNumber_;
    int binWidth_;
    int stride_;
    int gradType_;

    float maxMagnitude_ = 0;

    // Norm_function

    bool hogFlag_ = false;

    cv::Mat imageMagnitude_;
    cv::Mat imageOrientation_;

    std::vector<std::vector<std::vector<float>>> cellHistograms_;
    std::vector<float> hogFeatureVector_;
};

#endif //HOGDESCRIPTOR_H
