#include "dump.h"

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
            fprintf(png, "node%zu [shape=Mrecord; style = filled; fillcolor = \"#c0c6ff5b\"; color = \"#003a80\";\n", i);
            fprintf(png, "    label = \"SERVICE | index =  %zu | data = %d | { prev = %d | next = %d}  \"; ]\n\n", i, list.data[i],
            list.prev[i], list.next[i]);
        }
        else if (i == list.head)
        {
            fprintf(png, "node%zu [shape=Mrecord; style = filled; fillcolor = \"#b02a165b\"; color = \"#e90c1b\";\n", i);
            fprintf(png, "    label = \"HEAD | index =  %zu | data = %d | { prev = %d | next = %d}  \"; ]\n\n", i, list.data[i],
            list.prev[i], list.next[i]);
        }
        else if (i == list.tail)
        {
            fprintf(png, "node%zu [shape=Mrecord; style = filled; fillcolor = \"#337d215b\"; color = \"#125107\";\n", i);
            fprintf(png, "    label = \"TAIL | index =  %zu | data = %d | { prev = %d | next = %d}  \"; ]\n\n", i, list.data[i],
            list.prev[i], list.next[i]);
        }
        else if (list.next[i] != -1)
        {
            fprintf(png, "node%zu [shape=Mrecord; style = filled; fillcolor = \"#c0ffc05b\"; color = \"#55fe55\";\n", i);
            fprintf(png, "    label = \"USED | index =  %zu | data = %d | { prev = %d | next = %d}  \"; ]\n\n", i, list.data[i],
                list.prev[i], list.next[i]);
        }
        else
        {
            fprintf(png, "node%zu [shape=Mrecord; style = filled; fillcolor = \"#c0ffc05b\"; color = \"#b59814\";\n", i);
            fprintf(png, "    label = \"FREE | index =  %zu | data = %d | { prev = %d | next = %d}  \"; ]\n\n", i, list.data[i],
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

        if (next < 0 || (size_t)next > list.capacity || i == list.tail)
            continue;

        fprintf(png, "node%zu -> node%d [dir=both, constraint=false];\n", i, next);
    }

    fprintf(png, "}\n");

    fclose(png);
}

void print_html(list_t list, const char fname[], const char process[], const char pic_name[], const char reason[])
{
    FILE *png = fopen(fname, "w");

    prepare_png(list, pic_name);

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
    fprintf(png, "head index.....%zu\n", list.head);
    fprintf(png, "tail index.....%zu</h3>\n", list.tail);

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
        else if (list.next[i] == -1)
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
    fprintf(png, "<img src=%s width=1200px>", "MEOW.png");
    fprintf(png, "Text Diagram\n");

    size_t i = list.head;
    fprintf(png, "<p>HEAD");
    while (i != list.tail)
    {
        i = (size_t)list.next[i];
        fprintf(png, " -> [%zu]", i);
    }
    fprintf(png, " -> TAIL</p>");

    fclose(png);
}