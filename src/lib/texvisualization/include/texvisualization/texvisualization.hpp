#ifndef TEXHOG
#define TEXHOG

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>

/**
 * @brief Class for creating .tex files with plots of HOG
 */
class HOGPlots{
public:
    /**
     * @brief Construct a new HOGPlots object
     * 
     */
    HOGPlots() = default;

    /**
     * @brief Method to show the grid of cells on image
     * 
     * @param thickness Grid line thickness
     * @param cellSize Cell size in pixels
     */
    void HOGgrid(cv::Mat& image, float thickness, int cellSize);

    /**
     * @brief Method for creating a .tex file with the histogram of given cell
     * 
     * @param cellHistogram vector of cell histogram values
     * @param blockWidth width of the histogram block
     */
    void cellHistogramPlot(std::vector<float> cellHistogram, int blockWidth, const std::string& executablePath);
};

#endif 