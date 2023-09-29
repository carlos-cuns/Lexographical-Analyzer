// Carlos Cuns, Wilson Gil
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_SIZE 100
#define PROG_SIZE 1000
#define MAX_LEX 100

char *reserved[] = {
    NULL, "skip", NULL, NULL, "+", "-", "*", "/", "ifel", "=", "!=", "<", "<=", ">", ">=", "(",
    ")", ",", ";", ".", ":=", "begin", "end", "if", "then", "while", "do", "call", "const",
    "var", "procedure", "write", "read", "else"};

int progLen = 0;

int isNum(char *lexeme)
{
    if (lexeme == NULL)
        return 0;
    int len = strlen(lexeme);
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(lexeme[i]))
            return 0;
    }
    return 1;
}
// returns index of reserved word or -1 if is not
int isROrS(char *lexeme)
{
    if (lexeme == NULL)
        return -1;
    for (int i = 1; i <= 33; i++)
    {
        if (reserved[i] == NULL)
            continue;
        if (strcmp(lexeme, reserved[i]) == 0)
            return i;
    }
    return -1;
}
int isIden(char *lexeme)
{
    if (lexeme == NULL)
        return 0;
    int len = strlen(lexeme);
    if (len == 0 || !isalpha(lexeme[0]))
        return 0;
    for (int i = 1; i < len; i++)
    {
        if (!(isalpha(lexeme[i]) || isdigit(lexeme[i])))
            return 0;
    }
    return 1;
}
int *createTokenArr(char **lexems)
{
    return NULL;
}
void readELF(char *filename, char **lexems)
{
    char buffer;
    char workingStr[MAX_LEX];
    int i = 0;
    int j = 0;
    int checked = 0;

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("failed to open file\n");
        exit(1);
    }

    do
    {
        if (checked == 0)
            buffer = fgetc(file);
        else
            checked = 0;

        if (feof(file))
        {
            workingStr[j] = '\0';
            lexems[i] = malloc(sizeof(char) * (j + 1));
            strcpy(lexems[i], workingStr);
            break;
        }
        if (isblank(buffer) || buffer == '\n')
        {
            if (j > 0)
            {
                workingStr[j] = '\0';
                lexems[i] = malloc(sizeof(char) * (j + 1));
                strcpy(lexems[i], workingStr);
                i++;
                j = 0;
            }
            continue;
        }
        switch (buffer)
        {
        case '(':
        case ')':
        case '=':
        case '+':
        case '-':
        case ';':
        case '.':
        case ',':
        case '*':
            workingStr[j] = '\0';
            lexems[i] = malloc(sizeof(char) * (j + 1));
            strcpy(lexems[i], workingStr);
            i++;
            workingStr[0] = buffer;
            workingStr[1] = '\0';
            lexems[i] = malloc(sizeof(char) * 2);
            strcpy(lexems[i], workingStr);
            i++;
            j = 0;
            break;
        case ':':
            workingStr[j] = '\0';
            lexems[i] = malloc(sizeof(char) * (j + 1));
            strcpy(lexems[i], workingStr);
            i++;
            workingStr[0] = buffer;
            workingStr[1] = fgetc(file);
            workingStr[2] = '\0';
            lexems[i] = malloc(sizeof(char) * 3);
            strcpy(lexems[i], workingStr);
            i++;
            j = 0;
            break;
        case '>':
        case '<':
        case '!':
            workingStr[j] = '\0';
            lexems[i] = malloc(sizeof(char) * (j + 1));
            strcpy(lexems[i], workingStr);
            i++;
            char temp = buffer;
            buffer = fgetc(file);
            if (buffer == '=')
            {
                workingStr[0] = temp;
                workingStr[1] = buffer;
                workingStr[2] = '\0';
                lexems[i] = malloc(sizeof(char) * 3);
                strcpy(lexems[i], workingStr);
            }
            else
            {
                workingStr[0] = temp;
                workingStr[1] = '\0';
                lexems[i] = malloc(sizeof(char) * 2);
                strcpy(lexems[i], workingStr);
                checked = 1;
            }
            i++;
            j = 0;
            break;
        case '/':
            temp = buffer;
            buffer = fgetc(file);
            if (buffer == '*')
            {
                while (1)
                {
                    temp = buffer;
                    buffer = fgetc(file);
                    if (temp == '*' && buffer == '/')
                        break;
                }
            }
            else
            {
                checked = 1;
                workingStr[j] = '\0';
                lexems[i] = malloc(sizeof(char) * (j + 1));
                strcpy(lexems[i], workingStr);
                i++;
                workingStr[0] = temp;
                workingStr[1] = '\0';
                lexems[i] = malloc(sizeof(char) * 2);
                strcpy(lexems[i], workingStr);
                i++;
                j = 0;
            }
            break;
        default:
            workingStr[j] = buffer;
            j++;
            break;
        }

    } while (1);
    progLen = i;
    fclose(file);
}
void printLex(char **lexems)
{
    for (int i = 0; i < progLen; i++)
    {
        char *lex = lexems[i];
        printf("%-11s , isRorS %-2d, isIden %-3s, isNum %-3s\n",
               lex, isROrS(lex), isIden(lex) ? "Yes" : "No", isNum(lex) ? "Yes" : "No");
    }
}
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Include a file name\n");
        exit(1);
    }
    char **lexems = malloc(sizeof(char *) * PROG_SIZE);
    char filename[MAX_SIZE];

    strcpy(filename, argv[1]);
    readELF(filename, lexems);
    printLex(lexems);
    // printf("isReserved: %d\n", isReservedOrSpecial("var"));
    return 0;
}