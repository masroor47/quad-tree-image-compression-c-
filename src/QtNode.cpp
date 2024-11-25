
// #include <iostream>
#include <fstream>
#include "QtNode.h"


QtNode::QtNode(): color(0), upperR(0), upperC(0), size(0) {
    NW = nullptr;
    NE = nullptr;
    SW = nullptr;
    SE = nullptr;
}

QtNode::QtNode(int c, int upperR, int upperC, int size): color(c), upperR(upperR), upperC(upperC), size(size) {
    NW = nullptr;
    NE = nullptr;
    SW = nullptr;
    SE = nullptr;
}

bool QtNode::isLeaf() const {
    if (color == 0 || color == 1) {
        return true;
    }
    return false;
}

void QtNode::printQtNode(std::ofstream& output_file) const {
    output_file << "<" << color << ", ";
    if (NW == nullptr || NE == nullptr || SW == nullptr || SE == nullptr) {
        output_file << ", null, null, null, null>" << std::endl;
        return;
    } 
    output_file << NW->color << ", " 
                << NE->color << ", " 
                << SW->color << ", "
                << SE->color << ">" 
                << std::endl;    
}