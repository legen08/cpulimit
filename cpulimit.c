#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>
#include<signal.h>

void signal_handler(int x) {
	exit(0);
}

struct sigaction sig_act = {
	.sa_handler = signal_handler,
	.sa_flags = SA_NOCLDSTOP
};

void cpulimit(char* argv[]) {
	int pid = fork();
	if (pid == -1) {
		perror("fork error");
		return;
	} else {
		if (!pid) {
			if(execvp(argv[2], argv + 2) == -1) {
				perror("execvp error");
				return;
			}
		} else {
			sigaction(SIGCHLD, &sig_act, NULL);
			int sleep_time = atoi(argv[1]) * 10000;
			for(;;) {
				usleep(sleep_time);
				kill(pid, SIGSTOP);
				usleep(100000 - sleep_time);
				kill(pid, SIGCONT);
			}
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Недостаточно аргументов");
		return 1;
	}
	cpulimit(argv);
	return 0;
}
