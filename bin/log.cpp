#include "log.h"

//class LogManagement

LogManagement::LogManagement() {
    log_data.initialise("log_data");
}
void LogManagement::ShowFinance(int limit) {
    //进入前应当进行权限判断
    //time = 0
    if (limit == 0) {
        printf("\n");
        return;
    }

    int num;
    log_data.get_info(num, 1);

    //超限,失败
    if (limit > num) {
        throw Exception("Invalid\n");
    }
    //没有参数,全部输出
    if (limit == -1) {limit = num;}

    double inc = 0, dec = 0;
    for (int i = num - limit + 1; i <= num; ++i){
        class Log temp;
        log_data.read(temp, 4 + i * sizeof(Log));

        if (temp.if_earn) inc += temp.Amount;
        else dec += temp.Amount;
    }

    cout << "+ " << inc << " - " << dec << "\n";
}

void LogManagement::Report(Command &line, AccountManagement &accounts) {}

void LogManagement::Log_ch(Command &line) {}

void LogManagement::AddLog(Log &log) {}