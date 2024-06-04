#include "common.h"
#include "receiver.h"
#include "processor.h"
#include "responder.h"

PacketArray receiver_array = {NULL, 0, 10, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
ResultArray result_array = {NULL, 0, 10, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
volatile int pause_processors = 0;
volatile int shutdown_flag = 0;

int main() {
    pthread_t receiver_tid, processor_tid1, processor_tid2, responder_tid;

    receiver_array.packets = malloc(receiver_array.capacity * sizeof(Packet));
    result_array.results = malloc(result_array.capacity * sizeof(Result));

    pthread_create(&receiver_tid, NULL, receiver_thread, NULL);
    pthread_create(&processor_tid1, NULL, processor_thread, (void*)(intptr_t)1);
    pthread_create(&processor_tid2, NULL, processor_thread, (void*)(intptr_t)2);
    pthread_create(&responder_tid, NULL, responder_thread, NULL);

    pthread_join(receiver_tid, NULL);
    pthread_join(processor_tid1, NULL);
    pthread_join(processor_tid2, NULL);
    pthread_join(responder_tid, NULL);

    free(receiver_array.packets);
    free(result_array.results);
    return 0;
}
