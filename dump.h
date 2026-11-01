#ifndef DUMP_H
#define DUMP_H
#include "list.h"

const char * const PIC_NAME = "MEOW.txt";
const char * const FNAME = "LOGFILE.html";

void prepare_png(list_t list, const char fname[]);
void print_html(list_t list, const char fname[], const char process[], const char pic_name[], const char reason[]);
#endif