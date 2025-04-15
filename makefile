CC= gcc
all:server client show_ip 
%:%.c
	$(CC) $^ -o $@


