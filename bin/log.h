// Created by Leonard C on 2021/12/4.
//

#ifndef BOOKSTORE_LOG_H
#define BOOKSTORE_LOG_H

#include <vector>
#include <string>
#include <set>
#include "account.h"
#include "ull.h"
#include "command.h"

using std::unordered_map;
using std::set;
using std::vector;
using std::string;
using std::cout;
const int Maxn = 1e5;

enum Behavior {AddUser, Delete, Show, Buy, Select, Modify, Import};
struct Log {
    class User user;
    Behavior behavoir;
    char description[150];
    bool if_earn = false; // 表示是否是收入
    double Amount;
};

class LogManagement {
private:
//    fstream log_data_("log");
    int count_ = 0; // 交易笔数

public:
    LogManagement();

    void Report(class Command& line, class AccountManagement& accounts);

    void AddLog(Log& log); // 把 log 放进文件的同时还需要检查是否有交易

    void ShowFinance(int Limit = -1); // 若为 -1，则显示全部

    void Log(class Command& line); // log command，检查有无额外的 token
};

#endif //BOOKSTORE_LOG_H
