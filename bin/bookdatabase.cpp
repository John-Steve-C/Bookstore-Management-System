#include "bookdatabase.h"

//class ISBN

ISBN::ISBN(const std::string &s) {
    strcpy(value, s.c_str());
}

bool ISBN::operator==(const ISBN &rhs) const {
    return strcmp(value, rhs.value) == 0;
}

bool ISBN::operator<(const ISBN &rhs) const {
    return strcmp(value, rhs.value) < 0;
}

//class BookName

BookName::BookName(const std::string &s) {
    strcpy(value, s.c_str());
}

bool BookName::operator==(const BookName &rhs) const {
    return strcmp(value, rhs.value) == 0;
}

bool BookName::operator<(const BookName &rhs) const {
    return strcmp(value, rhs.value) < 0;
}

//class Author

Author::Author(const std::string &s) {
    strcpy(value, s.c_str());
}

bool Author::operator==(const Author &rhs) const {
    return strcmp(value, rhs.value) == 0;
}

bool Author::operator<(const Author &rhs) const {
    return strcmp(value, rhs.value) < 0;
}

//class Keyword

Keyword::Keyword(const std::string &s) {
    strcpy(value, s.c_str());
}

bool Keyword::operator==(const Keyword &rhs) const {
    return strcmp(value, rhs.value) == 0;
}

bool Keyword::operator<(const Keyword &rhs) const {
    return strcmp(value, rhs.value) < 0;
}

//class Book

Book::Book(int _id, const std::string &_isbn, const std::string &_bookName, const std::string &_author,
           const std::string &_keyword, int _quantity, double _price, double _total_cost) {

    book_ID = _id;
    strcpy(isbn.value, _isbn.c_str());
    strcpy(book_name.value, _bookName.c_str());
    strcpy(author.value, _author.c_str());
    strcpy(keyword.value, _keyword.c_str());
    quantity = _quantity;
    price = _price;
    total_cost = _total_cost;
}

Book::Book(int _id, const std::string &_isbn) {
    book_ID = _id;
    isbn = _isbn;
}

ostream &operator<<(ostream &output, const Book &obj) {
    output << obj.isbn.value;
    return output;
}

//class BookManagement

void BookManagement::Select(Command &line, AccountManagement &accounts, LogManagement &logs) {
    if (accounts.get_current_Priority() < 3) {
        throw Exception("Invalid\n");
    }

    string _isbn = line.next_token();
    vector<int> ans;
    isbn_to_pos.find_node(_isbn, ans);
    //不存在,创建新书
    //id_to_pos 和 isbn_to_pos 都要修改
    if (ans.empty()) {
        book_data.get_info(num, 1);
        num++;
        Book temp(num, _isbn);
        book_data.write_info(num, 1);

        int pos = book_data.write(temp);
        id_to_pos.add_node(UllNode(to_string(num), pos));
        isbn_to_pos.add_node(UllNode(_isbn, pos));
        return;
    }

    Book temp;
    book_data.read(temp, ans[0]);
    accounts.User_select(temp.book_ID);
}

//book_data和..._to_pos都要修改,但是id_to_pos不变
void BookManagement::Modify(Command &line, AccountManagement &accounts, LogManagement &logs) {
    if (accounts.get_current_Priority() < 3) {
        throw Exception("Invalid\n");
    }

    string temp_command = line.next_token();
    string _name, _isbn, _author, _keyword, _price;

    //没有参数,说明全部都要输出 或者 没有选中书
    if (temp_command.empty() ||
        !accounts.login_stack.back().selected_book_id) {
        throw Exception("Invalid\n");
    }

    int _id = accounts.login_stack.back().selected_book_id;
    vector<int> ans;
    id_to_pos.find_node(to_string(_id), ans);
    Book new_book;
    book_data.read(new_book, ans[0]);

    while (!temp_command.empty()) {
        if (temp_command[1] == 'n') {
            if (_name.empty()) {
                for (int i = 7; i < temp_command.length() - 1; ++i)
                    _name += temp_command[i];
                strcpy(new_book.book_name.value, _name.c_str());
            } else { //重复指定参数
                throw Exception("Invalid\n");
            }
        }

        if (temp_command[1] == 'K') {
            for (int i = 10; i < temp_command.length() - 1; ++i) {
                //判断 |
                _keyword += temp_command[i];
            }
        }
        if (temp_command[1] == 'I') {
            if (_isbn.empty()) {
                for (int i = 7; i < temp_command.length() - 1; ++i)
                    _isbn += temp_command[i];
                strcpy(new_book.isbn.value, _isbn.c_str());
            } else {
                throw Exception("Invalid\n");
            }
        }
        if (temp_command[1] == 'a') {
            if (_author.empty()) {
                for (int i = 9; i < temp_command.length() - 1; ++i)
                    _author += temp_command[i];
                strcpy(new_book.author.value, _author.c_str());
            } else {
                throw Exception("Invalid\n");
            }
        }
        if (temp_command[1] == 'p') {
            if (_price.empty()) {
                for (int i = 8; i < temp_command.length() - 1; ++i)
                    _price += temp_command[i];
                new_book.price = stod(_price);
            } else {
                throw Exception("Invalid\n");
            }
        }
        temp_command = line.next_token();
    }

    book_data.update(new_book, ans[0]);
}

void BookManagement::ImportBook(Command &line, AccountManagement &accounts, LogManagement &logs) {
    if (accounts.get_current_Priority() < 3) {
        throw Exception("Invalid\n");
    }

    string _quantity = line.next_token();
    string s = line.next_token();

    if (!accounts.login_stack.back().selected_book_id) {
        //没有选中书本
        throw Exception("Invalid\n");
    }

    double _tot = stod(s);
    int _id = accounts.login_stack.back().selected_book_id;
    Book temp;
    vector<int> ans;
    id_to_pos.find_node(to_string(_id), ans);
    book_data.read(temp, ans[0]);
    int cnt = _tot / temp.price;
    temp.quantity += cnt;

    temp.total_cost -= _tot;

    book_data.update(temp, ans[0]);
}

void BookManagement::Buy(Command &line, AccountManagement &accounts, LogManagement &logs) {
    if (accounts.get_current_Priority() < 1) {
        throw Exception("Invalid\n");
    }

    string _isbn = line.next_token();
    string s = line.next_token();
    int _quantity = 0;
    for (int i = 0; i < s.length(); ++i) {
        _quantity = _quantity * 10 + s[i] - '0';
    }
    //也可以写_quantity = stoi(s);

    vector<int> ans;
    isbn_to_pos.find_node(_isbn, ans);
    if (ans.empty()) {    //没有这种书
        throw Exception("Invalid\n");
    }
    Book temp;
    book_data.read(temp, ans[0]);
    if (temp.quantity < _quantity) {    //书不够
        throw Exception("Invalid\n");
    }

    temp.quantity -= _quantity;
    book_data.update(temp, ans[0]);
    cout << (double) temp.price * _quantity << endl;
}

void BookManagement::Show(Command &line, AccountManagement &accounts, LogManagement &logs) {
    //权限不足
    if (accounts.get_current_Priority() < 1) {
        throw Exception("Invalid\n");
    }

    string temp = line.next_token();
    string _name, _isbn, _author, _keyword;

    //没有参数,说明全部都要输出
    if (temp.empty()) {}

    while (!temp.empty()) {
        if (temp[1] == 'n') {
            for (int i = 7; i < temp.length() - 1; ++i)
                _name += temp[i];
        }
        if (temp[1] == 'K') {
            for (int i = 10; i < temp.length() - 1; ++i) {
                //判断 |
                _keyword += temp[i];
            }
        }
        if (temp[1] == 'I') {
            for (int i = 7; i < temp.length() - 1; ++i)
                _isbn += temp[i];
        }
        if (temp[1] == 'a') {
            for (int i = 9; i < temp.length() - 1; ++i)
                _author += temp[i];
        }
        temp = line.next_token();
    }

    //多关键字匹配,理论上最后只有一本书
    vector<int> ans;
    isbn_to_pos.find_node(_isbn, ans);

    name_to_pos.find_node(_name, ans);
    author_to_pos.find_node(_author, ans);
    keyword_to_pos.find_node(_keyword, ans);
}