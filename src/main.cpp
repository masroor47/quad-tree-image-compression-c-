#include <iostream>
#include <fstream>
#include "QuadTree.h"

int main(int argc, const char* argv[]) {

    if (argc != 4) {
        std::cerr << "Your command line needs 3 parameters: 1 input file, 2 output files" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[1]);
    std::ofstream output_file(argv[2]);
    std::ofstream log_file(argv[3]);

    if (!input_file.is_open() || !output_file.is_open() || !log_file.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return 1;
    }
    
    int rows, cols, min_val, max_val;
    input_file >> rows >> cols >> min_val >> max_val;

    QuadTree tree(rows, cols, min_val, max_val);
    output_file << "*** In main(), power2Size = " << tree.power2Size << std::endl;
    tree.zeroMatrix();
    tree.loadImage(input_file);
    output_file << "*** Below is the re-sized input image:" << std::endl; 
    tree.matrixToFile(tree.image, output_file);

    tree.buildQuadTree(0, 0, log_file);

    output_file << "*** Below is the pre-order traversal of Quad Tree:" << std::endl;
    tree.preOrder(*tree.QtRoot, output_file);

    output_file << "*** Below is the post-order traversal of Quad Tree:" << std::endl;
    tree.postOrder(*tree.QtRoot, output_file);

    log_file << "*** In main(): calling getLeaf() to recreate the image from Quad Tree" << std::endl;
    tree.getLeaf(*tree.QtRoot, log_file);
    output_file << "*** Below is the re-created image from Quad Tree" << std::endl;
    tree.matrixToFile(tree.newImage, output_file);
    output_file << "numRows, numCols, minVal, maxVal" << std::endl;

    input_file.close();
    output_file.close();
    log_file.close();
    return 0;
}