#ifndef DOTTER_H
#define DOTTER_H

#define print_dot(TEXT) fprintf(get_log_file(".dot"), TEXT)

void graph_start();
void graph_add_dot(void* address, int data, void* next, void* prev);
void gpaph_add_arrow();
void graph_end();

void graph_start()
{
   // fprintf(get_log_file(".dot"), "digraph {\n\n");
   print_dot("digraph {\n\n");
}

void graph_add_dot(void* address, int data, void* next, void* prev)
{
    fprintf(get_log_file(".dot"), "node%p [shape = Mrecord, label = \"value = %d\\naddress = %p\\nnext = %p\\nprev = %p\"];\n", address, data, address, next, prev);
}

void graph_add_arrow(void* address1, void* address2)
{
    fprintf(get_log_file(".dot"), "node%p -> node%p;\n", address1, address2);
}

void graph_end()
{
    fprintf(get_log_file(".dot"), "\n}\n");
}

#endif
