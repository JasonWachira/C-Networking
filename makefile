CC= gcc
all:server client show_ip 
%:%.c
	$(CC) $^ -o $@

clean:
	rm  -f server client show_ip


