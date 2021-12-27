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
    book_name = BookName();
    author = Author();
    keyword = Keyword();
    quantity = 0;
    price = 0;
    total_cost = 0;
}

ostream &operator<<(ostream &output, const Book &obj) {
    output << obj.isbn.value << '\t' << obj.book_name.value << '\t'
           << obj.author.value << '\t' << obj.keyword.value << '\t'
           << fixed << setprecision(2) << obj.price << '\t' << obj.quantity << '\n';
    //注意输出保留2位小数
    return output;
}

bool Book::operator<(const Book &rhs) const {
    return isbn < rhs.isbn;
}

bool Book::operator>(const Book &rhs) const {
    return rhs < *this;
}

bool Book::operator<=(const Book &rhs) const {
    return !(rhs < *this);
}

bool Book::operator>=(const Book &rhs) const {
    return !(*this < rhs);
}

//class BookManagement

BookManagement::BookManagement() {
    book_data.initialise("book_data");
    id_to_pos.init("book_id_to_pos");
    isbn_to_pos.init("isbn_to_pos");
    name_to_pos.init("name_to_pos");
    author_to_pos.init("author_to_pos");
    keyword_to_pos.init("keyword_to_pos");
}

void BookManagement::Select(Command &line, AccountManagement &accounts, LogManagement &logs) {
    if (accounts.get_current_Priority() < 3 || line.cnt != 2) {
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
//        book_data.write_info(num, 1);
        //不需要，因为下面有个write
        int pos = book_data.write(temp);
        id_to_pos.add_node(UllNode(to_string(num), pos));
        isbn_to_pos.add_node(UllNode(_isbn, pos));
        accounts.User_select(temp.book_ID);
        return;
    }

    Book temp;
    book_data.read(temp, ans[0]);
    accounts.User_select(temp.book_ID);
}

//book_data和..._to_pos都要修改,但是id_to_pos不变
//注意特判，如果修改的ISBN与原来相同,那么就不对
void BookManagement::Modify(Command &line, AccountManagement &accounts, LogManagement &logs) {
    if (accounts.get_current_Priority() < 3 || line.cnt <= 1) {
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
                _name = temp_command.substr(7, temp_command.length() - 8);
                //更新数据
                name_to_pos.delete_node(UllNode(new_book.book_name.value, ans[0]));
                name_to_pos.add_node(UllNode(_name, ans[0]));
                strcpy(new_book.book_name.value, _name.c_str());
            } else { //重复指定参数
                throw Exception("Invalid\n");
            }
        }

        if (temp_command[1] == 'k') {
            //更改分隔符
            string s = temp_command.substr(10, temp_command.length() - 11);
            Command new_key(s, '|'), old_key(new_book.keyword.value, '|');
            //old是旧的keyword
            string ns = new_key.next_token(), os = old_key.next_token();
            //先判断是否冲突，再删除旧的对应关系?
            //直接全部暴力删除
            while (!os.empty()) {
                keyword_to_pos.delete_node(UllNode(os, ans[0]));
                os = old_key.next_token();
            }
            while (!ns.empty()) {
                keyword_to_pos.add_node(UllNode(ns, ans[0]));
                ns = new_key.next_token();
            }
            strcpy(new_book.keyword.value, s.c_str());
        }
        if (temp_command[1] == 'I') {
            if (_isbn.empty()) {
                _isbn = temp_command.substr(6, temp_command.length() - 6);
                //不能修改相同的isbn
                if (strcmp(new_book.isbn.value, _isbn.c_str()) == 0) {
                    throw Exception("Invalid\n");
                }
                //不能有两本ISBN相同的书
                vector<int> tp_ans;
                isbn_to_pos.find_node(_isbn, tp_ans);
                if (!tp_ans.empty()) {
                    throw Exception("Invalid\n");
                }

                isbn_to_pos.delete_node(UllNode(new_book.isbn.value, ans[0]));
                isbn_to_pos.add_node(UllNode(_isbn, ans[0]));
                strcpy(new_book.isbn.value, _isbn.c_str());
            } else {
                throw Exception("Invalid\n");
            }
        }
        if (temp_command[1] == 'a') {
            if (_author.empty()) {
                _author = temp_command.substr(9, temp_command.length() - 10);
                author_to_pos.delete_node(UllNode(new_book.author.value, ans[0]));
                author_to_pos.add_node(UllNode(_author, ans[0]));
                strcpy(new_book.author.value, _author.c_str());
            } else {
                throw Exception("Invalid\n");
            }
        }
        if (temp_command[1] == 'p') {
            if (_price.empty()) {
                _price = temp_command.substr(7, temp_command.length() - 7);
                //没有映射关系,不用修改
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
    if (accounts.get_current_Priority() < 3 || line.cnt != 3) {
        throw Exception("Invalid\n");
    }

    string _quantity = line.next_token();
    string s = line.next_token();

    if (!accounts.login_stack.back().selected_book_id) {
        //没有选中书本
        throw Exception("Invalid\n");
    }

    int _id = accounts.login_stack.back().selected_book_id;
    Book temp;
    vector<int> ans;
    id_to_pos.find_node(to_string(_id), ans);
    book_data.read(temp, ans[0]);
    temp.quantity += stoi(_quantity);
    temp.total_cost += stod(s);

    Log tp_log;
    tp_log.if_earn = false;
    tp_log.Amount = stod(s);
    logs.AddLog(tp_log);

    book_data.update(temp, ans[0]);
}

void BookManagement::Buy(Command &line, AccountManagement &accounts, LogManagement &logs) {
    if (accounts.get_current_Priority() < 1 || line.cnt != 3) {
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
    cout << fixed << setprecision(2) << (double) temp.price * _quantity << endl;

    Log tp_log;
    tp_log.if_earn = true;
    tp_log.Amount = (double) temp.price * _quantity;
    logs.AddLog(tp_log);
}

void BookManagement::Show(Command &line, AccountManagement &accounts, LogManagement &logs) {
    //权限不足
    if (accounts.get_current_Priority() < 1) {
        throw Exception("Invalid\n");
    }

    string temp = line.next_token();
    string _name, _isbn, _author, _keyword;
    vector<int> ans;

    //没有参数,说明全部都要输出
    if (temp.empty()) {
        isbn_to_pos.find_all(ans);
        //按照ISBN升序输出
        if (!ans.empty()) {
            for (int i = 0; i < ans.size(); ++i) {
                Book temp;
                book_data.read(temp, ans[i]);
                cout << temp;
            }
        } else {
            cout << '\n';
        }
        return;
    }

    //否则开始匹配
    if (temp[1] == 'n') {
        _name = temp.substr(7, temp.length() - 8);
        if (_name.empty()) {
            throw Exception("Invalid\n");
        } else {
            name_to_pos.find_node(_name, ans);
            if (!ans.empty()) {
                priority_queue<Book,vector<Book>,greater<Book> > output;
                for (int i = 0; i < ans.size(); ++i) {
                    Book temp;
                    book_data.read(temp, ans[i]);
                    output.push(temp);
                }
                while (!output.empty()) {
                    cout << output.top();
                    output.pop();
                }
            } else {
                cout << '\n';
            }
            return;
        }
    }
    if (temp[1] == 'k') {
        for (int i = 10; i < temp.length() - 1; ++i) {
            if (temp[i] == '|') {
                throw Exception("Invalid\n");
            }//判断 | ,此处只会有一个关键词
            _keyword += temp[i];
        }
        if (_keyword.empty()) {
            throw Exception("Invalid\n");
        } else {
            keyword_to_pos.find_node(_keyword, ans);
            if (!ans.empty()) {
                priority_queue<Book,vector<Book>,greater<Book> > output;
                for (int i = 0; i < ans.size(); ++i) {
                    Book temp;
                    book_data.read(temp, ans[i]);
                    output.push(temp);
                }
                while (!output.empty()) {
                    cout << output.top();
                    output.pop();
                }
            } else {
                cout << '\n';
            }
            return;
        }
    }
    if (temp[1] == 'I') {
        _isbn = temp.substr(6, temp.length() - 6);
        if (_isbn.empty()) {
            throw Exception("Invalid\n");
        } else {
            isbn_to_pos.find_node(_isbn, ans);
            if (!ans.empty()) {
                for (int i = 0; i < ans.size(); ++i) {
                    Book temp;
                    book_data.read(temp, ans[i]);
                    cout << temp;
                }
            } else {
                cout << '\n';
            }
            return;
        }
    }
    if (temp[1] == 'a') {
        _author = temp.substr(9, temp.length() - 10);
        if (_author.empty()) {
            throw Exception("Invalid\n");
        } else {
            author_to_pos.find_node(_author, ans);
            if (!ans.empty()) {
                priority_queue<Book,vector<Book>,greater<Book> > output;
                for (int i = 0; i < ans.size(); ++i) {
                    Book temp;
                    book_data.read(temp, ans[i]);
                    output.push(temp);
                }
                while (!output.empty()) {
                    cout << output.top();
                    output.pop();
                }
            } else {
                cout << '\n';
            }
            return;
        }
    }
}