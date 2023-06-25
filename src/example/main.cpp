/**
 * @file main.cpp
 * @author Андрей Кадомцев (m2204942@edu.misis.ru)
 * @brief Тестовая программа - консольное приложение для работы с библиотекой.
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
 * @brief Cтруктура данных файла hogconfig.txt
 * 
 */
struct HOGSettings {
    int blockSize; //!< Размер блока скользящего окна в пикселях
    int cellSize; //!< Размер ячейки в пикселях
    int stride; //!< Шаг скользящего окна в пикселях
    int binNumber; //!< Количество корзин в гистограмме каждой ячейки
    int gradType; //!< Тип вычисления градиента (беззнаковый или со знаком)
    int binWidth; //!< Ширина корзин в гистограмме каждой ячейки
    std::string folderPath; //!< Путь к папкe для сохранения файлов
    bool saveVectorData; //!< Разрешение на сохранение итогового вектора алгоритма
    bool saveTexPlots; //!< Разрешение на сохранение .tex файлов с графиками
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
        std::cout << "Параметры успешно загружены из файла hogconfig!" << std::endl;
    } else {
        throw std::runtime_error("Невозможно открыть файл для чтения.");
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
        std::cout << "Настройки сохранены" << std::endl;
    } else {
        std::cerr << "Не удалось открыть файл для записи." << std::endl;
    }
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
        std::cout << "Использование программы: " << std::endl;
        std::cout << "./hogexe -test [1-3]: Демонстрация работы алгоритма на выбранном примере (1, 2 или 3)" << std::endl;
        std::cout << "./hogexe -settings: Показать настройки программы и алгоритма" << std::endl;
        std::cout << "./hogexe -p <path to image> : Демонстрация работы алгоритма на вашем изображении" << std::endl;
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
        std::cout << "Путь для сохранения данных: " << settings.folderPath << std::endl;
        std::cout << "Разрешение на сохранение итогового HOG вектора: " << settings.saveVectorData << std::endl;
        std::cout << "Разрешение на сохранение .tex файлов с графиками: " << settings.saveTexPlots << std::endl;
        std::cout << "------------------------------------------------------------------" << std::endl;
        std::cout << "Чтобы сменить параметры алгоритма введите 1" << std::endl;
        std::cout << "Чтобы изменить путь для сохранения данных введите 2" << std::endl;
        std::cout << "Чтобы изменить разрешение на сохранение вектора введите 3" << std::endl;
        std::cout << "Чтобы изменить разрешение на сохранение .tex файлов введите 4" << std::endl;
        std::cout << "Чтобы выйти из меню введите любое другое значение" << std::endl;
        int choice;
        std::cin >> choice;
        if (choice == 1){
            std::cout << "Введите новое значение Block Size" << std::endl;
            std::cin >> settings.blockSize;
            std::cout << "Введите новое значение Cell Size" << std::endl;
            std::cin >> settings.cellSize;
            std::cout << "Введите новое значение Gradient Type (360 or 180)" << std::endl;
            std::cin >> settings.gradType;
            settings.binWidth = settings.gradType / settings.binNumber;
            settings.stride = settings.blockSize / 2;
            HOGDescriptor checkhogparams(settings.blockSize, settings.cellSize, settings.stride, settings.binNumber, settings.gradType);
            saveSettingsToFile(settings);
            std::cout << "Изменения сохранены" << std::endl;
        }
        else if (choice == 2){
            std::cout << "Введите новый путь для сохранения данных" << std::endl;
            std::cin >> settings.folderPath;
            saveSettingsToFile(settings);
            std::cout << "Изменения сохранены" << std::endl;
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
            std::cout << "Выход из меню настроек" << std::endl;
        }
    }
    else if (std::string(argv[1]) == "-test" || std::string(argv[1]) == "-t") {
        if (argc != 3) {
            std::cerr << "Некорректный ввод.\nПодалуйста, введите номер тестового изображения (1, 2 или 3)." << std::endl;
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
                    std::cerr << "Некорректный ввод.\nПожалуйста, введите номер тестового изображения (1, 2 или 3)." << std::endl;
                    break;
            }
        }
    }
    else if (std::string(argv[1]) == "-p") {
        if (argc != 3) {
            std::cerr << "Некорректный ввод.\nПожалуйста, введите путь до изображения." << std::endl;
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
        std::cout << "Примеры использования программы: " << std::endl;
        std::cout << "./hogexe -test 2: Демонстрация работы алгоритма на втором тестовом изображении" << std::endl;
        std::cout << "./hogexe -settings: Показать настройки программы и алгоритма" << std::endl;
        std::cout << "./hogexe -p /path/to/image: Демонстрация работы алгоритма на пользовательском изображении" << std::endl;
        std::cout << "./hogexe -help: Справка по использованию программы" << std::endl;
    }
    return 0;
}