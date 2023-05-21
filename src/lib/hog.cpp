#include <lib/hogdescriptor.hpp>

HOGDescriptor::HOGDescriptor()
    : blockSize_(16), cellSize_(8), stride_(8), binNumber_(9), gradType_(GRADIENT_UNSIGNED), 
      binWidth_(gradType_ / binNumber_){
        check_ctor_params(blockSize_, cellSize_, stride_, binNumber_, gradType_);
    }

HOGDescriptor::HOGDescriptor(const size_t blockSize, const size_t cellSize, 
        const size_t stride, const size_t binNumber, const size_t gradType)
        : blockSize_(blockSize), cellSize_(cellSize != 0 ? cellSize : blockSize / 2),
    stride_(stride != 0 ? stride : blockSize / 2),
    binNumber_(binNumber),
    gradType_(gradType),
    binWidth_(gradType / binNumber){
        check_ctor_params(blockSize, cellSize, stride, binNumber, gradType);
    }

void check_ctor_params(const size_t blockSize, const size_t cellSize, const size_t stride, const size_t binNumber, const size_t gradType){
    if (blockSize < 2){
        throw std::invalid_argument("HOGDescriptor: blockSize must be >= 2");
    }
    if (cellSize < 2){
        throw std::invalid_argument("HOGDescriptor: cellSize must be >= 2");
    }

}