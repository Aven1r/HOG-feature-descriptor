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

/**
 * @brief Class for calculating the HOG features.
 */
class HOGDescriptor {
public:
    /**
     * @brief Default constructor for the HOGDescriptor class.
     */
    HOGDescriptor();
    /**
     * @brief Construct a new HOGDescriptor object
     * 
     * @param blockSize Block size for the sliding window in pixels
     * @param cellSize Size of the cell in pixels
     * @param stride Sliding window stride in pixels
     * @param binNumber Number of the bins in the histogram of each cell
     * @param gradType Type of the gradient calculation (unsigned or signed)
     */
    HOGDescriptor(const size_t blockSize, const size_t cellSize, 
        const size_t stride, const size_t binNumber, const size_t gradType);
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

    /**
     * @brief Method for plotting the cellsHistogram of HOGDescriptor as a bunch of arrows within the each cell on image
     * 
     * @param scale Scale of the arrows 
     * @param imposed Background image for reference
     */
    void HOGplot(float scale, bool imposed);

public:
    static const size_t GRADIENT_SIGNED = 360; //!< 360 degree spread of histogram channels
    static const size_t GRADIENT_UNSIGNED = 180; //!< 180 degree spread of histogram channels

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
     * @brief Get the Cell Histograms object
     * 
     * @return std::vector<std::vector<std::vector<float>>> 
     */
    std::vector<std::vector<std::vector<float>>> getCellHistograms();

    // /**
    //  * @brief Method for getting the visualization of the cell HOG
    //  * 
    //  * @param x Number of the cell in the x direction
    //  * @param y Number of the cell in the y direction
    //  */
    // void visualizeHOGCell(int x, int y);

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
    int blockSize_; //!< Block size for the sliding window in pixels
    int cellSize_; //!< Size of the cell in pixels
    int binNumber_; //!< Number of the bins in the histogram of each cell
    int binWidth_; //!< Width of the bins in the histogram of each cell
    int stride_; //!< Sliding window stride in pixels
    int gradType_; //!< Type of the gradient calculation (unsigned or signed)

    bool hogFlag_ = false; //!< Flag to check if the HOG feature vector has been computed

    cv::Mat imageMagnitude_; //!< Magnitude of the gradient image
    cv::Mat imageOrientation_; //!< Orientation of the gradient image

    std::vector<std::vector<std::vector<float>>> cellHistograms_; //!< Vector of cell histograms
    std::vector<float> hogFeatureVector_; //!< Final HOG feature vector
};

#endif //HOGDESCRIPTOR_H
