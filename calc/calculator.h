#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QVector>

extern "C" {
#include "smartcalc.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class calculator;
}
QT_END_NAMESPACE

class calculator : public QMainWindow {
  Q_OBJECT

 public:
  calculator(QWidget *parent = nullptr);
  ~calculator();

 private slots:
  void on_sym_clicked();
  void on_func_clicked();
  void on_bs_clicked();
  void on_clearAll_clicked();
  void on_equal_clicked();

  void on_tabWidget_currentChanged(int index);

  void on_DrawButton_clicked();

  void on_pushButton_clicked();

 private:
  Ui::calculator *ui;
};
#endif  // CALCULATOR_H
