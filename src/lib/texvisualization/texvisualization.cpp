#include <texvisualization/texvisualization.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void texHOG::cellHistogramPlot(std::vector<float> values, int binWidth, const std::string& executablePath){
    
    fs::path directoryPath = fs::path(executablePath);

    // Path to the folder where the plots will be saved
    fs::path folderPath = directoryPath / "plots";

    // Create the folder if it doesn't exist
    if (!fs::exists(folderPath))
        fs::create_directory(folderPath);

    // Create the file path
    fs::path filePath = folderPath / "cellhistogram.tex";

    // Open the file
    std::ofstream file(filePath);
    if (!file)
    {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    // Calculate the number of bins
    int numBins = values.size();

    // Calculate the maximum magnitude
    float maxMagnitude = *std::max_element(values.begin(), values.end()) + 0.2;

    // Generate the TeX code for the histogram
    file << "\\documentclass[border=5mm]{standalone}\n"
        << "\\usepackage{pgfplots}\n"
        << "\\pgfplotsset{compat=1.18}\n"
        << "\\begin{document}\n"
        << "\\begin{tikzpicture}\n"
        << "\\begin{axis}[\n"
        << "    ybar,\n"
        << "    width=10cm,\n"
        << "    enlargelimits=0.05,\n"
        << "    ylabel={Magnitude},\n"
        << "    symbolic x coords={";
    
    // Generate the x-axis labels
    for (int i = 0; i < numBins; ++i) {
        file << "" << i * binWidth << "-" << ((i + 1) * binWidth) << "";
        if (i != numBins - 1) {
            file << ",";
        }
    }

    file << "},\n"
        << "    xtick=data,\n"
        << "    tick label style={rotate=45, anchor=east},\n"
        << "    nodes near coords,\n"
        << "    nodes near coords align={vertical},\n"
        << "    ]\n"
        << "\\addplot coordinates {\n";

    // Generate the y-axis values
    for (int i = 0; i < numBins; ++i) {
        file << "(" << i * binWidth << "-" << ((i + 1) * binWidth) << "," << values[i] << ")";
        if (i != numBins - 1) {
            file << "\n";
        }
    }

    file << "};\n"
              << "\\end{axis}\n"
              << "\\end{tikzpicture}\n"
              << "\\end{document}\n";

    file << "Histogram created successfully." << std::endl;
}

void texHOG::blockHistogramPlot(std::vector<std::vector<float>> blockHistogram, int binWidth, const std::string& executablePath) {

    fs::path directoryPath = fs::path(executablePath);

    // Path to the folder where the plots will be saved
    fs::path folderPath = directoryPath / "plots";

    // Create the folder if it doesn't exist
    if (!fs::exists(folderPath))
        fs::create_directory(folderPath);

    // Create the file path
    fs::path filePath = folderPath / "blockhistogram.tex";

    // Open the file
    std::ofstream file(filePath);
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return;
    }
    
    // Calculate the number of bins
    int numCells = blockHistogram.size();
    int numBins = blockHistogram[0].size();

    // Calculate the maximum magnitude
    float maxMagnitude = 0;
    for (int i = 0; i < numCells; ++i) {
        for (int j = 0; j < numBins; ++j) {
            if (blockHistogram[i][j] > maxMagnitude) {
                maxMagnitude = blockHistogram[i][j];
            }
        }
    }
    

    // Generate the TeX code for the histogram
    file << "\\documentclass[border=5mm]{standalone}\n"
        << "\\usepackage{pgfplots}\n"
        << "\\pgfplotsset{compat=1.18}\n"
        << "\\begin{document}\n"
        << "\\begin{tikzpicture}\n"
        << "\\begin{axis}[\n"
        << "    ybar interval,\n"
        << "    width=15cm,\n"
        << "    enlargelimits=0.05,\n"
        << "    ylabel={Magnitude},\n"
        << "    symbolic x coords={";
    
    // Generate the x-axis labels
    for (int i = 0; i < (numBins + 1); ++i) {
        file << "" << i * binWidth << "-" << ((i + 1) * binWidth) << "";
        if (i != numBins) {
            file << ",";
        }
    }

    file << "},\n"
        << "    xtick=data,\n"
        << "    tick label style={rotate=45, anchor=east},\n"
        << "    ]\n";

    for (int i = 0; i < numCells; ++i) {
        // Generate the y-axis values
        file << "\\addplot coordinates {\n";
        for (int j = 0; j < numBins; ++j) {
            file << "(" << j * binWidth << "-" << ((j + 1) * binWidth) << "," << blockHistogram[i][j] << ")";
            if (j != numBins - 1) {
                file << "\n";
            }
        }
        file << "(" << numBins * binWidth << "-" << ((numBins + 1) * binWidth) << "," << 0 << ")";
        file << "};\n";
        file << "\\addlegendentry{Cell " << i + 1 << "}\n";
    }
    
        file << "\\end{axis}\n"
            << "\\end{tikzpicture}\n"
            << "\\end{document}\n";

    
    std::cout << "Block-plot created successfully." << std::endl;
}
