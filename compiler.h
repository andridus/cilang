#ifndef CILANGCOMPILER_H
#define CILANGCOMPILER_H

#include <stdio.h>
#include <stdbool.h>

struct pos
{
  int line;
  int col;
  const char* filename;
}

enum
{
  TOKEN_TYPE_IDENTIFIER,
  TOKEN_TYPE_KEYWORD,
  TOKEN_TYPE_OPERATOR,
  TOKEN_TYPE_SYMBOL,
  TOKEN_TYPE_NUMBER,
  TOKEN_TYPE_STRING,
  TOKEN_TYPE_COMMENT,
  TOKEN_TYPE_NEWLINE
};

struct token
{
  int type;
  int flags;

  union
  {
    char cval;
    const char* sval;
    unsigned int inum;
    unsigned long lnum;
    unsigned long long llnum;
    void* any;
  };

  // True if their is whitespace between the token and the next token
  // i.e * a for operator token * would mean whitespace would be set for token "a"
  bool whitespace;

  // (Hello world)
  const char* between_brackets;
};

struct lex_process;
typedef char (*LEX_PROCESS_NEXT_CHAR)(struct lex_process* process);
typedef char (*LEX_PROCESS_PEEK_CHAR)(struct lex_process* process);
typedef void (*LEX_PROCESS_PUSH_CHAR)(struct lex_process* process, char c);
struct lex_process_functions
{
  LEX_PROCESS_NEXT_CHAR next_char;
  LEX_PROCESS_PEEK_CHAR peek_char;
  LEX_PROCESS_PUSH_CHAR push_char;
};

struct lex_process
{
  struct pos pos;
  struct vector* token_vec;
  struct compiler_process* compiler;

  /*
  *
  *
  * ((50))
  */
  int current_expression_count;
  struct buffer* parenthenses_buffer;
  struct lex_process_functions* function;

  // This will be private data that the lexer does not understand
  // but the person using the lexer does understand.
  void* private;
}

enum
{
  COMPILER_FILE_COMPILED_OK,
  COMPILER_FAILED_WITH_ERRORS
};

struct compile_process {
  // The flags in regards to how this file shlould be compiled
  int flags;

  struct compile_process_input_file
  {
    FILE* fp;
    const char* abs_path;
  } cfile;

  FILE* ofile;
};
int compile_file(const char* filename, const char* out_filename, int flags);
struct compile_process* compile_process_create(const char* fielanme, const char* filename_out, int flags);
#endif