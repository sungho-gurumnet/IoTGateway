#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void _strncpy(char* dest, char* src, int len) {
	memmove(dest, src, len);
	dest[len] = '\0';
}

void remove_blank(char* str) {
	char* start = str;
	char* end = str + strlen(str);

	//cut head
	for(; start < end; start++) {
		if(*start != ' ' && *start != '"') {
			break;
		}
	}

	//cut tail
	for(; end > start; end--) {
		if(*(end - 1) != ' ' && *(end - 1) != '"') {
			break;
		}
	}

	_strncpy(str, start, end - start);
}	

#ifdef TIMER_LOG
#include <stdint.h>
#include <time.h>
static int fd = 0;
static int current_id = -1;
static char buf[128];

void timer_check(char* id) {
	char* next;
	char* prefix = strtok_r(id, ".", &next);
	if(!strcmp(prefix, "DMSLAB")) {
		int recv_id = strtol(next, NULL, 10);
		if(current_id != recv_id || !fd) {
			if(fd) {
				close(fd);
				fd = 0;
			}
			char name[32];
			sprintf(name, "timer_log%d.dat", recv_id);
			fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			current_id = recv_id;
		}
	}
}

void timer_end(uint64_t pre_us, uint64_t current_us) {
	if(fd) {
		sprintf(buf, "us:\t%ld\tmicro second\n", (current_us - pre_us));
		if(write(fd, buf, strlen(buf) + 1) < 0)
			printf("File Write error\n");
		printf("%s\n", buf);
	}
}
#endif
