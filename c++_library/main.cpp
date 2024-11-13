#include <iostream>
#include <list>
#include <ctime>
#include "cnn.hpp"
#include "mse.hpp"
using namespace std;
typedef unsigned int uint;
using namespace cppcnn;

// Function to quantize a weight (double) to int8_t
int16_t quantize_weight(double weight, double scaling_factor) {
    // Apply the scaling factor to adjust the precision
    double scaled_value = weight * scaling_factor;

    // Round the scaled value and cast to int8_t
    int16_t quantized_value = static_cast<int16_t>(round(scaled_value));

    return quantized_value;
}

// Function to dequantize a  n int16_t weight back to double
double dequantize_weight(int16_t quantized_weight, double scaling_factor) {
    // Reverse the scaling by dividing by the scaling factor
    return static_cast<double>(quantized_weight) / scaling_factor;
}

// Function to read weights from a file, quantize them, and save to a new file
void quantize_weights(const std::string& input_file, const std::string& output_file, double scaling_factor) {
    std::ifstream infile(input_file);
    std::ofstream outfile(output_file);

    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return;
    }

    double weight;
    std::vector<int16_t> quantized_weights;

    // Read weights from the input file and quantize them
    while (infile >> weight) {
        int16_t quantized_weight = quantize_weight(weight, scaling_factor);
        quantized_weights.push_back(quantized_weight);
    }

    // Write the quantized weights to the output file
    for (int16_t q_weight : quantized_weights) {
        outfile << q_weight << std::endl;
    }

    // Close the files
    infile.close();
    outfile.close();

    std::cout << "Quantized weights have been written to: " << output_file << std::endl;
}

void dequantize_weights(const std::string& input_file, const std::string& output_file, double scaling_factor) {
    std::ifstream infile(input_file);
    std::ofstream outfile(output_file);
    int ind = 0;
    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return;
    }

    int16_t quantized_weight;
    std::vector<double> dequantized_weights;

    // Read quantized weights from the input file and dequantize them
    while (infile >> quantized_weight) {
        ind++;
        double dequantized_weight = dequantize_weight(quantized_weight, scaling_factor);
        dequantized_weights.push_back(dequantized_weight);
    }

    // Write the dequantized weights to the output file
    for (double d_weight : dequantized_weights) {
        outfile << d_weight << std::endl;

    }
    cout << ind;
    // Close the files
    infile.close();
    outfile.close();

    std::cout << "Dequantized weights have been written to: " << output_file << std::endl;
}

int main()
{

    int8_t input1[28][28] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
        {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
        {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0},
        {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0},
        {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    int8_t input2[28][28] = {
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
        };

     vector<Array3d> vec;
     vector<double> im;
     for (int i = 0; i < 28; i++) {
         for (int j = 0; j < 28; j++) {
             im.push_back(input1[i][j]);
         }
     }
    vec.push_back(Array3d(28, 28, 1, im));

    // * PARAMETERS * 
    
    // path to database
    string filename_train_images = "../MNIST/train-images.idx3-ubyte";
    string filename_train_labels = "../MNIST/train-labels.idx1-ubyte";
    string filename_test_images = "../MNIST/t10k-images.idx3-ubyte";
    string filename_test_labels = "../MNIST/t10k-labels.idx1-ubyte";

    // Feature detector architecture
    Layer3D* f1 = new ConvLayer(5, 1, 3, 1, 0);
    Layer3D* p1 = new MaxPoolLayer(2, 2);
    Layer3D* f2 = new ConvLayer(5, 3, 6, 1, 0);
    Layer3D* p2 = new MaxPoolLayer(2, 2);
    list<Layer3D*> feature_detector{f1, p1, f2, p2};

    // Classifier architecture
    FCLayer l1(4*4*6, 64);
    FCLayer l2(64, 10);
    list<FCLayer> classifier{l1, l2};

    // Learning rates
    double classifier_lr = 0.03;
    double feature_detector_lr = 0.0003;



    MSE* loss = new MSE;
    
    // Number of epochs
    uint n_epoch = 18;
    
    
    // * MAIN * 
    
    // Building the network
    CNN net(feature_detector, classifier, loss,
            feature_detector_lr, classifier_lr);

    //Read and set the database
    MNIST_Handler database_loader;
    vector<Array3d> train_database_images =
                     database_loader.read_mnist_image(filename_train_images);
    cout << "train images loaded" << endl;
    vector<int8_t> train_database_labels =
                    database_loader.read_mnist_label(filename_train_labels);
    cout << "train labels loaded" << endl;
    vector<Array3d> test_database_images =
                     database_loader.read_mnist_image(filename_test_images);
    cout << "test images loaded" << endl;
    vector<int8_t> test_database_labels =
                    database_loader.read_mnist_label(filename_test_labels);
    cout << "test labels loaded" << endl;
    net.set_train_database(&test_database_images, &test_database_labels);
    net.set_test_database(&test_database_images, &test_database_labels);
    //
    // // Training phase
    net.train(n_epoch);
    cout << endl;




    net.save("WEIGHTS_SMALL_CNN");

    // Example input and output files
    std::string input_file = "WEIGHTS_SMALL_CNN.txt";
    std::string output_file = "quantized_weights.txt";

    // Scaling factor for quantization (modifiable as needed)
    double scaling_factor = 10000;

    // Accuracy test
    //net.load("WEIGHTS_FINAL15epochs.txt");

    // Call the function to quantize weights and write to a new file
    quantize_weights(input_file, output_file, scaling_factor);



    //l1.print_kernel();
    cout << "Accuracy on test dataset: " << net.test_accuracy() << endl;
    //cout << "FC2" << endl;
    //l2.print_kernel();
    
    for (auto pointer:feature_detector) delete pointer;
    return 0;
}