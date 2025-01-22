#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
int main(int argc, char *argv[]){
struct addrinfo *p, *res, hints;
int status;
char ipstr[INET6_ADDRSTRLEN];
memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;
if(argc != 2){
fprintf(stderr, "usage: network hostname\n");
return 2;
}
if((status=getaddrinfo(argv[1], NULL, &hints, &res)) != 0){
fprintf(stderr, "Failed to find ip addresses\n");
return 1;
}
printf("The IP addresses are:\n");
for(p=res;p!=NULL;p=p->ai_next){
void *addr;
char *ipver;
if(p->ai_family == AF_INET){
struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr; // casting p->ai_addr which is a struct sockaddr into a struct sockaddr_in 
addr = &(ipv4->sin_addr);
ipver = "IPV4";
}
else{
struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
addr = &(ipv6->sin6_addr);
ipver = "IPV6";

}
//convert the ip address to a string
inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
printf("%s: %s\n", ipver, ipstr);
}
freeaddrinfo(res); //free the linked list
return 0;
}
