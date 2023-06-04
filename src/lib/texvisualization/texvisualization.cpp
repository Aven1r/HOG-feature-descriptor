#include <texvisualization/texvisualization.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void HOGPlots::cellHistogramPlot(std::vector<float> values, int binWidth, const std::string& executablePath){
    
    fs::path directoryPath = fs::path(executablePath).remove_filename();

    fs::path folderPath = directoryPath / "plots"; // Replace "folder_name" with the desired folder name

    // Create the folder if it doesn't exist
    if (!fs::exists(folderPath))
        fs::create_directory(folderPath);

    // Create the file path
    fs::path filePath = folderPath / "histogram.tex";

    // Open the file
    std::ofstream file(filePath);
    if (!file)
    {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    auto y_max = std::max_element(values.begin(), values.end());
    auto x_max = values.size() * binWidth;

    file << "\\documentclass{article}" << std::endl;
    file << "\\usepackage{pgfplots}" << std::endl;
    file << "\\begin{document}" << std::endl;
    file << "\\begin{tikzpicture}" << std::endl;
    file << "\\begin{axis}[" << std::endl;
    file << " width=\\textwidth," << std::endl;
    file << " height=7cm," << std::endl;
    file << " xmin=0, xmax=" + std::to_string(x_max) + "," << std::endl;
    file << " ymin=0, ymax=" + std::to_string(*y_max + 0.3) + "," << std::endl;
    file << " xtick distance=" + std::to_string(binWidth) + "," << std::endl;
    file << "minor y tick num=0.1," << std::endl;
    file << "area style, \n]" << std::endl;

    file << "\\addplot+[ybar interval] plot coordinates {";

    for (int i = 0; i < values.size(); i++){
        file << "(" + std::to_string(i * binWidth) + "," + std::to_string(values[i]) + ") ";
    }

    file << "};" << std::endl;


    file << "\\end{axis}" << std::endl;
    file << "\\end{tikzpicture}" << std::endl;
    file << "\\end{document}" << std::endl;

    file.close();

    std::cout << "Histogram created successfully." << std::endl;
}