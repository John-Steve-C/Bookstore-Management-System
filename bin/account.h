//
// Created by Leonard C on 2021/12/4.
//

#ifndef BOOKSTORE_ACCOUNT_H
#define BOOKSTORE_ACCOUNT_H

#include <unordered_map>
#include <fstream>
#include "ull.h"

class Account_system {
    struct user_account{
        string user_id;
        string user_name;
        string password;
        short int priority;
        bool login;
        int cnt;
    };
    string userID;
public:
    void su(command a,string _user_id,string password_try) ;
    void log_out(command a);
    void register(command a,string _user_id,string _password_,string _user_name);
    void passwd(command a,string _user_id,string _old_password,string _new_password);
    void user_add(command a,string _user_id,string _password,short int _priority,string _user_name);
    void delete(command a,string _user_id);
private:
    unordered_map(std::string,user_account);
};

#endif //BOOKSTORE_ACCOUNT_H
