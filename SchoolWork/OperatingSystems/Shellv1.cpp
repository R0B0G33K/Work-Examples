#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sstream>
#include <cstddef>

using namespace std;


int commandCheck(string command, string args[], string fullstring) {

	if (command.compare(args[0]) == 0)
	{
		return 1;
	}
	else if (command.compare(args[1]) == 0)
	{
		std::string echo = fullstring.substr(fullstring.find(" ") + 1, fullstring.find('\0'));
		cout << echo << "\n";
		return 2;
	}
	else if (command.compare(args[2]) == 0)
	{
		return 3;
	}
	else if (command.compare(args[3]) == 0)
	{
		std::string help = fullstring.substr(fullstring.find(" ") + 1, fullstring.find('\0'));
		if (help.compare(args[4]) == 0){ cout << "The SET command will change the chosen variable to whatever the user sets. \n"; }
		else if (help.compare(args[0]) == 0) { cout << "The CD command will change the directory. \n"; }
		else if (help.compare(args[1]) == 0) { cout << "The ECHO command will print the rest of the command entered. \n"; }
		else if (help.compare(args[2]) == 0) { cout << "The EXIT command will exit the shell. \n"; }
		else { cout << "Please enter a command with help! \n"; }
		return 4;
	}
	else if (command.compare(args[4]) == 0)
	{
		return 5;
	}
	else
	{
		return 6;
	}
	return 0;
}


int main() {
	char input[1024];
	string args[5] = { "cd", "echo", "exit", "help", "set" };
	string prompt = "shell>: ";
	string fullstring;
	int Check = 0;

	while (Check != 3)
	{
		cout << prompt;
		cin.getline(input, 1024, '\n');
		std::string fullstring = input;
		std::string command = fullstring.substr(0, fullstring.find(" "));


		Check = commandCheck(command, args, fullstring);
		
		if (Check == 5) {
			std::string set = fullstring.substr(fullstring.find(" ") + 1, fullstring.find('\0'));
			std::string var = set.substr(0, set.find(" "));
			std::string change = set.substr(set.find(" ")+1, set.find('\0'));
			if (var.compare("PROMPT") == 0) {
				prompt = change;
			}
		}
		if (Check == 6) {
			pid_t parent = getpid();
			pid_t pid = fork();

			if (pid == -1) {
				/*error*/
			}
			else if (pid > 0) {
				int status;
				waitpid(pid, &status, 0);
			}
			else {
				int children = execvp(input, NULL);
				if (children < 0) {
					/*execution failed*/
				}
			}
		}
	}

	cout << "Good Bye! \n";

	return 0;
}