//
// Created by Leonard C on 2021/12/4.
//

#ifndef BOOKSTORE_BOOKDATABASE_H
#define BOOKSTORE_BOOKDATABASE_H

#include <set>
#include <unordered_map>
#include <vector>

#include "command.h"
#include "ull.h"
#include "log.h"

using std::string;

struct ISBN {
    char[21] value;

    ISBN(const std::string& s);

    bool operator==(const ISBN& rhs) const;

    bool operator<(const ISBN& rhs) const;
};

struct BookName {
    char[61] value;

    BookName(const std::string& s);

    bool operator==(const BookName& rhs) const;

    bool operator<(const BookName& rhs) const;
};

struct Author {
    char[61] value;

    Author(const std::string& s);

    bool operator==(const Author& rhs) const;

    bool operator<(const Author& rhs) const;
};

struct Keyword {
    char [61] value;

    Keyword(const std::string& s);

    bool operator==(const Keyword& rhs) const;

    bool operator<(const Keyword& rhs) const;
};

class Book {
private:
    ISBN isbn_;
    BookName book_name_;
    Author author_;
    Keyword keyword_;
    int quantity_ = 0;
    double price_ = 0;
    double total_cost_ = 0;

public:
    int book_ID_;

    Book();

    Book(int id, const std::string& isbn, const std::string& bookName, const std::string& author, const std::string& keyword, int quantity, double price, double _total_cost); // 这样方便构造，但注意 keyword 需要以升序重新排列

    Book(int id, const std::string& isbn); // 将除 ISBN 以外的部分全部为空字符串或 0



    friend std::ostream& operator<<(std::ostream&, const Book& book); // 用于输出
};

class BookManagement {
private:
    fstream book_data_("book_data"); // 用于储存所有数据的文件

    UnrolledLinkedList<book_name_index, ISBN, int, int> isbn_map_; // 第一个 int 忽略即可，填入时用 0 就行

    UnrolledLinkedList<book_name_index, BookName, ISBN, int> book_name_index_;

    UnrolledLinkedList<author_map_index, Author, ISBN, int> author_map_index_;

    UnrolledLinkedList<keyword_index, Keyword, ISBN, int> keyword_index_;

    // Other private variables ...

public:
    BookManagement();

    // 下面的指令请调用 accounts::getCurrentPriority() 来获取权限

    void Show(TokenScanner& line, AccountManagement& accounts, LogManagement& logs); // 先判断是不是 show finance（都是以 show 开头），然后分四种情况讨论，如无参数，则按照 ISBN 输出全部（traverse 函数）

    void Buy(TokenScanner& line, AccountManagement& accounts, LogManagement& logs);

    void Select(TokenScanner& line, AccountManagement& accounts, LogManagement& logs); // 检查是否有权限，检查是否有 ISBN，然后选中

    void Modify(TokenScanner& line, AccountManagement& accounts, LogManagement& logs); // 检查是否有权限

    void ImportBook(TokenScanner& line, AccountManagement& accounts, LogManagement& logs); // 检查是否有权限
};

#endif //BOOKSTORE_BOOKDATABASE_H
