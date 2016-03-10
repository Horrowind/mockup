#ifndef BASE_QUEUE_H
#define BASE_QUEUE_H


#define queue_t(type)                                                   \
    struct {                                                            \
        type* data;                                                     \
        int head;                                                       \
        int tail;                                                       \
        int length;                                                     \
    }

#define generate_queue(name, type)                                      \
    typedef struct {                                                    \
        type* data;                                                     \
        int head;                                                       \
        int tail;                                                       \
        int length;                                                     \
    } name##_t;                                                         \
                                                                        \
    void name##_init(name##_t* queue) {                                 \
        queue->head = 0;                                                \
        queue->tail = 0;                                                \
        queue->length = 2;                                              \
        queue->data = malloc(sizeof(type)*queue->length);               \
        for(int k = 0; k < queue->length; k++) {                        \
            queue->data[k] = 0;                                         \
        }                                                               \
    }                                                                   \
                                                                        \
    void name##_deinit(name##_t* queue) {                               \
        queue->length = 0;                                              \
        free(queue->data);                                              \
    }                                                                   \
                                                                        \
    void name##_enqueue(name##_t* queue, type data) {                   \
        if(queue->head == (queue->tail - 1 + queue->length) % queue->length) { \
            queue->data[queue->head] = data;                            \
            type* new_data = malloc(sizeof(type)*2*queue->length);      \
            memset(new_data, 0, sizeof(type)*2*queue->length);          \
            for(int i = 0; i < queue->length; i++) {                    \
                new_data[i] = queue->data[(i + queue->tail) % queue->length]; \
            }                                                           \
                                                                        \
            queue->tail = 0;                                            \
            queue->data[queue->length - 1] = boundary;                  \
            queue->head = queue->length;                                \
            queue->length *= 2;                                         \
            free(queue->data);                                          \
            queue->data = new_data;                                     \
        } else {                                                        \
            queue->data[queue->head] = boundary;                        \
            queue->head = (queue->head + 1) % queue->length;            \
        }                                                               \
    }                                                                   \
                                                                        \
    type name##_dequeue(queue_t* queue) {                               \
        type result = queue->data[queue->tail];                         \
        queue->tail = (queue->tail + 1) % queue->length;                \
        return result;                                                  \
    }                                                                   \
                                                                        \
    b32 name##_is_empty(queue_t* queue) {                               \
        return queue->tail == queue->head;                              \
    }                                                                   \

#if 0
void queue_test() {
    srand(time(0));
    queue_t queue;
    queue_init(&queue);

    for(int i = 0; i < 100000;) {
        if((rand() % 3 != 0) && !queue_is_empty(&queue)) {
            printf("Huhu: %i, %i\n", (int)queue_dequeue(&queue), queue.length);
        } else {
            queue_enqueue(&queue, i++);
        }
    }
    getchar();
    while(!queue_is_empty(&queue)) {
        printf("Huhu: %i, %i\n", (int)queue_dequeue(&queue), queue.length);
    }
}

void queue_test2() {
    queue_t queue;
    queue_init(&queue);
    for(int j = 0; ; j++) {
        for(int i = 0; i < queue.length; i++) {
            printf("%lx ", queue.data[i]);
        }
        getchar();
        queue_enqueue(&queue, j);
    }
}
#endif

#endif //BASE_QUEUE_H
