#include <stdio.h>
#include "parser.h"

static int pos = 1; // Position of currect input symbol
static char lookahead = 0; // Buffer for currect input symbol
static Token *lookatoken = 0; // Buffer for currect input token

int main(int argc, char **argv)
{
	printf("Enter infix expression: ");

	S();
	printf("\n");

	return 0;
}

void S(void)
{
	A();
	s();
}

void s(void)
{
	Token *token = get_next_token();

	if (token->type == TOKTYPE_PLUS) {
		Token_Delete(token);
		A();
		printf("+ ");
		s();
	} else if (token->type == TOKTYPE_MINUS) {
		Token_Delete(token);
		A();
		printf("- ");
		s();
	} else
		uget_token(token);
}

void A(void)
{
	B();
	a();
}

void a(void)
{
	Token *token = get_next_token();

	if (token->type == TOKTYPE_MUL) {
		Token_Delete(token);
		B();
		printf("* ");
		a();
	} else if (token->type == TOKTYPE_DIV) {
		Token_Delete(token);
		B();
		printf("/ ");
		a();
	} else
		uget_token(token);
}

void B(void)
{
	factor();
	b();
}

void b(void)
{
	Token *token = get_next_token();

	if (token->type == TOKTYPE_POWER) {
		Token_Delete(token);
		factor();
		printf("^ ");
		b();
	} else
		uget_token(token);
}

void factor(void)
{
	Token *token = get_next_token();

	switch (token->type) {
		case TOKTYPE_VAR:
		case TOKTYPE_NUM:
			printf("%s ", token->lexem);
			Token_Delete(token);
			break;
		case TOKTYPE_BRACE_OPEN:
			Token_Delete(token);
			S();

			token = get_next_token();
			if (token->type != TOKTYPE_BRACE_CLOSE)
				syntax_error("Must be ')'", token);

			break;
		case TOKTYPE_MINUS:
			Token_Delete(token);
			printf("0 ");
			S();
			printf("- ");
			break;
		default:
			Token_Delete(token);
			syntax_error("Unknown syntax", token);
			break;
	}
}



static Token *Token_New(int type, char *lexem)
{
	Token *this = (Token *) malloc(sizeof(Token));
	if (!this)
		return NULL;

	this->pos = pos-1;
	this->type = type;

	if (lexem)
		strcpy(this->lexem, lexem);

	return this;
}

static void Token_Delete(Token *this)
{free(this);}

static char get_next_char(void)
{
	pos++;
	return fgetc(stdin);
}

static void lex_error(char *msg)
{
	fprintf(stderr, "%s in position %d\n", msg, pos-1);
	exit(1);
}

static void syntax_error(char *msg, Token *token)
{
	fprintf(stderr, "%s in position %d\n", msg, token->pos);
	exit(1);
}

static Token *get_next_token(void)
{
	if (lookatoken) {
		Token *token = lookatoken;
		lookatoken = 0;
		return token;
	}

	while (lookahead == 0 || lookahead == ' ' || lookahead == '\t')
		lookahead = get_next_char();

	switch (lookahead) {
		case '+':
			lookahead = 0;
			return Token_New(TOKTYPE_PLUS, NULL);
		case '-':
			lookahead = 0;
			return Token_New(TOKTYPE_MINUS, NULL);
		case '/':
			lookahead = 0;
			return Token_New(TOKTYPE_DIV, NULL);
		case '*':
			lookahead = 0;
			return Token_New(TOKTYPE_MUL, NULL);
		case '^':
			lookahead = 0;
			return Token_New(TOKTYPE_POWER, NULL);
		case '(':
			lookahead = 0;
			return Token_New(TOKTYPE_BRACE_OPEN, NULL);
		case ')':
			lookahead = 0;
			return Token_New(TOKTYPE_BRACE_CLOSE, NULL);
		case '\n':
			lookahead = 0;
			return Token_New(TOKTYPE_EOF, NULL);
		case 'a'...'z':
		case 'A'...'Z': 
			return parseVar();
		case '0'...'9': 
			return parseNum();
		default:
			lex_error("Unknown lexem");
			return NULL;
	}
}

static void uget_token(Token *token)
{
	lookatoken = token;
}

static Token *parseVar(void)
{
	char lexem[LEXEM_LENGTH] = {0};
	int i = 0;

	do {
		lexem[i++] = lookahead;
		lookahead = get_next_char();
	} while (isalpha(lookahead));

	while (isdigit(lookahead)) {
		lexem[i++] = lookahead;
		lookahead = get_next_char();
	}

	return Token_New(TOKTYPE_VAR, lexem);
}

static Token *parseNum(void)
{
	char lexem[LEXEM_LENGTH] = {0};
	int i = 0;

	do {
		lexem[i++] = lookahead;
		lookahead = get_next_char();
	} while (isdigit(lookahead));

	if (lookahead == '.') {
		lookahead = get_next_char();

		if (isdigit(lookahead))
			lex_error("Bad number");

		do {
			lexem[i++] = lookahead;
			lookahead = get_next_char();
		} while (isdigit(lookahead));
	}

	if (isalpha(lookahead))
		lex_error("Unknown symbol");

	return Token_New(TOKTYPE_NUM, lexem);
}
