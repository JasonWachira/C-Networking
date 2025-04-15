CC= gcc
all:server client show_ip 
%:%.c
	$(CC) $^ -o $@

clean:
	rm -rf -f server client show_ip


