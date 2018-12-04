#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <ctype.h>
#include <wctype.h>

//содержимое текущего файла: число строк, слов и символов.
int charCount;
int wordCount;
int lineCount;
//сумарное количество число строк, слов и символов.
int totalCharCount = 0;
int totalWordCount = 0;
int totalLineCount = 0;
//считаем количство исмволов и строк
void countCharLine(int c){
    charCount++;
    if ((c) == L'\n')
    lineCount++;
}
//реализация функции wc
int wc (char *filename)
{
    FILE *fp;
    int ch;
    int isWord = 0;
    
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror(filename);
        return 0;
    }
    
    while ((ch = getc (fp)) != EOF)
    {
        if(!iswspace(ch))
        {
            isWord = 1;
        }
        else if (isWord)
        {
            wordCount ++; isWord = 0;
        }
        countCharLine(ch);
    }
    
    if(isWord) wordCount +=1;
    
    fclose(fp);
    return 1;
}
//считаем total
void counter(){
    totalCharCount += charCount;
    totalWordCount += wordCount;
    totalLineCount += lineCount;
    
    charCount = wordCount = lineCount = 0;
}
//сохраняем данные во временный файл
char * createFile() {
    char * fileName = "temp.txt";
    FILE * f = fopen(fileName, "w");
    int c;
    while ((c = getchar()) != EOF) {
        fputc(c, f);
    }
    fclose(f);
    return fileName;
}

void result (char *file, int ccount, int wcount, int lcount)
{
    printf ("\t%-i\t%-i\t%-i %s\n", lcount, wcount, ccount, file);
}

int main(int argc , char *argv[])
{
    //если функция без аргументов, создаем временный файл
    if(argc == 1){
        char * tempfilename = createFile();
        wc(tempfilename);
        remove(tempfilename);
        result ("", charCount, wordCount, lineCount);
    }
    else {
        for (int i = 1; i < argc; i++)
        {
            if (wc(argv[i])){
                result (argv[i], charCount, wordCount, lineCount);
                counter();
            }
        }
        if (argc > 2) result("total", totalCharCount, totalWordCount, totalLineCount);
    }
    return 0;
}
