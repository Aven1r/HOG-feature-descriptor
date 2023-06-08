/**
 * @file main.cpp
 * @author Андрей Кадомцев (m2204942@edu.misis.ru)
 * @brief Тестовая программа - консольное приложение с параметрами командной строки для работы с библиотекой.
 * @version 0.1
 * @date 2023-06-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <hogdescriptor/hogdescriptor.hpp>
#include <texvisualization/texvisualization.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

/**
 * @brief Cтруктура данных для файла hogconfig.txt
 * 
 */
struct HOGSettings {
    int blockSize;
    int cellSize;
    int stride;
    int binNumber;
    int gradType;
    int binWidth;
    std::string folderPath;
    std::string saveVectorData;
};

/**
 * @brief Функция загрузки данных из файла hogconfig.txt
 * 
 * @return HOGSettings - данные файла
 */
HOGSettings loadSettingsFromFile() {
    HOGSettings settings;
    std::string currentdir = INSTALL_PATH;
    std::ifstream file(currentdir + "/hogconfig.txt");
    if (file.is_open()) {
        // Read parameters from the file
        file >> settings.blockSize;
        file >> settings.cellSize;
        file >> settings.stride;
        file >> settings.binNumber;
        file >> settings.gradType;
        file >> settings.binWidth;
        file >> settings.folderPath;
        file >> settings.saveVectorData;
        
        file.close();
        std::cout << "Settings loaded from config.txt" << std::endl;
    } else {
        throw std::runtime_error("Unable to open the file for reading.");
    }
    return settings;
}

/**
 * @brief Точка запуска приложения
 * 
 * @param argc количество параметров введенных пользователем в командную строку
 * @param argv массив с введенными пользователем параметрами командной строки
 * @return int 
 */
int main(int argc, char** argv){
    std::string currentdir = INSTALL_PATH;
    HOGSettings settings = loadSettingsFromFile();
    if (argc < 2 || (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h"))) {
        std::cout << "Usage of hogLibrary example program: " << std::endl;
        std::cout << "hoglib -ex [1-3] : Shows how Library works on an example (1, 2 or 3)" << std::endl;
        std::cout << "hoglib -settings : Shows current settings of the library and allows to change them" << std::endl;
        std::cout << "hoglib -tex i j : Saves a .tex file with the histogram in the cell and block (i, j) of the test image" << std::endl;
        std::cout << "hoglib <path to image> : Shows the library's algorithm work on your image" << std::endl;
    }
    else if (std::string(argv[1]) == "-settings" && argc == 2) {
        std::cout << "------------------------CURRENT SETTINGS-------------------------" << std::endl;
        std::cout << "Block size: " << settings.blockSize << std::endl;
        std::cout << "Cell size: " << settings.cellSize << std::endl;
        std::cout << "Stride: " << settings.stride << std::endl;
        std::cout << "Bin number: " << settings.binNumber << std::endl;
        std::cout << "Gradient type: " << settings.gradType << std::endl;
        std::cout << "Bin width: " << settings.binWidth << std::endl;
        std::cout << "Folder path: " << settings.folderPath << std::endl;
        std::cout << "Permissions to save vector after hog computation: " << settings.saveVectorData << std::endl;
        std::cout << "------------------------------------------------------------------" << std::endl;
        std::cout << "If you want to change settings, please take a look at the settings.txt in the root folder of the project and rerun the installation process: " << INSTALL_PATH << std::endl;
    }
    else if (std::string(argv[1]) == "-tex") {
        if (argc != 4) {
            std::cerr << "Invalid input.\nPlease specify index for cell in the format i j, where i represents the row and j represents the column." << std::endl;
            return 0;
        }
        if (settings.folderPath == "NONE"){
            std::cerr << "Folder path not specified. Please specify it in the settings." << std::endl;
            return 0;
        }
        else {
            texHOG tex;
            HOGDescriptor hog(settings.blockSize, settings.cellSize, settings.stride, settings.binNumber, settings.gradType);
            std::string imagepath = currentdir + "/images/test2.jpg";
            cv::Mat img = cv::imread(imagepath);
            hog.computeHOG(img);
            auto vec = hog.getCellHistogram(std::stoi(argv[2]), std::stoi(argv[3]));
            tex.cellHistogramPlot(vec, settings.binWidth, settings.folderPath);
            auto blockVec = hog.getBlockHistogram(std::stoi(argv[2]), std::stoi(argv[3]));
            tex.blockHistogramPlot(blockVec, settings.binWidth, settings.folderPath);
            std::cout << "Done!\nHistograms saved to " << settings.folderPath << std::endl;
        }
    }
    else if (std::string(argv[1]) == "-ex") {
        if (argc != 3) {
            std::cerr << "Invalid input.\nPlease specify example number (1, 2 or 3)." << std::endl;
        }
        else {
            HOGDescriptor hog(settings.blockSize, settings.cellSize, settings.stride, settings.binNumber, settings.gradType);
            int example = std::stoi(argv[2]);
            std::string imagepath = currentdir;
            switch (example)
            {
                case 1:{
                    cv::Mat img = cv::imread(currentdir + "/images/test1.jpg");
                    hog.computeHOG(img);
                    cv::imshow("Original image", img);
                    hog.HOGgrid(img, 1, settings.cellSize);
                    hog.visualizeHOG(2, false);
                    if (settings.saveVectorData == "true") {
                        hog.saveVectorData(settings.folderPath, "test1_vector.txt");
                    }
                    cv::waitKey(0);
                    break;
                }
                case 2:{
                    cv::Mat img = cv::imread(currentdir + "/images/test2.jpg");
                    hog.computeHOG(img);
                    cv::imshow("Original image", img);
                    hog.HOGgrid(img, 1, settings.cellSize);
                    hog.visualizeHOG(1, false);
                    if (settings.saveVectorData == "true") {
                        hog.saveVectorData(settings.folderPath, "test2_vector.txt");
                    }
                    cv::waitKey(0);
                    break;
                }
                case 3:{
                    cv::Mat img = cv::imread(currentdir + "/images/test3.jpg");
                    hog.computeHOG(img);
                    cv::imshow("Original image", img);
                    hog.HOGgrid(img, 1, settings.cellSize);
                    hog.visualizeHOG(1, false);
                    if (settings.saveVectorData == "true") {
                        hog.saveVectorData(settings.folderPath, "test3_vector.txt");
                    }
                    cv::waitKey(0);
                    break;
                }
                default:
                    std::cerr << "Invalid input.\nPlease specify example number (1, 2 or 3)." << std::endl;
                    break;
            }
        }
    }
    else {
        std::string imagepath = argv[1];
        cv::Mat img = cv::imread(imagepath);
        HOGDescriptor hog(settings.blockSize, settings.cellSize, settings.stride, settings.binNumber, settings.gradType);
        hog.computeHOG(img);
        cv::imshow("Original image", img);
        hog.HOGgrid(img, 1, settings.cellSize);
        hog.visualizeHOG(1, false);
        if (settings.saveVectorData == "true") {
            hog.saveVectorData(settings.folderPath, (std::string(argv[1]) + "_vector.txt"));
        }
        cv::waitKey(0);
    }
    return 0;
}