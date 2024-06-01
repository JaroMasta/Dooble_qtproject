#include "customlistwidget.h"
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

CustomListWidget::CustomListWidget(QWidget *parent)
    : QListWidget(parent)
{
}

void CustomListWidget::dropEvent(QDropEvent *event)
{
    QListWidget::dropEvent(event);
    updateItemNumbers();
}

void CustomListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QListWidget::dragMoveEvent(event);
}

void CustomListWidget::updateItemNumbers()
{

   for (int i = 0; i < count(); ++i) {
       QListWidgetItem *currentItem = item(i);
       QWidget *currentWidget = itemWidget(currentItem);
       QList<QLabel *> all = currentWidget->findChildren<QLabel *>();
       (all.at(1))->setText(QString("Obrazek %1").arg(i + 1));

   }
}
