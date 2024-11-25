#ifndef QTNODE_H
#define QTNODE_H

#include <memory>
#include <fstream>

class QtNode {
    public:
        int color;
        int upperR;
        int upperC;
        int size;
        QtNode();
        QtNode(int, int, int, int);
        void printQtNode(std::ofstream&) const;
        bool isLeaf() const;

        std::unique_ptr<QtNode> NW;
        std::unique_ptr<QtNode> NE;
        std::unique_ptr<QtNode> SW;
        std::unique_ptr<QtNode> SE;
};

#endif