#ifndef DOTTER_H
#define DOTTER_H
#include <time.h>

#define print_dot(TEXT) fprintf(get_log_file(".dot"), TEXT)

void graph_start();
void graph_add_dot(void* address, int data, void* next, void* prev);
void gpaph_add_arrow(void* address1, void* address2, const char* color);
void graph_end();
static char* get_dot_file_name();

void graph_start()
{
   // fprintf(get_log_file(".dot"), "digraph {\n\n");
   print_dot("digraph {\n\n");
}

void graph_add_dot(void* address, int data, void* next, void* prev)
{
    fprintf(get_log_file(".dot"), "node%p [shape = Mrecord, fillcolor = \"#FFD0DC\",style = filled, color = \"#000000\", label = \"{value = %d|\\naddress = %p|\\nnext = %p|\\nprev = %p}\"];\n", address, data, address, next, prev);
}

void graph_add_arrow(void* address1, void* address2, const char* color)
{
    fprintf(get_log_file(".dot"), "node%p -> node%p [color = \"%s\"];\n", address1, address2, color);
}

void graph_end()
{
    fprintf(get_log_file(".dot"), "\n}\n");
    char cmd[200] = "";
    char* dot_file_name = get_dot_file_name();

    sprintf(cmd, "dot %s -Tpng -o 3.png\n", dot_file_name);
    printf("%s", cmd);
    system(cmd);
    free(dot_file_name);
}

static char* get_dot_file_name()
{
    time_t timer = time(NULL);
    struct tm* time = localtime(&timer);
    char* filename = (char*)calloc(100, sizeof(char));

    sprintf(filename, ".log/%d_%d_%d_%d_%d_log.dot", time->tm_hour, time->tm_min, time->tm_mday,
            time->tm_mon+1, time->tm_year+1900);

   // printf("%s\n", filename);
    return filename;
}

#endif
