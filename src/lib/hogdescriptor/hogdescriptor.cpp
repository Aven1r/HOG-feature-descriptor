#include "include/hogdescriptor/hogdescriptor.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;


//Проверка на валидность полей класса HOGDescriptor
void check_ctor_params(size_t blockSize, size_t cellSize, size_t stride, size_t binNumber, size_t gradType){
    if (blockSize < 2){
        throw std::invalid_argument("HOGDescriptor: blockSize must be >= 2");
    }
    if (cellSize < 2){
        throw std::invalid_argument("HOGDescriptor: cellSize must be >= 2");
    }
    if (blockSize % cellSize != 0){
        throw std::invalid_argument("HOGDescriptor: cellSize must be multiple of blockSize");
    }
    if (stride < 2){
        throw std::invalid_argument("HOGDescriptor: stride must be >= 2");
    }
    if (180 % binNumber != 0){
        throw std::invalid_argument("HOGDescriptor: binNumber must be a divisor of 180");
    }
    if (gradType != 180 && gradType != 360){
        throw std::invalid_argument("HOGDescriptor: gradType must be 180 or 360");
    }
}

HOGDescriptor::HOGDescriptor()
    : blockSize_(16), cellSize_(8), stride_(8), binNumber_(9), gradType_(GRADIENT_UNSIGNED), 
      binWidth_(GRADIENT_UNSIGNED / 9){
    }

HOGDescriptor::HOGDescriptor(const size_t blockSize, const size_t cellSize, 
        const size_t stride, const size_t binNumber, const size_t gradType)
        : blockSize_(blockSize), cellSize_(cellSize),
    stride_(stride),
    binNumber_(binNumber),
    gradType_(gradType),
    binWidth_(gradType / binNumber){
        check_ctor_params(blockSize, cellSize, stride, binNumber, gradType);
        }

HOGDescriptor::HOGDescriptor(const size_t blockSize, const size_t cellSize)
        : blockSize_(blockSize), cellSize_(cellSize),
    stride_(blockSize / 2),
    binNumber_(9),
    gradType_(GRADIENT_UNSIGNED),
    binWidth_(GRADIENT_UNSIGNED / 9){
        check_ctor_params(blockSize, cellSize,  stride_, binNumber_, gradType_);
        }

HOGDescriptor::~HOGDescriptor() {}

void HOGDescriptor::computeHOG(cv::Mat& image){
    
    // Проверка на валидность входного изображения
    if (!image.data)
        throw std::runtime_error("Invalid image!");
    if (image.rows < blockSize_ || image.cols < blockSize_)
        throw std::runtime_error("The image is smaller than blocksize!");
    
    // Извлечение амплитуды и направленности для каждого пикселя входного изображения
    computeGradientFeatures(image);

    // Подсчет гистограм для каждой из ячеек изображения
    cellHistograms_ = computeCellHistograms(imageMagnitude_, imageOrientation_, cellHistograms_); //18,144 values (cells_y*cells_x*binNumber_)

    // Подсчет финального вектора путем объединения нормализованных значений ячеек внутри скользящего блока
    hogFeatureVector_ = calculateHOGVector(cellHistograms_);

    hogFlag_ = true;
}

void HOGDescriptor::computeGradientFeatures(cv::Mat& image){
    // Compute each pixel's gradient magnitude and orientation
    // See https://learnopencv.com/histogram-of-oriented-gradients/
    image.convertTo(image, CV_32F, 1/255.0);
    cv::Mat gx, gy;
    cv::Sobel(image, gx, CV_32F, 1, 0, 1);
    cv::Sobel(image, gy, CV_32F, 0, 1, 1);
    cartToPolar(gx, gy, imageMagnitude_, imageOrientation_, 1);
}

std::vector<std::vector<std::vector<float>>> HOGDescriptor::computeCellHistograms(cv::Mat magnitude, cv::Mat orientation, std::vector<std::vector<std::vector<float>>>& cell_histograms){
    
    // Количество ячеек в каждом направлении
    size_t cells_y = static_cast<int>(magnitude.rows / cellSize_);
    size_t cells_x = static_cast<int>(magnitude.cols / cellSize_);

    cell_histograms.resize(cells_y);

    // Проход по всем ячейкам изображения
    for (size_t i = 0; i < cells_y; ++i) {
        cell_histograms[i].resize(cells_x);
        for (size_t j = 0; j < cells_x; ++j) {
            // Создание матрицы с областью ячейки
            cv::Rect cell = cv::Rect(cellSize_ * j, cellSize_ * i, cellSize_, cellSize_);
            // Подсчет ее гистограмы
            const std::vector<float> cell_histogram = cellHistogram(cv::Mat(imageMagnitude_, cell), cv::Mat(imageOrientation_, cell));
            cell_histograms[i][j] = cell_histogram;
        }
    }

    return cell_histograms;
}

std::vector<float> HOGDescriptor::cellHistogram(const cv::Mat& cellMagnitude, const cv::Mat& cellOrientation){
    std::vector<float> cell_histogram(binNumber_);
    if(gradType_ == GRADIENT_SIGNED) {
        for (size_t i = 0; i < cellMagnitude.rows; ++i) {
            const float* rowMagnitude = cellMagnitude.ptr<float>(i);
            const float* rowOrientation = cellOrientation.ptr<float>(i);
            for (size_t j = 0; j < cellMagnitude.cols; ++j) {
                cell_histogram.at(static_cast<int>(rowOrientation[j] / binWidth_)) += rowMagnitude[j];
            }
        }
    } else {
        for (size_t i = 0; i < cellMagnitude.rows; ++i) {
            const float* rowMagnitude = cellMagnitude.ptr<float>(i);
            const float* rowOrientation = cellOrientation.ptr<float>(i);
            for (size_t j = 0; j < cellMagnitude.cols; ++j) {
                float orientation = rowOrientation[j];
                if (orientation >= 180){
                    orientation -= 180;
                }
                cell_histogram.at(orientation / binWidth_) += rowMagnitude[j];
            }
        }
    }
    return cell_histogram;
}

std::vector<float> HOGDescriptor::getCellHistogram(int y, int x) {
    if (!hogFlag_) {
        throw std::runtime_error("HOG vector is not computed yet!");
    }

    if (y >= 0 && y < cellHistograms_.size() && x >= 0 && x < cellHistograms_[y].size()) {
        return cellHistograms_[y][x];
    } else {
        throw std::runtime_error("Invalid position!");
    }
}

std::vector<std::vector<float>> HOGDescriptor::getBlockHistogram(int y, int x) {
    if (!hogFlag_) {
        throw std::runtime_error("HOG vector is not computed yet!");
    }

    int numCellsInDirection = (blockSize_ / cellSize_);

    if (y >= 0 && y < cellHistograms_.size() - numCellsInDirection && x >= 0 && x < cellHistograms_[y].size() - numCellsInDirection) {
        std::vector<std::vector<float>> blockHistograms;

        for (int i = y; i < y + numCellsInDirection; i++) {
            for (int j = x; j < x + numCellsInDirection; j++) {
                if (i >= 0 && i < cellHistograms_.size() && j >= 0 && j < cellHistograms_[i].size()) {
                    blockHistograms.push_back(getCellHistogram(i, j));
                } else {
                    throw std::runtime_error("Invalid position within the block!");
                }
            }
        }

        return blockHistograms;
    } else {
        throw std::runtime_error("Invalid position!");
    }
}

std::vector<float> HOGDescriptor::getHOGFeatureVector(){
    if (hogFlag_ == false){
        throw std::runtime_error("HOG vector is not computed yet!");
    }
    return hogFeatureVector_;
}

const std::vector<float> HOGDescriptor::calculateHOGVector(const std::vector<std::vector<std::vector<float>>>& cell_histograms) {
    std::vector<float> hog_vector;

    // 
    int imageWidth = cell_histograms[0].size() * cellSize_;
    int imageHeight = cell_histograms.size() * cellSize_;
    int blocksX = (imageWidth - blockSize_) / stride_ + 1;
    int blocksY = (imageHeight - blockSize_) / stride_ + 1;

    // Проход по блокам изображения
    for (int y = 0; y < blocksY; y++) {
        for (int x = 0; x < blocksX; x++) {
            std::vector<float> blockHistogram;

            // Проход по ячейкам внутри блока
            for (int i = y * stride_ / cellSize_; i < (y * stride_ + blockSize_) / cellSize_; i++) {
                for (int j = x * stride_ / cellSize_; j < (x * stride_ + blockSize_) / cellSize_; j++){
                    blockHistogram.insert(blockHistogram.end(), cell_histograms[i][j].begin(), cell_histograms[i][j].end());
                }
            }
            // Block normalization (L2-Hys)
            normalizeBlockHistogram(blockHistogram);

            // Добавление подсчитанного вектора к финальному
            hog_vector.insert(hog_vector.end(), blockHistogram.begin(), blockHistogram.end());
        }
    }

    return hog_vector;
}

void HOGDescriptor::normalizeBlockHistogram(std::vector<float>& block_histogram) {
    //L2-hys normalization
    float sumOfSquares = 0.0;
    for (float value : block_histogram) {
        sumOfSquares += value * value;
    }
    float eps = 1e-5; // Small constant for numerical stability
    float sqrtSumOfSquares = std::sqrt(sumOfSquares + eps);
    for (float& value : block_histogram) {
        value /= sqrtSumOfSquares;
        value = std::min(value, 0.5f);
    }
}

void HOGDescriptor::visualizeHOG(float scale, bool imposed) {
    if (hogFlag_ == false){
        throw std::runtime_error("HOG vector is not computed yet!");
    }
    cv::Mat visualization;

    // Create a visualization image
    if (imposed == true){
        visualization = imageMagnitude_.clone();
    }
    else {
        visualization.create(imageMagnitude_.rows, imageMagnitude_.cols, CV_8UC3);
        visualization.setTo(cv::Scalar(0, 0, 0));
    }

    // Подсчет количества ячеек на изображении
    int cellsX = cellHistograms_[0].size();
    int cellsY = cellHistograms_.size();

    // Проход по ячейкам изображения
    for (int y = 0; y < cellsY; y++) {
        for (int x = 0; x < cellsX; x++) {

            std::vector<float> cellHistogram;
            
            // Добавление всех ячеек из блока где ячейка (y, x) является первой
            int numCellDirections = (blockSize_ / cellSize_);
            for (int i = 0; i < numCellDirections; i++) {
                for (int j = 0; j < numCellDirections; j++) {
                    if (y+i >= 0 && y+i < cellsY && x+j >= 0 && x+j < cellsX){
                        cellHistogram.insert(cellHistogram.end(), cellHistograms_[y+i][x+j].begin(), cellHistograms_[y+i][x+j].end());
                    }
                }
            }
            // Block normalization (L2-Hys)
            // Так как каждая ячейка играет роль в подсчете финального вектора более одного раза из за нормализации блоков,
            // То простых способов визуализации вектора на выходе алгоритма у нас нет.
            // Чтобы добиться хоть примерно правильной отрисовки,
            // мы применяем к каждой ячейке, в которой мы хотим нарисовать визуализацию ее гистограммы в виде стрелок,
            // нормализацию блока, где она учавствует 
            normalizeBlockHistogram(cellHistogram);

            // Удаление остальных ячеек вектора кроме [y][x]
            cellHistogram.erase(cellHistogram.begin() + binNumber_, cellHistogram.end());

            // Подсчет позиции вектора на изображении
            int cellX = x * cellSize_;
            int cellY = y * cellSize_;

            // Проход по всем корзинам гистограммы ячейки
            for (int bin = 0; bin < binNumber_; bin++) {
                // Амплитуда в текущей ячейке гистограммы с учетом пользовательского масштабирования
                float magnitude = cellHistogram[bin] * scale;

                // Подсчет угла
                float angle = bin * binWidth_;

                // Подсчет конечной точки стрелки
                float arrowX = cellX + cellSize_ / 2 + magnitude * cellSize_ / 2 * cos(angle);
                float arrowY = cellY + cellSize_ / 2 + magnitude * cellSize_ / 2 * sin(angle);

                // Отрисовка стрелки на изображении
                cv::arrowedLine(visualization, cv::Point(cellX + cellSize_ / 2, cellY + cellSize_ / 2),
                                cv::Point(arrowX, arrowY), cv::Scalar(255, 255, 255), 1);
            }
        }
    }

    cv::imshow("HOG Visualization", visualization);
}

void HOGDescriptor::HOGgrid(cv::Mat& image, float thickness, int cellSize) {
    cv::Mat imageWithCells = image.clone();

    for (int i = 0; i < image.rows; i += cellSize) {
        for (int j = 0; j < image.cols; j += cellSize) {
            cv::Point startPoint(j, i);
            cv::Point endPoint(j + cellSize - 1, i);
            cv::line(imageWithCells, startPoint, endPoint, cv::Scalar(255, 255, 255), thickness);

            startPoint = cv::Point(j, i);
            endPoint = cv::Point(j, i + cellSize - 1);
            cv::line(imageWithCells, startPoint, endPoint, cv::Scalar(255, 255, 255), thickness);
        }
    }

    cv::imshow("HOG Grid", imageWithCells);
}

void HOGDescriptor::saveVectorData(const std::string& executablePath, const std::string& vectorName){
    
    fs::path directoryPath = fs::path(executablePath);

    if (!fs::exists(directoryPath)){
        std::cerr << "Error: The path does not exist." << std::endl;
    }

    // Path to the folder where the plots will be saved
    fs::path folderPath = directoryPath / "vectors";

    // Create the folder if it doesn't exist
    if (!fs::exists(folderPath))
        fs::create_directory(folderPath);

    // Create the file path
    fs::path filePath = folderPath / vectorName;

    // Open the file
    std::ofstream file(filePath);
    if (!file)
    {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    std::vector<float> hogVector = getHOGFeatureVector();
    for (int i = 0; i < hogVector.size(); i++) {
        file << hogVector[i] << " ";
    }
    file.close();
    
    std::cout << "HOG вектор сохранен!" << std::endl;
}