#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <shlwapi.h>
#include <direct.h>

typedef struct Node {
    struct Node *children[16];
    char *hex_value;
} Node;

Node* new_node() {
    Node* node = (Node*)malloc(sizeof(Node));
    for (int i = 0; i < 16; i++) {
        node->children[i] = NULL;
    }
    node->hex_value = NULL;
    return node;
}

int insert(Node* node, const char* hex_str) {
    for (size_t i = 0; i < strlen(hex_str); i++) {
        int idx = hex_str[i] >= 'a' ? hex_str[i] - 'a' + 10 : hex_str[i] - '0';
        if (!node->children[idx]) {
            node->children[idx] = new_node();
        }
        node = node->children[idx];
    }
    if (node->hex_value) {
        printf("Duplicate value found: %s\n", hex_str);
        return 0;
    }
    node->hex_value = _strdup(hex_str);
    return 1;
}

void generate_random_hex(char* buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        int r = rand() % 16;
        buffer[i] = r >= 10 ? 'a' + r - 10 : '0' + r;
    }
    buffer[length] = '\0';
}

void write_tree_to_csv(FILE* file, Node* node) {
    if (node->hex_value) {
        fprintf(file, "%s\n", node->hex_value);
    }
    for (int i = 0; i < 16; i++) {
        if (node->children[i]) {
            write_tree_to_csv(file, node->children[i]);
        }
    }
}

void free_tree(Node* node) {
    for (int i = 0; i < 16; i++) {
        if (node->children[i]) {
            free_tree(node->children[i]);
        }
    }
    if (node->hex_value) {
        free(node->hex_value);
    }
    free(node);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc != 3) {
        printf("Usage: %s <num_random_hex_strings> <output_directory>\n", argv[0]);
        return 1;
    }

    int num_random_hex_strings = atoi(argv[1]);
    const char *output_directory = argv[2];

    // Hex generation and insertion logic
    int unique_count = 0;
    Node* tree = new_node();
    char random_hex[65];

    clock_t start_time = clock();

    for (int i = 0; i < num_random_hex_strings; i++) {
        generate_random_hex(random_hex, sizeof(random_hex) - 1);
        unique_count += insert(tree, random_hex);
    }

    clock_t end_time = clock();
    double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("The script took %.2f seconds to run at %d unique keys.\n", duration, unique_count);

    // Allocate memory for the output filename based on the output directory and the unique count
    int filename_length = snprintf(NULL, 0, "%s\\output_%d_unique_values.csv", output_directory, unique_count);
    char *output_filename = (char *)malloc(filename_length + 1);
    snprintf(output_filename, filename_length + 1, "%s\\output%d_unique_values.csv", output_directory, unique_count);
    FILE* file = fopen(output_filename, "w");
    if (file == NULL) {
      perror("Failed to open the output file");
      free(output_filename);
      return 1;
    }

    write_tree_to_csv(file, tree);
    fclose(file);

    // Free allocated memory
    free(output_filename);
    free_tree(tree);
    return 0;
}
