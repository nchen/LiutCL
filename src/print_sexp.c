/*
 * print_sexp.c
 *
 * Prints an inner structures of a S-expression to screen.
 *
 * Copyright (C) 2012-10-03 liutos
 */
#include "types.h"
#include "atom_proc.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern void print_cons(LispObject);

void print_sexp_notln(LispObject);
void print_cons(LispObject);

void print_atom(Atom atom)
{
    switch (TYPE(atom)) {
    case SYMBOL:
	/* printf("%s", atom->symbol_name); */
        {
            int i;

            for (i = 0; atom->symbol_name[i] != '\0'; i++)
                printf("%c", toupper(atom->symbol_name[i]));
        }
	break;
    case INTEGER:
	printf("%d", INTEGER(atom));
	break;
    case FUNCTION:
	if (TRUE == FUNC_FLAG(atom))
	    printf("#<FUNCTION C %p>", atom);
	else {
	    printf("#<FUNCTION I ");
	    print_cons(PARAMETERS(atom));
	    putchar(' ');
	    print_sexp_notln(EXPRESSION(atom));
	    putchar(' ');
	    printf("%p>", atom);
	}
	break;
    case STRING:
        printf("\"%s\"", atom->string);
        break;
    default :
	fprintf(stderr, "Unknown type '%d'\n", TYPE(atom));
	exit(1);
    }
}

void print_cons_core(LispObject cons)
{
    while (!is_tail(cons)) {
        /* The variable `cons' here would never be the lt_void
           so it's safe to use the macro CAR instead of SCAR. */
	if (CAR(cons)->type != CONS)
	    print_atom(CAR(cons));
	else
	    print_cons(CAR(cons));
	cons = CDR(cons);
	if (is_atom_object(cons)) {
	    if (cons != lt_nil) {
		printf(" . ");
		print_atom(cons);
	    }
	} else
            putchar(' ');
    }
}

void print_cons(LispObject cons)
{
    putchar('(');
    print_cons_core(cons);
    putchar(')');
}

void print_sexp_notln(LispObject object)
{
    if (CONS == object->type)
	print_cons(object);
    else
	print_atom(object);
}

void print_sexp(LispObject object)
{
    if (NULL == object) {
        printf("; No value\n");
        return;
    }
    print_sexp_notln(object);
    putchar('\n');
}