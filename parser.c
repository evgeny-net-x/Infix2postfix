#include "parser.h"

static int pos = 0; // Position of currect input symbol
static Token *lookatoken = NULL; // Buffer for currect input token

char *functionNames[] = {
	"sqrt",
	"cbrt",

	"abs",
	"ceil",
	"floor", 
	"round", 

	"cos", 
	"sin", 
	"tg",
	"tan",
	"acos",
	"asin",
	"arctg",
	"atan",

	"ln",
	"log2",
	"log10"
};

void parse(void)
{
	S();
}

static void S(void)
{
	A();
	s();
}

static void s(void)
{
	Token token;
	get_next_token(&token);

	if (token.type == TOKEN_TYPE_PLUS) {
		A();
		printf("+ ");
		s();
	} else if (token.type == TOKEN_TYPE_MINUS) {
		A();
		printf("- ");
		s();
	} else
		unget_token(&token);
}

static void A(void)
{
	B();
	a();
}

static void a(void)
{
	Token token;
	get_next_token(&token);

	if (token.type == TOKEN_TYPE_MUL) {
		B();
		printf("* ");
		a();
	} else if (token.type == TOKEN_TYPE_DIV) {
		B();
		printf("/ ");
		a();
	} else if (token.type == TOKEN_TYPE_MOD) {
		B();
		printf("%% ");
		a();
	} else
		unget_token(&token);
}

static void B(void)
{
	Token token;
	get_next_token(&token);

	if (token.type == TOKEN_TYPE_MINUS) {
		printf("0 ");
		C();
		printf("- ");
	} else {
		unget_token(&token);
		C();
	}
}

static void C(void)
{
	factor();
	c();
}

static void c(void)
{
	Token token;
	get_next_token(&token);

	if (token.type == TOKEN_TYPE_POWER) {
		get_next_token(&token);

		if (token.type == TOKEN_TYPE_MINUS) {
			printf("0 ");
			factor();
			printf("- ");
		} else {
			unget_token(&token);
			factor();
		}

		printf("^ ");
		c();
	} else
		unget_token(&token);
}

static void factor(void)
{
	Token token;
	get_next_token(&token);

	switch (token.type) {
		case TOKEN_TYPE_E:
		case TOKEN_TYPE_PI:
		case TOKEN_TYPE_NUM:
		case TOKEN_TYPE_VAR:
			printf("%s ", token.lexem);
			break;
		case TOKEN_TYPE_FUNCNAME:
			printf("%s ", token.lexem);

			get_next_token(&token);
			if (token.type != TOKEN_TYPE_BRACE_OPEN) {
				unget_token(&token);
				break;
			}

			S();

			get_next_token(&token);
			if (token.type != TOKEN_TYPE_BRACE_CLOSE)
				syntax_error("Must be ')' for function call", &token);

			printf("() ");

			break;
		case TOKEN_TYPE_BRACE_OPEN:
			S();

			get_next_token(&token);
			if (token.type != TOKEN_TYPE_BRACE_CLOSE)
				syntax_error("Must be ')'", &token);

			break;
		default:
			syntax_error("Unknown syntax", &token);
			break;
	}
}

static void lex_error(char *msg, char sym)
{
	fprintf(stderr, "%s in position %d (%c)\n", msg, pos, sym);
	exit(1);
}

static void syntax_error(char *msg, Token *token)
{
	fprintf(stderr, "%s in position %lu (%s)\n", msg, token->pos - strlen(token->lexem), token->lexem);
	exit(1);
}

static void unget_token(Token *token)
{
	if (!lookatoken)
		lookatoken = token_new();

	token_copy(lookatoken, token);
}

static char get_next_char(void)
{
	pos++;
	char sym = fgetc(stdin);

	if (sym == ' ' || sym == '\t') {
		do {
			pos++;
			sym = fgetc(stdin);
		} while (sym == ' ' || sym == '\t');

		pos--;
		ungetc(sym, stdin);
		return ' ';
	}

	return sym;
}

static void unget_char(char sym)
{
	pos--;
	ungetc(sym, stdin);
}

static void get_next_token(Token *token)
{
	if (lookatoken) {
		token_copy(token, lookatoken);

		token_delete(lookatoken);
		lookatoken = NULL;
		return;
	}

	char sym = get_next_char();
	if (sym == ' ')
		sym = get_next_char();

	switch (sym) {
		case '+':
			token_init(token, TOKEN_TYPE_PLUS, NULL, pos);
			break;
		case '-':
			token_init(token, TOKEN_TYPE_MINUS, NULL, pos);
			break;
		case '*':
			token_init(token, TOKEN_TYPE_MUL, NULL, pos);
			break;
		case '/':
			token_init(token, TOKEN_TYPE_DIV, NULL, pos);
			break;
		case '%':
			token_init(token, TOKEN_TYPE_MOD, NULL, pos);
			break;
		case '^':
			token_init(token, TOKEN_TYPE_POWER, NULL, pos);
			break;
		case '(':
			token_init(token, TOKEN_TYPE_BRACE_OPEN, NULL, pos);
			break;
		case ')':
			token_init(token, TOKEN_TYPE_BRACE_CLOSE, NULL, pos);
			break;
		case '\n':
			token_init(token, TOKEN_TYPE_EOF, NULL, pos);
			break;
		case 'a'...'z':
		case 'A'...'Z': 
			unget_char(sym);
			parseName(token);

			break;
		case '0'...'9':
			unget_char(sym);
			parseNum(token);
			break;
		default:
			lex_error("Unknown lexem", sym);
			token_init(token, TOKEN_TYPE_NULL, NULL, -1);
			break;
	}
}

static void parseName(Token *token)
{
	char lexem[LEXEM_LENGTH] = {0};
	int i = -1;

	do
		lexem[++i] = get_next_char();
	while (isalpha(lexem[i]) && i < LEXEM_LENGTH-1);

	while (isdigit(lexem[i]) && i < LEXEM_LENGTH-1)
		lexem[++i] = get_next_char();

	char sym = lexem[i];
	lexem[i] = '\0';

	if (isFunctionName(lexem))
		token_init(token, TOKEN_TYPE_FUNCNAME, lexem, pos);
	else if (tolower(lexem[0]) == 'p' && tolower(lexem[1]) == 'i' && lexem[2] == '\0')
		token_init(token, TOKEN_TYPE_PI, lexem, pos);
	else if (tolower(lexem[0]) == 'e' && lexem[1] == '\0')
		token_init(token, TOKEN_TYPE_E, lexem, pos);
	else {
		if (sym == ' ')
			sym = get_next_char();

		unget_char(sym);
		if (sym == '(')
			unget_char('*');

		token_init(token, TOKEN_TYPE_VAR, lexem, pos);
	}
}

static void parseNum(Token *token)
{
	char lexem[LEXEM_LENGTH] = {0};
	int i = -1;

	do {
		lexem[++i] = get_next_char();
	} while (isdigit(lexem[i]) && i < LEXEM_LENGTH-1);

	if (lexem[i] == '.' || lexem[i] == ',') {
		lexem[++i] = get_next_char();
		if (!isdigit(lexem[i]))
			lex_error("Bad number", lexem[i]);

		do
			lexem[++i] = get_next_char();
		while (isdigit(lexem[i]) && i < LEXEM_LENGTH-1);
	}

	unget_char(lexem[i]);

	if (isalpha(lexem[i]) || lexem[i] == '(') // добавляем в поток неявный * между числом и переменной/функцией/скобкой
		unget_char('*');

	lexem[i] = '\0';
	token_init(token, TOKEN_TYPE_NUM, lexem, pos);
}

static int isFunctionName(char *name)
{
	for (int i = 0; i < strlen(name); i++)
		name[i] = tolower(name[i]);

	for (int i = 0; i < sizeof(functionNames)/sizeof(functionNames[0]); i++)
		if (strcmp(name, functionNames[i]) == 0)
			return 1;

	return 0;
}

