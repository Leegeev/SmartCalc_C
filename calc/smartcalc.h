#ifndef S21_SMARTCALC
#define S21_SMARTCALC

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 257  // максимальный размер стека
#define ERROR 1
#define OPER 0
#define NUM 1

typedef struct {
  double nums[MAX_SIZE];  // стакан с числами
  int opers[MAX_SIZE];    // стакан с операторами
  int cur[MAX_SIZE];  // 1 - число, 0 - оператор, -1 - ошибка / пустота.
  int priority[MAX_SIZE];
  int top;
} stack;

// Stack
void print_stack(stack s);
void initStack(stack *s);
int isEmpty(stack *s);
int isFull(stack *s);
int push_num(stack *s, double newItem);
int push_oper(stack *s, int newItem, int priority);
int pop(stack *s);
int peek_cur(stack *s);
double peek_num(stack *s);
int peek_oper(stack *s);

// Parcer
int parcer(const char *exp, stack *polsk);
int char_to_num(const char *exp, int *pos, stack *num);
int char_to_oper(const char *exp, int *pos, stack *polsk);
int is_op(char c);
int is_func(const char *exp, int *pos);
int which_func(const char *exp, int *pos);
int check_priority(int op);

// Filter
int filter(stack *e);
int ops(int sym);
int isgjigit(int sym);
int function_gap(int num);
int check_brace(int sym);  // -1 0 1
int check_empty_braces(stack e, int pos);
int is_unary(int sym);
int check_oper(stack e, int pos);
int check_num(stack e, int pos);

// Polish_notation
void polish_notation(stack *exp, stack *polsk);

// Calculate
stack make_polsk(const char *expression, int *state);
double calculate(stack polsk, double x);
double calc_func(int func, double num);
double calc_op(double n1, double n2, int op);

// Main func
double mysmartcalc(const char *expression, int *state, double x);

typedef enum typ {
  number = 1,
  s21_x,
  open_brace,
  close_brace,
  s21_mod,
  s21_ln,
  s21_log,
  s21_cos,
  s21_sin,
  s21_tan,
  s21_acos,
  s21_asin,
  s21_atan,
  s21_sqrt,
} type_t;

// BONUS PART
double *difference_credit_calc(double sum, double duration, double percentage);
double *annuitet_credit_calc(double sum, double duration, double percentage);

#endif  // S21_SMARTCALC
