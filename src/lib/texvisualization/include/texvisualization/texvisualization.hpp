#ifndef TEXHOG
#define TEXHOG

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>

/**
 * @brief Class for creating .tex files with plots of HOG feature extraction process
 */
class texHOG{
public:
    /**
     * @brief Construct a new texHOG object
     * 
     */
    texHOG() = default;

    /**
     * @brief Method for creating a .tex file with the histogram of given cell
     * 
     * @param cellHistogram Cell histogram values
     * @param binWidth Width of the histogram block
     * @param executablePath Path to the .tex file
     * @param plotName Output file name
     */
    void cellHistogramPlot(std::vector<float> cellHistogram, int binWidth, const std::string& executablePath, const std::string& plotName);

    /**
     * @brief Method for creating a .tex file with the histograms of cell within given block
     * 
     * @param blockHistogram Matrix of cell histogram values
     * @param binWidth Width of the histogram block
     * @param executablePath Path to the .tex file
     * @param plotName Output file name
     */
    void blockHistogramPlot(std::vector<std::vector<float>> blockHistogram, int binWidth, const std::string& executablePath, const std::string& plotName);
};

#endif 