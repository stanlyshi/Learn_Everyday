/**
 * References:
 *
 * http://www.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html
 * http://stackoverflow.com/questions/9778806/serializing-a-class-with-a-pointer-in-c
 * http://stackoverflow.com/questions/504810/how-do-i-find-the-current-machines-full-hostname-in-c-hostname-and-domain-info
 *
 * Coding Style:
 *
 * http://www.cs.swarthmore.edu/~newhall/unixhelp/c_codestyle.html
 */

#include "rpc_types.h"


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netdb.h>

// Database structure to store procedure properties
struct proc_map_db {
    const char *proc_name;
      int n_params;
    fp_type fp;
};

#define	PORT_RANGE_LO	10000
#define PORT_RANGE_HI	10100

// Database declaration
struct proc_map_db proc_db[100];
int proc_db_index = 0;

const int buffer_size = 512;

/*
 * mybind() -- a wrapper to bind that tries to bind() to a port in the
 * range PORT_RANGE_LO - PORT_RANGE_HI, inclusive.
 *
 * Parameters:
 *
 * sockfd -- the socket descriptor to which to bind
 *
 * addr -- a pointer to struct sockaddr_in. mybind() works for AF_INET sockets only.
 * Note that addr is and in-out parameter. That is, addr->sin_family and
 * addr->sin_addr are assumed to have been initialized correctly before the call.
 * Also, addr->sin_port must be 0, or the call returns with an error. Up on return,
 * addr->sin_port contains, in network byte order, the port to which the call bound
 * sockfd.
 *
 * returns int -- negative return means an error occurred, else the call succeeded.
 */
int mybind(int sockfd, struct sockaddr_in *addr) {
    if(sockfd < 1) {
	fprintf(stderr, "mybind(): sockfd has invalid value %d\n", sockfd);
	return -1;
    }

    if(addr == NULL) {
	fprintf(stderr, "mybind(): addr is NULL\n");
	return -1;
    }

    if(addr->sin_port != 0) {
	fprintf(stderr, "mybind(): addr->sin_port is non-zero. Perhaps you want bind() instead?\n");
	return -1;
    }

    unsigned short p;
    for(p = PORT_RANGE_LO; p <= PORT_RANGE_HI; p++) {
    	addr->sin_port = htons(p);
    	int b = bind(sockfd, (const struct sockaddr *)addr, sizeof(struct sockaddr_in));
    	if(b < 0) {
    	    continue;
    	}
    	else {
    	    break;
    	}
    }

    if(p > PORT_RANGE_HI) {
	fprintf(stderr, "mybind(): all bind() attempts failed. No port available...?\n");
	return -1;
    }

    /* Note: upon successful return, addr->sin_port contains, in network byte order, the
     * port to which we successfully bound. */
    return 0;
}

/**
 * Creates and a new socket instance and returns socket identifier.
 */
int create_socket(const int domain, const int type, const int protocol) {

    int socketDescriptor = socket(domain, type, protocol);

    if (socketDescriptor == -1) {
        perror("Failed to create socket.");
        exit(0);
    }

    return socketDescriptor;
}

/**
 * Binds a socket identifier to host address.
 */
void bind_socket(int socket) {

    struct sockaddr_in myAddress;

    memset((char *)&myAddress, 0, sizeof(myAddress));
    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    myAddress.sin_port = htons(0);
    // Assign port
    if(mybind(socket, (struct sockaddr_in*)&myAddress) < 0 ) {
        perror("Could't bind port to socket.");
        exit(0);
    }

    // Print port
    printf(" %d", ntohs(myAddress.sin_port));
    fflush(stdout);
}

/**
 * Registers a new procedure to the procedure database.
 */
bool register_procedure(const char *procedure_name, const int nparams, fp_type fnpointer) 
{

    int i = 0;
    while (i < proc_db_index){
        if( (strcmp( proc_db[i].proc_name, procedure_name) == 0)
            && (nparams == proc_db[i].n_params) ) {
            return false;
        }
        i++;
    }

    proc_db[proc_db_index].proc_name = procedure_name;
    proc_db[proc_db_index].n_params = nparams;
    proc_db[proc_db_index].fp = fnpointer;
    proc_db_index++;

    return true;
}

/**
 * Serializes int values into a character buffer.
 */
static unsigned char *int_serialize(unsigned char *buffer, int value) 
{

    int shift = 0;
    const int shift_eight = 8;
    const int shift_sixteen = 16;
    const int shift_twentyfour = 24;

    buffer[shift] = value >> shift;
    buffer[++shift] = value >> shift_eight;
    buffer[++shift] = value >> shift_sixteen;
    buffer[++shift] = value >> shift_twentyfour;

    return buffer + shift + 1;
}

/**
 * Deserializes buffer received from client.
 */
return_type deserialize(unsigned char * buffer){

    return_type ret;
    ret.return_val = NULL;
    ret.return_size = 0;

    // Gets size of procedure name
    int proc_size = *(int*)buffer;
    unsigned char *aliased_buff = buffer + sizeof(int);

    if (proc_size <= 1) {
        printf("\nProcedure size was zero. Ret not set.\n");
        return ret;
    }

    // Gets actual procedure name
    char recv_proc_name[proc_size];
    memset(recv_proc_name, 0, sizeof(recv_proc_name));
    char *dummyaliased_buff = (char*)aliased_buff;

    int i = 0;
    while (i < proc_size) {
        recv_proc_name[i] = dummyaliased_buff[i];
        i++;
    }

    aliased_buff += proc_size;

    // Gets number of parameters for procedure
    int recv_num_args = *(int *)aliased_buff;
    aliased_buff = aliased_buff + 4;

    // Find the procedure from the database
    fp_type fp;
    arg_type *arg_list;
    int proc_found = 0;

    i = 0;
    while (i < proc_db_index){
        if(strcmp(proc_db[i].proc_name, recv_proc_name) == 0 &&
                proc_db[i].n_params == recv_num_args){
            fp = proc_db[i].fp;
            proc_found = 1;
        }
        i++;
    }

    // Proceeds to get arguments if procedure was found in database
    if (proc_found == 1) {

        arg_type *head_node = (arg_type*)malloc(sizeof(arg_type));
        arg_type *curr_node = head_node;

        // Build linked list of arguments as we unpack from buffer
        i = 0;
        while (i < recv_num_args) {
            int recv_arg_size = *(int *)aliased_buff;
            aliased_buff += sizeof(int);

            unsigned char *recv_arg_val = (unsigned char*)malloc(sizeof(recv_arg_size));

            int j = 0;
            while (j < recv_arg_size) {
                recv_arg_val[j] = aliased_buff[j];
                j++;
            }

            arg_type *ptr = (arg_type*)malloc(sizeof(arg_type));
            ptr->next = NULL;

            curr_node->arg_size = recv_arg_size;
            curr_node->arg_val = (void*)recv_arg_val;

            curr_node->next = ptr;
            curr_node = ptr;

            aliased_buff += recv_arg_size;
            i++;
        }

        arg_list = head_node;

        // Call function ptr to compute and return result
        ret = fp(recv_num_args, arg_list);
    } else {
        printf("\nCould not find function in database. Ret not set.\n");
    }

    return ret;
}

/**
 * Main loop that keeps server running and processing incoming procedure calls.
 */
void launch_server() 
{

    struct sockaddr_in serv_addr;
    struct sockaddr_in remote_addr;
    socklen_t addr_len = sizeof(remote_addr);
    int socket;

    // Look up and print hostname
    struct addrinfo hints, *info, *p;
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    printf("%s", hostname);
    struct hostent* h;
    h = gethostbyname(hostname);

    // Creates a UDP socket
    socket = create_socket(AF_INET, SOCK_DGRAM, 0);
    bind_socket(socket);

    int received_size;
    unsigned char buffer[buffer_size];

    for (;;) {
        memset(buffer, 0, sizeof(buffer));

        // Populate buffer with data from client
        received_size = recvfrom(socket, (void *)buffer, buffer_size,
            0, (struct sockaddr *)&remote_addr, &addr_len);

        // If we recieved data from client, move onto deserializing it
        if (received_size > 0) {

            return_type ret;
            ret = deserialize(buffer);

            // Process and send result back to client
            unsigned char send_buf[512];
            unsigned char *tmp = int_serialize(send_buf, ret.return_size);
            unsigned char *castedarg = (unsigned char*)ret.return_val;

            int k = 0;
            while (k < ret.return_size) {
                tmp[k] = castedarg[k];
                k++;
            }

            sendto(socket, send_buf, buffer_size, 0, (struct sockaddr *)&remote_addr, addr_len);
        }
    }
}
