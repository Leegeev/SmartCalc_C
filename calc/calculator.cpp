#include "calculator.h"

#include <sstream>

#include "ui_calculator.h"

calculator::calculator(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::calculator) {
  ui->setupUi(this);

  connect(ui->zero, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->one, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->two, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->three, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->four, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->five, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->six, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->seven, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->eight, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->nine, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->mult, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->div, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->minus, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->plus, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->dot, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->openbrace, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->closebrace, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->mod, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->power, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));
  connect(ui->ex, SIGNAL(clicked()), this, SLOT(on_sym_clicked()));

  connect(ui->sin, SIGNAL(clicked()), this, SLOT(on_func_clicked()));
  connect(ui->asin, SIGNAL(clicked()), this, SLOT(on_func_clicked()));
  connect(ui->sqrt, SIGNAL(clicked()), this, SLOT(on_func_clicked()));
  connect(ui->cos, SIGNAL(clicked()), this, SLOT(on_func_clicked()));
  connect(ui->acos, SIGNAL(clicked()), this, SLOT(on_func_clicked()));
  connect(ui->ln, SIGNAL(clicked()), this, SLOT(on_func_clicked()));
  connect(ui->tan, SIGNAL(clicked()), this, SLOT(on_func_clicked()));
  connect(ui->atan, SIGNAL(clicked()), this, SLOT(on_func_clicked()));
  connect(ui->log, SIGNAL(clicked()), this, SLOT(on_func_clicked()));

  connect(ui->clearAll, SIGNAL(clicked()), this, SLOT(on_clearAll_clicked()));
  connect(ui->BackSpace, SIGNAL(clicked()), this, SLOT(on_bs_clicked()));

  connect(ui->equal, SIGNAL(clicked()), this, SLOT(on_equal_clicked()));
}

calculator::~calculator() { delete ui; }

void calculator::on_func_clicked() {
  QPushButton *button = (QPushButton *)sender();
  ui->screen->setText(ui->screen->text() + button->text() + '(');
}

void calculator::on_sym_clicked() {
  QPushButton *button = (QPushButton *)sender();
  QString tmp = ui->screen->text();
  static int dot_counter = 0;
  const QString operators = "+-*/^";
  const QString digits = "1234567890()x";
  const QString plusminus = "+-";
  if ((tmp.isEmpty() && plusminus.contains(button->text())) ||
      digits.contains(button->text())) {
    ui->screen->setText(tmp + button->text());
  } else if (button->text().contains('.') && dot_counter == 0) {
    ui->screen->setText(tmp + ".");
    dot_counter += 1;
  } else if (!(operators.contains(tmp.right(1))) &&
             tmp.right(3).compare("mod") != 0 && button->text() != ".") {
    ui->screen->setText(tmp + button->text());
    dot_counter = 0;
  }
}

void calculator::on_bs_clicked() {
  QString text = ui->screen->text();  // получить текст из элемента интерфейса
  text.chop(1);  // удалить последний символ из текста
  ui->screen->setText(
      text);  // установить измененный текст в элемент интерфейса
}

void calculator::on_clearAll_clicked() { ui->screen->setText(""); }

void calculator::on_equal_clicked() {
  int state = 0;
  double tmp = 0;
  std::string str;
  if (ui->exInput->text().isEmpty() && ui->screen->text().contains('x')) {
    state = 1;
  } else {
    tmp = mysmartcalc(ui->screen->text().toStdString().c_str(), &state,
                      ui->exInput->text().toDouble());
    str = std::to_string(tmp);
  }
  if (state) {
    ui->screen->setText("ERROR");
  } else {
    ui->screen->setText(QString::fromStdString(str));
  }
}

void calculator::on_tabWidget_currentChanged(int index) {
  switch (index) {
    case 0:
      ui->tabWidget->resize(430, 350);
      setFixedSize(430, 350);
      break;
    case 1:
      ui->tabWidget->resize(800, 620);
      setFixedSize(800, 620);
      break;
    case 2:
      ui->tabWidget->resize(420, 420);
      setFixedSize(420, 420);
      break;
    case 3:

      break;
  }
}

void calculator::on_DrawButton_clicked() {
  ui->plot->clearPlottables();
  double Xmin = 0, Xmax = 0, Ymin = 0, Ymax = 0;
  int state = 0;

  // XMAX
  if (ui->Xmax->text().isEmpty()) {
    ui->Xmax->setText("ERROR");
    state = 1;
  } else {
    Xmax = ui->Xmax->text().toDouble();
  }

  // XMIN
  if (ui->Xmin->text().isEmpty()) {
    ui->Xmin->setText("ERROR");
    state = 1;
  } else {
    Xmin = ui->Xmin->text().toDouble();
  }

  // YMIN
  if (ui->Ymin->text().isEmpty()) {
    ui->Ymin->setText("ERROR");
    state = 1;
  } else {
    Ymin = ui->Ymin->text().toDouble();
  }

  // YMAX
  if (ui->Ymax->text().isEmpty()) {
    ui->Ymax->setText("ERROR");
    state = 1;
  } else {
    Ymax = ui->Ymax->text().toDouble();
  }

  QVector<double> x, y;
  double prev = 0, i;
  int graph_counter = 0;
  stack polsk = make_polsk(ui->func->text().toStdString().c_str(), &state);
  if (state == 0) {
    ui->plot->yAxis->setRange(Ymin, Ymax);
    ui->plot->xAxis->setRange(Xmin, Xmax);
    i = abs(Xmax - Xmin) / 10000;
    for (double xdot = Xmin, tmp = 0; xdot <= Xmax; xdot += i) {
      tmp = calculate(polsk, xdot);
      if (abs(abs(prev) - abs(tmp)) > 425) {
        ui->plot->addGraph();
        ui->plot->graph(graph_counter)->setData(x, y);
        graph_counter += 1;
        x.clear();
        y.clear();
      } else {
        x.push_back(xdot);
        y.push_back(tmp);
        prev = tmp;
      }
    }
    ui->plot->addGraph();
    ui->plot->graph(graph_counter)->setData(x, y);
    ui->plot->replot();
    y.clear();
    x.clear();
  }
}

/* Формула расчета аннуитетного платежа (А) представляет собой следующее
 * соотношение: А=К*(П/(1+П)-М-1), где К – сумма кредита, П – процентная ставка,
 * М – количество месяцев.
 * Такой прием используют при подсчете выплат по ипотеке и потребительским
 * займам.
 */
void calculator::on_pushButton_clicked() {
  double sum = 0, duration = 0, percentage = 0;
  int state = 0, payment_type = -1;

  if (ui->inputCreditSumm->text().isEmpty()) {
    state = 1;
    ui->inputCreditSumm->setText("ERROR");
  } else {
    sum = ui->inputCreditSumm->text().toDouble();
  }

  if (ui->inputCreditDuration->text().isEmpty()) {
    state = 1;
    ui->inputCreditDuration->setText("ERROR");
  } else {
    duration = ui->inputCreditDuration->text().toDouble();
  }

  if (ui->inputCreditPercentage->text().isEmpty()) {
    state = 1;
    ui->inputCreditPercentage->setText("ERROR");
  } else {
    percentage = ui->inputCreditPercentage->text().toDouble();
  }

  if (ui->creditDuration->currentText().contains("лет")) {
    duration *= 12;
  }
  if (ui->creditAnnuitet->isChecked()) {
    payment_type = 0;
  } else if (ui->creditDifference->isChecked()) {
    payment_type = 1;
  } else {
    ui->outputPayment->setText("Ну что там с деньгами?");
  }

  if (payment_type == 0 && state == 0) {  // аннуитетный
    double *res = annuitet_credit_calc(sum, duration, percentage);
    ui->outputPayment->setText(
        QString::fromStdString(std::to_string(res[0])).chopped(4));
    ui->outputOverPay->setText(
        QString::fromStdString(std::to_string(res[1])).chopped(4));
    ui->outputTotalSum->setText(
        QString::fromStdString(std::to_string(res[2])).chopped(4));
    free(res);
  } else if (payment_type == 1 && state == 0) {  // дифференцированный
    double *res = difference_credit_calc(sum, duration, percentage);
    ui->outputPayment->setText(
        QString::fromStdString(std::to_string(res[0])).chopped(4));
    ui->outputPayment->setText(
        ui->outputPayment->text() + "..." +
        QString::fromStdString(std::to_string(res[1])).chopped(4));
    ui->outputOverPay->setText(
        QString::fromStdString(std::to_string(res[2])).chopped(4));
    ui->outputTotalSum->setText(
        QString::fromStdString(std::to_string(res[3])).chopped(4));
    free(res);
  }
}
