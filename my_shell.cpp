#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>



int launch(char** args) 
{
	pid_t child, wpid;

	child = fork();
	
	if (child < 0) { // Error forking
    		perror("fork:");
	}
	
	if (child == 0) { // Child process
		if (execvp(args[0], args) == -1) {
    			perror("Child process:");
    		}
    		exit(EXIT_FAILURE);
  	} 
  	if (child > 0) {
	    	int status = -1;
	    	wpid = waitpid(child, &status, 0);
	}

	return 1;
}

int execute(char** args) {
  
	if (args[0] == NULL) {
		return 1;
	}

	return launch(args); // return 1 if the shell should continue running, 0 if it should terminate  
}


char* read_line() { // Read a line of input from stdin
  
  char* line = NULL;
  // getline will allocate memory itself
  size_t bufsize = 0; 
  getline(&line, &bufsize, stdin);
  return line;
  
}

#define BUFSIZE 64
#define DELIM " \t\r\n\a"

char** split_line(char* line){ // Split a line into tokens
	int buffersize = BUFSIZE;
	char** tokens = new char*[buffersize];
	char* token;
	int position = 0;
	
	if (!tokens) {
    		exit(EXIT_FAILURE);
    	}
	
	token = std::strtok(line, DELIM);

	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= buffersize) {
			buffersize += BUFSIZE;
			if(std::realloc(tokens, buffersize * 2) == nullptr){
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, DELIM);
  	}
	tokens[position] = NULL;
	return tokens;
}


int main(int argc, char **argv) {
	
	char* line;
	char** args;
	int status;
  
	
	while (status){
    		
    		std::cout << "myShell>"; 	
    		
		line = read_line();
		args = split_line(line);
		status = execute(args);
		
		delete line;
		delete args;
	}
 
	return 0;
}
