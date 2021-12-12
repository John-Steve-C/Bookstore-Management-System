//
// Created by Leonard C on 2021/12/4.
//

#ifndef BOOKSTORE_LOG_H
#define BOOKSTORE_LOG_H

#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include "ull.h"

using std::unordered_map;
using std::set;
using std::vector;
using std::string;
using std::cout;
const int Maxn = 1e5;
class Log{
private:
    int Time;
    string RecordOp[Maxn];
    vector<double> Finance;
    unordered_map<string, vector<string>> EmployeeOp;
    unordered_map<string, vector<string>> CustomerOp;
    vector<string> OwnerOp;
public:

    Log() : Time(0){}
    void report_op(const string& name){}//todo:员工操作记录指令
    void show_finance(const int& t){}//todo:财务记录查询
    void report_finance(){}//todo:财务记录报告
    void report_employee(){}//todo:遍历map，输出全体员工的所有操作记录
    void report_customer(){}//todo:顾客
    void report_owner(){}//todo：店主
    void log(){
        report_finance();
        report_employee();
        report_customer();
        report_owner();
    }
};

#endif //BOOKSTORE_LOG_H
