/*
 * Copyright (C) 2015 Michael Frey
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       blafasel 
 *
 * @author      Michael Frey <mail@mfrey.net>
 *
 * @}
 */

#include <stdio.h>

#include "msg.h"
#include "net/mqttsn/mqttsn.h"
#include "net/mqttsn/commands.h"

#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6/netif.h"

#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIMITER   " \t\r\n\a"

kernel_pid_t pid;


ipv6_addr_t* get_address(void)
{
    /** ipv6 address to use */
    ipv6_addr_t *result;
    /** registered network interfaces */
    kernel_pid_t interfaces[GNRC_NETIF_NUMOF];
    /* get the number of addresses stored in the IPv6 interfaces */
    size_t number_of_addresses = gnrc_netif_get(interfaces);

    if (number_of_addresses > 0) {
        /** always take the first interface */
        gnrc_ipv6_netif_t *entry = gnrc_ipv6_netif_get(interfaces[0]);
        for (int i = 0; i < GNRC_IPV6_NETIF_ADDR_NUMOF; i++) {
            if ((ipv6_addr_is_link_local(&entry->addrs[i].addr)) && !(entry->addrs[i].flags & GNRC_IPV6_NETIF_ADDR_FLAGS_NON_UNICAST)) {
                result = &entry->addrs[i].addr;
            }
        }
    }

    return result;
}

/**
 *
 */
char* mqttsn_shell_read_line(void)
{
    char *line = NULL;
    size_t size = 0; 
    getline(&line, &size, stdin);
    return line;
}

/**
 *
 */
char** mqttsn_shell_split_line(char *line)
{
    uint8_t size = TOKEN_BUFFER_SIZE, position = 0;
    char **tokens = malloc(size * sizeof(char));
    char *token;

    if (!tokens) {
       	printf("meh");
       	exit(-1);
    }

    token = strtok(line, TOKEN_DELIMITER);
    
    while(token != NULL) {
	tokens[position] = token;
	position++;

	if (position >= size) {
	    size += TOKEN_BUFFER_SIZE;
	    tokens = realloc(tokens, size * sizeof(char));

	    if (!tokens) {
		printf("meh");
		exit(-1);
	    }
	}

	token = strtok(NULL, TOKEN_DELIMITER);
    }

    tokens[position] = NULL;
    return tokens;
}

/**
 *
 */
uint8_t mqttsn_shell_execute(char **arguments)
{
    if (arguments[0] == NULL) {
	return 0;
    }

    if (strcmp("quit", arguments[0]) == 0) {
	return 0;
    }

    msg_t message;
    /** check if it is a 'set' command */
    if (strncmp("set", arguments[0], 3) == 0) {
	if (arguments[1] != NULL) {
	    /** check if it is a 'set will' command */
	    if (strncmp("will", arguments[1], 4) == 0) {
		if ((arguments[2] && arguments[3]) != NULL) {
		    if (strncmp("message", arguments[2], 7) == 0) { 
		        printf("set will was called\n");
			printf("%s\n", arguments[3]);
		       	message.type = MQTTSN_CMD_SET_WILL_MSG;

		    }else if (strncmp("topic", arguments[2], 5) == 0) {
		        message.type = MQTTSN_CMD_SET_WILL_TOPIC;

		    } else {

		    }
		}
	    }
	}
	return 1;
    } else if (strncmp("dump", arguments[0], 4) == 0) {
	message.type = MQTTSN_CMD_SET_WILL_TOPIC;

    } else if (strncmp("start", arguments[0], 5) == 0) {
       	ipv6_addr_t *source = get_address();
       	pid = mqttsn_init(*source, 1883, false, 0, NULL, 0);
	printf("mqtt-sn pid is %d\n", pid);
	fflush(stdout);
	return 1;
    }

    msg_send_receive(&message, &message, pid);
    return 1;
}

void mqttsn_shell(void) 
{
    char *line;
    char **arguments;
    uint8_t status;

    do {
	printf("> ");
	line = mqttsn_shell_read_line();
	arguments = mqttsn_shell_split_line(line);
	status = mqttsn_shell_execute(arguments);
    } while (status);
}

int main(void)
{
    puts("mqtt-sn example");
/*
    ipv6_addr_t *source = get_address();

    msg_t m;
    m.type = MQTTSN_CMD_STATUS;
    kernel_pid_t pid = mqttsn_init(*source, 1883, false, 0, NULL, 0);
    msg_send_receive(&m, &m, pid);  
*/
    mqttsn_shell();
    /* main thread exits */
    return 0;
}
