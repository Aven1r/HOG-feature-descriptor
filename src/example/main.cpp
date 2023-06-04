#include <hogdescriptor/hogdescriptor.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

int main(){
    HOGDescriptor descriptor;
    cv::Mat image = cv::imread("/Users/avenir/vscode/С++/cpp_github/HOG-feature-descriptor/src/example/test2.JPG");
    descriptor.computeHOG(image);
    auto histograms = descriptor.getHOGFeatureVector();
    std::cout << "Histogram size: " << histograms.size() << "\n";
    for (auto i : histograms){
        std::cout << i << " ";
    }
    //To-do: add visualization for cell and block
    // descriptor.visualizeHOG(0.3, false);
    descriptor.HOGplot(10,10);
    
    return 0;
}