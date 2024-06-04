#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 6002
#define PACKET_SIZE 368

typedef struct {
    uint16_t packet_id;
    uint16_t size;
    uint16_t option;
    float data[10];
} Packet;

void generate_random_data(float* data, int size) {
    for (int i = 0; i < size; i++) {
        data[i] = (float)rand() / RAND_MAX * 100.0;
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    Packet packet;
    int packet_id = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    srand(time(NULL));

    while (1) {
        packet.packet_id = packet_id++;
        packet.size = PACKET_SIZE;
        packet.option = rand() % 5;
        generate_random_data(packet.data, 10);

        if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Send failed");
        } else {
            printf("Packet sent: ID=%d, Option=%d\n", packet.packet_id, packet.option);
        }

        sleep(1);
    }

    close(sockfd);
    return 0;
}

