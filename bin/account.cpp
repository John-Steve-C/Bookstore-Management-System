#include "account.h"

//class UserID

UserID::UserID(std::string userID) {
    strcpy(value, userID.c_str());
}

std::string UserID::get_UserID() const {
    return value;
}

bool UserID::operator==(const UserID &rhs) const {
    if (strcmp(value, rhs.value) == 0) return true;
    else return false;
}

bool UserID::operator<(const UserID &rhs) const {
    if (strcmp(value, rhs.value) < 0) return true;
    else return false;
}

//class User

User::User(const std::string &data) {

}

User::User(const std::string &_ID, const std::string &_name, const std::string &_password, int _priority) {
    strcpy(ID.value, _ID.c_str());
    strcpy(user_name, _name.c_str());
    strcpy(password, _password.c_str());
    priority = _priority;
}

void User::change_password(const std::string &newPassword) {
    strcpy(password, newPassword.c_str());
}

int User::get_priority() const {
    return priority;
}

//class AccountManegement

AccountManagement::AccountManagement() {
    user_data.initialise("user_data");//MemoryRiver
    id_to_pos.init("account_id_to_pos");//ull

    num = 0;
    //加入root账户,注意判断是否已经存在root,防止重复加入
    //root是ID
    User root("root", "", "sjtu", 7);
    user_data.get_info(num, 1);
    if (num) return; //已经存在
    int pos = user_data.write(root);

    id_to_pos.add_node(UllNode(root.ID.value, pos));
}

AccountManagement::AccountManagement(const string &file_name) {
    user_data.initialise("user_data");
    id_to_pos.init(file_name);

}

void AccountManagement::switch_User(Command &line) {
    //登录的时候,不仅要查找是否注册过,
    //还要在登录栈中查找是否已经登录过?
    //似乎可以重复登录同一账户?
    string ID = line.next_token();
    vector<int> ans;
    id_to_pos.find_node(ID, ans);
    if (ans.empty()) {
        throw Exception("Invalid\n");
    }//没注册过,失败

    int cur_priority = get_current_Priority();
    User temp;
    user_data.read(temp, ans[0]);

    string s = line.next_token();
    //权限足够
    if (cur_priority >= temp.get_priority()) {
        LogInAccount tp;
        tp.user = temp;

        login_stack.push_back(tp);
        return;
    }

    //权限不够
    // 没输入密码,或者密码错误
    if (s.empty() || strcmp(s.c_str(), temp.password) != 0) {
        //异常判断:密码错误
        throw Exception("Invalid\n");
    } else {
        LogInAccount tp;
        tp.user = temp;

        login_stack.push_back(tp);
        return;
    }
}

void AccountManagement::LogOut() {
    if (login_stack.empty()) {
        throw Exception("Invalid\n");
    }    //没有登录账户,异常

    login_stack.pop_back();
}

void AccountManagement::register_User(Command &line) {
    string _ID = line.next_token();
    string _password = line.next_token();
    string _name = line.next_token();

    vector<int> ans;
    id_to_pos.find_node(_ID, ans);
    if (!ans.empty()) {
        throw Exception("Invalid\n");
    } //重复,失败

    User temp(_ID, _name, _password, 1);
    //只能注册权限为1的顾客账户
    int pos = user_data.write(temp);
    id_to_pos.add_node(UllNode(_ID, pos));
}

void AccountManagement::add_User(Command &line, LogManagement &logs) {
    string _ID = line.next_token();
    string _password = line.next_token();
    string _priority = line.next_token();
    string _name = line.next_token();

    if (get_current_Priority() < _priority[0] - '0' ||
        get_current_Priority() < 3) {//权限不足,失败
        throw Exception("Invalid\n");
    }

    vector<int> ans;
    id_to_pos.find_node(_ID, ans);
    if (!ans.empty()) {
        throw Exception("Invalid\n");
    } //重复,失败

    User temp(_ID, _name, _password, _priority[0] - '0');
    int pos = user_data.write(temp);
    id_to_pos.add_node(UllNode(_ID, pos));
}

void AccountManagement::change_password(Command &line) {
    string ID = line.next_token();
    string old_password = line.next_token();
    string new_password = line.next_token();

    vector<int> ans;
    id_to_pos.find_node(ID, ans);
    if (ans.empty()) {
        throw Exception("Invalid\n");
    } //不存在,失败

    User temp;
    user_data.read(temp, ans[0]);

    if (get_current_Priority() == 7) {
        //权限为7,上方的old读到的就是new
        temp.change_password(old_password);
        user_data.update(temp, ans[0]);
        return;
    }

    if (strcmp(temp.password, old_password.c_str() ) != 0) {
        //密码错误
        throw Exception("Invalid\n");
    } else {
        temp.change_password(new_password);
        user_data.update(temp, ans[0]);
    }
}

int AccountManagement::get_current_Priority() const {
    if (login_stack.empty()) return 0;
    return login_stack.back().user.get_priority();
}

void AccountManagement::remove_User(Command &line, LogManagement &logs) {
    if (get_current_Priority() < 7) {
        //权限不足
        throw Exception("Invalid\n");
    }

    string ID = line.next_token();
    vector<int> ans;
    id_to_pos.find_node(ID, ans);
    if (ans.empty()) {
        throw Exception("Invalid\n");
    } //不存在,失败

    //已经登录,失败
    for (auto item:login_stack){
        if (item.user.ID.get_UserID() == ID) {
            throw Exception("Invalid\n");
        }
    }

    User temp;
    user_data.Delete(ans[0]);
    id_to_pos.delete_node(UllNode(ID, ans[0]));
}

void AccountManagement::User_select(int book_id) {
    login_stack.back().selected_book_id = book_id;
}