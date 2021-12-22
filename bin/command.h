//
// Created by Leonard C on 2021/12/12.
//

#ifndef BOOKSTORE_COMMAND_H
#define BOOKSTORE_COMMAND_H

#include "account.h"
#include "log.h"
#include "bookdatabase.h"

#include <string>
#include <ostream>

using std::string;
using std::fstream;

class Command {
private:
    string buffer = "";//存储字符串的缓冲区
    int cur = 0;//当前指针的位置
    char delimiter = ' ';//分隔符

public:
    Command() = default;

    Command(const Command &rhs);

    Command(char _delimiter);

    Command(const std::string &in, char _delimiter = ' ');

    ~Command() = default;

    string next_token();//取出下一个Token,更新cur

    void clear();

    friend istream &operator>>(istream &input, Command &obj);

    friend ostream &operator<<(ostream &os, const Command &command);

    void set_delimiter(char new_delimiter);
};

#endif //BOOKSTORE_COMMAND_H
