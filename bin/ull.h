#pragma region HEADER

// Ull class implements an unrolled linked list.
// It's easy to write and use a ULL.
//
// Some parameters are defined in the form of
// macro in the following, but modifying these
// parameters is INVALID. In other words, you
// SHOULD NOT modify any content of this header
// file unless you know what you are doing.

#define ULL_EXPORT
#ifdef ULL_EXPORT
#define ULL_API __declspec(dllexport)
#else
#define ULL_API __declspec(dllimport)
#endif

#ifndef ULL_H
#define ULL_H

#include <cstdio>
#include <iostream>
#include <fstream>

#include <algorithm>

#include <vector>
#include <string>
#include <cstring>
#include "file_io.h"

#define BLOCK_SIZE 317
#define BLOCK_SPLIT_THRESHOLD 170
#define BLOCK_SPLIT_LEFT 85
#define BLOCK_MERGE_THRESHOLD 20

#pragma endregion

using namespace std;
class UllNode {
// Stores key-value data
// The data type of key is `int` and the data type
// of value is `char[64]`, but the constructor only
// accepts `std::string` as value.
public:
    int offset,val;//val是操作的'时间',是排序的第二关键字
    //offset是节点在文件中的位置
    char str[64];//实际上存储的信息，用来比对

    UllNode();
    UllNode(const int &arg1, const std::string &arg2);
    bool operator<(const UllNode &x) const;
    bool operator==(const UllNode &x) const;
    UllNode &operator=(const UllNode &right);
};

class UllBlock {
// For ULL class internal use only
public:
    int nxt,pre;//指针
    int num,siz;//num指链表中的第几块,siz是块的大小
    UllNode array[BLOCK_SIZE];

    UllBlock();
    UllBlock(const int _siz,const UllNode *_array);
    UllBlock &operator=(const UllBlock &right);

    bool add(const UllNode &x);//在块中新添加元素
    bool del(const UllNode &x);//删除块中的元素
    UllBlock split(const int &x);//把当前块拆分为长度为 siz-x 和 x 的两块
    //返回的是长度为x的新块
    UllBlock & merge(const UllBlock &x);//将当前块与x合并
    //指针尚未进行更改
    void search(const std::string &key, std::vector<int> &temp_array);
    //在当前块中查找键值位key的Node，返回一个vector
};

class Ull {//专门用来处理与文件交互的操作
// 'Unrolled Linked List'
// A data structure used for file data storage.
// The advantage is that it is easy to write.
// But it is inferior to 'B+ Tree' in performance.
private:
    const std::string file_name;
    int siz;//块的个数
    int offset[BLOCK_SIZE];//每个块在文件中的位置
    UllNode head[BLOCK_SIZE];//每个块的头节点？
    MemoryRiver<UllBlock> block_list;

    inline void del_block(const int &pos);//删除第pos块

public:
    //文件读入,arg就是filename
    Ull(const std::string &arg);

    ~Ull();

    void find_node(const std::string &key, std::vector<int> &temp_array);
    // Returns an empty array if the node doesn't exist
    // temp_array is used to store the position of existing nodes

    void add_node(const UllNode &node);

    int delete_node(const UllNode &node);

    int find_position(const UllNode &node) const;//找到x所在的块，返回块的编号

#ifdef PPL_DEBUG
    void debugPrint();
#endif

};


#endif //ULL_H
