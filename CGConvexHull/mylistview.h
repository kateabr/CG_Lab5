#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H

#include "matrix3x2.h"
#include "tablemodel.h"
#include <QtWidgets>

class MyListView : public QListView {
  Q_OBJECT
public:
  MyListView(QWidget *parent = nullptr) : QListView(parent) {}

  void update(Matrix3x2 &tr, bool center, bool redraw = true) {
    if ((model()->rowCount() > 0) && (!selectedIndexes().empty())) {
      auto list = selectedIndexes();
      static_cast<TableModel *>(model())->update(tr, list, center);
      if (redraw)
        emit itemsChanged();
    }
  }

  void intersect() {
    if ((model()->rowCount() > 0) && (selectedIndexes().size() == 2)) {
      static_cast<TableModel *>(model())->intersect(selectedIndexes()[0].row(),
                                                    selectedIndexes()[1].row());
    }
  }

signals:
  void itemsChanged();

protected:
  void keyPressEvent(QKeyEvent *e) {
    if ((e->key() == Qt::Key_Delete) && (model()->rowCount() > 0) &&
        (!selectedIndexes().empty())) {
      model()->removeRow(selectedIndexes()[0].row());
      emit itemsChanged();
    }
  }
};

#endif // MYLISTVIEW_H
