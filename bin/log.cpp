#include "log.h"
#include "account.h"

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

    log_data.get_info(num, 1);
    //超限,失败
    if (limit > num || limit > 2147483647 || num > 2147483647) {
        throw Exception("Invalid\n");
    }
    //没有参数,全部输出
    if (limit == -1) {limit = num;}

    double inc = 0, dec = 0;
    for (int i = num - limit + 1; i <= num; ++i){
        class Log temp;
        log_data.read(temp, 8 + (i - 1) * sizeof(Log));

        if (temp.if_earn) inc += temp.Amount;
        else dec += temp.Amount;
    }

    cout << "+ " << fixed << setprecision(2) << inc
         << " - " << fixed << setprecision(2) << dec << "\n";
}

void LogManagement::Report_employee(Command &line, AccountManagement &accounts) {
    if (accounts.get_current_Priority() < 7 || line.cnt != 2) {
        throw Exception("Invalid\n");
    }
}

void LogManagement::Report_myself(Command &line, AccountManagement &accounts) {
    if (accounts.get_current_Priority() < 3 || line.cnt != 2) {
        throw Exception("Invalid\n");
    }
}

void LogManagement::Log_ch(Command &line) {}

void LogManagement::AddLog(Log &log) {
    log_data.write(log);
}