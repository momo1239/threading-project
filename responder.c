#include "responder.h"
#include "common.h"

void handle_query(int sockfd, struct sockaddr_in* client_addr, uint16_t packet_id);
void handle_status(int sockfd, struct sockaddr_in* client_addr);
void handle_list(int sockfd, struct sockaddr_in* client_addr);
void handle_pause(int sockfd, struct sockaddr_in* client_addr);
void handle_shutdown(int sockfd, struct sockaddr_in* client_addr);

void* responder_thread(void* arg) {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[256];
    int n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(RESPONDER_PORT);

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (!shutdown_flag) {
        n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        if (n < 0) {
            perror("Receive failed");
            continue;
        }
        buffer[n] = '\0';

        if (strncmp(buffer, "query", 5) == 0) {
            uint16_t packet_id;
            sscanf(buffer + 6, "%hu", &packet_id);
            handle_query(sockfd, &client_addr, packet_id);
        } else if (strcmp(buffer, "status") == 0) {
            handle_status(sockfd, &client_addr);
        } else if (strcmp(buffer, "list") == 0) {
            handle_list(sockfd, &client_addr);
        } else if (strcmp(buffer, "pause") == 0) {
            handle_pause(sockfd, &client_addr);
        } else if (strcmp(buffer, "shutdown") == 0) {
            handle_shutdown(sockfd, &client_addr);
        }
    }

    close(sockfd);
    pthread_exit(NULL);
}

void handle_query(int sockfd, struct sockaddr_in* client_addr, uint16_t packet_id) {
    char response[1024];
    int found = 0;

    pthread_mutex_lock(&result_array.mutex);
    for (int i = 0; i < result_array.count; i++) {
        if (result_array.results[i].packet_id == packet_id) {
            snprintf(response, sizeof(response), "Packet ID: %hu, Size: %hu, Data: [", result_array.results[i].packet_id, result_array.results[i].size);
            for (int j = 0; j < 10; j++) {
                char data_str[32];
                snprintf(data_str, sizeof(data_str), "%lf", result_array.results[i].data[j]);
                strcat(response, data_str);
                if (j < 9) strcat(response, ", ");
            }
            strcat(response, "]");
            found = 1;
            break;
        }
    }
    pthread_mutex_unlock(&result_array.mutex);

    if (!found) {
        snprintf(response, sizeof(response), "Packet ID %hu not found", packet_id);
    }

    sendto(sockfd, response, strlen(response), 0, (struct sockaddr*)client_addr, sizeof(*client_addr));
}

void handle_status(int sockfd, struct sockaddr_in* client_addr) {
    char response[64];
    snprintf(response, sizeof(response), "Receiver array: %s, Result array: %s", receiver_array.count == receiver_array.capacity ? "Full" : "Not Full", result_array.count == result_array.capacity ? "Full" : "Not Full");
    sendto(sockfd, response, strlen(response), 0, (struct sockaddr*)client_addr, sizeof(*client_addr));
}

void handle_list(int sockfd, struct sockaddr_in* client_addr) {
    char response[1024] = "Packet IDs: [";
    char id_str[16];

    pthread_mutex_lock(&result_array.mutex);
    for (int i = 0; i < result_array.count; i++) {
        snprintf(id_str, sizeof(id_str), "%hu", result_array.results[i].packet_id);
        strcat(response, id_str);
        if (i < result_array.count - 1) strcat(response, ", ");
    }
    pthread_mutex_unlock(&result_array.mutex);
    strcat(response, "]");

    sendto(sockfd, response, strlen(response), 0, (struct sockaddr*)client_addr, sizeof(*client_addr));
}

void handle_pause(int sockfd, struct sockaddr_in* client_addr) {
    pause_processors = !pause_processors;
    char response[32];
    snprintf(response, sizeof(response), "Processor threads %s", pause_processors ? "paused" : "resumed");
    sendto(sockfd, response, strlen(response), 0, (struct sockaddr*)client_addr, sizeof(*client_addr));
}

void handle_shutdown(int sockfd, struct sockaddr_in* client_addr) {
    shutdown_flag = 1;
    pthread_cond_broadcast(&receiver_array.cond);
    pthread_cond_broadcast(&result_array.cond);
    char response[32];
    snprintf(response, sizeof(response), "Shutdown initiated");
    sendto(sockfd, response, strlen(response), 0, (struct sockaddr*)client_addr, sizeof(*client_addr));
}
