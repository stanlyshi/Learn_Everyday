#!/bin/bash

gcc common.c debug.c  sctp_auth.c sctp_events.c  -lsctp sctp_client.c -o client.out
gcc common.c debug.c  sctp_auth.c sctp_events.c  -lsctp sctp_server.c -o server.out
