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
    //val need to be changed
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
    UllBlock tp_block;
    int x = lower_bound(head , head + siz , node) - head;
    block_list.read(tp_block, offset[x]);
//    if (!tp_block.del(x)) return;
    //先判断是否重复
    vector<int> ans;
    tp_block.search(node.str,ans);
    if (ans.empty()){
        bool f = tp_block.add(node);
        if (f) tp_block.split(x);//大小超出限制,需要split

        block_list.write(tp_block);
    }

    return;
}

int Ull::delete_node(const UllNode &node) {
    UllBlock tp_block;
    int x = lower_bound(head , head + siz , node) - head;
    block_list.read(tp_block, offset[x]);

    vector<int> ans;
    tp_block.search(node.str,ans);
    if (!ans.empty()){
        tp_block.del(node);
        if (tp_block.siz < xxx) tp_block.merge();

        block_list.update();
    }
    return;
}

void Ull::find_node(const std::string &key, std::vector<int> &temp_array) {
    UllNode tp_node;
    tp_node.str = key; tp_node.val = 0;
    int x = lower_bound(head , head + siz , tp_node) - head - 1;
    //找到key在第x块
    int pos = offset[x];

    UllBlock tp_block;
//    vector<int> ans;
    block_list.read( tp_block,pos);//如何只把块x读入到内存tp2中?
    //这样似乎会把后面的数据全读入进去?
    tp_block.search(key,temp_array);
}

void Ull::del_block(const int &pos) {//删除第pos块
    block_list.Delete(offset[pos]);
    for (int i = pos; i < siz; ++i) {//暴力向前移动
        offset[i] = offset[i + 1];
        head[i] = head[i + 1];
    }
}

