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

file_io 	->	ull	->	main	->	{account,	bookdatabase,	log}

## 分块实现：

### 块状链表

接口：ull.h

以下的bookdatabase, account类都是要存储在块状链表中的。

对第一次的模板做出了修改。

```cpp
class UllNode {
// Stores key-value data
// The data type of key is `int` and the data type
// of value is `char[64]`, but the constructor only
// accepts `std::string` as value.
public:
    int offset,val;//val是操作的'时间',是排序的第二关键字
    //offset是节点在文件中的位置
    char str[64];//实际上存储的信息，用来比对

    UllNode();
    UllNode(const int &arg1, const std::string &arg2);
    bool operator<(const UllNode &x) const;
    bool operator==(const UllNode &x) const;
    UllNode &operator=(const UllNode &right);
};

class UllBlock {
// For ULL class internal use only
public:
    int nxt,pre;//指针
    int num,siz;//num指链表中的第几块,siz是块的大小
    UllNode array[BLOCK_SIZE];

    UllBlock();
    UllBlock(const int _siz,const UllNode *_array);
    UllBlock &operator=(const UllBlock &right);

    bool add(const UllNode &x);//在块中新添加元素
    bool del(const UllNode &x);//删除块中的元素
    UllBlock split(const int &x);//把当前块拆分为长度为 siz-x 和 x 的两块
    //返回的是长度为x的新块
    UllBlock & merge(const UllBlock &x);//将当前块与x合并
    //指针尚未进行更改
    void search(const std::string &key, std::vector<int> &temp_array);
    //在当前块中查找键值位key的Node，返回一个vector
};

class Ull {
// 'Unrolled Linked List'
// A data structure used for file data storage.
// The advantage is that it is easy to write.
// But it is inferior to 'B+ Tree' in performance.
private:
    const std::string file_name;
    int siz;//块的个数
    int offset[BLOCK_SIZE];//每个块在文件中的位置
    UllNode head[BLOCK_SIZE];//每个块的头节点？
    MemoryRiver<UllBlock> block_list;

    inline void del_block(const int &pos);//删除第pos块

public:
    //文件读入,arg就是filename
    Ull(const std::string &arg);

    ~Ull();

    void find_node(const std::string &key, std::vector<int> &temp_array);
    // Returns an empty array if the node doesn't exist
    // temp_array is used to store the position of existing nodes

    void add_node(const UllNode &node);

    int delete_node(const UllNode &node);

    int find_position(const UllNode &node) const;//找到x所在的块，返回块的编号

#ifdef PPL_DEBUG
    void debugPrint();
#endif
};
```

### 文件读写类

接口：file_io.h

实际上就是上次小作业中的MemoryRiver。

```cpp
template<class T, int info_len = 0>//
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    int num_T = 0;
    vector<int> next_index;

public:
    MemoryRiver() = default;

    MemoryRiver(const string &file_name) : file_name(file_name) {}

    void initialise(string FN = "") { }

    //在文件合适位置写入 类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    //todo: It's really important！
    int write(T &t) { }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) { }

    //读出位置索引index对应的T对象的值并fi赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) { }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index) { }

};
```

### 命令读取类

接口：command.h

思路是在main函数中，利用Token等读取具体的指令，把权限{0}、命令类型、命令参数三者分开读取，并将其分派到相应的接口中，分别实现。

```cpp
class Command {
private:
    string cmd;
    int len;
public:
    void get_token(){ }
    //未完待续
};
```

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
    set<string> book_list;
    unordered_map<string,Book> map;
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

## 开发记录：

### 块状链表的实现

- [x] UllBlock中，add，del，merge，split操作
- [ ] Ull中，add_node，del_node，操作
