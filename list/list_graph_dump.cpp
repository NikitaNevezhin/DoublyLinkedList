#include "list.h"

int ListGenerateNodeDump(FILE* dot_file, List* list, int data_idx, INDEX_TYPE idx_type) // ListGenerateDotFile helper func
{
    assert(dot_file);
    assert(list);

    const char* bgcolor = "#40E0D0"; // case of busy elems

    if (idx_type == FREE_HEAD || idx_type == FREE_MIDDLE)
        bgcolor = "#98FF98 "; // case of free elems

    else if (idx_type == ERROR_NODE)
        bgcolor = "#FF6B4A";

    else if (idx_type == LOST_NODE)
        bgcolor = "#E066FF";

    fprintf(
            dot_file, 
            "        node_%d [label=<\n"
            "            <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" COLOR=\"#354733\" BGCOLOR=\"%s\" CELLPADDING=\"7\">\n"
            "                <TR><TD COLSPAN=\"2\" ALIGN=\"CENTER\"><FONT POINT-SIZE=\"3\"><BR/></FONT>INDEX=%d<FONT POINT-SIZE=\"3\"><BR/></FONT></TD></TR>\n"
            "                <TR><TD COLSPAN=\"2\" ALIGN=\"CENTER\"><FONT POINT-SIZE=\"3\"><BR/></FONT>DATA=%d<FONT POINT-SIZE=\"3\"><BR/></FONT></TD></TR>\n"
            "                <TR>\n"
            "                    <TD PORT=\"p\" WIDTH=\"70\" ALIGN=\"CENTER\"><FONT POINT-SIZE=\"3\"><BR/></FONT>PREV=%d<FONT POINT-SIZE=\"3\"><BR/></FONT></TD>\n"
            "                    <TD PORT=\"n\" WIDTH=\"70\" ALIGN=\"CENTER\"><FONT POINT-SIZE=\"3\"><BR/></FONT>NEXT=%d<FONT POINT-SIZE=\"3\"><BR/></FONT></TD>\n"
            "                </TR>\n"
            "            </TABLE>\n"
            "        >];\n\n",
            data_idx, bgcolor, data_idx, list->data[data_idx].value, list->data[data_idx].prev, list->data[data_idx].next
    );
    
    if (idx_type == HEAD)
    {
        fprintf(dot_file, "        {rank=same; node_%d; head_label;}\n", data_idx);
        fprintf(dot_file, "        head_label -> node_%d\n\n", data_idx);
    }

    else if (idx_type == TAIL)
    {
        fprintf(dot_file, "        {rank=same; node_%d; tail_label;}\n", data_idx);
        fprintf(dot_file, "        tail_label -> node_%d\n\n", data_idx);        
    }

    else if (idx_type == FREE_HEAD)
    {
        fprintf(dot_file, "        {rank=same; node_%d; free_head_label;}\n", data_idx);
        fprintf(dot_file, "        free_head_label -> node_%d\n\n", data_idx);
    }
        
    return 0;
}

int GenerateBusyCluster(FILE* dot_file, List* list)
{
    assert(dot_file);
    assert(list);

    fprintf(dot_file, "    subgraph cluster_busy_nodes {\n");
    fprintf(dot_file, "        head_label [shape=plain, label=<\n"
                      "            <TABLE BORDER=\"1\" CELLBORDER=\"0\" COLOR=\"#354733\" BGCOLOR=\"#FEF08A\" CELLPADDING=\"6\">\n"
                      "                <TR><TD ALIGN=\"CENTER\">HEAD</TD></TR>\n"
                      "</TABLE>\n"
                      ">];\n\n"
    );

    fprintf(dot_file, "        tail_label [shape=plain, label=<\n"
                      "            <TABLE BORDER=\"1\" CELLBORDER=\"0\" COLOR=\"#354733\" BGCOLOR=\"#F6B8B8\" CELLPADDING=\"6\">\n"
                      "                <TR><TD ALIGN=\"CENTER\">TAIL</TD></TR>\n"
                      "</TABLE>\n"
                      ">];\n\n"
    );
    
    int elem_counter = 0;
    int curr_idx = list->head;
    bool* seen = (bool*)calloc(list->capacity, sizeof(bool));

    while (curr_idx != -1)
    {   
        if (elem_counter >= list->capacity || seen[curr_idx])
        {
            PRINT_RED("Loop found - ListPrint is stopped\n");
            break; 
        }

        seen[curr_idx] = true;

        INDEX_TYPE idx_type = UNKNOWN;
        if (curr_idx == list->head)  idx_type = HEAD;
        
        else if (curr_idx == list->tail)  idx_type = TAIL;

        else    idx_type = MIDDLE;
        
        ListGenerateNodeDump(dot_file, list, curr_idx, idx_type);
        
        curr_idx = list->data[curr_idx].next;
        elem_counter++;
    }

    for (int i = 0; i < list->capacity; i++)    seen[i] = false;

    elem_counter = 0;
    int prev_idx = -1;
    curr_idx = list->head;

    while (curr_idx != -1)
    {   
        if (elem_counter >= list->capacity || seen[curr_idx])
        {
            PRINT_RED("Loop found - ListPrint is stopped\n");
            ListGenerateNodeDump(dot_file, list, curr_idx, ERROR_NODE);
            ListGenerateNodeDump(dot_file, list, prev_idx, ERROR_NODE);
            fprintf(dot_file, "        node_%d:s -> node_%d:s [color=\"#FF6B4A\", label=<<FONT POINT-SIZE=\"9\">LOOP</FONT><BR/>>, fontcolor=\"#FF6B4A\", dir=both];\n", prev_idx, curr_idx);
            break; 
        }

        seen[curr_idx] = true;
        
        ListElem* curr_elem = &list->data[curr_idx];

        if (seen[curr_elem->next])  // loop found
        {
            prev_idx = curr_idx;
            curr_idx = list->data[curr_idx].next;
            elem_counter++;
            continue; // loop will be found on the next iteration
        }

        if (curr_elem->prev != -1 && curr_elem->prev < list->capacity && curr_elem->prev >= 0)
        {   
            if (curr_elem->prev == prev_idx)  // right back link
                fprintf(dot_file, "        node_%d:p -> node_%d:n [splines=false];\n", curr_idx, curr_elem->prev);
            
            else
            {   
                ListGenerateNodeDump(dot_file, list, curr_elem->prev, ERROR_NODE);
                ListGenerateNodeDump(dot_file, list, curr_idx, ERROR_NODE);
                fprintf(dot_file, "        node_%d:s -> node_%d:s [color=\"#FF6B4A\", label=<<FONT POINT-SIZE=\"9\">INVALID LINK</FONT><BR/>>, fontcolor=\"#FF6B4A\", style=\"dashed\"];\n", curr_idx, curr_elem->prev);
            }
        }

        else if (curr_idx != list->head || (curr_idx == list->head && curr_elem->prev != -1)) // only head prev can be -1
        {
            fprintf(dot_file, "        node_%d:p -> node_%d:p [color=\"#FF6B4A\", label=<<FONT POINT-SIZE=\"9\">INDEX OUT</FONT><BR/>>, fontcolor=\"#FF6B4A\", style=\"dotted\"];\n", curr_idx, curr_idx);
        }

        if (curr_elem->next != -1 && curr_elem->next < list->capacity) // we suppose next is always right
            fprintf(dot_file, "        node_%d:n -> node_%d:p [splines=false];\n", curr_idx, curr_elem->next);
        
        prev_idx = curr_idx;
        curr_idx = list->data[curr_idx].next;
        elem_counter++;
    }

    fprintf(dot_file, "    }\n\n");

    return 0;
}

int GenerateFreeCluster(FILE* dot_file, List* list)
{
    assert(dot_file);
    assert(list);

    fprintf(dot_file, "    subgraph cluster_free_nodes {\n");
    fprintf(dot_file, "        free_head_label [shape=plain, label=<\n"
                      "            <TABLE BORDER=\"1\" CELLBORDER=\"0\" COLOR=\"#354733\" BGCOLOR=\"#FEF08A\" CELLPADDING=\"7\">\n"
                      "                <TR><TD ALIGN=\"CENTER\">FREE</TD></TR>\n"
                      "</TABLE>\n"
                      ">];\n\n"
    );
    int elem_counter = 0;
    int curr_idx = list->free_head;

    while (curr_idx != -1)
    {
        if (elem_counter > list->capacity)
        {
            PRINT_RED("Loop found - ListPrint is stopped\n");
            break;             
        }

        INDEX_TYPE idx_type = UNKNOWN;
        if (curr_idx == list->free_head)  idx_type = FREE_HEAD;

        else    idx_type = FREE_MIDDLE;
        
        ListGenerateNodeDump(dot_file, list, curr_idx, idx_type);
        
        curr_idx = list->data[curr_idx].next;
        elem_counter++;        
    }

    elem_counter = 0;
    curr_idx = list->free_head;

    while (curr_idx != -1)
    {   
        if (elem_counter >= list->capacity)
        {
            PRINT_RED("Loop found - ListPrint is stopped\n");
            break; 
        }
        
        ListElem* curr_elem = &list->data[curr_idx];

        if (curr_elem->prev != -1 && curr_elem->prev < list->capacity)
            fprintf(dot_file, "        node_%d:p -> node_%d:n\n", curr_idx, curr_elem->prev);

        if (curr_elem->next != -1 && curr_elem->next < list->capacity)
            fprintf(dot_file, "        node_%d:n -> node_%d:p\n", curr_idx, curr_elem->next);
        
        curr_idx = list->data[curr_idx].next;
        elem_counter++;
    }

    fprintf(dot_file, "    }\n\n");

    return 0;
}

int GenerateLostCluster(FILE* dot_file, List* list)
{
    assert(dot_file);
    assert(list);

    fprintf(dot_file, "    subgraph cluster_lost_nodes {\n");

    bool* seen = (bool*)calloc(list->capacity, sizeof(bool));

    // ----------------------------
    // ITERATION THROUGH BUSY NODES
    // ----------------------------

    int elem_counter = 0;
    int curr_idx = list->head;

    while (curr_idx != -1)
    {   
        if (elem_counter >= list->capacity || seen[curr_idx])
        {
            PRINT_RED("Loop found - ListPrint is stopped\n");
            break; 
        }
        
        seen[curr_idx] = true;
        curr_idx = list->data[curr_idx].next;
        elem_counter++;
    }

    // ----------------------------
    // ITERATION THROUGH FREE NODES
    // ----------------------------

    elem_counter = 0;
    curr_idx = list->free_head;

    while (curr_idx != -1)
    {   
        if (elem_counter >= list->capacity || seen[curr_idx])
        {
            PRINT_RED("Loop found - ListPrint is stopped\n");
            break; 
        }
        
        seen[curr_idx] = true;
        curr_idx = list->data[curr_idx].next;
        elem_counter++;
    }

    for (int i = 0; i < list->capacity; i++)
    {
        if (!seen[i])  ListGenerateNodeDump(dot_file, list, i, LOST_NODE);
    }

    fprintf(dot_file, "    }\n\n");

    free(seen);
    return 0;
}

int ListGenerateDotFile(List* list, const char* dot_filename)
{
    assert(list);
    assert(dot_filename);
    // ASSERT_LIST_OK(list);

    FILE* dot_file = fopen(dot_filename, "w");

    if (dot_file == NULL)
    {
        PRINT_RED("%s failed to open %s - cannot generate .dot file for graph dump\n", __func__, dot_filename);
        return 1;
    }

    fprintf(dot_file, "digraph G {\n");
    fprintf(dot_file, "    rankdir=LR;\n");
    fprintf(dot_file, "    nodesep=0.6;\n\n");
    fprintf(dot_file, "    ranksep=0.7;\n");
    fprintf(dot_file, "    ratio=\"auto\";\n");
    fprintf(dot_file, "    graph [dpi=150];\n");
    fprintf(dot_file, "    node [shape=plain, fontcolor=\"#212529\", fontname=\"Arial\", fontsize=11];\n");
    fprintf(dot_file, "    edge [color=\"#354733\", arrowsize=0.6]\n\n");

    GenerateLostCluster(dot_file, list);
    GenerateFreeCluster(dot_file, list);
    GenerateBusyCluster(dot_file, list);

    fprintf(dot_file, "}\n");

    fclose(dot_file);

    return 0;
}

int ListGenerateHtmlFile(List* list,  
                         const char* html_filename, const char* file, int line, const char* func)
{
    assert(list);
    assert(html_filename);

    static int dump_counter = 1;

    const char* dot_filename = "temp_dot_file.dot"; // this file's gonna be destroyed
    const char* svg_filename = "temp_svg_file.svg"; // this file's gonna be destroyed

    ListGenerateDotFile(list, dot_filename);

    // creating svg
    char temp_term_command[1969] = {};

    sprintf(temp_term_command, "dot -Tsvg %s -o %s", dot_filename, svg_filename);

    const char* terminal_command = temp_term_command;

    system(terminal_command);

    FILE* html_file = fopen(html_filename, "w");
    FILE* svg_file = fopen(svg_filename, "r");

    assert(html_file);
    assert(svg_file);

    fprintf(html_file,
    "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <title>List Dump</title>\n"
    "    <style>\n"
    "        body { font-family: Arial, sans-serif; background: #f8fafc; padding: 30px; color: #1e293b; }\n"
    "        .container {\n"
    "            max-width: 1200px;\n"
    "            margin: 0 auto;\n"
    "            background: white;\n"
    "            padding: 24px;\n"
    "            border-radius: 8px;\n"
    "            border: 1px solid #e2e8f0;\n"
    "            box-shadow: 0 1px 3px rgba(0,0,0,0.1);\n"
    "            text-align: center;\n"
    "        }\n"
    "        h2 { margin-top: 0; color: #0f172a; text-align: center; }\n"
    "        \n"
    "        /* Красивая штриховая линия из тире на всю ширину контейнера */\n"
    "        .dash-line {\n"
    "            border: none;\n"
    "            border-top: 2px dashed #94a3b8;\n" // Сделали цвет чуть темнее для четкости
    "            margin: 25px 0;\n"
    "            width: 100%%;\n"
    "        }\n"
    "        \n"
    "        .light-log {\n"
    "            display: inline-block;\n"
    "            font-family: 'Courier New', monospace;\n"
    "            font-size: 14px;\n"
    "            line-height: 1.6;\n"
    "            color: #334155;\n"
    "            text-align: left;\n" // Текст внутри выровнен, но сам блок по центру
    "            margin: 0 auto;\n"
    "            padding: 0;\n"
    "        }\n"
    "        .graph-container { overflow-x: auto; padding: 15px; border: 1px solid #f1f5f9; background: #fafafa; border-radius: 6px; }\n"
    "        svg { max-width: 100%%; height: auto; display: block; margin: 0 auto; }\n"
    "    </style>\n</head>\n<body>\n"
    "    <div class=\"container\">\n"
    "        <h2>List structure dump number 1</h2>\n"
    "        <hr class=\"dash-line\">\n" // ЛИНИЯ ИЗ ТИРЕ 1: Под заголовком
    );


    ListStatus list_status = ListVerify(list);
    char* str_list_status = (char*)calloc(169, sizeof(char));
    SprintAllErrors(str_list_status, list_status);

    char* html_list_status = (char*)calloc(169, sizeof(char));;
    if (list_status == OKAY)
        sprintf(html_list_status, "<span style=\"color: #4ade80; font-weight: bold;\">LIST IS OKAY</span>");

    else
        sprintf(html_list_status, "<span style=\"color: #f87171; font-weight: bold;\">%s</span>", str_list_status);

    fprintf(html_file, 
    "        <pre class=\"light-log\">\n"
    "ListDump was called in %s:%d in function %s()\n"
    "list_status = %s\n"
    "Size:      %d\n"
    "Capacity:  %d\n"
    "Head:      %d\n"
    "Tail:      %d\n"
    "Free head: %d\n"
    "data [%p]\n"
    "        </pre>\n\n",
    file, line, func, html_list_status, list->size, list->capacity, list->head, list->tail, list->free_head, list->data
    );

    fprintf(html_file, 
    "        <hr class=\"dash-line\">\n"
    "        <div class=\"graph-container\">\n"
    );

    int ch = 0;
    while ((ch = fgetc(svg_file)) != EOF)  fputc(ch, html_file);  // copying svg file to html

    fprintf(html_file, 
        "        </div>\n"
        "    </div>\n"
        "</body>\n</html>\n"
    );

    fclose(svg_file);
    fclose(html_file);

    remove(dot_filename);
    remove(svg_filename);

    dump_counter++;
    return 0;
}

int ListGraphDump(List* list,
                  const char* html_filename, const char* file, int line, const char* func)
{
    assert(list);

    ListGenerateHtmlFile(list, html_filename, file, line, func);
    
    return 0;
}