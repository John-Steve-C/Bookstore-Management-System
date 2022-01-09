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

    log_data.get_info(num, 1);
    for (int i = 1;i <= num;++i){
        class Log temp;
        log_data.read(temp, 8 + (i - 1) * sizeof(Log));
        vector<int> ans;
        User tp_user;
        accounts.id_to_pos.find_node(temp.user_ID, ans);
        accounts.user_data.read(tp_user, ans[0]);
        if (tp_user.get_priority() >= 3) {//只输出员工的操作
            cout << temp.user_ID << ' ';
            Print(temp.behavoir);
            cout << endl;
        }
    }
}

void LogManagement::Report_myself(Command &line, AccountManagement &accounts) {
    if (accounts.get_current_Priority() < 3 || line.cnt != 2) {
        throw Exception("Invalid\n");
    }

    log_data.get_info(num, 1);
    for (int i = 1;i <= num;++i){
        class Log temp;
        log_data.read(temp, 8 + (i - 1) * sizeof(Log));
        if (strcmp(temp.user_ID, accounts.login_stack.back().user.ID.value) == 0){
            cout << temp.user_ID << ' ';
            Print(temp.behavoir);
            cout << endl;
        }
    }
}

void LogManagement::Log_All(Command &line, AccountManagement& accounts) {
    if (accounts.get_current_Priority() < 7 || line.cnt != 1) {
        throw Exception("Invalid\n");
    }

    log_data.get_info(num, 1);
    for (int i = 1;i <= num;++i){
        class Log temp;
        log_data.read(temp, 8 + (i - 1) * sizeof(Log));
        cout << temp.user_ID << ' ';
        Print(temp.behavoir);
        cout << endl;
    }
}

void LogManagement::AddLog(Log &log) {
    log_data.write(log);
}

void LogManagement::Print(const Behavior &x) {
    if (x == BUY) cout << "Buy";
    else if (x == LOGIN) cout << "Login";
    else if (x == ADDUSER) cout << "Add_user";
    else if (x == DELETE) cout << "Delete";
    else if (x == IMPORT) cout << "Import";
    else if (x == SELECT) cout << "Select";
    else if (x == MODIFY) cout << "Modify";
    else if (x == SHOW) cout << "Show";
}