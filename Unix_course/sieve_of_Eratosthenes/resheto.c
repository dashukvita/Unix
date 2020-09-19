#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>

typedef struct node {
    unsigned long value;
    struct node *next;
} node_t;

typedef struct list {
    struct node *begin;
    struct node *end;
    unsigned long count;
} list_t;

struct arg_struct {
    unsigned long start;
    unsigned long end;
    list_t *threads_mas;
};


unsigned long num_threads;
unsigned long max_prime;
list_t *main_list;

int push(list_t *list, unsigned long value) {
    node_t *temp = (struct node *) malloc(sizeof(node_t));
    
    temp->value = value;
    temp->next = NULL;
    if (!list->count){
        list->begin = list->end = temp;
    } else {
        list->end->next = temp;
        list->end = temp;
    }
    list->count++;
    
    return 0;
}

void add_main_list(list_t *from, list_t *to) {
    node_t *current = from->begin;
    
    while (current != NULL) {
        push(to, current->value);
        current = current->next;
    }
}

int find_max_prime_num_threads(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        printf("Неверные входные параметры!\n");
        exit(1);
    }
    else if (argc == 2){
        max_prime = 1000000;
    }
    else
        max_prime = atoi(argv[2]);
    
    if (max_prime < 2){
        printf("Введен неверный параметр для поиска простого числа!\n");
        exit(1);
    }
    num_threads = atoi(argv[1]);
    if (num_threads < 1) {
        printf("Введено неверное число нитей!\n");
        exit(1);
    }
    return 0;
}

void *search_simple_numbers(void *params) {
    struct arg_struct *args = (struct arg_struct *) params;
    unsigned long from = args->start;
    unsigned long to = args->end;
    
    for(unsigned long i = from; i <= to; i++) {
        node_t *current = main_list->begin;
        
        int prime = 1;
        while (current != NULL) {
            if (i % current->value == 0) {
                prime = 0;
            }
            current = current->next;
        }
        if (prime) {
            push(args->threads_mas, i);
        }
    }
    
    pthread_exit(0);
}

int create_threads(unsigned long  begin_num, unsigned long end_num) {
    int thstart_result;
    pthread_t th_ids[num_threads];
    pthread_attr_t attrs[num_threads];
    struct arg_struct params[num_threads];
    
    list_t *th_mas[num_threads];
    for (int j = 0; j < num_threads; ++j) {
        th_mas[j] = malloc(sizeof(list_t));
        th_mas[j]->count = 0;
    }
    
    unsigned long c = (end_num - begin_num) / num_threads;
    for (int i = 0; i < num_threads; i++) {
        unsigned long start = i * c + begin_num + 1;
        unsigned long end = start + c - 1;
        
        params[i].start = start;
        if (i + 1 == num_threads) {
            params[i].end = end_num;
        } else {
            params[i].end = end;
        }
        params[i].threads_mas = th_mas[i];
        
        pthread_attr_init(&attrs[i]);
        thstart_result = pthread_create(&th_ids[i], &attrs[i], search_simple_numbers, (void *) &params[i]);
        if (thstart_result != 0) {
            printf ("Ошибка при создании нити!\n");
            exit(1);
        }
    }
    
    for (unsigned long i = 0; i < num_threads; i++) {
        pthread_join(th_ids[i], NULL);
    }
    
    for (unsigned long n = 0; n < num_threads; ++n) {
        add_main_list(th_mas[n], main_list);
    }
    
    return 0;
}

int block_resheto(){
    unsigned long begin_num = 10;
    unsigned long square = begin_num * begin_num;
    unsigned long end_num = (square > max_prime) ? max_prime : square;
    
    int continue_prog = 1;
    while (continue_prog) {
        if (create_threads(begin_num, end_num) != 0) {
            printf("Ошибка при добавлении простого числа!\n");
            exit(1);
        }
        
        if (end_num == max_prime) {
            continue_prog = 0;
        }
        begin_num = end_num;
        square = begin_num * begin_num;
        end_num = (square > max_prime) ? max_prime : square;
    }
    
    return 0;
}

void add_simple_numbers() {
    main_list = malloc(sizeof(list_t));
    main_list->count = 0;
    
    push(main_list, 2);
    push(main_list, 3);
    push(main_list, 5);
    push(main_list, 7);
}

int print_list(list_t *list) {
    node_t *current = list->begin;
    
    while (current != NULL) {
        printf("%lu ", current->value);
        current = current->next;
    }
    
    return 0;
}

int main(int argc, char *argv[]) {
    find_max_prime_num_threads(argc, argv);
    add_simple_numbers();
    printf("%lu", max_prime);
    if (max_prime > 10){
        block_resheto();
    }
    print_list(main_list);
}

