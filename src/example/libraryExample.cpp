#include <hogdescriptor/hogdescriptor.hpp>
#include <texvisualization/texvisualization.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

int main(){
    // Input image
    cv::Mat img = cv::imread("path/to/image.jpg");

    // Initialize HOGDescriptor class object, compute HOG features and visualize them.
    HOGDescriptor hog;
    hog.computeHOG(img);
    hog.visualizeHOG(1, false);

    // Save HOG features to .txt file
    hog.saveVectorData("path/to/file", "filename");

    texHOG plots;
    auto cellhist = hog.getCellHistogram(7, 9);
    plots.cellHistogramPlot(cellhist, 20, "path/to/folder", 'filename');
}