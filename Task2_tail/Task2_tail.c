#include<stdio.h>
#include<string.h>
#include <stdlib.h>

//считаем количество строк
int countLines(FILE * file)
{
    int countLines = 0;
    char ch = 0;
    
    while ((ch=fgetc(file))!=EOF)
    {
        if (ch == '\n')
            ++ countLines;
    }
    if(countLines != 0) countLines ++;
    return countLines;
}

//выводим 10 или менее последних строк
void writeLine(FILE * file, int count)
{
    int i = 0;
    char ch = 0;
    int startRead = 0;
    startRead = (count > 10) ? count - 10 : count;

    while (!feof(file) && !ferror(file))
    {
        while(i < startRead && count > 10)
        {
            if((ch=fgetc(file))== '\n' && ch != EOF)
            {
                i++;
            }
        }
        if((ch=getc(file)) != EOF)
        {
            putchar(ch);
        }
    }
}

//реализация функции tail
void tail(FILE * file){
    int count = 0;
    
    count = countLines(file);
    rewind(file);
    writeLine(file, count);
}

//сохраняем введенные данные в файл
FILE fullFile(FILE * f){
    char ch = 0;
    
    while ((ch = getchar()) != EOF) {
        fputc(ch, f);
    }
    return * f;
}

int main(int argc , char *argv[])
{
    FILE *file;
    //если функция без аргументов, создаем временный файл
    if(argc == 1){
        if ((file = tmpfile()) == NULL)
        {
            printf("Ошибка открытия файла.\n");
            return EXIT_FAILURE;
        }
        else{
            fullFile(file);
        }
        rewind(file);
        tail(file);
        fclose (file);
    }
    else {
        for (int i = 1; i < argc; i++)
        {
            file  = fopen(argv[i] , "r");
            if(file == NULL)
            {
                printf("Ошибка открытия файла.\n");
                return EXIT_FAILURE;
            }
            if (i == 1)printf("==> %s <==\n", argv[i]);
            else printf("\n==> %s <==\n", argv[i]);
            tail(file);
            fclose(file);
        }
    }
    return 0;
}
