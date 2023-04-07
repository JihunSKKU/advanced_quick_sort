#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ATTRIBUTE_TYPE 50
#define MAX_ATTRIBUTE_KEY_LENGTH 10
#define MAX_ATTRIBUTE_LENGTH 1000

typedef struct {
    char attribute[MAX_ATTRIBUTE_KEY_LENGTH + 1];
    int order;
} key;


void read_file(int *num_objects, key **key_attributes, char ***attributes) {
    // Open input file for reading
    FILE *input_file = fopen("hw1_input.txt", "r");
    if(!input_file) {
        fprintf(stderr, "File open Error: %s\n", input_file);
        exit(1);
    }

    // Read number of objects
    fscanf(input_file, "%d", num_objects);

    // Read attributes type
    char cp; // Scan char pointer
    int attribute_type = 0; // Variable for (the number of attributes for each object - 1)
    int key_attribute_index = 0; // Variable for index of the key attribute

    while((cp = fgetc(input_file)) != '$'); // Delete dummy ('\n', '$')
    // Find the index of key attributes
    while((cp = fgetc(input_file)) != '$') {
        if(cp == '*')
            key_attribute_index = attribute_type;
        else if(cp == ':')
            attribute_type++;
    }

    // Variable declaration
    int i; // Loop variable
    *key_attributes = (key *)malloc(sizeof(key) * (*num_objects));
    *attributes = (char **)malloc(sizeof(char *) * (*num_objects));
    for(i = 0; i < *num_objects; i++)
        (*attributes)[i] = (char *)malloc(sizeof(char) * (MAX_ATTRIBUTE_KEY_LENGTH 
                                + attribute_type * MAX_ATTRIBUTE_LENGTH + MAX_ATTRIBUTE_TYPE));
    
    int key_index; // Index of (i)th key_attributes
    int attribute_index; // Index of (i)th attributes
    int key_check; // Variable to check if the current attribute is a key
    
    // Read Attributes
    cp = fgetc(input_file); // Delete dummy ('\n')
    for(i = 0; i < *num_objects; i++) {
        key_index = 0;
        attribute_index = 0;
        key_check = 0;

        while((cp = fgetc(input_file)) != '\n' && cp != EOF) {
            if(cp == ':')
                key_check++;
            
            else if(key_check == key_attribute_index)
                (*key_attributes)[i].attribute[key_index++] = cp;

            (*attributes)[i][attribute_index++] = cp;
        }

        (*key_attributes)[i].attribute[key_index] = '\0';
        (*key_attributes)[i].order = i;
        (*attributes)[i][attribute_index] = '\0';
    }

    // Close input file
    fclose(input_file);
}


void write_file(int num_objects, key *key_attributes, char **attributes) {
    // Open output file
    FILE *output_file = fopen("hw1_output.txt", "w");
    if(!output_file) {
        fprintf(stderr, "File open Error: %s\n", output_file);
        exit(1);
    }

    int i; // Loop variable
    for(i = 0; i < num_objects - 1; i++)
        fprintf(output_file, "%s\n", attributes[key_attributes[i].order]);
    fprintf(output_file, "%s", attributes[key_attributes[i].order]);

    // Close output file
    fclose(output_file);
}

/* Function that swaps key1 and key2 */
void swap(key *key1, key *key2) {
    key temp;

    temp.order = key1->order;
    strcpy(temp.attribute, key1->attribute);
    key1->order = key2->order;
    strcpy(key1->attribute, key2->attribute);
    key2->order = temp.order;
    strcpy(key2->attribute, temp.attribute);
}

/* Insertion sort */
void insertion_sort(key *key_attributes, int low, int high) {
    int i, j;
    key pivot;
    for(i = low + 1; i <= high; i++) {
        strcpy(pivot.attribute, key_attributes[i].attribute);
        pivot.order = key_attributes[i].order;

        for(j = i - 1; j >= low && strcmp(pivot.attribute, key_attributes[j].attribute) < 0; j--) {
            strcpy(key_attributes[j+1].attribute, key_attributes[j].attribute);
            key_attributes[j+1].order = key_attributes[j].order;
        }
        
        strcpy(key_attributes[j+1].attribute, pivot.attribute);
        key_attributes[j+1].order = pivot.order;
    }
}

/* This function performs the partitioning process of the quick_sort
 * to sort an array of key_attributes. */
int partition(key *key_attributes, int low, int high) {
    /* Randomized pivot */
    srand(time(NULL));
    int random = low + rand() % (high - low);
    swap(&key_attributes[random], &key_attributes[low]);

    int left = low + 1, right = high;
    char pivot[MAX_ATTRIBUTE_KEY_LENGTH + 1];
    strcpy(pivot, key_attributes[low].attribute);

    while(left < right) {
        while(strcmp(key_attributes[left].attribute, pivot) < 0)
            left++;
        while(strcmp(key_attributes[right].attribute, pivot) > 0)
            right--;
        if(left < right)
            swap(&key_attributes[left++], &key_attributes[right--]);
    }

    swap(&key_attributes[low], &key_attributes[right]);
    return right;
}

/* If the length of the key_attributes array is 10 or less, insertion_sort is used for sorting;
 * otherwise, ramdomized quick_sort is used for sorting. */
void quick_sort(key *key_attributes, int low, int high) {
    if(high - low + 1 <= 10) {
        insertion_sort(key_attributes, low, high);
        return;
    }
    else if(low < high) {
        int pivot = partition(key_attributes, low, high);
        quick_sort(key_attributes, low, pivot - 1);
        quick_sort(key_attributes, pivot + 1, high);
    }
}

int main() {
    // Variable declaration
    int num_objects;
    key *key_attributes;
    char **attributes;

    // Read attributes in hw1_input.txt
    read_file(&num_objects, &key_attributes, &attributes);

    // Sorting
    quick_sort(key_attributes, 0, num_objects - 1);

    // Write sorting attributes in hw1_output.txt
    write_file(num_objects, key_attributes, attributes);

    // Memory allocated
    int i;
    free(key_attributes);
    for(i = 0; i < num_objects; i++)
        free(attributes[i]);
    free(attributes);
}