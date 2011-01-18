#include "Black.h"

void readFileContents(GrowableString &gs, const char *filename, int &num_chars)
{
    FILE *fin = fopen(filename, "r");
    num_chars = 0;
    int c;
    while((c = fgetc(fin)) != EOF)
    {
        num_chars++;
        gs.growTo(num_chars);
        gs.Val[num_chars-1] = c;
    }
    fclose(fin);
}
