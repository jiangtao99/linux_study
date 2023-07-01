#include <myhead.h>
int main()
{
    char sentence[] = "./add 3 4";
    char *word;
    word = strtok(sentence, " ");
    printf("word = %s\n", word);
    while((word = strtok(NULL, " ")) != NULL)
    {
        printf("word = %s\n", word);    
    }

    return 0;
}