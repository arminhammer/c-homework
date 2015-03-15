#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

#define NELEMS(x) ((sizeof (x))/(sizeof ((x)[0])))

// attack target
char *prog = "./ihttpd";

int main(int argc, char *argv[]) {
	int i;
	int retval;

	char cmd[MAX];

	char *args[] = {"arg",
		"a longer argument line for testing",
		"nice crash",
		"-",
		"%s",
		// Caution: in "system" call, command is interpreted by the shell.
		"spooky ; ls /",
		"20f",
		"e34",
		"-help",
		"-target 4",
		"31",
		"200",
		"--t 4",
		"-t 45",
		"-d kjkfsdf",
		"-n fgkd",
		"fksdjfdslkfjsdklfjdsklfjdsklfjdslkfjdsklfjdsklfjdslkfjdsklfjdslkfjdslkfjdslfkjsdklfjsdfldsjfdsjfdslkfjsdklfj",
		"-r",
		"-r /var/www",
		"-r /var/wwww",
		"-r /var/www/",
		"-rootdir /var/www",
		"-rootdir /var/wwww",
		"-rootdir /var/www/",
		"-m",
		"-m /var/www",
		"-m /var/wwww",
		"-m /var/www/",
		"-moddir /var/www",
		"-moddir /var/wwww",
		"-moddir /var/www/",
		"-p",
		"-p 203392302920329",
		//"-p 32302.12032",
		"-p assaldkj",
		"-port",
		"-port 203392302920329",
		"-port 32302.12032",
		"-port assaldkj",
		"-h",
		"--help",
		"-rootdir /var/www/ -m /home/mod/",
		"-c /etc/config.cfg",
		"--config /etc/config.cfg",
		"-c",
		"--config",
	};


for ( i = 0; i < 		NELEMS(args);
	i++ ) {
		// construct command to be executed
		snprintf(cmd, sizeof(cmd), "%s %s", prog, args[i]);

		fprintf(stdout, "\nExecuting: %s\n\n", cmd);
		/*
		 * DO NOT use "system" when input is not trusted. It can be disastrous.
		 * Use exec family of functions, NOT INCLUDING execlp and execvp.
		 */
		retval = system(cmd);

		fprintf(stdout, "\n\nReturn value: %d (%s)\n", retval, strerror(retval));
	}

	return 0;
}
