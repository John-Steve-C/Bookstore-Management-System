//
// Created by Leonard C on 2021/12/12.
//

#ifndef BOOKSTORE_COMMAND_H
#define BOOKSTORE_COMMAND_H

#include "account.h"
#include "log.h"
#include "bookdatabase.h"

class Command {
private:
    string cmd;
    int len;
public:
    void get_token(){ }
};

#endif //BOOKSTORE_COMMAND_H
