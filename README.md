# Memory Management with Paging - Operating Systems UFSC
## Project developed for the Operating Systems course of the Information Systems program at the Federal University of Santa Catarina (UFSC)

## Compiling and running the program:

Run the following commands in the project's terminal:
To compile:
```gcc main.c -o main```

To run:
```./main```

## Implementation Details:

### 1. **Physical and Logical Memory Initialization**
- Physical memory: `Frame` vector (global `physical_memory`).
- Logical memory is indirectly represented by page tables and physical frames. Random initialization is done on the frames (in `create_process`).
- **`initialize_memory()` function**:
    - Divides the physical memory into frames according to the `PAGE_SIZE`.
    - Starts each frame as free, with no associated process and no data.
    - Builds a linked list of free frames (`free_frames_list`).

### 2. **Data Structures**
- **Frame**: A struct/"object" that represents a frame in physical memory:
    - `process_id`: The process occupying the frame.
    - `page_num`: The page number stored in the frame.
    - `data`: Byte array.
    - `is_free`: Flag indicating the availability of the frame.

### 3. **Process Creation**
- **`create_process(process_id, size)` function**:
    - Checks for duplicate `process_id`s and verifies if the process size exceeds `MAX_PROCESS_SIZE`.
    - Calculates how many pages will be used (`page_count`).
    - Checks if there are enough free frames in `free_frames_list`, a list representing non-contiguous allocation.
    - Allocates the process structure and its page table.
    - For each page:
        - Removes a frame from the free frames list.
        - Marks it as occupied, associating it with the process and page.
        - Fills the frame with random values.
        - Updates the page table.
    - Adds this process to the `processes` array.

### 4. **Implementation Visualization**
- **`print_page_table(process_id)` function**:
    - Searches for the process by its `process_id`, displaying its size and number of pages. It also shows the physical frame number.
- **`list_all_processes()` function**:
    - Displays a list with the data of all created processes.
- **`show_physical_memory()` function**:
    - Evaluates each frame in physical memory, indicating whether it is free or not. When occupied, it shows the process ID and page number.
- **`main()` function**:
    - Contains the user interface with options to create a process and check the page table, logical memory, and physical memory.

## Meeting the requirements:
**Upon initialization, the system prompts for these 3 settings:**
- 1. Physical Memory Size Configuration (default: 1024)
- 2. Page Size Configuration (default: 32)
- 3. Maximum Process Size Configuration (default: 256)

## Simulated Example

In the example below, we use the default settings (physical memory: 1024, page size: 32, max process size: 256) to simulate an insufficient memory error. The error occurred because the 5th process created with a size of 256 exceeds the total 1024 bytes:

![Insufficient Memory](images/MemoInsuficiente.png)

In this other example, we can see process 1, which was created with a size of 256 and allocated to the respective pages/frames below.

Using option **3.** from the menu returns a similar result, displaying all created processes.

![Page Table](images/TabelaDePagProcesso1.png)

Finally, it is possible to view the physical memory occupation through option **4.** in the menu.

![Physical Memory](images/MemoFisica.png)

## Developed by:
- Felipe Delduqui Alves Pinto Flávio (23100769)
- Arthur Paulo Rodrigues (23100747)
- Roberto Gabriel Ferreira (23100739)
