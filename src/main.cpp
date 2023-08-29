#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <pwd.h>  

using namespace std;

bool run = true;

string prompt(string prompt) {
    string command;

    cout << prompt;
    getline(cin, command);

    return command;
}

passwd *getuser() {
    uid_t userid;
    struct passwd* pwd;
    userid = getuid();
    pwd = getpwuid(userid);
    return pwd;
}

bool specialcom(string command) {
    if (command == "exit") {
        run = false;
    } else if (command.substr(0, 2) == "cd") {
        string homedir = getuser()->pw_dir;

        for (int i = 0; i < command.length(); i++) {
            if (command[i] == '~') {
                command.replace(i, 1, homedir);
            }
        }

        chdir(command.substr(3).c_str());
    } else {
        return false;
    }

    return true;
}

int main(int argc, char *argv[]) {
    while (run) {
        string command = prompt("> ");

        if (!specialcom(command))
            system(command.c_str());

        command.clear();
    }

    return 0;
}