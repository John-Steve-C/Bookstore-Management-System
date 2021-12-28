//
// Created by Leonard C on 2021/12/25.
//

#ifndef BOOKSTORE_EXCEPTION_H
#define BOOKSTORE_EXCEPTION_H

#include <string>
#include <exception>

class Exception : public exception{
private:
    string err = "";

public:
    Exception() = default;
    explicit Exception(const string &s){
        err = s;
    }
    string what(){
        return err;
    }
};

#endif //BOOKSTORE_EXCEPTION_H
