//
// Created by Leonard C on 2021/12/4.
//

#include "ull.h"
#include <cstring>
#include <sstream>

//
//---------UllNode---------
//

UllNode *head,*rear;
bool UllNode::operator<(const UllNode &x) const {
    if ( strcmp(str,x.str) < 0 ) return true;
    else return false;
}
UllNode::UllNode() {
    offset = 0;
}
UllNode::UllNode(const int &arg1, const std::string &arg2) {
    offset = arg1;
    stringstream ss;
    ss << arg2;
    ss >> str;
}
UllNode &UllNode::operator=(const UllNode &right) {
    strcpy(str, right.str);
    offset = right.str;

}

//
//---------UllBlock---------
//

UllBlock::UllBlock() {

}
UllBlock &UllBlock::operator=(const UllBlock &right) {
    next = right.next;
    pre = right.pre;
    num = right.num;
    for (int i = 0;i < BLOCK_SIZE;++i)
        array[i] = right.array[i];
}

//
//---------Ull---------
//

Ull::Ull(const std::string &arg) {

}
Ull::~Ull() {}

void Ull::addNode(const UllNode &node) {
    UllNode *temp = new UllNode;
    temp = node;

}

int Ull::deleteNode(const UllNode &node) {}

void Ull::findNode(const std::string &key, std::vector<int> &array) {}

void Ull::delBlock(const int &offset) {
    UllBlock *tp1,*tp2;

}

void Ull::mergeBlock(const int &offset1, const int &offset2) {}

int Ull::nextBlock(const int &offset) {}

UllBlock *Ull::splitBlock(const int &offset) {
    UllNode *temp;
    UllBlock *nulBlock;
    if (offset < BLOCK_SIZE) {
        temp = new UllNode;
        temp->offset = offset;

    }

    return nulBlock;
}

void Ull::maintain() {

}
