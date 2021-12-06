# Bookstore Management System

## 功能概述：

1. 支持账户注册、登录
2. 图书的借阅、归还，信息存储
3. 记录交易、员工操作、财务报表的日志系统
3. 文件读写
3. 数据存储：块状链表

## 命名规则：

- 类的名字：大写字母开头
- 变量的名字，函数命名：小写字母 + 下划线
- 函数的形式参数：下划线 + 原变量名（发生重复时）

## 文件架构：

$ull->main->{account,bookdatabase,log}$

## 分块实现：

### 块状链表 & 文件读写

接口：ull.h

以下的bookdatabase, account类都是要存储在块状链表中的。

```cpp
class UllNode {
// Stores key-value data
// The data type of key is `int` and the data type
// of value is `char[64]`, but the constructor only
// accepts `std::string` as value.
public:
    int offset;//操作的时间
    //并不是Block中的第offset个数据
    char str[64];
    bool operator<(const UllNode &x) const;
    // Compares str

    UllNode();
    UllNode(const int &arg1, const std::string &arg2);
    UllNode &operator=(const UllNode &right);
};

class UllBlock {
// For ULL class internal use only
public:
//    int nxt,pre;
    //指针
    UllBlock *next,*pre;
    int num;//整个链表中的第几块
    UllNode array[BLOCK_SIZE];
    UllBlock();
    UllBlock &operator=(const UllBlock &right);
};

class Ull {
// 'Unrolled Linked List'
// A data structure used for file data storage.
// The advantage is that it is easy to write.
// But it is inferior to 'B+ Tree' in performance.
private:
    const std::string file_name;
    int siz;
    std::fstream fi, fo, fi2, fo2, fip, fip2, fop, fop2;
    // File Input/Output fstream objects
    // P for private methods

    inline int nextBlock(const int &offset);
    inline void delBlock(const int &offset);
    void mergeBlock(const int &offset1, const int &offset2);
    UllBlock *splitBlock(const int &offset);

public:
    //文件读入
    Ull(const std::string &arg);
    ~Ull();
    void findNode(const std::string &key, std::vector<int> &array);
    // Returns an empty array if the node doesn't exist

    void addNode(const UllNode &node);
    int deleteNode(const UllNode &node);
    void maintain();

#ifdef PPL_DEBUG
    void debugPrint();
#endif

};
```

~~可能会重新写一个文件读写类（类似小作业MemoryRiver.h）~~

### 命令读取类

接口：command.h

思路是在main函数中，利用Token等读取具体的指令，把权限{0}、命令类型、命令参数三者分开读取，并将其分派到相应的接口中，分别实现。

目前尚未完成QWQ

### 账户系统

接口：account.h

```cpp
class Account_system {
private:
    class user_account{
        string user_id;
        string user_name;
        string password;
        short int priority;
        bool login;
        int cnt;
    };
    string userID;
    unordered_map(std::string,user_account);
    
public:
    //todo:登录（注意登录栈的实现）
    void su(command a,string _user_id,string password_try) {}
    //todo:注销
    void log_out(command a){}
    //todo:注册
    void register(command a,string _user_id,string _password_,string _user_name){}
    //todo:修改密码
    void passwd(command a,string _user_id,string _old_password,string _new_password){}
    //todo:创建账户
    void user_add(command a,string _user_id,string _password,short int _priority,string _user_name){}
    //todo:删除账户
    void delete(command a,string _user_id){}

};

```

### 图书系统

接口：bookdatabase.h


```cpp
class Book_system{
private:
    class Book{
    private:
        string ISBN,book_name,author,keyword[maxn];
        int quantity,book_NO;
        double price,total_cost;
    public:
        Book(){}
    }
    //todo:维护有序性，maybe useful?
    set<string> Book_list;
    unordered_map<string,Book> MAP;
    //注意到:某些信息是不能重复的(如ISBN)，需要进行判断

public:
    //todo:展示书本的信息，可以有缺省参数
    void show(string _ISBN = "",string _name = "",string _author = "",string _keyword[]){}
    //todo:购买图书
    void buy(string _ISBN,const int& _quantity){}
    //todo:选择图书，以便进行接下来的操作
    void select(string _ISBN){}
    //todo:修改图书信息
    void modify(string _ISBN = "",string _name = "",string -author = "",string _keyword = "",double _price = 0){}
    //todo:图书进货
    void import(int _quantity,double _total_cost){}
}
```


### 日志系统

接口：log.h

```cpp
class Log_system{
private:
    int Time;
    string RecordOp[Maxn];
    vector<double> Finance;
    unordered_map<string, vector<string>> EmployeeOp;
    unordered_map<string, vector<string>> CustomerOp;
    vector<string> OwnerOp;
public:

    Log() : Time(0){}
    //todo:打印某个员工操作记录
    void report_op(const string& name){}
    //todo:财务记录查询
    void show_finance(const int& t){}
    //todo:财务记录报告
    void report_finance(){}
    //todo:遍历map，打印全体员工的所有操作记录
    void report_employee(){}
    //todo:打印顾客
    void report_customer(){}
    //todo:店主
    void report_owner(){}
    //todo:打印所有的日志
    void report_log(){
        report_finance();
        report_employee();
        report_customer();
        report_owner();
    }
};
```

## Expectations:

> Rome was not built in a day.
>
> Understand the knowledge and complete the great project, day by day.
>
> ——by Leonard_chen

