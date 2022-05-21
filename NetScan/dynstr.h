#pragma once

typedef struct STRING {
	char* str;
	int nSize;
} STRING;

STRING InitString();
void AppendString(STRING* s, const char* appendStr);
void FreeString(STRING* s);
