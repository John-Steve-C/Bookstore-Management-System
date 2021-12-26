#include "command.h"

//class Command

Command::Command(char _delimiter) {
    delimiter = _delimiter;
}

Command::Command(const Command &rhs) {
    buffer = rhs.buffer;
    cur = rhs.cur;
    delimiter = rhs.delimiter;
}

Command::Command(const std::string &in, char _delimiter) {
    delimiter = _delimiter;
    buffer = in;
    cur = 0;//过滤行首的分隔符
    while (buffer[cur] == delimiter) cur++;
}

string Command::next_token() {
    int j = cur,len = buffer.length();
    string temp = "";
    if (cur >= len) return temp;

    while (buffer[j] != delimiter && j < len && buffer[j] != '\r'){
        temp += buffer[j];
        j++;
    }

    while (buffer[j] == delimiter && j < len && buffer[j] != '\r') j++;
    cur = j;
    return temp;
}

void Command::clear() {
    buffer = "";
    cur = 0;
    delimiter = ' ';
    return;
}

istream &operator>>(istream &input, Command &obj) {
    input >> obj.buffer;
    return input;
}

ostream &operator<<(ostream &os, const Command &command) {
    os << "buffer: " << command.buffer << " cur: " << command.cur << " delimiter: " << command.delimiter;
    return os;
}

void Command::set_delimiter(char new_delimiter) {
    delimiter = new_delimiter;
}
