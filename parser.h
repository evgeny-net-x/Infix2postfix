#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define LEXEM_LENGTH 10

/*
	LL(1) GRAMMAR:

	S -> S '+' A
	   | S '-' A
	   | A

	A -> A '*' B
	   | A '/' B
	   | B

	B -> B '^' factor
	   | factor

	factor -> 'num'
	   | 'var'
	   | '(' S ')'
	   | '-' S


	GRAMMAR WITHOUT LEFT RECURSION:
	S -> A s

	s -> '+' A s
	   | '-' A s
	   | eps

	A -> B a

	a -> '*' B a
	   | '/' B a
	   | eps

	B -> factor b

	b -> '^' factor b
	   | eps

	factor -> 'num'
	   | 'var'
	   | '(' S ')'
	   | '-' S
*/

enum { // TOKTYPE - TOKEN TYPE
	TOKTYPE_EOF = 100,

	TOKTYPE_PLUS,
	TOKTYPE_MINUS,
	TOKTYPE_MUL,
	TOKTYPE_DIV,
	TOKTYPE_POWER,

	TOKTYPE_VAR,
	TOKTYPE_NUM,

	TOKTYPE_BRACE_OPEN,
	TOKTYPE_BRACE_CLOSE
};

typedef struct Token {
	int pos;
	int type;
	char lexem[LEXEM_LENGTH];
} Token;

void S(void);
void s(void);

void A(void);
void a(void);

void B(void);
void b(void);

void factor();

static Token *Token_New(int, char *);
static void Token_Delete(Token *);

static Token *get_next_token(void);
static char get_next_char(void);
static void uget_token(Token *);
static void lex_error(char *);
static void syntax_error(char *, Token *);
static Token *parseVar(void);
static Token *parseNum(void);

#endif
