#include <check.h>

#include "./calc/smartcalc.h"

START_TEST(test) {
  int state = 0;
  double res = 0.;
  char example[256] = {"-(-(-((sin(cos(48)^(2)))/(tan(2)^(2)))+3*(-6)))"};
  res = mysmartcalc(example, &state, 1.);
  ck_assert_ldouble_eq_tol(-18.0834475532, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example1[256] = {"-100 / (8 + 2 * 7) * 0.5 ^ 2 + 8 - 6"};
  res = mysmartcalc(example1, &state, 1.);
  ck_assert_ldouble_eq_tol(0.86363636363, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example2[256] = {"2mod((sin(cos(48)^(2)))/(tan(2)^2))"};
  res = mysmartcalc(example2, &state, 1.);
  ck_assert_ldouble_eq_tol(0.0807062756, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example3[256] = {"((sin(95)+cos(96))^(2))/(log(sqrt(tan(1))))"};
  res = mysmartcalc(example3, &state, 1.);
  ck_assert_ldouble_eq_tol(2.6282352071, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example4[256] = {"sqrt((sin(cos(48)^2))/tan(2)^2)"};
  res = mysmartcalc(example4, &state, 1.);
  ck_assert_ldouble_eq_tol(0.28887290152, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example5[256] = {"(5-10)*777+(25-sin(10))"};
  res = mysmartcalc(example5, &state, 1.);
  ck_assert_ldouble_eq_tol(-3859.45597889, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example6[256] = {"-(100*(-1))+0.000001-0.00001"};
  res = mysmartcalc(example6, &state, 1.);
  ck_assert_ldouble_eq_tol(99.999991, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example7[256] = {"atan(acos(asin(sin(cos(tan(log(5)))))))"};
  res = mysmartcalc(example7, &state, 1.);
  ck_assert_ldouble_eq_tol(0.69897000433, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example8[256] = {"-ln(444^111)"};
  res = mysmartcalc(example8, &state, 1.);
  ck_assert_ldouble_eq_tol(-676.63652643, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example9[256] = {"77777777+7777*7777/2000000"};
  res = mysmartcalc(example9, &state, 1.);
  ck_assert_ldouble_eq_tol(77777807.240865, res, 1e-6);
  ck_assert_uint_eq(state, 0);

  char example10[256] = {"-(100^(-100)+tan(2)*(-5))"};
  res = mysmartcalc(example10, &state, 1.);
  ck_assert_ldouble_eq_tol(-10.9251993163, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example11[256] = {"-(cos(555)*asin(0.5))-777.098765*sqrt(256)"};
  res = mysmartcalc(example11, &state, 1.);
  ck_assert_ldouble_eq_tol(-12433.3251353, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example12[256] = {"(1/1/1/(-(sin(-(cos(-1/(-ln(log(3)))))))))"};
  res = mysmartcalc(example12, &state, 1.);
  ck_assert_ldouble_eq_tol(4.63078860872, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example13[256] = {"cos(-1/(-ln(log(3))))"};
  res = mysmartcalc(example13, &state, 1.);
  ck_assert_ldouble_eq_tol(0.21766052492, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example14[256] = {"-(6544+765)-4+3/(432)*3-(-3)+(cos(3))"};
  res = mysmartcalc(example14, &state, 1.);
  ck_assert_ldouble_eq_tol(-7310.96915916, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example15[256] = {"-cos(5)"};
  res = mysmartcalc(example15, &state, 1.);
  ck_assert_ldouble_eq_tol(-0.28366218546, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example16[256] = {"-sin(-cos(5))"};
  res = mysmartcalc(example16, &state, 1.);
  ck_assert_ldouble_eq_tol(0.27987335076, res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example17[256] = {"2mod2*2"};
  res = mysmartcalc(example17, &state, 1.);
  ck_assert_ldouble_eq_tol(0., res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example18[256] = {"2^3^2"};
  res = mysmartcalc(example18, &state, 1.);
  ck_assert_ldouble_eq_tol(512., res, 1e-7);
  ck_assert_uint_eq(state, 0);

  char example19[256] = {"coz(siz(sqrz(taz(moz(ataz(asiz(acoz(2))))))))"};
  res = mysmartcalc(example19, &state, 1.);
  ck_assert_ldouble_eq_tol(0., res, 1e-7);
  ck_assert_uint_eq(state, 1);

  char example20[256] = {
      "-sin(x)+3*10mod7/4^2-13.5*acos(0.5)/"
      "asin(-0.5)+(-tan(123))-(+sqrt(9))*log(100)"};
  res = mysmartcalc(example20, &state, 10.);
  ck_assert_ldouble_eq_tol(21.15109363, res, 1e-7);
  ck_assert_uint_eq(state, 0);
}

Suite *tests() {
  Suite *s = suite_create("TEST");
  TCase *tc = tcase_create("all test");
  tcase_add_test(tc, test);
  suite_add_tcase(s, tc);
  return s;
}

int main() {
  int no_failed = 0;

  Suite *suite = tests();
  SRunner *test_1 = srunner_create(suite);
  srunner_set_fork_status(test_1, CK_NOFORK);
  srunner_run_all(test_1, CK_NORMAL);
  no_failed += srunner_ntests_failed(test_1);
  srunner_free(test_1);

  return (no_failed < 1) ? EXIT_SUCCESS : EXIT_FAILURE;
}
