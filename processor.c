#include "processor.h"
#include "common.h"

void* processor_thread(void* arg) {
    int thread_id = (intptr_t)arg;
    Packet packet;
    Result result;

    while (!shutdown_flag) {
        pthread_mutex_lock(&receiver_array.mutex);
        while (receiver_array.count == 0 && !shutdown_flag) {
            pthread_cond_wait(&receiver_array.cond, &receiver_array.mutex);
        }

        if (shutdown_flag) {
            pthread_mutex_unlock(&receiver_array.mutex);
            break;
        }

        packet = receiver_array.packets[--receiver_array.count];
        pthread_mutex_unlock(&receiver_array.mutex);

        if (pause_processors) {
            continue;
        }

        if ((thread_id == 1 && (packet.option == 0 || packet.option == 1 || packet.option == 2)) ||
            (thread_id == 2 && (packet.option == 0 || packet.option == 3 || packet.option == 4))) {
            process_packet(packet, &result);

            pthread_mutex_lock(&result_array.mutex);
            if (result_array.count < result_array.capacity) {
                result_array.results[result_array.count++] = result;
                pthread_cond_signal(&result_array.cond);
            } else if (result_array.count == result_array.capacity && result_array.capacity < MAX_PACKETS) {
                result_array.capacity *= 2;
                result_array.results = realloc(result_array.results, result_array.capacity * sizeof(Result));
                result_array.results[result_array.count++] = result;
                pthread_cond_signal(&result_array.cond);
            }
            pthread_mutex_unlock(&result_array.mutex);
        }
    }

    pthread_exit(NULL);
}

void process_packet(Packet packet, Result* result) {
    result->packet_id = packet.packet_id;
    result->size = sizeof(Result);
    for (int i = 0; i < 10; i++) {
        switch (packet.option) {
            case 0:
                result->data[i] = packet.data[i];
                break;
            case 1:
                result->data[i] = packet.data[i] + 1;
                break;
            case 2:
                result->data[i] = packet.data[i] - 1;
                break;
            case 3:
                result->data[i] = packet.data[i] * 3;
                break;
            case 4:
                result->data[i] = packet.data[i] * packet.data[i];
                break;
            default:
                result->data[i] = packet.data[i];
        }
    }
}
