#ifndef TEXHOG
#define TEXHOG

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>

/**
 * @brief Класс для создания .tex файлов для визуализации процесса
 */
class texHOG{
public:
    /**
     * @brief Конструктор класса
     * 
     */
    texHOG() = default;

    /**
     * @brief Метод для создания .tex файла с графиком гистограммы ячейки
     * 
     * @param cellHistogram Гистограмма ячейки
     * @param binWidth Ширина блока гистограммы
     * @param executablePath Путь к файлу .tex
     */
    void cellHistogramPlot(std::vector<float> cellHistogram, int binWidth, const std::string& executablePath);

    /**
     * @brief Метод для создания файла .tex с гистограммами ячеек в заданном блоке
     * 
     * @param blockHistogram Матрица гистограмм ячеек
     * @param binWidth Ширина блока гистограммы
     * @param executablePath Путь к файлу .tex
     */
    void blockHistogramPlot(std::vector<std::vector<float>> blockHistogram, int binWidth, const std::string& executablePath);
};

#endif 