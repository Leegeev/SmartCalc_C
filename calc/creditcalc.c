#include "smartcalc.h"

double *annuitet_credit_calc(double sum, double duration, double percentage) {
  double *res = (double *)calloc(sizeof(double), 3);
  double rate = percentage / 12. / 100;
  res[0] = sum *
           (rate * pow((1 + rate), duration) / (pow((1 + rate), duration) - 1));
  res[2] = duration * res[0];  // общая выплата
  res[1] = res[2] - sum;       // переплата по кредиту
  return res;
}

double *difference_credit_calc(double sum, double duration, double percentage) {
  double *res = (double *)calloc(sizeof(double), 4);
  double rate = percentage / 12. / 100, currpay = 0.;
  for (int i = 1; i <= duration; i++, res[3] += currpay) {
    currpay = sum / duration + (sum - (i - 1) * sum / duration) * rate;
    if (i == 1) res[0] = currpay;
    if (i == duration) res[1] = currpay;
  }
  res[2] = res[3] - sum;  // переплата по кредиту
  return res;
}  // 0 - first pay, 1 - last pay, 2 - overpay, 3 - totalpay
