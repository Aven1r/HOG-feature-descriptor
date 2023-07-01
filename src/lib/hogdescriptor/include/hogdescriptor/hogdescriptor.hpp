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
 * @brief Class for calculating the HOG (Histogram of oriented gradients) features.
 */
class HOGDescriptor {
public:
    /**
     * @brief Default constructor for the HOGDescriptor class.
     */
    HOGDescriptor();
    /**
     * @brief Construct a new HOGDescriptor object with the given parameters
     * 
     * @param blockSize Block size of the sliding window
     * @param cellSize Size of the cell
     * @param stride Sliding window stride
     * @param binNumber Number of the bins in the histogram for each cell
     * @param gradType Type of the gradient calculation (unsigned or signed)
     */
    HOGDescriptor(const size_t blockSize, const size_t cellSize, 
        const size_t stride, const size_t binNumber, const size_t gradType);
    /**
     * @brief Construct a new HOGDescriptor object
     * 
     * @param blockSize Block size of the sliding window
     * @param cellSize Size of the cell
     */
    HOGDescriptor(const size_t blockSize, const size_t cellSize);
    /**
     * @brief Destroy the HOGDescriptor object
     */
    ~HOGDescriptor();

public:
    /**
     * @brief Method to visualize the final vector in separate window
     * 
     * @param scale Scale of the arrows 
     * @param imposed Background magnitude image for reference
     */
    void visualizeHOG(float scale, bool imposed);
    /**
     * @brief Method to show the grid of cells on image
     * 
     * @param image Input image
     * @param thickness Grid line thickness
     * @param cellSize Cell size in pixels
     */
    void HOGgrid(cv::Mat& image, float thickness, int cellSize);

public:
    static const size_t GRADIENT_SIGNED = 360; //!< 360 degree spread of histogram channels
    static const size_t GRADIENT_UNSIGNED = 180; //!< 180 degree spread of histogram channels
    /**
     * @brief Method for computing HOG features
     * 
     * @param image: Input image
     */
    void computeHOG(cv::Mat& image);

    /**
     * @brief Method for getting the HOG feature vector
     * 
     * @return Vector of features
     */
    std::vector<float> getHOGFeatureVector();

    /**
     * @brief Get the Cell Histogram object
     * 
     * @param y Cell row position
     * @param x Cell column position
     * @return Histogram vector for the cell
     */
    std::vector<float> getCellHistogram(int y, int x);
    /**
     * @brief Get the Block Histogram object
     * 
     * @param y first cell row position
     * @param x first cell column position
     * @return Block histogram matrix
     */
    std::vector<std::vector<float>> getBlockHistogram(int y, int x);
    /**
     * @brief Save hog vector in a file
     * 
     * @param executablePath Path where file will be saved
     * @param vectorName Output vector name 
     */
    void saveVectorData(const std::string& executablePath, const std::string& vectorName);

private:
    /**
     * @brief Function to compute each pixel's gradient magnitude and orientation
     * 
     * @param image: Input image
     */
    void computeGradientFeatures(cv::Mat& image);

    /**
     * @brief Compute the HOG feature vectors for each cell in the image.
     * 
     * @param gradient: Gradient matrix
     * @param orientation:  Orientation matrix
     * @param cell_histograms:  Output vector of HOG feature vectors for each cell
     * @return Matrix of histograms
     */
    std::vector<std::vector<std::vector<float>>> computeCellHistograms(cv::Mat magnitude, cv::Mat orientation, std::vector<std::vector<std::vector<float>>>& cell_histograms);

    /**
     * @brief Method to compute the histogram for the given cell
     * 
     * @param cellMagnitude Cell magnitude matrix
     * @param cellOrientation Cell orientation matrix
     */
    std::vector<float> cellHistogram(const cv::Mat& cellMagnitude, const cv::Mat& cellOrientation);

    /**
     * @brief Function to normalize the HOG feature vectors for each block of cells in the image
     * 
     * @param block: Vector of histograms representing the cells within a block
     */
    void normalizeBlockHistogram(std::vector<float>& block_histogram);

    /**
     * @brief Method to calculate the HOG feature vector
     * 
     * @param cell_histograms Matrix of histograms
     * @return Final vector
     */
    const std::vector<float> calculateHOGVector(const std::vector<std::vector<std::vector<float>>>& cell_histograms);

private:
    int blockSize_; //!< Block size of the sliding window
    int cellSize_; //!< Size of the cell in pixels
    int binNumber_; //!< Number of the bins in the histogram of each cell
    int binWidth_; //!< Width of the bins in the histogram of each cell
    int stride_; //!< Sliding window stride in pixels
    int gradType_; //!< Type of the gradient calculation (unsigned or signed)

    bool hogFlag_ = false; //!< Flag to check if the HOG feature vector has been computed

    cv::Mat imageMagnitude_; //!< Magnitude of the gradients
    cv::Mat imageOrientation_; //!< Orientation of the gradients

    std::vector<std::vector<std::vector<float>>> cellHistograms_; //!< Matrix of cell histograms
    std::vector<float> hogFeatureVector_; //!< Final vector of features
};

#endif //HOGDESCRIPTOR_H
