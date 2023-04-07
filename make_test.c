#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_N 10000 // 최대 객체 수
#define MIN_N 1000 // 최소 객체 수
#define MAX_T 50 // 최대 속성 수
#define MAX_M 20 // 최대 속성 이름 길이
#define MAX_VAL 1000 // 최대 속성 값 길이
#define MAX_KEY_LEN 10 // 키 속성 값 길이


// 랜덤 문자열 생성 함수
void rand_string(char *str, size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < size; i++) {
        int index = rand() % (int)(sizeof(charset) - 1);
        str[i] = charset[index];
    }
    str[size] = '\0';
}

int main() {
    srand(time(NULL));
    int i, j;

    // 객체 수, 속성 수, 속성 이름 배열 생성
    // int n = MAX_N;
    // int t = MAX_T;
    int n = rand() % (MAX_N - MIN_N + 1) + MIN_N;
    int t = rand() % MAX_T + 1;
    char attr_names[MAX_M+1];
    
    // input.txt 파일 생성 및 객체 정보 쓰기
    FILE *fp = fopen("hw1_input.txt", "w");
    fprintf(fp, "%d\n$\n", n);
    
    int key = rand() % t;

    for (j = 0; j < t; j++) {
        // rand_string(attr_names, MAX_M);
        rand_string(attr_names, rand() % MAX_M + 1);
        if(j == 0)
            fprintf(fp, "%s", attr_names);
        else
            fprintf(fp, ":%s", attr_names);

        if(key == j)
            fprintf(fp, "*");
    }
    fprintf(fp, "\n$\n");
    
    char random_attr[MAX_VAL + 1];
    for(i=0; i<n; i++) {
        for(j = 0; j < t; j++) {
            if(key == j)
                // rand_string(random_attr, MAX_KEY_LEN);
                rand_string(random_attr, rand() % MAX_KEY_LEN + 1);
            else
                // rand_string(random_attr, MAX_VAL);
                rand_string(random_attr, rand() % MAX_VAL + 1);

            if(j == 0)
                fprintf(fp, "%s", random_attr);
            else
                fprintf(fp, ":%s", random_attr);
        }
        fprintf(fp, "\n");
    }
    
    fclose(fp);
}