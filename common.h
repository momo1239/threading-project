#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#define RECEIVER_PORT 6002
#define RESPONDER_PORT 6003
#define MAX_PACKETS 100

typedef struct {
    uint16_t packet_id;
    uint16_t size;
    uint16_t option;
    float data[10];
} Packet;

typedef struct {
    uint16_t packet_id;
    uint16_t size;
    double data[10];
} Result;

typedef struct {
    Packet *packets;
    int count;
    int capacity;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} PacketArray;

typedef struct {
    Result *results;
    int count;
    int capacity;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} ResultArray;

extern PacketArray receiver_array;
extern ResultArray result_array;
extern volatile int pause_processors;
extern volatile int shutdown_flag;

void process_packet(Packet packet, Result* result);

enum Command {
    CMD_LIST = 0x1,
    CMD_QUERY = 0x2,
    CMD_STATUS = 0x3,
    CMD_PAUSE = 0x4,
    CMD_SHUTDOWN = 0x5
};

#endif
