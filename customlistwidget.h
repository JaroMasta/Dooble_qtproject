#ifndef CUSTOMLISTWIDGET_H
#define CUSTOMLISTWIDGET_H

#include <QListWidget>
#include <QDropEvent>
#include <QDragMoveEvent>

class CustomListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit CustomListWidget(QWidget *parent = nullptr);

protected:
    void dropEvent(QDropEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;

private:
    void updateItemNumbers();
};

#endif // CUSTOMLISTWIDGET_H

