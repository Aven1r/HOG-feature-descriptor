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
 * @brief Класс для работы с дескриптором HOG (Гистограмма направленных градиентов)
 */
class HOGDescriptor {
public:
    /**
     * @brief Конструктор для инициализации объекта класса HOGDescriptor с параметрами по умолчанию
     */
    HOGDescriptor();
    
    /**
     * @brief Конструктор для класса HOGDescriptor со всеми параметрами
     * 
     * @brief Конструктор для класса HOGDescriptor со всеми параметрами
     * @param blockSize Размер блока скользящего окна в пикселях
     * @param cellSize Размер ячейки в пикселях
     * @param stride Шаг скользящего окна в пикселях
     * @param binNumber Количество корзин в гистограмме каждой ячейки
     * @param gradType Тип вычисления градиента (беззнаковый или со знаком)
     */
    HOGDescriptor(const size_t blockSize, const size_t cellSize, 
        const size_t stride, const size_t binNumber, const size_t gradType);
    
    /**
     * @brief Конструктор для создания нового объекта HOGDescriptor
     * 
     * @param blockSize Размер блока скользящего окна в пикселях
     * @param cellSize Размер ячейки в пикселях
     * 
     */
    HOGDescriptor(const size_t blockSize, const size_t cellSize);
    
    /**
     * @brief Деструктор для класса HOGDescriptor
     */
    ~HOGDescriptor();

public: // Публичные методы для визуализации
    /**
     * @brief Метод для визуализации гистограмм ячеек HOGDescriptor в виде стрелок внутри каждой ячейки на изображении
     * 
     * @param scale Масштаб стрелок
     * @param imposed Изображение фона для справки по амплитуде
     */
    void visualizeHOG(float scale, bool imposed);

    /**
     * @brief Метод для отображения сетки ячеек на изображении
     * 
     * @param thickness Толщина линий сетки
     * @param cellSize Размер ячейки в пикселях
     */
    void HOGgrid(cv::Mat& image, float thickness, int cellSize);

public:
    static const size_t GRADIENT_SIGNED = 360; //!< Разброс градиента на 360 градусов
    static const size_t GRADIENT_UNSIGNED = 180; //!< Разброс градиента на 180 градусов

    /**
     * @brief Метод для вычисления гистограмм HOG
     * 
     * @param image Входное изображение
     * @return std::vector<float> - Вектор гистограммы HOG для изображения
     */
    void computeHOG(cv::Mat& image);

    /**
     * @brief Метод для получения вектора гистограммы HOG
     * 
     * @return std::vector<float> 
     */
    std::vector<float> getHOGFeatureVector();

    /**
     * @brief Получение гистограммы ячейки
     * 
     * @param y Позиция ячейки по вертикали
     * @param x Позиция ячейки по горизонтали
     * @return std::vector<float> Вектор гистограммы для ячейки
     */
    std::vector<float> getCellHistogram(int y, int x);

    /**
     * @brief Получение гистограммы блока
     * 
     * @param y Позиция первой ячейки блока по вертикали
     * @param x Позиция первой ячееки блока по горизонали
     * @return std::vector<std::vector<float>> 
     */
    std::vector<std::vector<float>> getBlockHistogram(int y, int x);

    /**
     * @brief Сохранение вектора HOG в файл
     * 
     * @param executablePath Путь, где будет сохранен файл
     * @param vectorName Имя вектора
     */
    void saveVectorData(const std::string& executablePath, const std::string& vectorName);

private:
    /**
     * @brief Функция для вычисления амплитуды и ориентации градиента каждого пикселя
     * 
     * @param image Входное изображение
     */
    void computeGradientFeatures(cv::Mat& image);

    /**
     * @brief Вычисление гистограмм HOG для каждой ячейки изображения.
     * 
     * @param magnitude Амплитуда градиента, вычисленная с помощью computeGradient()
     * @param orientation Ориентация градиента, вычисленная с помощью computeGradient()
     * @param cell_histograms Выходной вектор гистограмм HOG для каждой ячейки
     * @return 3D-вектор гистограмм ячеек
     */
    std::vector<std::vector<std::vector<float>>> computeCellHistograms(cv::Mat magnitude, cv::Mat orientation, std::vector<std::vector<std::vector<float>>>& cell_histograms);

    /**
     * @brief Метод для вычисления гистограммы для данной ячейки
     * 
     * @param cellMagnitude Матрица амплитуд ячейки
     * @param cellOrientation Матрица ориентаций ячейки
     */
    std::vector<float> cellHistogram(const cv::Mat& cellMagnitude, const cv::Mat& cellOrientation);

    /**
     * @brief Функция для нормализации значений из гистограмм HOG для объедененных ячеек из блока
     * 
     * @param block Вектор значений, представляющих гистограммы ячеек для всего блока
     */
    void normalizeBlockHistogram(std::vector<float>& block_histogram);

    /**
     * @brief Метод для вычисления вектора гистограммы HOG
     * 
     * @param cell_histograms 3D-вектор гистограмм ячеек
     * @return Финальный вектор
     */
    const std::vector<float> calculateHOGVector(const std::vector<std::vector<std::vector<float>>>& cell_histograms);

private:
    int blockSize_; //!< Размер блока скользящего окна в пикселях
    int cellSize_; //!< Размер ячейки в пикселях
    int binNumber_; //!< Количество корзин в гистограмме каждой ячейки
    int binWidth_; //!< Ширина корзин в гистограмме каждой ячейки
    int stride_; //!< Шаг скользящего окна в пикселях
    int gradType_; //!< Тип вычисления градиента (беззнаковый или со знаком)

    bool hogFlag_ = false; //!< Флаг, указывающий, был ли вычислен вектор гистограммы HOG

    cv::Mat imageMagnitude_; //!< Амплитуда градиента изображения
    cv::Mat imageOrientation_; //!< Ориентация градиента изображения

    std::vector<std::vector<std::vector<float>>> cellHistograms_; //!< Вектор гистограмм ячеек
    std::vector<float> hogFeatureVector_; //!< Финальный вектор гистограммы HOG

};

#endif //HOGDESCRIPTOR_H
