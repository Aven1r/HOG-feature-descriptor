#include <hogdescriptor.hpp>
#include <iostream>

void check_ctor_params(size_t blockSize, size_t cellSize, size_t stride, size_t binNumber, size_t gradType){
        if (blockSize < 2){
        throw std::invalid_argument("HOGDescriptor: blockSize must be >= 2");
    }
    if (cellSize < 2){
        throw std::invalid_argument("HOGDescriptor: cellSize must be >= 2");
    }
    if (cellSize % blockSize != 0){
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
    
    // Check the input image
    if (!image.data)
        throw std::runtime_error("Invalid image!");
    if (image.rows < blockSize_ || image.cols < blockSize_)
        throw std::runtime_error("The image is smaller than blocksize!");
    
    // Extract the magnitude and orientation of every pixel
    computeGradientFeatures(image);

    // Calculate the histogram of gradients in (cellSize x cellSize) cells //2016 cells
    cellHistograms_ = computeCellHistograms(imageMagnitude_, imageOrientation_, cellHistograms_); //18,144 values (cells_y*cells_x*binNumber_)

    hogFeatureVector_ = calculateHOGVector(cellHistograms_);

    hogFlag_ = true;
}

void HOGDescriptor::computeGradientFeatures(cv::Mat& image){
    // See https://learnopencv.com/histogram-of-oriented-gradients/
    image.convertTo(image, CV_32F, 1/255.0);
    cv::Mat gx, gy;
    cv::Sobel(image, gx, CV_32F, 1, 0, 1);
    cv::Sobel(image, gy, CV_32F, 0, 1, 1);
    cartToPolar(gx, gy, imageMagnitude_, imageOrientation_, 1);
}

std::vector<std::vector<std::vector<float>>> HOGDescriptor::computeCellHistograms(cv::Mat magnitude, cv::Mat orientation, std::vector<std::vector<std::vector<float>>>& cell_histograms){
    
    size_t cells_y = static_cast<int>(magnitude.rows / cellSize_);
    size_t cells_x = static_cast<int>(magnitude.cols / cellSize_);

    cell_histograms.resize(cells_y);

    for (size_t i = 0; i < cells_y; ++i) {
        cell_histograms[i].resize(cells_x);
        for (size_t j = 0; j < cells_x; ++j) {
            cv::Rect cell = cv::Rect(cellSize_ * j, cellSize_ * i, cellSize_, cellSize_);
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
            const float* ptr_row_mag = cellMagnitude.ptr<float>(i);
            const float* ptr_row_ori = cellOrientation.ptr<float>(i);
            for (size_t j = 0; j < cellMagnitude.cols; ++j) {
                cell_histogram.at(static_cast<int>(ptr_row_ori[j] / binWidth_)) += ptr_row_mag[j];
            }
        }
    } else {
        for (size_t i = 0; i < cellMagnitude.rows; ++i) {
            const float* ptr_row_mag = cellMagnitude.ptr<float>(i);
            const float* ptr_row_ori = cellOrientation.ptr<float>(i);
            for (size_t j = 0; j < cellMagnitude.cols; ++j) {
                float orientation = ptr_row_ori[j];
                if (orientation >= 180){
                    orientation -= 180;
                }
                cell_histogram.at(orientation / binWidth_) += ptr_row_mag[j];
            }
        }
    }
    return cell_histogram;
}

std::vector<std::vector<std::vector<float>>> HOGDescriptor::getCellHistograms(){
    if (hogFlag_ == false){
        throw std::runtime_error("HOG vector is not computed yet!");
    }
    return cellHistograms_;
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

    // Iterate over the blocks in the image
    for (int y = 0; y < blocksY; y++) {
        for (int x = 0; x < blocksX; x++) {
            std::vector<float> blockHistogram;

            // Iterate over the cells within the block
            for (int i = y * stride_ / cellSize_; i < (y * stride_ + blockSize_) / cellSize_; i++) {
                for (int j = x * stride_ / cellSize_; j < (x * stride_ + blockSize_) / cellSize_; j++){
                    blockHistogram.insert(blockHistogram.end(), cell_histograms[i][j].begin(), cell_histograms[i][j].end());
                }
            }
            // Perform block normalization (L2-Hys)
            normalizeBlockHistogram(blockHistogram);

            // Append the block histogram to the HOG feature vector
            hog_vector.insert(hog_vector.end(), blockHistogram.begin(), blockHistogram.end());
        }
    }

    return hog_vector;
}

void HOGDescriptor::normalizeBlockHistogram(std::vector<float>& block_histogram) {
    float sumOfSquares = 0.0;
    for (float value : block_histogram) {
        sumOfSquares += value * value;
    }
    float eps = 1e-5; // Small constant for numerical stability
    float sqrtSumOfSquares = std::sqrt(sumOfSquares + eps);
    for (float& value : block_histogram) {
        value /= sqrtSumOfSquares;
        value = std::min(value, 0.2f);
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

    // Calculate the number of cells in the x and y directions
    int cellsX = cellHistograms_[0].size();
    int cellsY = cellHistograms_.size();

    // Iterate over each cell in the image
    for (int y = 0; y < cellsY; y++) {
        for (int x = 0; x < cellsX; x++) {
            // Get the cell histogram for the current cell
            std::vector<float> cellHistogram = cellHistograms_[y][x];
            //normalizeBlockHistogram(cellHistogram);

            // Calculate the cell position in the image
            int cellX = x * cellSize_;
            int cellY = y * cellSize_;

            // Iterate over each bin in the cell histogram
            for (int bin = 0; bin < binNumber_; bin++) {
                // Get the magnitude of the current bin
                float magnitude = cellHistogram[bin] * scale;

                // Calculate the angle of the current bin
                float angle = bin * binWidth_;

                // Calculate the endpoint of the arrow
                float arrowX = cellX + cellSize_ / 2 + magnitude * cellSize_ / 2 * cos(angle);
                float arrowY = cellY + cellSize_ / 2 + magnitude * cellSize_ / 2 * sin(angle);

                // Draw an arrow from the cell center to the endpoint
                cv::arrowedLine(visualization, cv::Point(cellX + cellSize_ / 2, cellY + cellSize_ / 2),
                                cv::Point(arrowX, arrowY), cv::Scalar(255, 255, 255), 1);
            }
        }
    }

    // Display or save the visualization image as needed
    cv::imshow("HOG Visualization", visualization);
    cv::waitKey(0);
}

void HOGDescriptor::visualizeHOGCell(int cellX, int cellY) {
    if (hogFlag_ == false){
        throw std::runtime_error("Cell histograms is not computed just yet!");
    }
    // Create a histogram
    cv::Mat histogram(256, 256, CV_8UC1, cv::Scalar(0));

    // Generate some random numbers (replace with your own data)
    std::vector<float> numbers = cellHistograms_[cellY][cellX];

    // Update the histogram based on the numbers
    for (int number : numbers)
    {
        histogram.at<uchar>(255 - (number), number++);
    }

    // Normalize the histogram for display
    cv::normalize(histogram, histogram, 0, 255, cv::NORM_MINMAX);

    // Create an image to display the histogram
    cv::Mat histogramImage(256, 256, CV_8UC3, cv::Scalar(0, 0, 0));

    // Draw the histogram
    for (int i = 0; i < 256; i++)
    {
        cv::line(histogramImage, cv::Point(i, 256), cv::Point(i, 256 - histogram.at<uchar>(i)), cv::Scalar(255, 255, 255));
    }

    // Display the histogram
    cv::namedWindow("Histogram");
    cv::imshow("Histogram", histogramImage);
    cv::waitKey(0);
}