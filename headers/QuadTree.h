#ifndef QUADTREE_H
#define QUADTREE_H

#include <memory>
#include <fstream>
#include "QtNode.h"

class QuadTree {
    public:
        std::unique_ptr<QtNode> QtRoot;
        int numRows;
        int numCols;
        int minVal;
        int maxVal;
        int power2Size;
        std::unique_ptr<int[]> image;
        std::unique_ptr<int[]> newImage;

        QuadTree(int, int, int, int);
        int computePower2(int, int);
        void loadImage(std::ifstream&);
        void zeroMatrix();
        
        void buildQuadTree(int, int, std::ofstream&);
        std::unique_ptr<QtNode> recursiveBuild(int, int, int, std::ofstream&);
        int addKidsColor(const QtNode&);

        void matrixToFile(const std::unique_ptr<int[]>& matrix, std::ofstream&);

        void preOrder(const QtNode&, std::ofstream&);
        void postOrder(const QtNode&, std::ofstream&);

        void fillNewImage(const QtNode&, std::ofstream&) const;

        void getLeaf(const QtNode&, std::ofstream&) const;

};

#endif