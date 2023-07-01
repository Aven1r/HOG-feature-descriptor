#include <hogdescriptor/hogdescriptor.hpp>
#include <texvisualization/texvisualization.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>


/**
 * @brief Data structure for hogconfig.txt file
 */
struct HOGSettings {
    int blockSize; //!< Block size of the sliding window
    int cellSize; //!< Size of the cell in pixels
    int stride; //!< Sliding window stride in pixels
    int binNumber; //!< Number of the bins in the histogram of each cell
    int gradType; //!< Type of the gradient calculation (unsigned or signed)
    int binWidth; //!< Width of the bins in the histogram of each cell
    std::string folderPath; //!< Path to the output folder
    bool saveVectorData; //!< Permission to save .txt files with the vector data
    bool saveTexPlots; //!< Permission to save .tex files with the plots
};

/**
 * @brief Method to load settings from file
 */
HOGSettings loadSettingsFromFile() {
    HOGSettings settings;
    std::string currentdir = INSTALL_PATH;
    std::ifstream file(currentdir + "/hogconfig.txt");
    if (file.is_open()) {

        file >> settings.blockSize;
        file >> settings.cellSize;
        file >> settings.stride;
        file >> settings.binNumber;
        file >> settings.gradType;
        file >> settings.binWidth;
        file >> settings.folderPath;
        file >> settings.saveVectorData;
        file >> settings.saveTexPlots;
        
        file.close();
        std::cout << "Settings loaded!" << std::endl;
    } else {
        throw std::runtime_error("Unable to open file with settings.");
    }
    return settings;
}

void saveSettingsToFile(const HOGSettings& settings) {
    std::ofstream file("hogconfig.txt");
    if (file.is_open()) {
        file << settings.blockSize << std::endl;
        file << settings.cellSize << std::endl;
        file << settings.stride << std::endl;
        file << settings.binNumber << std::endl;
        file << settings.gradType << std::endl;
        file << settings.binWidth << std::endl;
        file << settings.folderPath << std::endl;
        file << settings.saveVectorData << std::endl;
        file << settings.saveTexPlots << std::endl;
        file.close();
        std::cout << "Settings saved!" << std::endl;
    } else {
        std::cerr << "Cannot open file with settings for writing." << std::endl;
    }
}


int main(int argc, char** argv){
    std::string currentdir = INSTALL_PATH;
    HOGSettings settings = loadSettingsFromFile();
    if (argc < 2 || (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h"))) {
        std::cout << "Program usage: " << std::endl;
        std::cout << "./hogexe -test [1-3]: Demo of the HOG algorithm work (1, 2 or 3)" << std::endl;
        std::cout << "./hogexe -settings: Show current settings" << std::endl;
        std::cout << "./hogexe -p <path to image> : Process your image" << std::endl;
    }
    else if ((std::string(argv[1]) == "-settings" || std::string(argv[1]) == "-s") && argc == 2) {
        std::cout << "------------------------Текущие настройки-------------------------" << std::endl;
        std::cout << "Block size: " << settings.blockSize << std::endl;
        std::cout << "Cell size: " << settings.cellSize << std::endl;
        std::cout << "Stride: " << settings.stride << std::endl;
        std::cout << "Bin number: " << settings.binNumber << std::endl;
        std::cout << "Gradient type: " << settings.gradType << std::endl;
        std::cout << "Bin width: " << settings.binWidth << std::endl;
        std::cout << "-----Прочее-----" << std::endl;
        std::cout << "Output folder: " << settings.folderPath << std::endl;
        std::cout << "Permission to save .txt files with the vector data: " << settings.saveVectorData << std::endl;
        std::cout << "Permission to save .tex files with the plots: " << settings.saveTexPlots << std::endl;
        std::cout << "------------------------------------------------------------------" << std::endl;
        std::cout << "To change settings, enter 1" << std::endl;
        std::cout << "To change output folder, enter 2" << std::endl;
        std::cout << "To change .txt files permission, enter 3" << std::endl;
        std::cout << "To change .tex files permission, enter 4" << std::endl;
        std::cout << "Press any other key to exit" << std::endl;
        int choice;
        std::cin >> choice;
        if (choice == 1){
            std::cout << "Enter new value for Block Size" << std::endl;
            std::cin >> settings.blockSize;
            std::cout << "Enter new value for Cell Size"  << std::endl;
            std::cin >> settings.cellSize;
            std::cout << "Choose the Gradient Type (360 or 180)" << std::endl;
            std::cin >> settings.gradType;
            settings.binWidth = settings.gradType / settings.binNumber;
            settings.stride = settings.blockSize / 2;
            HOGDescriptor checkhogparams(settings.blockSize, settings.cellSize, settings.stride, settings.binNumber, settings.gradType);
            saveSettingsToFile(settings);
        }
        else if (choice == 2){
            std::cout << "Input path to the output folder" << std::endl;
            std::cin >> settings.folderPath;
            saveSettingsToFile(settings);
        }
        else if (choice == 3){
            if (settings.saveVectorData == true){
                settings.saveVectorData = false;
                saveSettingsToFile(settings);
            }
            else{
                settings.saveVectorData = true;
                saveSettingsToFile(settings);
            }
        }
        else if (choice == 4){
            if (settings.saveTexPlots == true){
                settings.saveTexPlots = false;
                saveSettingsToFile(settings);
            }
            else{
                settings.saveTexPlots = true;
                saveSettingsToFile(settings);
            }
        }
        else{
            std::cout << "Exiting the program" << std::endl;
        }
    }
    else if (std::string(argv[1]) == "-test" || std::string(argv[1]) == "-t") {
        if (argc != 3) {
            std::cerr << "Incorrect number of arguments. Enter ./hogexe -t 1, 2 or 3" << std::endl;
        }
        else {
            int example = std::stoi(argv[2]);
            switch (example)
            {
                case 1:{
                    HOGDescriptor hog(32, 16);
                    cv::Mat img = cv::imread(currentdir + "/images/test1.jpg");
                    hog.computeHOG(img);
                    if (settings.saveVectorData) {
                        hog.saveVectorData(settings.folderPath, "test1_vector.txt");
                    }
                    if (settings.saveTexPlots) {
                        texHOG plots;
                        auto cellhist = hog.getCellHistogram(6, 8);
                        plots.cellHistogramPlot(cellhist, settings.binWidth, settings.folderPath, "test1_cellhist.tex");
                        auto blockhist = hog.getBlockHistogram(6, 8);
                        plots.blockHistogramPlot(blockhist, settings.binWidth, settings.folderPath, "test1_blockhist.tex");
                    }
                    cv::imshow("Original image", img);
                    hog.HOGgrid(img, 1, 16);
                    hog.visualizeHOG(2, false);
                    cv::waitKey(0);
                    break;
                }
                case 2:{
                    HOGDescriptor hog(20, 10);
                    cv::Mat img = cv::imread(currentdir + "/images/test2.jpg");
                    hog.computeHOG(img);
                    if (settings.saveVectorData) {
                        hog.saveVectorData(settings.folderPath, "test2_vector.txt");
                    }
                    if (settings.saveTexPlots) {
                        texHOG plots;
                        auto cellhist = hog.getCellHistogram(7, 9);
                        plots.cellHistogramPlot(cellhist, settings.binWidth, settings.folderPath, "test2_cellhist.tex");
                        auto blockhist = hog.getBlockHistogram(6, 8);
                        plots.blockHistogramPlot(blockhist, settings.binWidth, settings.folderPath, "test2_blockhist.tex");
                    }
                    cv::imshow("Original image", img);
                    hog.HOGgrid(img, 1, 8);
                    hog.visualizeHOG(1, true);
                    cv::waitKey(0);
                    break;
                }
                case 3:{
                    HOGDescriptor hog(50, 25);
                    cv::Mat img = cv::imread(currentdir + "/images/test3.jpg");
                    hog.computeHOG(img);
                    cv::imshow("Original image", img);
                    hog.HOGgrid(img, 1, 12);
                    hog.visualizeHOG(1, true);
                    if (settings.saveVectorData) {
                        hog.saveVectorData(settings.folderPath, "test3_vector.txt");
                    }
                    if (settings.saveTexPlots) {
                        texHOG plots;
                        auto cellhist = hog.getCellHistogram(7, 9);
                        plots.cellHistogramPlot(cellhist, settings.binWidth, settings.folderPath, "test3_cellhist.tex");
                        auto blockhist = hog.getBlockHistogram(6, 8);
                        plots.blockHistogramPlot(blockhist, settings.binWidth, settings.folderPath, "test3_blockhist.tex");
                    }
                    cv::waitKey(0);
                    break;
                }
                default:
                    std::cerr << "Incorrect example number. Enter 1, 2 or 3" << std::endl;
                    break;
            }
        }
    }
    else if (std::string(argv[1]) == "-p") {
        if (argc != 3) {
            std::cerr << "Invalid path." << std::endl;
        }
        std::string imagepath = argv[2];

        // Extract the filename from the image path
        std::string filename = imagepath.substr(imagepath.find_last_of("/\\") + 1);
        // Remove the file extension from the filename
        std::string vectorFilename = filename.substr(0, filename.find_last_of("."));
        
        cv::Mat img = cv::imread(imagepath);
        HOGDescriptor hog(settings.blockSize, settings.cellSize, settings.stride, settings.binNumber, settings.gradType);
        hog.computeHOG(img);
        cv::imshow("Original image", img);
        hog.HOGgrid(img, 1, settings.cellSize);
        hog.visualizeHOG(1, false);
        if (settings.saveVectorData) {
            hog.saveVectorData(settings.folderPath, (vectorFilename + "_vector.txt"));
        }
        if (settings.saveTexPlots) {
            texHOG plots;
            auto cellhist = hog.getCellHistogram(7, 9);
            plots.cellHistogramPlot(cellhist, settings.binWidth, settings.folderPath, (vectorFilename + "_cellhist.tex"));
            auto blockhist = hog.getBlockHistogram(6, 8);
            plots.blockHistogramPlot(blockhist, settings.binWidth, settings.folderPath, (vectorFilename + "_blockhist.tex"));
        }
        cv::waitKey(0);
    }
    else {
        std::cout << "Program usage example: " << std::endl;
        std::cout << "./hogexe -test 2: Demo of HOG algorithm on test2.jpg" << std::endl;
        std::cout << "./hogexe -settings: Show settings" << std::endl;
        std::cout << "./hogexe -p /path/to/image: Process user image" << std::endl;
        std::cout << "./hogexe -help: Show program usage" << std::endl;
    }
    return 0;
}