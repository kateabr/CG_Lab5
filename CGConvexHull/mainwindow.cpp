#include "mainwindow.h"
#include "matrix3x2.h"
#include "primitives.h"
#include "tablemodel.h"
#include "ui_mainwindow.h"
#include <functional>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  TableModel *tModel = new TableModel();

  ui->primitivesList->setModel(tModel);
  ui->canvas->setTableModel(tModel);
  ui->primitivesList->setSelectionMode(QAbstractItemView::ExtendedSelection);

  connect(ui->primitivesList, &MyListView::itemsChanged, [&]() {
    ui->canvas->clearArea();
    ui->canvas->repaint();
  });

  connect(ui->applyButton, &QPushButton::clicked, ui->primitivesList,
          &MyListView::update);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_applyButton_clicked() {}
