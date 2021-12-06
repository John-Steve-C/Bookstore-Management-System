//
// Created by Leonard C on 2021/12/4.
//

#ifndef BOOKSTORE_BOOKDATABASE_H
#define BOOKSTORE_BOOKDATABASE_H

#include <set>
#include <unordered_map>
using std::string;
class Book_database{
private:
    class Book{
    private:
        string ISBN,book_name,author,keyword[maxn];
        int quantity,book_NO;
        double price,total_cost;
    public:
        Book(){}
    }
    //todo:维护有序性
    set<string> Book_list;
    unordered_map<string,Book> MAP;

public:
    void show(string _ISBN = "",string _name = "",string _author = "",string _keyword[]){}
    void buy(string _ISBN,const int& _quantity){}
    void select(string _ISBN){}
    void modify(string _ISBN = "",string _name = "",string -author = "",string _keyword = "",double _price = 0){}
    void import(int _quantity,double _total_cost){}
}

#endif //BOOKSTORE_BOOKDATABASE_H
