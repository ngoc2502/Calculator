#include<iostream>
#include<fstream>
#include<string>
#include"stdio.h"
#pragma warning(disable:4996)

using namespace std;

struct Snode
{
	char key;
	float num;
	Snode* pnext;
};

struct stack
{
	Snode* ptop;
};

Snode* newnode(char val)
{
	Snode* node = new Snode;
	if (node == nullptr) return nullptr;
	node->key = val;
	node->num = 0;
	node->pnext = nullptr;
}

Snode* newnodeNum(float val)
{
	Snode* node = new Snode;
	if (node == nullptr) return nullptr;
	node->num = val;
	node->key = '\0';
	node->pnext = nullptr;
}

void initialize(stack** r_stack)
{
	*r_stack = new stack{ nullptr };
}

bool isEmpty(stack* r_stack)
{
	if (r_stack->ptop == nullptr) return true;
	return false;
}

void push(stack* u_stack, char r_val)
{
	Snode* n = newnode(r_val);
	if (isEmpty(u_stack))
	{
		u_stack->ptop = n;
		return;
	}
	n->pnext = u_stack->ptop;
	u_stack->ptop = n;
	return;
}

void pushNum(stack* u_stack, float r_val)
{
	Snode* n = newnodeNum(r_val);
	if (isEmpty(u_stack))
	{
		u_stack->ptop = n;
		return;
	}
	n->pnext = u_stack->ptop;
	u_stack->ptop = n;
	return;
}

char pop(stack* u_stack)
{
	if (isEmpty(u_stack)) return '\0';
	char ch = u_stack->ptop->key;
	Snode* temp = u_stack->ptop->pnext;
	delete u_stack->ptop;
	u_stack->ptop = temp;
	return ch;
}

float  popNum(stack* u_stack)
{
	if (isEmpty(u_stack)) return '\0';
	float num = u_stack->ptop->num;
	Snode* temp = u_stack->ptop->pnext;
	delete u_stack->ptop;
	u_stack->ptop = temp;
	return num;
}

char peek(stack* r_stack)
{
	if (isEmpty(r_stack))
		return '\0';
	return r_stack->ptop->key;
}

void destack(stack* s)
{
	for (Snode* n = s->ptop; s->ptop != nullptr; n->pnext)
	{
		pop(s);
	}
	return;
}

int precedence(char x)
{
	if (x == '\0') return -1;
	if (x == '(') return 0;
	if (x == '+' || x == '-') return 1;
	if (x == '*' || x == '/' || x == '%') return 2;
	if (x == '^') return 3;
	return -2;
}

void infix_to_postfix(char* infix, char* postfix)
{
	stack* s = nullptr;
	initialize(&s);
	int  po = 0;
	char x;
	char token = '\0';
	for (int i = 0; infix[i] != '\0'; i++)
	{
		token = infix[i];
		if (token >= '0' && token <= '9' || token == '.')
		{
			postfix[po++] = token;
		}
		else
			if (token == '(') push(s, token);
			else
				if (token == ')')
				{
					while (x = peek(s) != '(')
					{
						postfix[po++] = pop(s);
					}
					if (peek(s) != '(') return;
					else pop(s);
				}
				else
				{
					postfix[po++] = ' ';
					while (precedence(token) <= precedence(x = peek(s)) && !isEmpty(s))
					{
						postfix[po++] = pop(s);
					}
					push(s, token);
				}
	}
	while (!isEmpty(s))
	{
		if (peek(s) != '(')
		{
			x = pop(s);
			postfix[po++] = x;
		}
		else pop(s);
	}
	postfix[po++] = '\0';
	destack(s);
}

int postfix_to_val(char* postfix, float& result)
{

	stack* s = nullptr;
	initialize(&s);
	int i = 0;
	float val1 = 0.0;
	float val2 = 0.0;
	char* p;
	p = &postfix[0];
	while (*p != '\0')
	{
		while (*p == ' ')
			p++;
		if (*p >= '0' && *p <= '9')
		{
			float num = 0;
			while (*p >= '0' && *p <= '9')
			{
				num = num * 10 + *p - 48;
				p++;
			}
			if (*p == '.')
			{
				p++;
				float temp = 10;
				while (*p >= '0' && *p <= '9')
				{
					num = num + float(*p - 48) / temp;
					temp = temp * 10;
					p++;
				}
			}
			pushNum(s, num);
		}
		else
		{
			val2 = popNum(s);
			val1 = popNum(s);
			switch (*p)
			{
			case '+':
				result = val1 + val2;
				break;
			case '-':
				result = val1 - val2;
				break;
			case '/':
				if (val2 == 0.0)
				{
					destack(s);
					return -1;
					break;
				}
				else
				{
					result = float(val1) / val2;
					break;
				}
			case '*':
				result = val1 * val2;
				break;
			case '^':
				result = pow(val1, val2);
				break;
			default:
				destack(s);
				return 1;
			}
			pushNum(s, result);
			p++;
		}
	}
	result = popNum(s);
	destack(s);
	return 0;
}
int postfix_to_val2(char postfix[], float& result)
{
	stack* s = nullptr;
	initialize(&s);
	int i = 0;
	float val1 = 0.0;
	float val2 = 0.0;
	char* p;
	p = &postfix[0];
	while (*p != '\0')
	{
		if (*p >= '0' && *p <= '9')
		{
			float num = 0.0;
			num = *p - float(48);
			pushNum(s, num);
			p++;
		}
		else
		{
			val2 = popNum(s);
			val1 = popNum(s);
			switch (*p)
			{
			case '+':
				result = val1 + val2;
				break;
			case '-':
				result = val1 - val2;
				break;
			case '/':
				if (val2 == 0.0)
				{
					destack(s);
					return -1;
				}
				else
				{
					result = float(val1) / val2;
					break;
				}
			case '*':
				result = val1 * val2;
				break;
			case '^':
				result = pow(val1, val2);
				break;
			default:
				destack(s);
				return 1;
			}
			pushNum(s, result);
			p++;
		}
	}
	result = popNum(s);
	destack(s);
	return 0;
}
int main(int argc, char* argv[])
{
	if (argc > 4)
	{
		cout << "Too much parameters!";
		return -1;
	}
	if (argc < 4)
	{
		cout << "Need more parameters!";
		return -1;
	}
	float result = 0.0;

	FILE* fin = nullptr;
	FILE* fout = nullptr;

	fin = fopen(argv[2], "r");
	if (!fin)
	{
		cout << "Error open filein!";
		return -1;
	}
	fout = fopen(argv[3], "w");
	if (!fout)
	{
		cout << "Error open fileout!";
		return -1;
	}
	char* infix = new char[100];
	char* posfix = new char[100];
	if (strcmp(argv[1], "-i") == 0)
	{
		fscanf(fin, "%s", infix);
		infix_to_postfix(infix, posfix);
		postfix_to_val(posfix, result);
		cout << "\nCheck result at " << argv[3]<<endl;
		if (postfix_to_val(posfix, result) == -1) fprintf(fout, "%s", "Division by zero!");
		else if (postfix_to_val(posfix, result) == 1) fprintf(fout, "%s", "Invalid operator!");
		else
			fprintf(fout, "%f", result);
	}
	else
	{
		fscanf(fin, "%s", posfix);
		postfix_to_val2(posfix, result);
		cout << "\nCheck result at " << argv[3]<<endl;
		if (postfix_to_val2(posfix, result) == -1) fprintf(fout, "%s", "Division by zero!");
		else if (postfix_to_val2(posfix, result) == 1) fprintf(fout, "%s", "Invalid operator!");
		else
			fprintf(fout, "%f", result);
	}

	fclose(fin);
	fclose(fout);
	delete[]infix;
	infix = nullptr;
	delete[] posfix;
	posfix = nullptr;
	return 0;
}