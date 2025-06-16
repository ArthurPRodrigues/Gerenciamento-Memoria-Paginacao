#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define PHYSICAL_MEM_SIZE 1024
#define PAGE_SIZE 32
#define MAX_PROCESS_SIZE 256

void initialize_memory();
void create_process(int process_id, int size);
void print_page_table(int process_id);
void list_all_processes();
void show_physical_memory();

// Estrutura do quadro de memória física
typedef struct {
    int process_id;
    int page_num;
    unsigned char data[PAGE_SIZE];
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
typedef struct FreeFrameNode {
    int frame_num;
    struct FreeFrameNode *next;
} FreeFrameNode;

// Variáveis globais
Frame physical_memory[PHYSICAL_MEM_SIZE / PAGE_SIZE];
FreeFrameNode *free_frames_list = NULL;
Process *processes[100];
int process_count = 0;


void initialize_memory() {
    int total_frames = PHYSICAL_MEM_SIZE / PAGE_SIZE;
    for (int i = 0; i < total_frames; i++) {
        physical_memory[i].is_free = true;
        physical_memory[i].process_id = -1;
        physical_memory[i].page_num = -1;
        memset(physical_memory[i].data, 0, PAGE_SIZE);

        FreeFrameNode *new_node = (FreeFrameNode *)malloc(sizeof(FreeFrameNode));
        new_node->frame_num = i;
        new_node->next = free_frames_list;
        free_frames_list = new_node;
    }

}

void create_process(int process_id, int size) {

    for (int i = 0; i < process_count; i++) {
        if (processes[i]->process_id == process_id) {
            printf("Erro: processo %d já existe.\n", process_id);
            return;
        }
    }

    if (size > MAX_PROCESS_SIZE) {
        printf("Erro: tamanho do processo excede o máximo permitido.\n");
        return;
    }

    int page_count = size / PAGE_SIZE;
    if (size % PAGE_SIZE != 0) page_count++;

    int free_frames_count = 0;
    FreeFrameNode *current = free_frames_list;
    while (current != NULL) {
        free_frames_count++;
        current = current->next;
    }

    if (free_frames_count < page_count) {
        printf("Erro: memória insuficiente. Necessário: %d quadros. Disponível: %d.\n", page_count, free_frames_count);
        return;
    }

    Process *new_process = (Process *)malloc(sizeof(Process));
    new_process->process_id = process_id;
    new_process->size = size;
    new_process->page_count = page_count;
    new_process->page_table = (PageTableEntry *)malloc(sizeof(PageTableEntry) * page_count);

    for (int i = 0; i < page_count; i++) {
        int frame_num = free_frames_list->frame_num;
        FreeFrameNode *to_free = free_frames_list;
        free_frames_list = free_frames_list->next;
        free(to_free);

        physical_memory[frame_num].is_free = false;
        physical_memory[frame_num].process_id = process_id;
        physical_memory[frame_num].page_num = i;

        for (int j = 0; j < PAGE_SIZE; j++) {
            physical_memory[frame_num].data[j] = rand() % 256;
        }

        new_process->page_table[i].frame_num = frame_num;
    }

    processes[process_count++] = new_process;
    printf("Processo %d criado com sucesso (%d bytes, %d páginas).\n", process_id, size, page_count);
}

void print_page_table(int process_id) {
    for (int i = 0; i < process_count; i++) {
        if (processes[i]->process_id == process_id) {
            Process *p = processes[i];
            printf("\n===== Tabela de Páginas =====\n");
            printf("Processo %d\n", p->process_id);
            printf("Tamanho: %d bytes\n", p->size);
            printf("Páginas: %d\n", p->page_count);
            for (int j = 0; j < p->page_count; j++) {
                printf("Página %d → Quadro %d\n", j, p->page_table[j].frame_num);
            }
            return;
        }
    }
    printf("Erro: processo %d não encontrado.\n", process_id);
}

void list_all_processes() {
    if (process_count == 0) {
        printf("Nenhum processo criado.\n");
        return;
    }

    printf("\n===== Lista de Processos =====\n");
    for (int i = 0; i < process_count; i++) {
        Process *p = processes[i];
        printf("\nProcesso %d:\n", p->process_id);
        printf("Tamanho: %d bytes | Páginas: %d\n", p->size, p->page_count);
        for (int j = 0; j < p->page_count; j++) {
            printf("  Página %d → Quadro %d\n", j, p->page_table[j].frame_num);
        }
    }
}

void show_physical_memory() {
    int total_frames = PHYSICAL_MEM_SIZE / PAGE_SIZE;
    printf("\n===== ESTADO DA MEMÓRIA FÍSICA =====\n");
    for (int i = 0; i < total_frames; i++) {
        if (physical_memory[i].is_free) {
            printf("Quadro %02d: LIVRE\n", i);
        } else {
            printf("Quadro %02d: OCUPADO | Processo %d | Página %d\n",
                   i,
                   physical_memory[i].process_id,
                   physical_memory[i].page_num);
        }
    }
}

// Função principal
int main() {
    srand(time(NULL));
    initialize_memory();

    int opcao, pid, size;

    while (1) {
        printf("\n===== MENU =====\n");
        printf("1. Criar processo\n");
        printf("2. Ver tabela de páginas\n");
        printf("3. Ver memória lógica (todos os processos)\n");
        printf("4. Ver memória física\n");
        printf("0. Sair\n");

        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 0) break;

        switch (opcao) {
            case 1:
                printf("ID do processo: ");
                scanf("%d", &pid);
                printf("Tamanho do processo (em bytes): ");
                scanf("%d", &size);
                create_process(pid, size);
                break;

            case 2:
                printf("ID do processo: ");
                scanf("%d", &pid);
                print_page_table(pid);
                break;

            case 3:
                list_all_processes();
                break;

            case 4:
                show_physical_memory();
                break;

            default:
                printf("Opção inválida.\n");
                break;
        }
    }

    // Liberação de memória
    for (int i = 0; i < process_count; i++) {
        free(processes[i]->page_table);
        free(processes[i]);
    }

    return 0;
}