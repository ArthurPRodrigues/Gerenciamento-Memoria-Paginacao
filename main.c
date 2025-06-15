#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define PHYSICAL_MEM_SIZE 1024
#define PAGE_SIZE 32
#define MAX_PROCESS_SIZE 256

// Estrutura do quadro de memória física
typedef struct {
    int process_id; //Processo utilizando essa memória física, -1 se vazio
    int page_num; // Número da página, -1 se vazio
    unsigned char data[PAGE_SIZE]; //Conteúdo
    bool is_free;
} Frame;

// Estrutura da linha da tabela de páginas
typedef struct {
    int frame_num;
} PageTableEntry;

// Estrutura dos processos
typedef struct {
    int process_id;
    int size;
    int page_count;
    PageTableEntry *page_table;
} Process;

// Gerenciador de memória vazia (Lista encadeada)
typedef struct {
    int frame_num;
    struct FreeFrameNode *next // próximo nó da lista
} FreeFrameNode;

// Variáveis globais
Frame physical_memory [PHYSICAL_MEM_SIZE / PAGE_SIZE]; //cria um vetor chamado memória física
FreeFrameNode *free_frames_list = NULL; //cria uma lista encadeada ainda vazia
Process *processes[100]; //cria um array de 100 campos para processos
int process_count = 0; //contador de processos

// Inicializa memória e lista de quadros vazios
void initialize_memory() {
    int total_frames = PHYSICAL_MEM_SIZE / PAGE_SIZE;

    //Criando quadros vazios
    for ( int i=0; i< total_frames ; i++) {
        physical_memory[i].is_free = true;
        physical_memory[i].process_id = -1;
        physical_memory[i].page_num = -1;
        memset(physical_memory[i].data, 0, PAGE_SIZE); //o conteúdo dos blocos do quadro ficam algo como [0, 0, 0, 0, 0]
    

        //Colocando esse quadros na lista de frames vazios
        FreeFrameNode *new_node = (FreeFrameNode*)malloc(sizeof(FreeFrameNode)); // Reserva tal quantidade de bytes no heap (alocação dinâmica)
        new_node->frame_num = i; //"->" é equivalente a um ".", mas dentro de uma struct
        new_node->next = free_frames_list; //aponta para um outro nó da lista
        free_frames_list = new_node; // início = new_node que voce acabou de criar
    }


}

// Cria novo processo
void create_process(int process_id, int size) {

    // Checar se ID já existe
    for (int i = 0; i< process_count; i++) {
        if (processes[i]->process_id == process_id) {
            printf("Error: process id %d already exists\n", process_id);
            return;
        }
    }

    // Checar se tamanho do processo é menor ou igual do que tamanho máximo permitido
    if (size > MAX_PROCESS_SIZE) {
        printf("Error: process size %d exceeds maximum allowed size of %d bytes\n", size, MAX_PROCESS_SIZE);
        return;
    }

    // Calcular o número de páginas necessárias para esse processo
    int page_count = size / PAGE_SIZE;
    if (size % page_count != 0) page_count++; // parte de uma página precisa ficar em uma página a mais (não cheia) 

    // Checar se tem quadros disponíveis para essas páginas
    int free_frames_count = 0; //contador de quadros vazios
    FreeFrameNode *current = free_frames_list;
    while (current != NULL) {
        free_frames_count++;
        current = current->next;
    }

    if (free_frames_count = 0) {
        printf("Error: not enough memory to allocate process.\nRequired: %d frames.\nAvailable: %d frames", page_count, free_frames_count);
        return;
    }

    // Create new process
}