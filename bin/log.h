// Created by Leonard C on 2021/12/4.
//

#ifndef BOOKSTORE_LOG_H
#define BOOKSTORE_LOG_H

#include <vector>
#include <string>
#include <iomanip>

#include "ull.h"
#include "file_io.h"
#include "command.h"
#include "Exception.h"

class AccountManagement;
class User;
//类或结构体的前向声明,只能用来定义指针对象或引用
using std::vector;
using std::string;
const int Maxn = 1e5;

enum Behavior {LOGIN, ADDUSER, DELETE, SHOW, BUY, SELECT, MODIFY, IMPORT};
class Log {
public:
    char *user_ID;
    //不用指针会报错?
    Behavior behavoir;
    char description[150];
    bool if_earn = false; // 表示是否是收入
    double Amount = 0;

    Log() = default;
};

class LogManagement {
private:
    MemoryRiver<Log> log_data;
    int num = 0; // 交易笔数

public:
    LogManagement();

    void report_employee(Command& line, AccountManagement& accounts);

    void report_myself(Command &line, AccountManagement& accounts);

    void add_log(Log& log); // 把 log 放进文件的同时还需要检查是否有交易

    void show_finance(int limit = -1); // 若为 -1，则显示全部

    void log_all(Command &line, AccountManagement& accounts); //

    void Print(const Behavior &x);
};

#endif //BOOKSTORE_LOG_H
