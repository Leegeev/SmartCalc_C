#include "smartcalc.h"

stack make_polsk(const char *expression, int *state) {
  stack tokens = {0}, polsk = {0};
  initStack(&tokens);
  initStack(&polsk);
  *state = parcer(expression, &tokens);

  if (!*state) {
    *state = filter(&tokens);
  }

  if (!*state) {
    polish_notation(&tokens, &polsk);
  }
  return polsk;
}

double mysmartcalc(const char *expression, int *state, double x) {
  stack polsk = make_polsk(expression, state);
  double res = 0.;
  if (!(*state)) {
    res = calculate(polsk, x);
    if (isnan(res) || isinf(res)) *state = 1;
  }
  return res;
}

double calculate(stack polsk, double x) {
  stack tmp = {0};
  double num1 = 0., num2 = 0.;
  initStack(&tmp);
  for (int i = 0; (polsk.cur[i] != -1); i++) {
    if (polsk.cur[i] == 2) {
      polsk.cur[i] = 1;
      polsk.nums[i] = x;
    }
    if (check_num(polsk, i)) {
      push_num(&tmp, polsk.nums[i]);
    } else if (function_gap(polsk.opers[i])) {
      num1 = calc_func(polsk.opers[i], peek_num(&tmp));
      pop(&tmp);
      push_num(&tmp, num1);
    } else if (polsk.opers[i] == '~' || polsk.opers[i] == '#') {
      num1 = peek_num(&tmp);
      if (polsk.opers[i] == '~') num1 *= -1;
      pop(&tmp);
      push_num(&tmp, num1);
    } else {
      num1 = peek_num(&tmp);
      pop(&tmp);
      num2 = peek_num(&tmp);
      pop(&tmp);
      push_num(&tmp, calc_op(num2, num1, polsk.opers[i]));
    }
  }
  return peek_num(&tmp);
}

double calc_op(double n1, double n2, int op) {
  switch (op) {
    case s21_mod: {
      n1 = fmod(n1, n2);
      break;
    }
    case '*': {
      n1 = n1 * n2;
      break;
    }
    case '/': {
      n1 = n1 / n2;
      break;
    }
    case '-': {
      n1 = n1 - n2;
      break;
    }
    case '+': {
      n1 = n1 + n2;
      break;
    }
    case '^': {
      n1 = pow(n1, n2);
      break;
    }
  }
  return n1;
}

double calc_func(int func, double num) {
  switch (func) {
    case s21_ln: {
      num = log(num);
      break;
    }

    case s21_sin: {
      num = sin(num);
      break;
    }
    case s21_cos: {
      num = cos(num);
      break;
    }
    case s21_tan: {
      num = tan(num);
      break;
    }
    case s21_log: {
      num = log10(num);
      break;
    }
    case s21_acos: {
      num = acos(num);
      break;
    }
    case s21_asin: {
      num = asin(num);
      break;
    }
    case s21_atan: {
      num = atan(num);
      break;
    }
    case s21_sqrt: {
      num = sqrt(num);
      break;
    }
  }
  return num;
}

void polish_notation(stack *exp, stack *polsk) {
  stack ops = {0};
  initStack(&ops);
  for (int i = 0; exp->cur[i] != -1; i++) {
    if (check_num(*exp, i)) {
      push_num(polsk, exp->nums[i]);
      if (exp->cur[i] == 2) polsk->cur[polsk->top] = 2;
    } else if (exp->opers[i] == '(') {
      push_oper(&ops, '(', 0);
    } else if (exp->opers[i] == ')') {
      for (int j = i; peek_oper(&ops) != '('; j++) {
        push_oper(polsk, peek_oper(&ops), 0);
        pop(&ops);
      }
      pop(&ops);
      if (function_gap(peek_oper(&ops))) {
        push_oper(polsk, peek_oper(&ops), 0);
        pop(&ops);
      }
    } else {  // операторы
      if (!isEmpty(&ops)) {
        while (
            (!(isEmpty(&ops)) && (ops.priority[ops.top] >= exp->priority[i])) &&
            !function_gap(exp->opers[i]) &&
            !(exp->opers[i] == '^' && ops.opers[ops.top] == '^')) {
          push_oper(polsk, peek_oper(&ops), 0);
          pop(&ops);
        }
      } else {
      }
      push_oper(&ops, exp->opers[i], exp->priority[i]);
    }
  }
  while (!isEmpty(&ops)) {
    push_oper(polsk, peek_oper(&ops), ops.priority[ops.top]);
    pop(&ops);
  }
}

int filter(stack *e) {
  int res = 0, brace_counter = 0;
  for (int i = 0; (e->cur[i] != -1 && res == 0); i++) {
    if (e->cur[i] == OPER) {
      res = 0;
      int oper = e->opers[i];
      if (i == 0 && ops(oper))
        res = 1;  // если это не унарный оператор и он стоит первым в выражении
      brace_counter += check_brace(oper);
      if (brace_counter < 0) res = 1;  // баланс скобок
      if (oper == '(' && check_empty_braces(*e, i))
        res = 1;  // проверка на пустые скобки
      if ((oper == '(' || function_gap(oper)) && i > 0 && check_num(*e, i - 1))
        res = 1;  // если перед скобкой или функцией стоит число - значит
                  // пропущен оператор
      if (is_unary(oper)) {
        if (!((function_gap(e->opers[i + 1]) || e->opers[i + 1] == '(') &&
              check_oper(*e, i + 1)))
          res = 1;  // унарный оператор может стоять только перед ( или функцией
      }
      if (res == 0 &&
          ops(oper)) {  // если текущая лексема - оператор, то идет проверка на
                        // соседние от операторов элементы
        if (!(i > 0 && (check_num(*e, i - 1) || e->opers[i - 1] == ')')))
          res = 1;  // оператор может стоять только после числа или )
        if (check_oper(*e, i + 1) && ops(e->opers[i + 1]))
          res = 1;  // оператор не может стоять только перед другим оператором
        if (e->cur[i + 1] == -1) res = 1;
      }
    }
    if (res) break;
  }
  if (brace_counter != 0) res = 1;
  return res;
}

int is_unary(int sym) { return sym == '~' || sym == '#'; }
int check_empty_braces(stack e, int pos) {
  return (check_oper(e, pos + 1) && e.opers[pos + 1] == ')');
}

int check_oper(stack e, int pos) { return e.cur[pos] == OPER ? 1 : 0; }

int check_num(stack e, int pos) {
  return (e.cur[pos] == NUM || e.cur[pos] == 2) ? 1 : 0;
}

int check_brace(int sym) {
  int res = 0;
  if (sym == '(') res++;
  if (sym == ')') res--;
  return res;
}

int function_gap(int num) { return num >= s21_ln && num <= s21_sqrt; }

int parcer(const char *exp, stack *polsk) {
  int flag = 0, unary = 0;
  double tmp;
  for (int i = 0; i < (int)strlen(exp); i++) {
    if (exp[i] == ' ') continue;
    if (isgjigit(exp[i])) {
      if (peek_cur(polsk) == OPER) {
        if (peek_oper(polsk) == '~') unary = -1;
        if (peek_oper(polsk) == '#') unary = 1;
      }
      flag = char_to_num(exp, &i, polsk);
      if (unary) {
        tmp = peek_num(polsk);
        pop(polsk);  // удаление числа
        pop(polsk);  // удаление унарной операции
        push_num(polsk, tmp * unary);
        unary = 0;
      }
    } else if (exp[i] == 'x' || exp[i] == 'X') {
      push_num(polsk, 0.);
      polsk->cur[polsk->top] = 2;
    } else {
      flag = char_to_oper(exp, &i, polsk);
    }
    if (flag) break;
  }
  return flag;  // 1 - ошибка при парсинге. INPUT ERROR
}

int char_to_num(const char *exp, int *pos, stack *polsk) {
  char *copy = (char *)calloc(sizeof(char), MAX_SIZE + 1);
  char *free_me = copy;
  strcpy(copy, exp);
  double tmp;
  int dot_flag = -1, i = 0, res = 0;  // 0 - ok; 1 - ERROR;
  for (i = *pos; i < (int)strlen(exp); i++) {
    if (!isgjigit(copy[i])) {
      if (copy[i] == '.') {
        dot_flag++;
      } else {
        break;
      }
      if (dot_flag > 0) break;
    }
  }
  copy[i] = '\0';
  copy += *pos;
  *pos = i - 1;
  res = sscanf(copy, "%lf", &tmp);
  push_num(polsk, tmp);
  free(free_me);
  return (dot_flag <= 0 && res == 1) ? 0 : 1;
}

int check_priority(int op) {
  int res = -1;
  if (op == '(' || op == ')') {
    res = 0;
  } else if (op == '+' || op == '-') {
    res = 1;
  } else if (op == '*' || op == '/' || op == s21_mod) {
    res = 2;
  } else if (function_gap(op)) {
    res = 3;
  } else if (op == '~' || op == '#' || op == '^') {
    res = 4;
  }
  return res;
}

int char_to_oper(const char *exp, int *pos,
                 stack *polsk) {  // 1 - знак ошибки; 0 - все ок
  int res = 1, priority = 0;
  char sym = exp[*pos];
  if (is_op(sym) || sym == '(' || sym == ')') {
    priority = check_priority(sym);
    if ((peek_oper(polsk) == '(' || isEmpty(polsk)) &&
        (sym == '-' ||
         sym == '+')) {  // если это оператор, стоящий в самом начале или после
                         // открывающейся скобки - он унарный.
      if (sym == '-') push_oper(polsk, '~', 5);  // ~ - unary minus
      if (sym == '+') push_oper(polsk, '#', 4);  // # - unary plus
    } else {
      push_oper(polsk, sym, priority);
    }
    res = 0;
  } else {
    res = is_func(exp, pos);
    if (res != 1) {
      priority = check_priority(res);
      push_oper(polsk, res, priority);
      if (res != s21_mod) push_oper(polsk, '(', 0);
    }
  }
  return res == 1 ? 1 : 0;
}

int is_op(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int ops(int sym) { return is_op(sym) || sym == s21_mod; }

int is_func(const char *exp, int *pos) {
  int res = 1;
  if (res == 1 && !memcmp(*pos + exp, "ln(", 3)) res = s21_ln;
  if (res == 1 && !memcmp(*pos + exp, "mod", 3)) res = s21_mod;
  if (res == 1 && !memcmp(*pos + exp, "cos(", 3)) res = s21_cos;
  if (res == 1 && !memcmp(*pos + exp, "sin(", 3)) res = s21_sin;
  if (res == 1 && !memcmp(*pos + exp, "tan(", 3)) res = s21_tan;
  if (res == 1 && !memcmp(*pos + exp, "log(", 3)) res = s21_log;
  if (res == 1 && !memcmp(*pos + exp, "acos(", 3)) res = s21_acos;
  if (res == 1 && !memcmp(*pos + exp, "atan(", 3)) res = s21_atan;
  if (res == 1 && !memcmp(*pos + exp, "asin(", 3)) res = s21_asin;
  if (res == 1 && !memcmp(*pos + exp, "sqrt(", 3)) res = s21_sqrt;

  if (res == s21_ln || res == s21_mod) *pos += 2;
  if (res >= s21_log && res <= s21_tan) *pos += 3;
  if (res >= s21_acos && res <= s21_sqrt) *pos += 4;
  return res;
}

// инициализация стека
void initStack(stack *s) {
  s->top = -1;
  s->cur[0] = -1;
}

// проверка на пустоту стека
int isEmpty(stack *s) { return s->top == -1; }

// проверка на переполнение стека
int isFull(stack *s) { return s->top == MAX_SIZE - 1; }

// добавление элемента в стек
int push_num(stack *s, double newItem) {
  int res = 0;
  if (isFull(s)) {
    res = 1;
  } else {
    s->top++;
    s->nums[s->top] = newItem;
    s->cur[s->top] = 1;
    s->cur[s->top + 1] = -1;
  }
  return res;  // 0 - OK; 1 - stack is full
}

int push_oper(stack *s, int newItem, int priority) {
  int res = 0;
  if (isFull(s)) {
    res = 1;
  } else {
    s->top++;
    s->opers[s->top] = newItem;
    s->priority[s->top] = priority;
    s->cur[s->top] = 0;
    s->cur[s->top + 1] = -1;
  }
  return res;  // 0 - OK; 1 - stack is full
}

// удаление элемента из стека
int pop(stack *s) {
  int res = 0;
  if (isEmpty(s)) {
    res = 1;
  } else {
    int tmp = s->top;
    s->priority[tmp] = 0;
    s->opers[tmp] = 0;
    s->nums[tmp] = 0;
    s->cur[tmp] = -1;
    s->top--;
  }
  return res;
}

// возврат элемента из вершины стека
int peek_cur(stack *s) { return s->cur[s->top]; }
double peek_num(stack *s) { return s->nums[s->top]; }
int peek_oper(stack *s) { return s->opers[s->top]; }

int isgjigit(int sym) { return (sym >= '0' && sym <= '9') ? 1 : 0; }

// void print_stack(stack s) {
//   for (int i = 0; i <= s.top; i++) {
//     if (s.cur[i] == NUM) {
//       printf("%lf\n", s.nums[i]);
//     } else {
//       if (function_gap(s.opers[i])) {
//         printf("func %d\n", s.priority[i]);
//       } else {
//         if (s.opers[i] == s21_mod) {
//           printf("mod %d\n", s.priority[i]);
//         } else {
//           printf("%c %d\n", s.opers[i], s.priority[i]);
//         }
//       }
//     }
//   }
// }