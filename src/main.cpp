#include <iostream>
#include <cstring>
#include <pwd.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>

#include "config.h"

using namespace std;

bool run = true;

void handleTerm(int signum) {
    if (signum == SIGINT) {
        if (getpid() != -1)
            kill(getpid(), SIGTERM);
    }
}

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

string genpromptstr(string format) {
    // %u - username
    // %d - directory

    string current_dir = getcwd(0, 0);
    string home_dir = getuser()->pw_dir;

    if (current_dir.substr(0, home_dir.size()) == home_dir)
        current_dir.replace(0, home_dir.length(), "~");

    for (int chr = 0; chr < (int)format.size(); chr++) {
        if (format[chr] == '%') {
            switch (format[chr+1]) {
                case 'u':
                    format.replace(chr, 2, getuser()->pw_name);
                    break;

                case 'd':
                    format.replace(chr, 2, current_dir);
                    break;
                
                default:
                    break;
            }
        }
    }

    return format;
}

bool specialcom(char **argv) {
    if ((string)argv[0] == "exit") {
        run = false;
    } else if ((string)argv[0] == "cd") {
        string homedir = getuser()->pw_dir;
        
        for (int arg = 0; argv[arg] != nullptr; arg++) {
            string argstr = (string)argv[arg];

            for (int i = 0; i < (int)argstr.size(); i++){
                if (argstr[i] == '~') {
                    argstr.replace(i, 1, homedir);
                }
            }

            argv[arg] = strdup(argstr.c_str());
        }

        if (argv[1] == nullptr)
            argv[1] = strdup(homedir.c_str());

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
        argv[i] = new char[args[i].size() + 1];
        strcpy(argv[i], args[i].c_str());
    }
    argv[args.size()] = nullptr;

    return argv;
}

int main(int argc, char *argv[]) {
    chdir(getuser()->pw_dir);
    signal(SIGINT, SIG_IGN);

    putenv((char *)"SHELL=vish");

    for (int arg = 1; arg < argc; arg++) {
        if (!strcmp(argv[arg], "--version")) {
            cout << "VISH, version 1.0.0" << endl;
            return 0;
        }
    }

    string command;
    char **args;

    while (run) {
        command = prompt(genpromptstr(propmtFormat));
        
        if (command.length() <= 0)
            continue;

        args = parsecom(command);

        if (!specialcom(args)) {
            run = false;

            pid_t pid = fork();
            
            if (pid < 0) {
                return 1;
            } else if (pid == 0) {
                signal(SIGINT, handleTerm);
                exit(
                    execvp(args[0], args)
                );
            } else {
                int childStatus;
                waitpid(pid, &childStatus, 0);
                if (childStatus == 65280)
                    cerr << "vish: Unknown command: " << args[0] << endl;
                run = true;
            }
        }

        command.clear();
        free(args);
    }

    return 0;
}