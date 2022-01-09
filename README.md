# Bookstore 开发文档

项目： Bookstore-2021

文档作者：刘祎禹

实现者：陈文韬

## 程序功能概述 Overview of Program Functions

### 基础功能 Basic Functions

- 程序首次运行时即创建用户名为 `root`，密码为 `sjtu`，权限为 `{7}` 的超级管理员账户

- 指令简介

  - 指令合法字符集为标准 Ascii 字符

  - 单个指令最大长度为 `1024` 字节

  - 允许的空白符仅为空格，单个指令被空白符分割为多个部分

    - 多个连续空白符效果与一个空白符等价
    - 行首行末允许出现多余空白符
    - 如无特殊说明禁止省略或另增空白符

  - 指令中第一个部分必须为指令关键词，指令中关键词部分必须与指令格式完全匹配

  - `[x]` 表示一串有特定限制的用户自定义字符串

  - `(a|b|c)` 表示此处仅能出现 `a`, `b`, `c` 中其一

  - `(x)?` 表示此处可以出现零次或一次 `x`

  - `(x)+` 表示此处可以出现一次或多次 `x`

  - ```css
    # 基础指令
    quit
    exit
    
    # 账户系统指令
    su [User-ID] ([Password])?
    logout
    register [User-ID] [Password] [User-Name]
    passwd [User-ID] ([Old-Password])? [New-Password]
    useradd [User-ID] [Password] [Priority] [User-Name]
    delete [User-ID]
    
    # 图书系统指令
    show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]")?
    buy [ISBN] [Quantity]
    select [ISBN]
    modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
    import [Quantity] [Total-Cost]
    
    # 日志系统指令
    report myself
    show finance ([Time])?
    report finance
    report employee
    log
    ```

- 在用户输入一条指令后，如果合法则执行对应操作，如果有则输出操作结果；如果指令非法或操作失败则输出 `Invalid\n`

  - 仅有空白符的指令合法且无输出内容
  - 除非有特殊说明，如果输入指令对应的输出内容非空，则结尾有 `\n` 字符；输出内容为空则不输出任何字符

- `quit` 和 `exit` 指令功能为正常退出系统

- 书店管理系统服务于 **店主（超级管理员）**，**员工（管理员）以及顾客（用户）**。为满足其不同需求，故需账户系统管理账户及权限数据，并提供相应功能

- **权限说明**

  - 本文档中以 `{x}` 表示权限等级，数字越大则权限越高
  - 账户的权限等级有 `{7}`, `{3}` 和 `{1}`
  - 未登录任何账户的状态下当前账户权限视为 `{0}`
  - 低权限账户能够访问的功能严格是高权限账户能够访问的功能的**子集**
  - 指令的权限等级表示能够执行该指令的最低当前账户权限需求

- **各权限可访问功能说明**

  - **店主 `{7}`**：使用 `root` 账户，可以访问书店系统所有功能，包括日志和账户管理功能
  - **员工 `{3}`**：可以访问修改图书数据相关功能等
  - **顾客 `{1}`**：可以注册账户，查询 / 购买图书
  - **游客 `{0}`**：可以注册账户
  - 本系统中有且仅有以上四种权限等级，其余权限等级非法

- **登录栈**

  - 系统支持嵌套登录，即允许多个账户同时处于登录状态

- 输入的指令视为最后登录的账户操作

- 退出系统视为登出所有已登录账户

### 附加功能 Advanced Functions

暂无


## 主体逻辑说明 Description of the Main Part

*注：在处理数据中使用 `std:string`，在储存时使用 `char*`*

### 运行 Operating

- 首先`AccountManagement` 以及 `BookManagement` 类的构造函数中，会先检查文件是否存在，如不存在，则建立文件。
- 程序为一个循环的 try-catch 块
- 每次读入一行，输入command，分析输入内容，调用函数



### 储存 Storing

#### 账户储存

账户储存由2个（~~2类~~）文件组成：

- 储存所有信息的数据表（按**时间**先后次序）

- 由用户 ID 为依据的指向数据的索引表（按 **ID的字典序** 排序的 **块状链表**）

每次查找都先在索引表中进行顺序查找~~（其实可以两次**二分**查找）~~，然后访问数据表



#### 书目储存

书目储存由5个文件组成：

- 由 `book_id` 决定，储存图书信息的数据表
- 由 自定义的 `book_id`为依据的索引表
- 由 ISBN 为依据的的索引表
- 由书名为依据的索引表
- 由作者为依据的索引表
- 由关键词为依据的索引表



#### 日志储存

- 储存日志文件



## 储存结构 Storage Structure

### 概览 Overview

1. 用户信息 `user_data`
2. 用户 ID 对应映射表 `id_to_pos`
3. 图书信息表 `book_data`
4. 书本 ID 对应映射表 `id_to_pos`（虽然名字相同，但出于不同的类中，不会引起混淆）
5. ISBN-Book 映射表 `isbn_to_pos`
6. 书名-图书索引表 `name_to_pos`
7. 作者-图书索引表 `author_to_pos`
8. 关键词-图书索引表 `keyword_to_pos`
8. 日志表 `log`



### 详细结构 Detailed Structure

*注：每个文件以二进制方式储存，最小储存单元为一个 class，每次写入或读入一个 class，其长度可用 `sizeof()` 获得。注意：字符类型的情况下需用 `char[<length> + 1]` 而非 `std::string` 完成，否则无法正常读写。*

1. **用户信息** （1-Based， 0表示空指针) 
     - `User-ID`：用户 ID
       - 合法字符集：数字，字母，下划线
       - 最大长度：30
     - `Password`：用户密码
       - 合法字符集：数字，字母，下划线
       - 最大长度：30
     - `UserName`：用户名
       - 合法字符集：除不可见字符以外 Ascii 字符
       - 最大长度：30
     - `Priority`：用户权限
       - 合法字符集：数字 `1, 3, 7`
       - 最大长度：1
2. **用户 ID 对应映射表**（以 `User-ID` 字典序排序的块状链表）
   - `User-ID`
   - `id_to_pos`
3. **图书信息表**
   - `ISBN`：Internal Strange Book Number
     - 合法字符集：除不可见字符以外 Ascii 字符
     - 最大长度：20
     - 特殊说明：本系统中任何两本图书不应有相同 `ISBN` 信息
   - `Book-Name`：图书名字
     - 合法字符集：除不可见字符和英文双引号以外 Ascii 字符
     - 最大长度：60
   - `Author`：图书作者名字  
     - 合法字符集：除不可见字符和英文双引号以外 Ascii 字符
     - 最大长度：60
   - `Keyword`：图书关键词
     - 合法字符集：除不可见字符和英文双引号以外 Ascii 字符
     - 最大长度：60
     - 特殊说明：`keyword` 内容以 `|` 为分隔可以出现多段信息。如 `math|magic|quantum` 表示该图书具有三个无顺序的关键词 `math`, `magic` 和 `quantum`
   - `Quantity`：购买数量
     - 合法字符集：数字
     - 最大长度：10
     - 特殊说明：数值不超过 `2'147'483'647`
   - `Price`：图书单价
     - 合法字符集：数字和 `.`
     - 最大长度：13
     - 特殊说明：本系统中浮点数输入输出精度固定为小数点后两位
   - `Total-Cost`：交易总额
     - 合法字符集：数字和 `.`
     - 最大长度：13
     - 特殊说明：本系统中浮点数输入输出精度固定为小数点后两位
4. **ISBN-Book 映射表**（以 `ISBN` 进行字典序排序的块状链表）
   - `ISBN`
   - `isbn_to_pos`
5. **书名-图书索引表**（以 `Book-Name` 进行字典序排序的块状链表）
   - `Book-Name`
   - `ISBN`
   - `name_to_pos`
6. **作者-图书索引表**（以 `Author` 进行字典序排序的块状链表）
   - `Author`
   - `ISBN`
   - `author_to_pos`
7. **关键词-图书索引表** （以 `Keyword` 进行字典序排序的块状链表）
   - `Keyword`
   - `ISBN`
   - `Pionter-to-Book-Data`
8. **日志表**
   - `user_id`
   - `Behavior`
   - `Amount`



## 代码结构及说明 Structures and Descriptions of Code

*如果开发人员代码风格和文档中的代码风格不同，请自行更改为自己的风格*

### main.cpp

- `int main();`
  - `AccountManagement accounts;` 
  -  `BookManagement books;`
  - `LogManagement logs;`
  - 接下来是一个 `while (ture) {} catch (int) {}` 循环，内部调用 `void next_token();` 函数，读取信息后进行进一步的操作。

### command.h

```cpp
#ifndef BOOKSTORE_COMMAND_H
#define BOOKSTORE_COMMAND_H

#include <string>
#include <ostream>
#include <istream>

//#include "Exception.h"
//同样要前置声明?
//class Exception;

using std::string;
using std::istream;
using std::ostream;

class Command {
    friend class AccountManagement;
    friend class BookManagement;
private:
    string buffer = "";//存储字符串的缓冲区
    int cur = 0;//当前指针的位置
    char delimiter = ' ';//分隔符

public:
    int cnt = 0;
    Command() = default;

    Command(const Command &rhs);

    Command(char _delimiter);

    Command(const std::string &in, char _delimiter = ' ');

    ~Command() = default;

    void count(); //统计参数个数

    string next_token();//取出下一个Token,更新cur

    void clear();

    friend istream &operator>>(istream &input, Command &obj);

    friend ostream &operator<<(ostream &os, const Command &command);

    void set_delimiter(char new_delimiter);
};
```

### accounts.h 

```CPP
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
    friend class LogManagement;
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

    void switch_User(Command &line, LogManagement &logs); // su command

    void LogOut(Command &line, LogManagement &logs); // logout command

    void register_User(Command &line, LogManagement &logs); // register command
    //权限为0,只能加顾客

    void change_password(Command &line, LogManagement &logs); // passwd command

    void add_User(Command &line, LogManagement &logs); // useradd command
    //权限为3,可以加员工

    void remove_User(Command &line, LogManagement &logs); // delete command

    void User_select(int book_id); // 对于当前用户选中书本

    [[nodiscard]] int get_current_Priority() const;
};
```

### bookdatabase.h 
*注：可以调用 `AccountManagement::getCurrentPriority()` 来获取权限*

```CPP
#include <set>
#include <vector>
#include <iomanip>
#include <queue>
#include <cmath>

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

    bool operator<(const Book &rhs) const;

    bool operator>(const Book &rhs) const;

    bool operator<=(const Book &rhs) const;

    bool operator>=(const Book &rhs) const;
};

class BookManagement {
private:
    int num = 0; //当前的书本总数,存放在book_data的第一个位置
    MemoryRiver<Book> book_data; // 用于储存所有数据的文件

    //按照不同关键子进行排序的块状链表
    //其实就是索引文件
    Ull id_to_pos;
    Ull isbn_to_pos;
    Ull name_to_pos;
    Ull author_to_pos;
    Ull keyword_to_pos;
    //如何修改?

public:
    BookManagement();
    // 下面的指令请调用 accounts::getCurrentPriority() 来获取权限
    void show(Command &line, AccountManagement &accounts,
              LogManagement &logs);
    // 先判断是不是 show finance（都是以 show 开头），然后分四种情况讨论，如无参数，则按照 ISBN 输出全部（traverse 函数）
    // 我把这个判断的过程放在main函数中实现,事实上line中的cur在第二个关键词

    void buy(Command &line, AccountManagement &accounts, LogManagement &logs);

    void select(Command &line, AccountManagement &accounts, LogManagement &logs); // 检查是否有权限，检查是否有 ISBN，然后选中

    void modify(Command &line, AccountManagement &accounts, LogManagement &logs); // 检查是否有权限

    void import_book(Command &line, AccountManagement &accounts, LogManagement &logs); // 检查是否有权限

    bool is_visible(const string &x);
};
```

### ull.h 
```CPP
#include <cstdio>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include "file_io.h"

#define LIST_LEN 65
#define BLOCK_SIZE 500
#define BLOCK_SPLIT_THRESHOLD BLOCK_SIZE << 1
#define BLOCK_MERGE_THRESHOLD 250
//从上到下:链表的总长度,块的大小,分裂的max,合并的min
#pragma endregion

using namespace std;
class UllNode {
private:
    int offset, val;//val就是value,是排序的第二关键字
    //offset是节点在文件中的位置
    char str[64];//实际上存储的index,用来比对,第一关键字

public:
    friend class UllBlock;
    friend class Ull;

    UllNode() = default;

    UllNode(const std::string &arg1, const int &value, const int &arg2 = 0) {
        //arg1是第一关键字,arg2同理
        strcpy(str, arg1.c_str());
        //注意转换，存储的是char[]，读入的是string
        val = value;
        offset = arg2;
    }

    bool operator<(const UllNode &x) const {//重载小于号,用于lower_bound
        if (strcmp(str, x.str) == 0) return val < x.val;
        else return strcmp(str, x.str) < 0;
    }

    bool operator>(const UllNode &x) const {
        if (strcmp(str, x.str) == 0) return val > x.val;
        else return strcmp(str, x.str) > 0;
    }

    bool operator>=(const UllNode &x) const {
        return !(*this < x);
    }

    bool operator<=(const UllNode &x) const {
        return !(*this > x);
    }

    bool operator==(const UllNode &x) const {
        return (strcmp(str, x.str) == 0) && (val == x.val);
    }

    bool operator!=(const UllNode &x) const {
        return !(*this == x);
    }

    UllNode &operator=(const UllNode &right) {
        strcpy(str, right.str);
        offset = right.offset;
        val = right.val;
        return *this;
    }
};

class UllBlock {
private:
    int nxt, pre, pos;//指针,pos是块首的位置
    int siz;//siz是块的大小
    UllNode array[(BLOCK_SIZE << 1) + 10];//最大空间要2倍

public:
    friend class Ull;

    UllBlock() {
        nxt = pre = siz = 0;
        pos = 0;
    }

    UllBlock(const int _siz, const UllNode *_array) {
        siz = _siz;
        memcpy(array, _array, _siz * sizeof(UllNode));
    }

    UllBlock &operator=(const UllBlock &right) {
        nxt = right.nxt;
        pre = right.pre;
        pos = right.pos;
        siz = right.siz;
        memcpy(array, right.array, siz * sizeof(UllNode));
        return *this;
    }

    UllNode front() {
        return array[0];
    }

    UllNode back() {//注意到,判断是否在块中,要与最后一个最大的元素比较
        if (!siz) return array[0];
        return array[siz - 1];
    }

    bool add(const UllNode &x) {//在块中新添加元素x,0说明合法
        int pos = std::lower_bound(array, array + siz, x) - array;
        //二分查找第一个不小于x的元素，返回的是指针(位置)
        if (array[pos] == x) return 1;//已经存在,不合法
        siz++;
        for (int i = siz - 1; i > pos; --i)
            array[i] = array[i - 1];//后方的元素向后移一位
        array[pos] = x;
        return siz >= (BLOCK_SIZE << 1);//判断大小是否超出限制(2倍的长度)，需要split
    }

    bool del(const UllNode &x) {//删除块中的元素x,0说明合法
        int pos = std::lower_bound(array, array + siz, x) - array;
        if (array[pos] != x) return 1;//不存在
        siz--;
        for (int i = pos; i < siz; ++i)
            array[i] = array[i + 1];//向前移动一位
        array[siz] = UllNode();
        return 0;
    }

    void search(const std::string &key, std::vector<int> &temp_array) {
        for (int i = 0; i < siz; ++i) {
            if (array[i].str == key)
                temp_array.push_back(array[i].val);
        }
    }//在当前块中查找键值位key的Node，返回一个存储val的vector
};

class Ull {//专门用来处理与文件交互的操作
//    friend class AccountManagement;
//    friend class BookManagement;
//    friend class LogManagement;
private:
    const std::string file_name;
    int num, fpos;//块的个数和头指针
    //注意:把num存到了文件的头部,需要每次get/write
    MemoryRiver<UllBlock> block_list;

    inline void del_block(const int &pos) {
    }//删除第pos块

public:
    //文件读入,arg就是filename,将二者关联起来
    Ull() {
        num = 0;
        fpos = 8;
    }

    Ull(const std::string &arg) {
        block_list.initialise(arg);
        num = 0;
        fpos = 8;
    }

    void init(const std::string &file) {
        block_list.initialise(file);
        num = 0;
        fpos = 8;
    }

    ~Ull() = default;

    void find_node(const std::string &key, std::vector<int> &temp_array) {
        UllBlock tp_block;
        block_list.get_info(num, 1);
        for (int i = fpos; i; i = tp_block.nxt) {
            block_list.read(tp_block, i);
            if (key >= tp_block.front().str && key <= tp_block.back().str) {
                tp_block.search(key, temp_array);
            }
        }
    }

    void add_node(const UllNode &node) {
        UllBlock new_block;
        block_list.get_info(num, 1);
        if (!num) {//第一次
            new_block.siz = 1;
            new_block.array[0] = node;
            new_block.pos = block_list.write(new_block);
            block_list.update(new_block, new_block.pos);
            //注意,要更新到文件里
            //以下两行代码,已经由上述的write在MemoryRiver中实现
//            num++;
//            block_list.write_info(num, 1);
        } else {
            int cnt = 0;//cnt是当前遍历到第几块
            for (int i = fpos; i; i = new_block.nxt) {
                cnt++;
                block_list.read(new_block, i);
                if (node < new_block.back() || cnt == num) {
                    new_block.add(node);
                    block_list.update(new_block, i);
                    break;
                }
            }
            if (new_block.siz > BLOCK_SPLIT_THRESHOLD)
                split_block(new_block.pos);
        }
    }

    void delete_node(const UllNode &node) {
        block_list.get_info(num, 1);
        UllBlock tp_block;
        for (int i = fpos; i; i = tp_block.nxt) {
            block_list.read(tp_block, i);
            //可能会发生跨越情况
            if (node < tp_block.front() || node > tp_block.back())
                continue;
            else {
                tp_block.del(node);
                block_list.update(tp_block, i);
                if (node > tp_block.back())
                    break;
            }
        }
        if (tp_block.siz <= BLOCK_MERGE_THRESHOLD)
            merge_block(tp_block.pos);
    }

    void split_block(const int &pos) {
        UllBlock tp_block, new_block;
        //在下方的write中已经更新
//        block_list.get_info(num, 1);
//        num++;
//        block_list.write_info(num, 1);

        block_list.read(tp_block, pos);

        int len = tp_block.siz / 2;
        new_block.siz = tp_block.siz - len;
        memcpy(new_block.array, tp_block.array + len, new_block.siz * sizeof(UllNode));
        tp_block.siz = len;

        new_block.pre = tp_block.pos;//新的块接到后面
        new_block.nxt = tp_block.nxt;
        new_block.pos = block_list.write(new_block);
        tp_block.nxt = new_block.pos;

        block_list.update(tp_block, tp_block.pos);
        block_list.update(new_block, new_block.pos);
    }

    void merge_block(const int &pos) {//把pos合并到pre
        if (pos == 8) return;//头节点
        UllBlock tp_block, pre_block, nxt_block;

//        block_list.get_info(num, 1);
//        num--;
//        block_list.write_info(num, 1);

        block_list.read(tp_block, pos);
        block_list.read(pre_block, tp_block.pre);

        if (tp_block.nxt) {//不是尾节点
            block_list.read(nxt_block, tp_block.nxt);
            nxt_block.pre = pre_block.pos;
            block_list.update(nxt_block, nxt_block.pos);
        }

        pre_block.nxt = tp_block.nxt;
        memcpy(pre_block.array + pre_block.siz, tp_block.array, tp_block.siz * sizeof(UllNode));
        pre_block.siz += tp_block.siz;

        block_list.update(pre_block, pre_block.pos);
        block_list.Delete(tp_block.pos);

        if (pre_block.siz >= BLOCK_MERGE_THRESHOLD)
            split_block(pre_block.pos);
    }

    //返回所有UllNode的val(实际上就是pos)
    void find_all(vector<int> &ans){
        UllBlock tp_block;
        for (int i = fpos; i ; i = tp_block.nxt) {
            block_list.read(tp_block, i);
            for (int j = 0; j < tp_block.siz; ++j) {
                ans.push_back(tp_block.array[j].val);
            }
        }
    }

};
```

### log.h

```CPP
#include <vector>
#include <string>
#include <iomanip>

#include "ull.h"
#include "file_io.h"
#include "command.h"
#include "Exception.h"

class AccountManagement;
class User;
//类或结构体的前向声明,只能用来定义指针对象或引用
using std::vector;
using std::string;
const int Maxn = 1e5;

enum Behavior {LOGIN, ADDUSER, DELETE, SHOW, BUY, SELECT, MODIFY, IMPORT};
class Log {
public:
    char *user_ID;
    //不用指针会报错?
    Behavior behavoir;
    char description[150];
    bool if_earn = false; // 表示是否是收入
    double Amount = 0;

    Log() = default;
};

class LogManagement {
private:
    MemoryRiver<Log> log_data;
    int num = 0; // 交易笔数

public:
    LogManagement();

    void report_employee(Command& line, AccountManagement& accounts);

    void report_myself(Command &line, AccountManagement& accounts);

    void add_log(Log& log); // 把 log 放进文件的同时还需要检查是否有交易

    void show_finance(int limit = -1); // 若为 -1，则显示全部

    void log_all(Command &line, AccountManagement& accounts); //

    void Print(const Behavior &x);
};
```
