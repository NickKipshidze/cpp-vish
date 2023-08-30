#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <pwd.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>

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

bool specialcom(char **argv) {
    if ((string)argv[0] == "exit") {
        run = false;
    } else if ((string)argv[0] == "cd") {
        string homedir = getuser()->pw_dir;

        for (int arg = 0; argv[arg] != nullptr; arg++) {
            string args = (string)argv[arg];

            for (int i = 0; i < (int)args.size(); i++){
                if (args[i] == '~') {
                    args.replace(i, 1, homedir);
                }
            }

            strcpy(argv[arg], (char *)args.c_str());
        }

        chdir(argv[1]);
    } else {
        return false;
    }

    return true;
}

char **parsecom(string command) {
    vector<string> args;
    istringstream iss(command);
    string token;
    while (iss >> token) {
        args.push_back(token);
    }
    
    char** argv = new char*[args.size() + 1];
    for (size_t i = 0; i < args.size(); i++) {
        argv[i] = const_cast<char*>(args[i].c_str());
    }
    argv[args.size()] = nullptr;

    return argv;
}

int main() {
    chdir(getuser()->pw_dir);

    while (run) {
        string command = prompt((string)getcwd(0, 0) + (string)" > "); 

        char **argv = parsecom(command);

        if (!specialcom(argv)) {
            run = false;

            pid_t pid = fork();
            
            if (pid < 0) {
                return 1;
            } else if (pid == 0) {
                execvp(argv[0], argv);
            } else {
                int childStatus;
                waitpid(pid, &childStatus, 0);
                run = true;
            }
        }

        command.clear();
    }

    return 0;
}