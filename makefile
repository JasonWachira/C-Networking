CC= gcc

all:show_ip

show_ip: show_ip.c
	$(CC) -o show_ip show_ip.c

