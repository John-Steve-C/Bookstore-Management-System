//
// Created by Leonard C on 2021/12/4.
//

#ifndef BOOKSTORE_BOOKDATABASE_H
#define BOOKSTORE_BOOKDATABASE_H

#include <set>
#include <unordered_map>
#include <vector>

#include "command.h"
#include "account.h"
#include "ull.h"
#include "log.h"
#include "Exception.h"

using std::string;

struct ISBN {
    char value[61];

    ISBN() = default;

    ISBN(const std::string &s);

    bool operator==(const ISBN &rhs) const;

    bool operator<(const ISBN &rhs) const;
};

struct BookName {
    char value[61];

    BookName() = default;

    BookName(const std::string &s);

    bool operator==(const BookName &rhs) const;

    bool operator<(const BookName &rhs) const;
};

struct Author {
    char value[61];

    Author() = default;

    Author(const std::string &s);

    bool operator==(const Author &rhs) const;

    bool operator<(const Author &rhs) const;
};

struct Keyword {
    char value[61];

    Keyword() = default;

    Keyword(const std::string &s);

    bool operator==(const Keyword &rhs) const;

    bool operator<(const Keyword &rhs) const;
};

class Book {
    friend class BookManagement;

private:
    ISBN isbn;
    BookName book_name;
    Author author;
    Keyword keyword;
    //todo:keyword 可以有多个!!
//    vector<Keyword> keyword;
    int quantity = 0;
    double price = 0;
    double total_cost = 0;

public:
    int book_ID;

    Book() = default;

    // 这样方便构造，但注意 keyword 需要以升序重新排列
    Book(int _id, const std::string &_isbn, const std::string &_bookName,
         const std::string &_author, const std::string &_keyword,
         int _quantity, double _price, double _total_cost);

    Book(int id, const std::string &isbn); // 将除 ISBN 以外的部分全部为空字符串或 0

    friend std::ostream &operator<<(std::ostream &output, const Book &book); // 用于输出
};

class BookManagement {
private:
    int num = 0; //当前的书本总数,存放在book_data的第一个位置
    MemoryRiver<Book, 1> book_data; // 用于储存所有数据的文件

    //按照不同关键子进行排序的块状链表
    //其实就是索引文件
    Ull id_to_pos;
    Ull isbn_to_pos;
    Ull name_to_pos;
    Ull author_to_pos;
    Ull keyword_to_pos;
    //如何修改?

public:
    BookManagement() = default;
    // 下面的指令请调用 accounts::getCurrentPriority() 来获取权限
    void Show(Command &line, AccountManagement &accounts,
              LogManagement &logs);
    // 先判断是不是 show finance（都是以 show 开头），然后分四种情况讨论，如无参数，则按照 ISBN 输出全部（traverse 函数）
    // 我把这个判断的过程放在main函数中实现,事实上line中的cur在第二个关键词

    void Buy(Command &line, AccountManagement &accounts, LogManagement &logs);

    void Select(Command &line, AccountManagement &accounts, LogManagement &logs); // 检查是否有权限，检查是否有 ISBN，然后选中

    void Modify(Command &line, AccountManagement &accounts, LogManagement &logs); // 检查是否有权限

    void ImportBook(Command &line, AccountManagement &accounts, LogManagement &logs); // 检查是否有权限
};

#endif //BOOKSTORE_BOOKDATABASE_H
