#include "token.h"

Token token_(int type, char *lexem, int pos)
{
	Token token;
	token_init(&token, type, lexem, pos);

	return token;
}

Token *token_init(Token *this, int type, char *lexem, int pos)
{
	this->pos = pos;
	this->type = type;

	if (lexem)
		strncpy(this->lexem, lexem, LEXEM_LENGTH);

	return this;
}

Token *token_copy(Token *this, Token *token)
{
	this->pos = token->pos;
	this->type = token->type;
	strncpy(this->lexem, token->lexem, LEXEM_LENGTH);

	return this;
}

Token *token_new(void)
{
	Token *this = (Token *) malloc(sizeof(Token));
	if (!this)
		return NULL;

	this->pos = -1;
	this->type = 0;
	for (int i = 0; i < LEXEM_LENGTH; i++)
		this->lexem[i] = '\0';

	return this;
}

void token_delete(Token *this)
{
	free(this);
}

