#include "h_files/dump.h"

static int pic_num = 0;

void prepare_png(list_t list, const char fname[])
{
    FILE *png = fopen(fname, "w");

    fprintf(png, "digraph {\n\n");
    fprintf(png, "rankdir = LR;\n");
    fprintf(png, "splines=ortho;\n");
    for (size_t i = 0; i <= list.capacity; i++)
    {
        if (i == 0)
        {
            fprintf(png, "node%zu [shape=Mrecord; style = filled; fillcolor = \"#c0c6ff5b\"; color = \"#000000\";\n", i);
            fprintf(png, "    label = \"<status> SERVICE | index =  %zu | data = %d | { <prev> prev = %d | next = %d}  \"; ]\n\n", i, list.data[i],
            list.prev[i], list.next[i]);
        }
        else if (i == (size_t)get_head(list))
        {
            fprintf(png, "node%zu [shape=Mrecord; style = filled; fillcolor = \"#7e0e775b\"; color = \"#030303\";\n", i);
            fprintf(png, "    label = \"<status> HEAD | index =  %zu | data = %d | { <prev> prev = %d | next = %d}  \"; ]\n\n", i, list.data[i],
            list.prev[i], list.next[i]);
        }
        else if (i == (size_t)get_tail(list))
        {
            fprintf(png, "node%zu [shape=Mrecord; style = filled; fillcolor = \"#337d215b\"; color = \"#000000\";\n", i);
            fprintf(png, "    label = \"<status> TAIL | index =  %zu | data = %d | { <prev> prev = %d | next = %d}  \"; ]\n\n", i, list.data[i],
            list.prev[i], list.next[i]);
        }
        else if (list.data[i] != POISON)
        {
            fprintf(png, "node%zu [shape=Mrecord; style = filled; fillcolor = \"#c0ffc05b\"; color = \"#000000\";\n", i);
            fprintf(png, "    label = \"<status> USED | index =  %zu | data = %d | { <prev> prev = %d | next = %d}  \"; ]\n\n", i, list.data[i],
                list.prev[i], list.next[i]);
        }
        else
        {
            fprintf(png, "node%zu [shape=Mrecord; style = filled; fillcolor = \"#d3fc1b5b\"; color = \"#000000\";\n", i);
            fprintf(png, "    label = \"<status> FREE | index =  %zu | data = %d | { <prev> prev = %d | next = %d}  \"; ]\n\n", i, list.data[i],
                list.prev[i], list.next[i]);
        }

    }

    for (size_t i = 0; i < list.capacity; i++)
    {
        fprintf(png, "node%zu -> node%zu [style=invis, weight=100];\n", i, i + 1);
    }

    for (size_t i = 1; i <= list.capacity; i++)
    {
        int next = list.next[i];
        int prev = list.prev[i];
        int next_prev = list.next[prev];
        int prev_next = list.prev[next];

        if (next > 0 && (size_t)next <= list.capacity && (size_t)prev_next == i)
            fprintf(png, "node%zu -> node%d [color=\"green\", penwidth=1, constraint = false];\n", i, next);
        else if (next != 0)
            fprintf(png, "node%zu -> node%d [color=\"red\", penwidth=1, constraint = false];\n", i, next);

        if (prev > 0 && (size_t)prev <= list.capacity && (size_t)next_prev == i)
            fprintf(png, "node%zu -> node%d [color=\"blue\", penwidth=1, constraint = false];\n", i, prev);
        else if (prev != 0)
            fprintf(png, "node%zu -> node%d [color=\"red\", penwidth=1, constraint = false];\n", i, prev);

    }

    fprintf(png, "}\n");

    fclose(png);
}

void print_html(list_t list, const char fname[], const char process[], const char pic_name[], const char reason[])
{
    FILE *png = fopen(fname, "a"); // TODO all operations

    pic_num++;

    char pic_file_name[MAX_FILE_NAME_LEN] = {};
    sprintf(pic_file_name, "txt_files/%s%c.txt", pic_name, pic_num + '0');

    char png_file_name[MAX_FILE_NAME_LEN] = {};
    sprintf(png_file_name, "png_files/%s%c.png", pic_name, pic_num + '0');


    prepare_png(list, pic_file_name);
    char system_command[MAX_COMMAND_NAME_LEN] = {};
    sprintf(system_command, "dot %s -Tpng -o %s", pic_file_name, png_file_name);
    system(system_command);

    fprintf(png, "<h1> DUMP <font color=red>%s:</font>%s</h1>\n\n", process, reason);

    fprintf(png, "<style>\n");
    fprintf(png, "    table {\n");
    fprintf(png, "        border-collapse: collapse;\n");
    fprintf(png, "    }\n\n");
    fprintf(png, "    th, td {\n");
    fprintf(png, "        border:1px solid black;\n");
    fprintf(png, "        padding: 0.5px 1px;\n");
    fprintf(png, "        text-align: center;\n");
    fprintf(png, "    }\n");
    fprintf(png, "</style>\n\n");

    fprintf(png, "<pre>\n");

    fprintf(png, "<h3 style=\"line-height: 0.9;\">size...........%zu\n", list.size);
    fprintf(png, "capacity.......%zu\n", list.capacity);
    fprintf(png, "head index.....%d\n", get_head(list));
    fprintf(png, "tail index.....%d</h3>\n", get_tail(list));

    fprintf(png, "<table>\n");
    fprintf(png, "    <tr>\n");
    fprintf(png, "        <th>Index</th>\n");
    fprintf(png, "        <th>Meaning</th>\n");
    fprintf(png, "        <th>Prev</th>\n");
    fprintf(png, "        <th>Next</th>\n");
    fprintf(png, "        <th>Status</th>\n");
    fprintf(png, "    </tr>\n");
    for (size_t i = 0; i <= list.capacity; i++)
    {
        fprintf(png, "    <tr>\n");
        fprintf(png, "        <td>%zu</td>\n", i);
        fprintf(png, "        <td>%d</td>\n", list.data[i]);
        fprintf(png, "        <td>%d</td>\n", list.prev[i]);
        fprintf(png, "        <td>%d</td>\n", list.next[i]);
        if (i == 0)
        {
            fprintf(png, "        <td>Service</td>\n");
        }
        else if (list.data[i] == POISON)
        {
            fprintf(png, "        <td>Free</td>\n");
        }
        else
            fprintf(png, "        <td>Used</td>\n");


        fprintf(png, "    </tr>\n");
    }
    fprintf(png, "</table>\n");
    fprintf(png,"</pre>\n");
    fprintf(png, "Graph Visualization\n");

    fprintf(png, "<img src=%s width=1200px>", png_file_name);
    fprintf(png, "Text Diagram\n");

    list_elem_t i = get_head(list);
    fprintf(png, "<p>HEAD:[%d]", i);
    size_t steps = 0;
    while (i != get_tail(list) && steps++ < list.capacity && i <= (list_elem_t)list.capacity)
    {
        i = (size_t)list.next[i];
        fprintf(png, " -> [%d]", i);
    }
    if (i == get_tail(list))
        fprintf(png, ":TAIL</p>");

    fclose(png);
}