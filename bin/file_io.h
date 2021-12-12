//
// Created by Leonard C on 2021/12/12.
//

#ifndef BOOKSTORE_FILE_IO_H
#define BOOKSTORE_FILE_IO_H

#include <fstream>
#include <vector>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::vector;

template<class T, int info_len = 0>//
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    int num_T = 0;
    vector<int> next_index;

public:
    MemoryRiver() = default;

    MemoryRiver(const string &file_name) : file_name(file_name) {}

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //在文件合适位置写入 类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    //todo: It's really important！
    int write(T &t) {
        int index;
        if (next_index.empty()){
            index = info_len * sizeof(int) + num_T * sizeofT;
        }
        else {
            index = next_index.back();
            next_index.pop_back();
        }
        ++num_T;

        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char *> (&t),sizeofT);
        file.close();
        return index;
    }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) {
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast <char *> (&t), sizeofT);
        file.close();
        return;
    }

    //读出位置索引index对应的T对象的值并fi赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) {
        file.open(file_name);
        file.seekg(index);
        file.read(reinterpret_cast <char *> (&t), sizeofT);
        file.close();
        return;
    }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index) {
        next_index.push_back(index);
    }

};

#endif //BOOKSTORE_FILE_IO_H
