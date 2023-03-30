#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Node {
    struct Node* children[16];
    char* hex_value;
} Node;

Node* new_node() {
    Node* node = (Node*)malloc(sizeof(Node));
    for (int i = 0; i < 16; i++) {
        node->children[i] = NULL;
    }
    node->hex_value = NULL;
    return node;
}

void insert(Node* node, const char* hex_str) {
    for (size_t i = 0; i < strlen(hex_str); i++) {
        int idx = hex_str[i] >= 'a' ? hex_str[i] - 'a' + 10 : hex_str[i] - '0';
        if (!node->children[idx]) {
            node->children[idx] = new_node();
        }
        node = node->children[idx];
    }
    node->hex_value = strdup(hex_str);
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

void generate_random_hex(char* buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        int r = rand() % 16;
        buffer[i] = r >= 10 ? 'a' + r - 10 : '0' + r;
    }
    buffer[length] = '\0';
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <number_of_random_hex_strings>\n", argv[0]);
        return 1;
    }

    int num_random_hex_strings = atoi(argv[1]);
    srand(time(NULL));
    Node* tree = new_node();
    char random_hex[65];
    clock_t start_time = clock();

    for (int i = 0; i < num_random_hex_strings; i++) {
        generate_random_hex(random_hex, sizeof(random_hex) - 1);
        insert(tree, random_hex);
    }

    clock_t end_time = clock();
    double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("The script took %.2f seconds to run at %d keys.\n", duration, num_random_hex_strings);

    // Write the tree to a CSV file
    char output_filename[64];
    snprintf(output_filename, sizeof(output_filename), "Coutput_%d_values.csv", num_random_hex_strings);
    FILE* file = fopen(output_filename, "w");
    if (file == NULL) {
        perror("Failed to open the output file");
        return 1;
    }
    write_tree_to_csv(file, tree);
    fclose(file);

    // Free allocated memory
    free_tree(tree);

    return 0;
}
