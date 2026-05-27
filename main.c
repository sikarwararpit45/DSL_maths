#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "compiler.h"
#include "vm.h"
#include "ast_printer.h"

#define MAX_SOURCE_SIZE 8192

static void run_source(const char* source)
{
    Parser parser;

    parser_init(&parser, source);

    ASTNode* program = parse_program(&parser);

    Bytecode bc = {0};

    compile(program, &bc);

    bc.code[bc.code_size++] = OP_HALT;

    vm_run(&bc);

    printf("\n===== AST =====\n");
    print_ast(program, 0);
    printf("================\n\n");
}

static void run_file(const char* filename)
{
    FILE* file = fopen(filename, "r");

    if (!file)
     {
        printf("Could not open file: %s\n", filename);
        return;
    }

    char source[MAX_SOURCE_SIZE];

    int size = fread(source, 1, sizeof(source) - 1, file);

    source[size] = '\0';

    fclose(file);

    run_source(source);
}

static void repl()
{
    char input[1024];

    printf("Educational DSL Compiler + VM\n");

    while (1)
    {
        printf("dsl> ");

        fgets(input, sizeof(input), stdin);

        if (strncmp(input, "exit", 4) == 0)
        {
            break;
        }

        run_source(input);
    }
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        run_file(argv[1]);
    }
    else
    {
        repl();
    }

    return 0;
}