#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <linux/rtnetlink.h>
#include <linux/veth.h>
#include <net/if.h>
#include <stdarg.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#define MIN_PIDS                10
#define MAX_PIDS                32768
#define BUFF_SIZE               300
#define MIN_WEIGHT              10
#define MAX_WEIGHT              1000
#define MAX_BUF_SIZE            100
#define MAX_CPU_SHARES          100
#define COMAND_MAX_SIZE         200
#define MAX_MEMORY_ALLOCABLE    4294967296
#define CPU_SHARES_CHUNK_SIZE   0x400 
#ifndef ISOLATE_NETNS_H
#define ISOLATE_NETNS_H

#define DEBUG                   0     // used to enamble debug prints

#define printErr(msg)   do { \
                            fprintf(stdout, "[ERROR] - %s failed (errno: %d):" \
                            "%s\n", msg, errno, strerror(errno)); \
                            exit(EXIT_FAILURE); \
                           } while (0)

#define debug_print(msg) \
	do { if (DEBUG) fprintf(stdout, "[DEBUG] - %s", msg); } while (0)

#define MAX_PAYLOAD 1024

struct nl_req {
    struct nlmsghdr n;
    struct ifinfomsg i;
    char buf[MAX_PAYLOAD];
};

struct _rule {
	char *table;
	char *entry;
	char *type;
	char *iface;
	char *oface;
	char *saddr;
	char *daddr;
};

struct _addr_t {
	unsigned int addr;
	unsigned int mask;
};

#define NLMSG_TAIL(nmsg) \
	((struct rtattr *) (((void *) (nmsg)) + NLMSG_ALIGN((nmsg)->nlmsg_len)))

int create_socket(int domain, int type, int protocol);
void if_up(char *ifname, char *ip, char *netmask);
void create_veth(int sock_fd, char *ifname, char *peername);
void move_if_to_pid_netns(int sock_fd, char *ifname, int netns);
int get_netns_fd(int pid);
void get_child_entrypoint(int optind, char **arguments, size_t size,
    char ***child_entrypoint);

int drop_root_privileges(void);



#define NLMSG_STRING(nl, attr, data) \
	_nlmsg_put((nl), (attr), (data), (strlen((data)) + 1))
#define NLMSG_ATTR(nl, attr) \
	_nlmsg_put((nl), (attr), (NULL), (0))

int _nl_socket_init(void);
int _nlmsg_recieve(int fd);
int _nlmsg_send(int fd, struct nlmsghdr *nlmsg);
void _nlmsg_put(struct nlmsghdr *nlmsg, int type, void *data, size_t len);
int _ipt_rule(struct _rule *rule);
struct _addr_t *  _init_addr(const char *ip);
void _free_addr(struct _addr_t *addr);

#endif //ISOLATE_NETNS_H
