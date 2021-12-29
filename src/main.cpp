#include <iostream>
#include <string>

#include "ull.h"
#include "command.h"
#include "account.h"
#include "bookdatabase.h"
#include "log.h"
#include "Exception.h"

int main() {
    int line = 0;

//    freopen("test.in","r",stdin); freopen("ans.txt","w",stdout);

    string command_input;
    AccountManagement accounts;
    BookManagement books;
    LogManagement logs;

    while (getline(cin, command_input)) {
        line++;
//        cout << line << endl;
        try {
            Command cmd(command_input);
            //todo:长度上限
            if (command_input.length() > 1024) {
                throw Exception("Invalid\n");
            }
            //todo:是否有不在ACSII范围中的的字符
            for (int i = 0;i < command_input.length(); ++i) {
                if (command_input[i] < 0 || command_input[i] > 127) {
                    throw Exception("Invalid\n");
                }
            }
            //todo:判断空行！
            if (cmd.cnt == 0) { continue; }

            string order = cmd.next_token();
            if (order == "exit" || order == "quit") {
                return 0;
            }
            //todo:account_system
            if (order == "su") accounts.switch_User(cmd);
            else if (order == "logout") accounts.LogOut(cmd);
            else if (order == "register") accounts.register_User(cmd);
            else if (order == "passwd") accounts.change_password(cmd);
            else if (order == "useradd") accounts.add_User(cmd, logs);
            else if (order == "delete") accounts.remove_User(cmd, logs);

            //todo:book_system
            else if (order == "show") {
                Command temp_cmd(cmd);
                if (temp_cmd.next_token() == "finance") {
                    //说明是日志
                    //权限判断()
                    if (accounts.get_current_Priority() < 7)
                        throw Exception("Invalid\n");
                    string limit = temp_cmd.next_token();
                    if (limit.empty()) logs.ShowFinance();
                    else logs.ShowFinance(stoi(limit));
                } else {//说明是书本
                    books.Show(cmd, accounts, logs);
                }
            }
            else if (order == "buy") books.Buy(cmd, accounts, logs);
            else if (order == "select") books.Select(cmd, accounts, logs);
            else if (order == "modify") books.Modify(cmd, accounts, logs);
            else if (order == "import") books.ImportBook(cmd, accounts, logs);

            //todo:log_system
            else if (order == "report") {
                order = cmd.next_token();
                if (order == "myself") {}
                else if (order == "finance") {
                    logs.ShowFinance();
                }
                else if (order == "employee") {}
            }
            else if (order == "log") {}

            //todo:错误的情况
            else throw Exception("Invalid\n");
        }
        catch (Exception &s) {
//            cout << line << ' ' << s.what();
            cout << s.what();
        }
        catch (std::invalid_argument& e) {//stod和stoi的异常
            cout << "Invalid\n";
        }
        catch (std::out_of_range& e) {
            cout << "Invalid\n";
        }
    }

    return 0;
}
