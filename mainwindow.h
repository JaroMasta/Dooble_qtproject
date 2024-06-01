#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include "customlistwidget.h"
#include "imageitem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVector<QVector<ImageItem*>> generateAllGroups(const QVector<ImageItem*>& images, int groupSize);
    void generateGroups(const QVector<ImageItem*>& images, int groupSize, int index, QVector<ImageItem*>& currentGroup, QVector<QVector<ImageItem*>>& groups);
    QVector<ImageItem*> least_counts(QVector<ImageItem*> Images);

private slots:
    void on_rozpocznij_clicked();

    void on_jakgrac_clicked();

    void updateLabel(int value);

    void on_dodajdoA_clicked();



    void on_generujpdf_clicked();



    void on_dodajdoB_clicked();
    QVector<QVector<QVector<ImageItem*>>> GENERATE(int imagesoncard);
    QVector<ImageItem*> select_least_counted_objects(QVector<QVector<ImageItem*>>& A);
private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QVector<ImageItem*> images;
    QVector<ImageItem*> images_B;

public:
    int imgcounter_A = 1;
    int imgcounter_B = 1;

};
#endif // MAINWINDOW_H
