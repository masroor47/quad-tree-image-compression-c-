#include <iostream>
#include <fstream>
#include "QtNode.h"
#include <QuadTree.h>


QuadTree::QuadTree(int rows, int cols, int min, int max): numRows(rows), numCols(cols), minVal(min), maxVal(max) {
    this->power2Size = computePower2(rows, cols);
    image = std::make_unique<int[]>(power2Size * power2Size);
    newImage = std::make_unique<int[]>(power2Size * power2Size);
    zeroMatrix();
}

int QuadTree::computePower2(int rows, int cols) {
    int size = rows > cols ? rows : cols;
    int power2Size = 1;
    while (power2Size < size) {
        power2Size *= 2;
    }
    return power2Size;
}

void QuadTree::zeroMatrix() {
    for (int i = 0; i < power2Size * power2Size; i++) {
        image[i] = 0;
        newImage[i] = 0;
    }
}

void QuadTree::loadImage(std::ifstream& input_file) {
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            input_file >> image[r * numCols + c];
        }
    }
}

void QuadTree::matrixToFile(const std::unique_ptr<int[]>& matrix, std::ofstream& output_file) {
    for (int r = 0; r < power2Size; r++) {
        for (int c = 0; c < power2Size; c++) {
            output_file << matrix[r * power2Size + c] << " ";
        }
        output_file << std::endl;
    }
}

void QuadTree::buildQuadTree(int upR, int upC, std::ofstream& log_file) {
    QtRoot = recursiveBuild(upR, upC, power2Size, log_file);
}

std::unique_ptr<QtNode> QuadTree::recursiveBuild(int upR, int upC, int size, std::ofstream& log_file) {
    log_file << "*** Entering buildQuadTree: " << "upR=" << upR << ", upC=" << upC << ", size=" << size << std::endl;
    auto newNode = std::make_unique<QtNode>(-1, upR, upC, size);

    if (size == 1) {
        newNode->color = image[upR * power2Size + upC];
    } else {
        int halfSize = size / 2;
        newNode->NW = recursiveBuild(upR, upC, halfSize, log_file);
        newNode->NE = recursiveBuild(upR, upC+halfSize, halfSize, log_file);
        newNode->SW = recursiveBuild(upR+halfSize, upC, halfSize, log_file);
        newNode->SE = recursiveBuild(upR+halfSize, upC+halfSize, halfSize, log_file);

        int sumColor = addKidsColor(*newNode);
        log_file << "*** sumColor 4 children's color is: " << sumColor << std::endl;
        if (sumColor == 0) {
            newNode->color = 0;
        } else if (sumColor == 4) {
            newNode->color = 1;
        } else {
            newNode->color = 5;
        }
        log_file << "*** Printing QtNode below" << std::endl;
        newNode->printQtNode(log_file);
        log_file << "*** Leaving buildQuadTree" << std::endl;
    }
    return newNode;
}

int QuadTree::addKidsColor(const QtNode& node) {
    int nwColor = node.NW->color;
    int neColor = node.NE->color;
    int swColor = node.SW->color;
    int seColor = node.SE->color;
    return nwColor + neColor + swColor + seColor;
}

void QuadTree::preOrder(const QtNode& node, std::ofstream& output_file) {
    if (node.isLeaf()) {
        node.printQtNode(output_file);
    } else {
        node.printQtNode(output_file);
        preOrder(*(node.NW), output_file);
        preOrder(*(node.NE), output_file);
        preOrder(*(node.SW), output_file);
        preOrder(*(node.SE), output_file);
    }
}

void QuadTree::postOrder(const QtNode& node, std::ofstream& output_file) {
    if (node.isLeaf()) {
        node.printQtNode(output_file);
    } else {
        postOrder(*(node.NW), output_file);
        postOrder(*(node.NE), output_file);
        postOrder(*(node.SW), output_file);
        postOrder(*(node.SE), output_file);
        node.printQtNode(output_file);
    }
}

void QuadTree::getLeaf(const QtNode& node, std::ofstream& log_file) const {
    log_file << "*** Entering getLeaf method" << std::endl;
    if (node.isLeaf()) {
        fillNewImage(node, log_file);
    } else {
        getLeaf(*node.NW, log_file);
        getLeaf(*node.NE, log_file);
        getLeaf(*node.SW, log_file);
        getLeaf(*node.SE, log_file);
    }
    log_file << "*** Leaving getLeaf method" << std::endl;
}

void QuadTree::fillNewImage(const QtNode& node, std::ofstream& log_file) const {
    log_file << "*** Entering fillNewImage method" << std::endl;
    int color, R, C, sz;
    color = node.color;
    R = node.upperR;
    C = node.upperC;
    sz = node.size;
    for (int i = R; i < R+sz; i++) {
        for (int j = C; j < C+sz; j++) {
            newImage[i*power2Size + j] = color;
        }
    }
    log_file << "*** Leaving fillNewImage method" << std::endl;
}
