//
// Created by Leonard C on 2021/12/4.
//

#ifndef BOOKSTORE_ACCOUNT_H
#define BOOKSTORE_ACCOUNT_H

#include "ull.h"
#include "command.h"
#include "log.h"
#include "Exception.h"

#include <fstream>
#include <cstdio>

struct UserID {
    char value[31];

    UserID(std::string userID);

    UserID() = default;

    std::string get_UserID() const;

    bool operator==(const UserID &rhs) const;

    bool operator<(const UserID &rhs) const;
};

class User {
    friend class AccountManagement;
private:
    char user_name[31];
    int priority;
    char password[31];

public:
    UserID ID;

    User() = default;

    User(const std::string &data);

    User(const std::string &_ID, const std::string &_name,
         const std::string &_password, int _priority = 0);

    void change_password(const std::string &newPassword);

    [[nodiscard]] int get_priority() const;
    //nodiscard说明该函数的返回值必须被调用

};

struct LogInAccount {
    User user;
    int selected_book_id = 0;
};

class AccountManagement {
    friend class BookManagement;
private:
    int num;
    std::vector<LogInAccount> login_stack; // 用于储存登录的账户及其选定的书本 id，不可使用 ISBN 作为指定对象，因为 ISBN 可能会被之后其他用户改变
    //登录栈

    MemoryRiver<User> user_data; // 用于储存所有数据的文件
    //预留首位用来存储块数

    Ull id_to_pos; // 第一个 int 忽略即可，填入时用 0 就行
    //相当于一个映射关系,把userID映射到对应的文件位置
    //就是索引文件

public:
    AccountManagement(); // 注意检查是否有用户名为 root，密码为 sjtu，权限为 {7} 的超级管理员账户，如没有，则添加该用户

    AccountManagement(const string &file_name); // 注意检查是否有用户名为 root，密码为 sjtu，权限为 {7} 的超级管理员账户，如没有，则添加该用户

    void switch_User(Command &line); // su command

    void LogOut(Command &line); // logout command

    void register_User(Command &line); // register command
    //权限为0,只能加顾客

    void change_password(Command &line); // passwd command

    void add_User(Command &line, LogManagement &logs); // useradd command
    //权限为3,可以加员工

    void remove_User(Command &line, LogManagement &logs); // delete command

    void User_select(int book_id); // 对于当前用户选中书本

    [[nodiscard]] int get_current_Priority() const;
};

#endif //BOOKSTORE_ACCOUNT_H
