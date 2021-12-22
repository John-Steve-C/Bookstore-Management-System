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
    //加入root账户
    user_data.initialise("user_data");
//    id_to_pos 如何初始哈?
    num = 0;
//    User("root", , ,7);
}

void AccountManagement::switch_User(Command &line) {
    string ID = line.next_token();
    vector<int> ans;
    id_to_pos.find_node(ID, ans);
    if (ans.empty()) {

    }//重复注册,失败

    int cur_priority = get_current_Priority();
    User temp;
    user_data.read(temp, ans[0]);

    string s = line.next_token();
    //权限足够
    if (cur_priority >= temp.get_priority()) {
        LogInAccount tp;
        tp.user = temp;

        tp.selected_book_id = 1;//要修改
        login_stack.push_back(tp);
        return;
    }

    //权限不够
    // 没输入密码,或者密码错误
    if (s.empty() || strcmp(s.c_str(), temp.password)) {
        //异常判断:密码错误

    } else {
        LogInAccount tp;
        tp.user = temp;

        login_stack.push_back(tp);
        return;
    }
}

void AccountManagement::LogOut(Command &line) {
    if (login_stack.empty()) {

    }    //没有登录账户,异常

    login_stack.pop_back();
    return;
}

void AccountManagement::register_User(Command &line) {
    string _ID = line.next_token();
    string _password = line.next_token();
    string _name = line.next_token();

    vector<int> ans;
    id_to_pos.find_node(_ID, ans);
    if (!ans.empty()) {

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

    }

    vector<int> ans;
    id_to_pos.find_node(_ID, ans);
    if (!ans.empty()) {

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

    } else {
        temp.change_password(new_password);
        user_data.update(temp, ans[0]);
    }
}

int AccountManagement::get_current_Priority() const {
    return login_stack.back().user.get_priority();
}

void AccountManagement::remove_User(Command &line, LogManagement &logs) {
    if (get_current_Priority() < 7) {
        //权限不足
    }

    string ID = line.next_token();
    vector<int> ans;
    id_to_pos.find_node(ID, ans);
    if (ans.empty()) {

    } //不存在,失败

    //已经登录,失败
    for (auto item:login_stack){
        if (item.user.ID.get_UserID() == ID) {

        }
    }

    User temp;
    user_data.Delete(ans[0]);
    id_to_pos.delete_node(UllNode(ID, ans[0]));
}

void AccountManagement::User_select(int book_id) {}