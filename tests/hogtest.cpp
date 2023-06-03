// #include <doctest/doctest.h>
// #include <hogdescriptor.hpp>

// TEST_CASE("HOGDescriptor: output vector"){
//     HOGDescriptor descriptor;
//     SUBCASE("Vector size"){
//         cv::Mat image = cv::imread("/Users/avenir/vscode/С++/cpp_github/HOG-feature-descriptor/src/example/test2.JPG");
//         descriptor.computeHOG(image);
//         auto histograms = descriptor.getHOGFeatureVector();
//         CHECK(histograms.size() == 0);
//     }
// }