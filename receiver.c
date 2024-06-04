#include "receiver.h"
#include "common.h"

void* receiver_thread(void* arg) {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    Packet packet;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(RECEIVER_PORT);

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (!shutdown_flag) {
        if (recvfrom(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&client_addr, &addr_len) < 0) {
            perror("Receive failed");
            continue;
        }

        pthread_mutex_lock(&receiver_array.mutex);
        if (receiver_array.count < receiver_array.capacity) {
            receiver_array.packets[receiver_array.count++] = packet;
            pthread_cond_signal(&receiver_array.cond);
        } else if (receiver_array.count == receiver_array.capacity && receiver_array.capacity < MAX_PACKETS) {
            receiver_array.capacity *= 2;
            receiver_array.packets = realloc(receiver_array.packets, receiver_array.capacity * sizeof(Packet));
            receiver_array.packets[receiver_array.count++] = packet;
            pthread_cond_signal(&receiver_array.cond);
        }
        pthread_mutex_unlock(&receiver_array.mutex);
    }

    close(sockfd);
    pthread_exit(NULL);
}
