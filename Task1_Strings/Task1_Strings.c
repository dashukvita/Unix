#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

//печать словаря
void printArray(char *array[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("\"%s\"\n", array[i]);
    }
}

//сравнение двух слов указать тип
int words_compare(const void *a, const void *b) {
    const char *pa = *(const char **) a;
    const char *pb = *(const char **) b;
    
    return strcmp(pa, pb);
}

//создаем двумерный массив
char ** dynamicArrayAlloc(int N, int M){
    char **arr = (char **)malloc(N*sizeof(char *));
    for(int i = 0; i < N; i++) {
        arr[i] = (char *)malloc(M*sizeof(char));
    }
    return arr;
}

//заполняем двумерный массив
void addWords(char **dictionary, int countWords, char *buffer){
    int i = 0;
    int j = 0;
    int isWord = 0;
    int n = 0;
    int ch = 0;
    
    while (buffer[n]) {
        ch = buffer[n];
        switch (ch) {
            case ';':
                if (isWord) {i+=1; isWord = 0; j = 0;}
                dictionary[i][j] = ';'; i+=1; j=0;
                break;
            case '|':
                if (isWord) {i+=1; isWord = 0; j = 0;}
                if(buffer[n+1] == '|') {n+=1; dictionary[i][j] = '|'; j+=1;}
                dictionary[i][j] = '|'; i+=1; j=0;
                break;
            case '&':
                if (isWord) {i+=1; isWord = 0; j = 0;}
                if(buffer[n+1] == '&') {n+=1; dictionary[i][j] = '&'; j+=1;}
                dictionary[i][j] = '&'; i+=1; j=0;
                break;
            default:
                if (!isspace(ch)){
                    isWord = 1;
                    dictionary[i][j] = buffer[n]; j++;
                }
                else{
                    if (isWord) {i+=1; isWord = 0; j = 0;}
                }
                break;
        }
        n++;
    }
}

//считаем количество введенных слов
int countWord(char *buffer) {
    int count = 0;
    int isWord = 0;
    char const *p = buffer;
    int i = 0;
    
    while (*p) {
        switch (*p) {
            case ';':
                if (isWord) {count +=1; isWord = 0;}
                count +=1;
                break;
            case '|':
                if(buffer[i+1] == '|') {p+=1; i+=1;}
                if (isWord) {count +=1; isWord = 0;}
                count +=1;
                break;
            case '&':
                if(buffer[i+1] == '&') {p+=1; i+=1;}
                if (isWord) {count +=1; isWord = 0;}
                count +=1;
                break;
            default:
                if (!isspace(*p)){ isWord = 1;}
                else{
                    if (isWord) {count +=1; isWord = 0;}
                }
                break;
        }
        p++;
        i++;
    }
    if (isWord) {count +=1;}
    return count;
}

//считаем текст
char * inputLine(int startSize) {
    char *str;
    int ch;
    int len = 0;
    str = realloc(NULL, sizeof(char) * startSize);
    if (!str) {
        return str;
    }
    while ((ch = getchar()) != EOF) {
        str[len++] = ch;
        if (len == startSize) {
            str = realloc(str, sizeof(char) * (startSize += 16));
            if (!str) {
                return str;
            }
        }
    }
    str[len++] = '\0';
    
    return realloc(str, sizeof(char) * len);
}

//длина максимальной строки
int maxLen(char *buffer) {
    char const *p = buffer;
    int maxLen = 0;
    int countChar = 0;
    
    while (*p !=0) {
        countChar++;
        if (isspace(*p)){
            if(maxLen < countChar) maxLen = countChar;
            countChar = 0;
        }
        p++;
    }
    if (maxLen == 0) maxLen = countChar+1;
    return maxLen-1;
}

void deleteBufferDic(char *buffer, char **dictionary, int countword)
{
    free(buffer);
    for(int i = 0; i < countword; i++){
        free(dictionary[i]);
    }
    free(dictionary);
}

int main(int argc, const char * argv[])
{
    char * buffer = inputLine(10);
    printf("Введенная строка= \"%s\".\n", buffer);
    int countword = countWord(buffer);
    printf("Количество слов %i\n", countword);
    int maxlen = maxLen(buffer);
    char **dic = dynamicArrayAlloc(countword, maxlen);
    addWords(dic,countword, buffer);
    printf("Отсортированный массив:\n");
    qsort(dic, countword, sizeof(char *), words_compare);
    printArray(dic, countword);
    deleteBufferDic(buffer, dic, countword);
    return 0;
}
