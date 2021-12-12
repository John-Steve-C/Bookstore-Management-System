//
// Created by Leonard C on 2021/12/4.
//

#include "ull.h"
#include <cstring>
#include <sstream>

//
//---------UllNode---------
//

UllNode::UllNode() = default;

UllNode::UllNode(const int &arg1, const std::string &arg2) {
    offset = arg1;
    stringstream ss;
    ss << arg2;
    ss >> str;
}

bool UllNode::operator<(const UllNode &x) const {
    if (strcmp(str, x.str) == 0) return val < x.val;
    else return strcmp(str, x.str) < 0;
}

bool UllNode::operator==(const UllNode &x) const {
    return (strcmp(str, x.str) == 0) && (val == x.val);
}

UllNode &UllNode::operator=(const UllNode &right) {
    strcpy(str, right.str);
    offset = right.offset;
    val = right.val;
    return *this;
}

//
//---------UllBlock---------
//

UllBlock::UllBlock() = default;

UllBlock::UllBlock(const int _siz, const UllNode *_array) {
    siz = _siz;
    memcpy(array, _array, _siz * sizeof(UllNode));
}

UllBlock &UllBlock::operator=(const UllBlock &right) {
    nxt = right.nxt;
    pre = right.pre;
    num = right.num;
    siz = right.siz;
    memcpy(array, right.array, siz * sizeof(UllNode));
}

bool UllBlock::add(const UllNode &x) {
    int pos = std::lower_bound(array, array + siz, x) - array;
    //二分查找第一个不小于x的元素，返回的是指针(位置)
    if (array[pos] == x) return 1;//已经存在,不合法
    siz++;
    for (int i = siz - 1; i > pos; --i)
        array[i] = array[i - 1];//后方的元素向后移一位
    array[pos] = x;
    return siz < (BLOCK_SIZE << 1);//判断大小是否超出限制，需要split
}

bool UllBlock::del(const UllNode &x) {
    int pos = std::lower_bound(array, array + siz, x) - array;
    if (array[pos] != x) return 0;//不存在
    siz--;
    for (int i = pos; i < siz; ++i)
        array[i] = array[i + 1];//向前移动一位
    array[siz] = UllNode();
    return 1;
}

UllBlock UllBlock::split(const int &x) {
    siz -= x;//相当于删除
    return UllBlock(x, array + x);//从第x位开始的'新数组'
}

UllBlock &UllBlock::merge(const UllBlock &x) {
    memcpy(array + siz, x.array, x.siz * sizeof(UllNode));
    siz += x.siz;
    return *this;
}

void UllBlock::search(const std::string &key, std::vector<int> &temp_array) {
    for (int i = 0; i < siz; ++i) {
        if (array[i].str == key)
            temp_array.push_back(i);
    }
}

//
//---------Ull---------
//

Ull::Ull(const std::string &arg) {
    block_list(arg);
}

Ull::~Ull() {}

//todo: 完善以下操作，并保证其正确性
void Ull::add_node(const UllNode &node) {
    UllBlock temp;
    int _pos = find_position(node);
    block_list.read(temp, offset[_pos]);
    if (!temp.del(x)) return;

}

int Ull::delete_node(const UllNode &node) {
    vector<int> p;
    findNode(node.str, p);
    for (int i = 0; i < p.size(); ++i) {
        int pos = p[i];
        block_list.Delete(pos);
    }

}

void Ull::find_node(const std::string &key, std::vector<int> &temp_array) { }

void Ull::del_block(const int &pos) {
    block_list.Delete(offset[pos]);
    for (int i = pos; i < siz; ++i) {//暴力向前移动
        offset[i] = offset[i + 1];
        head[i] = head[i + 1];
    }
}

int Ull::find_position(const UllNode &node) const {
    int _pos = lower_bound(head, head + siz, x) - head;
    return _pos;
}