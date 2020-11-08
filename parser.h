#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include "token.h"

enum {
	TOKEN_TYPE_NULL = 0,
	TOKEN_TYPE_EOF,

	TOKEN_TYPE_PLUS,
	TOKEN_TYPE_MINUS,
	TOKEN_TYPE_MUL,
	TOKEN_TYPE_DIV,
	TOKEN_TYPE_MOD,
	TOKEN_TYPE_POWER,

	TOKEN_TYPE_E,
	TOKEN_TYPE_PI,
	TOKEN_TYPE_VAR,
	TOKEN_TYPE_NUM,
	TOKEN_TYPE_FUNCNAME,

	TOKEN_TYPE_BRACE_OPEN,
	TOKEN_TYPE_BRACE_CLOSE
};

/*
	LL(1) GRAMMAR:

	S -> S '+' A
	   | S '-' A
	   | A

	A -> A '*' B
	   | A '/' B
	   | A '%' B
	   | B

	B -> '-' C
	   | C

	C -> C '^' factor
	   | C '^' '-' factor
	   | factor

	factor -> 'num'
	   | '(' S ')'
	   | 'var'
	   | 'funcName' '(' S ')'


	GRAMMAR WITHOUT LEFT RECURSION:
	S -> A s

	s -> '+' A s
	   | '-' A s
	   | eps

	A -> B a

	a -> '*' B a
	   | '/' B a
	   | '%' B a
	   | eps

	B -> '-' C
	   | C

	C -> factor c

	c -> '^' factor c
	   | '^' '-' factor c
	   | eps

	factor -> 'num'
	   | '(' S ')'
	   | 'var'
	   | 'funcName' '(' S ')'
*/

void parse(void);

static void S(void);
static void s(void);

static void A(void);
static void a(void);

static void B(void);

static void C(void);
static void c(void);

static void factor(void);

static void get_next_token(Token *);
static char get_next_char(void);
static void unget_char(char);
static void unget_token(Token *);

static void parseName(Token *);
static void parseNum(Token *);

static void lex_error(char *, char);
static void syntax_error(char *, Token *);

static int isFunctionName(char *);

#endif
