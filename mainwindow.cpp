#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QHBoxLayout>
#include "customlistwidget.h"
#include <QPdfWriter>
#include <QPainter>
#include <QMap>
#include <QMessageBox>
#include <cmath>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    model(new QStandardItemModel(this))
{

    ui->setupUi(this);
    ui->umiescobiektylabel->hide();
    ui->leftlist->hide();
    ui->rightlist->hide();
    ui->label->hide();
    ui->dodajdoA->hide();
    ui->dodajdoB->hide();
    ui->generujpdf->hide();

    ui->dllabel->hide();
    ui->elemlabel->hide();
    ui->ile_elem->hide();
    ui->szerlabel->hide();
    ui->dlugosc->hide();
    ui->szerokosc->hide();
    ui->rozmiarlabel->hide();

    ui->leftlist->setDragEnabled(true);
    ui->leftlist->setAcceptDrops(true);
    ui->leftlist->setDragDropMode(QAbstractItemView::InternalMove);
    ui->rightlist->setDragEnabled(true);
    ui->rightlist->setAcceptDrops(true);
    ui->rightlist->setDragDropMode(QAbstractItemView::InternalMove);

    ui->ile_elem->setMinimum(4);
    ui->ile_elem->setMaximum(9);
    ui->ile_elem->setTickInterval(1);
    ui->ile_elem->setSingleStep(1);
    ui->ile_elem->setTickPosition(QSlider::TicksBelow);

    // Ustawienie początkowej wartości QLabel
    ui->label_obok->setText(QString::number(ui->ile_elem->value()));

    // Połączenie sygnału valueChanged z odpowiednim slotem
    connect(ui->ile_elem, &QSlider::valueChanged, this, &MainWindow::updateLabel);



}
void MainWindow::updateLabel(int value)
{
    ui->label_obok->setText(QString::number(value));
}

void MainWindow::on_rozpocznij_clicked()
{
    ui->rozpocznij->hide();
    ui->doobleheader->hide();
    ui->umiescobiektylabel->show();
    ui->leftlist->show();
    ui->rightlist->show();
    ui->label->show();
    ui->dodajdoA->show();
    ui->dodajdoB->show();
    ui->generujpdf->show();
    ui->dllabel->show();
    ui->elemlabel->show();
    ui->ile_elem->show();
    ui->szerlabel->show();
    ui->dlugosc->show();
    ui->szerokosc->show();
    ui->rozmiarlabel->show();

}
void MainWindow::on_jakgrac_clicked()
{
    ui->rozpocznij->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::on_dodajdoA_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Images"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileNames.isEmpty()) {

        for (const QString &fileName : fileNames) {


            QPixmap pixmap(fileName);


            QPixmap scaledPixmap = pixmap.scaled(ui->dlugosc->value(), ui->szerokosc->value(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


            QWidget *widget = new QWidget;
            QHBoxLayout *layout = new QHBoxLayout;
            QLabel *label = new QLabel;
            label->setPixmap(scaledPixmap);
            QPushButton *button = new QPushButton("usuń");
            layout->addWidget(label);
            layout->addWidget(button);
            QLabel *labeltext = new QLabel(QString("Obrazek %1").arg(imgcounter_A));
            labeltext->setTextInteractionFlags(Qt::NoTextInteraction);
            layout->addStretch();
            layout->addWidget(labeltext);
            widget->setLayout(layout);


            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(widget->sizeHint());
            ui->leftlist->addItem(item);
            ui->leftlist->setItemWidget(item, widget);

            connect(button, &QPushButton::clicked, [this, item]() {

                int row = ui->leftlist->row(item);


                delete item;
                imgcounter_A--;

                // Aktualizacja numeracji wszystkich elementów w liście
                for (int i = row; i < ui->leftlist->count(); ++i) {
                    QListWidgetItem *currentItem = ui->leftlist->item(i);
                    QWidget *currentWidget = ui->leftlist->itemWidget(currentItem);
                    QList<QLabel *> all = currentWidget->findChildren<QLabel *>();
                    (all.at(1))->setText(QString("Obrazek %1").arg(i + 1));



                }
            });

            imgcounter_A++;
        }
    }
}




void MainWindow::on_generujpdf_clicked()
{   //Wywołanie głównej funkcji generującej
    QVector<QVector<QVector<ImageItem*>>> chosen_vector = GENERATE(ui->ile_elem->value());

    // Wybór miejsca zapisu pliku PDF
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty()) {
        return;
    }


    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);


    int margin = 50;
    int spacing = 20;
    int imageWidth = ui->dlugosc->value();
    int imageHeight = ui->szerokosc->value();
    int itemsPerRow;
    int currentX = margin;
    int currentY = margin;

    // Pobierz rozmiary strony
    int pageWidth = pdfWriter.width();
    int pageHeight = pdfWriter.height();

    // Iteracja po kartach
    for (int k = 0; k < 2; ++k) {
        if (k) {
            pdfWriter.newPage();
            currentX = margin;
            currentY = margin;
        }
        for (int i = 0; i < chosen_vector[k].size(); ++i) {
            const QVector<ImageItem*>& card = chosen_vector[k][i];
            int numItems = card.size();
            int cardHeight;
            // Obliczanie liczby elementów w rzędzie
            if (numItems <= 4) {
                itemsPerRow = 2;
                cardHeight = 2 * imageHeight;
            } else if (numItems <= 9) {
                itemsPerRow = 3;
                cardHeight = 3 * imageHeight;
            } else {
                itemsPerRow = 4;
                cardHeight = 4 * imageHeight;
            }


            int cardWidth = itemsPerRow * imageWidth;




            if (currentX + cardWidth  + margin> pageWidth) {

                currentX = margin;
                currentY += cardHeight;


                if (currentY + cardHeight + margin > pageHeight) {

                    pdfWriter.newPage();
                    currentX = margin;
                    currentY = margin;
                }
            }


            for (int j = 0; j < numItems; ++j) {
                int row = j / itemsPerRow;
                int col = j % itemsPerRow;
                int x = currentX + col * imageWidth;
                int y = currentY + row * imageHeight;
                painter.drawPixmap(x, y, card[j]->getPixmap()->scaled(imageWidth, imageHeight, Qt::KeepAspectRatio));
            }


            currentX += cardWidth + spacing;
        }
    }
    painter.end();
    exit(0);
}



QVector<QVector<QVector<ImageItem*>>> MainWindow::GENERATE(int imagesoncard) {
    // Zapisz wszystkie obrazki z lewego listWidgeta do wektora 'images'
    for (int i = 0; i < ui->leftlist->count(); ++i) {
        QListWidgetItem *currentItem = ui->leftlist->item(i);
        QWidget *currentWidget = ui->leftlist->itemWidget(currentItem);
        QList<QLabel *> all = currentWidget->findChildren<QLabel *>();
        QPixmap* pixmap = new QPixmap(all.at(0)->pixmap(Qt::ReturnByValue));
        ImageItem* img = new ImageItem(pixmap, i);
        images.append(img);
    }

    // Zapisz wszystkie obrazki z prawego listWidgeta do wektora 'images_B'
    for (int i = 0; i < ui->rightlist->count(); ++i) {
        QListWidgetItem *currentItem = ui->rightlist->item(i);
        QWidget *currentWidget = ui->rightlist->itemWidget(currentItem);
        QList<QLabel *> all = currentWidget->findChildren<QLabel *>();
        QPixmap* pixmap = new QPixmap(all.at(0)->pixmap(Qt::ReturnByValue));
        ImageItem* img = new ImageItem(pixmap, i);
        images_B.append(img);
    }

    // Generuj wszystkie możliwe grupy obrazków o rozmiarze 'imagesoncard'
    QVector<QVector<ImageItem*>> A = generateAllGroups(images, imagesoncard);
    QVector<QVector<ImageItem*>> B = generateAllGroups(images_B, imagesoncard);

    QVector<QVector<ImageItem*>> A_chosen;
    QVector<QVector<ImageItem*>> B_chosen;
    QVector<QVector<ImageItem*>> Excluded_A;
    QVector<QVector<ImageItem*>> Excluded_B;
    int count;

    // Główna pętla: wybieraj karty dopóki są dostępne
    while (!A.empty()) {
        count = 0;
        int stop_loop_1 = A.size();

        // Szukaj odpowiedniej karty w A
        while (count <= stop_loop_1) {
            QVector<ImageItem*> card = select_least_counted_objects(A);

            // Jeśli nie udało się znaleźć karty, kończ i zwróć dotychczas wybrane karty
            if (card.size() == 0) {
                if (A_chosen.size() > B_chosen.size()) {
                    A_chosen.pop_front();
                }
                if (A_chosen.size() < B_chosen.size()) {
                    B_chosen.pop_front();
                }
                return QVector<QVector<QVector<ImageItem*>>> {A_chosen, B_chosen};
            }

            // Sprawdź, czy wybrana karta nie tworzy niedozwolonych par z wykluczonymi kartami z B
            bool match_touples = false;
            QVector<QVector<ImageItem*>> touples = generateAllGroups(card, 2);
            for (int i = 0; i < touples.size(); ++i) {
                for (int j = 0; j < Excluded_B.size(); ++j) {
                    if (touples[i][0]->get_number() == Excluded_B[j][0]->get_number() &&
                        touples[i][1]->get_number() == Excluded_B[j][1]->get_number()) {
                        match_touples = true;
                        break;
                    }
                }
                if (match_touples)
                    break;
            }

            // Jeśli karta jest poprawna
            if (!match_touples) {
                // Zwiększ liczniki użycia obrazków
                for (auto& image : card) {
                    (*image)++;
                }

                // Dodaj kartę do wybranych z A
                A_chosen.append(card);

                // Dodaj wygenerowane pary do wykluczonych z A
                for (auto& touple : touples) {
                    Excluded_A.append(touple);
                }

                // Usuń z B kartę odpowiadającą wybranej karcie z A
                for (int i = 0; i < B.size(); ++i) {
                    QVector<ImageItem*>& result = B[i];
                    for (int j = 0; j < card.size(); ++j) {
                        if (card[j]->get_number() != result[j]->get_number()) {
                            break;
                        }
                        if (j == card.size() - 1) {
                            B.removeAt(i);
                        }
                    }
                }
                break;
            }

            count++;
        }

        // Jeśli nie udało się nic znaleźć w A
        if (count == stop_loop_1) {
            if (A_chosen.size() > B_chosen.size()) {
                A_chosen.pop_front();
            }
            if (A_chosen.size() < B_chosen.size()) {
                B_chosen.pop_front();
            }
            return QVector<QVector<QVector<ImageItem*>>> {A_chosen, B_chosen};
        }

        int count2 = 0;
        int stop_loop_2 = B.size();

        // Szukaj odpowiedniej karty w B
        while (count2 <= stop_loop_2) {
            QVector<ImageItem*> cardB = select_least_counted_objects(B);

            // Jeśli nie udało się znaleźć karty, kończ
            if (cardB.size() == 0) {
                if (A_chosen.size() > B_chosen.size()) {
                    A_chosen.pop_front();
                }
                if (A_chosen.size() < B_chosen.size()) {
                    B_chosen.pop_front();
                }
                return QVector<QVector<QVector<ImageItem*>>> {A_chosen, B_chosen};
            }

            // Sprawdź, czy karta nie tworzy niedozwolonych par z wykluczonymi kartami z A
            bool match_touples_2 = false;
            QVector<QVector<ImageItem*>> touplesB = generateAllGroups(cardB, 2);
            for (int i = 0; i < touplesB.size(); ++i) {
                for (int j = 0; j < Excluded_A.size(); ++j) {
                    if (touplesB[i][0]->get_number() == Excluded_A[j][0]->get_number() &&
                        touplesB[i][1]->get_number() == Excluded_A[j][1]->get_number()) {
                        match_touples_2 = true;
                        break;
                    }
                }
                if (match_touples_2)
                    break;
            }

            // Jeśli karta jest poprawna
            if (!match_touples_2) {
                // Zwiększ liczniki użycia obrazków
                for (auto& image : cardB) {
                    (*image)++;
                }

                // Dodaj kartę do wybranych z B
                B_chosen.append(cardB);

                // Dodaj wygenerowane pary do wykluczonych z B
                for (auto& touple : touplesB) { // TODO: Można zwiększyć wydajność sprawdzając czy touples już istnieją
                    Excluded_B.append(touple);
                }

                // Usuń z A kartę odpowiadającą wybranej karcie z B
                for (int i = 0; i < A.size(); ++i) {
                    QVector<ImageItem*>& result = A[i];
                    for (int j = 0; j < cardB.size(); ++j) {
                        if (cardB[j]->get_number() != result[j]->get_number()) {
                            break;
                        }
                        if (j == cardB.size() - 1) {
                            A.removeAt(i);
                            break;
                        }
                    }
                }
                break;
            }

            count2++;
        }

        // Jeśli nie udało się nic znaleźć w B
        if (count2 == stop_loop_2) {
            if (A_chosen.size() > B_chosen.size()) {
                A_chosen.pop_front();
            }
            if (A_chosen.size() < B_chosen.size()) {
                B_chosen.pop_front();
            }
            return QVector<QVector<QVector<ImageItem*>>> {A_chosen, B_chosen}; // TUTAJ kończy się algorytm
        }
    }

    // Na koniec upewnij się, że A_chosen i B_chosen są tej samej wielkości
    if (A_chosen.size() > B_chosen.size()) {
        A_chosen.pop_front();
    }
    if (A_chosen.size() < B_chosen.size()) {
        B_chosen.pop_front();
    }
    return QVector<QVector<QVector<ImageItem*>>> {A_chosen, B_chosen};
}


void MainWindow::on_dodajdoB_clicked()

{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Images"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileNames.isEmpty()) {

        for (const QString &fileName : fileNames) {


            QPixmap pixmap(fileName);


            QPixmap scaledPixmap = pixmap.scaled(ui->dlugosc->value(), ui->szerokosc->value(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);


            QWidget *widget = new QWidget;
            QHBoxLayout *layout = new QHBoxLayout;
            QLabel *label = new QLabel;
            label->setPixmap(scaledPixmap);
            QPushButton *button = new QPushButton("usuń");
            layout->addWidget(label);
            layout->addWidget(button);
            QLabel *labeltext = new QLabel(QString("Obrazek %1").arg(imgcounter_B));
            labeltext->setTextInteractionFlags(Qt::NoTextInteraction);
            layout->addStretch();
            layout->addWidget(labeltext);
            widget->setLayout(layout);

            // Tworzenie elementu listy
            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(widget->sizeHint());
            ui->rightlist->addItem(item);
            ui->rightlist->setItemWidget(item, widget);

            connect(button, &QPushButton::clicked, [this, item]() {

                int row = ui->rightlist->row(item);


                delete item;
                imgcounter_B--;

                // Aktualizacja numeracji wszystkich elementów w liście
                for (int i = row; i < ui->rightlist->count(); ++i) {
                    QListWidgetItem *currentItem = ui->rightlist->item(i);
                    QWidget *currentWidget = ui->rightlist->itemWidget(currentItem);
                    QList<QLabel *> all = currentWidget->findChildren<QLabel *>();
                    (all.at(1))->setText(QString("Obrazek %1").arg(i + 1));



                }
            });

            imgcounter_B++;
        }
    }
}




// Funkcja rekurencyjna do generowania wszystkich możliwych grup obiektów
void MainWindow::generateGroups(const QVector<ImageItem*>& images, int groupSize, int index, QVector<ImageItem*>& currentGroup, QVector<QVector<ImageItem*>>& groups) {
    if (currentGroup.size() == groupSize) {
        groups.append(currentGroup);
        return;
    }

    for (int i = index; i < images.size(); ++i) {
        currentGroup.append(images[i]);
        generateGroups(images, groupSize, i + 1, currentGroup, groups);
        currentGroup.pop_back();
    }
}

// Funkcja główna do generowania wszystkich możliwych grup obiektów
QVector<QVector<ImageItem*>> MainWindow:: generateAllGroups(const QVector<ImageItem*>& images, int groupSize) {
    QVector<QVector<ImageItem*>> groups;
    QVector<ImageItem*> currentGroup;
    generateGroups(images, groupSize, 0, currentGroup, groups);
    return groups;
}


// Funkcja do porównywania obiektów na podstawie liczby wystąpień
bool compare(const ImageItem* a, const ImageItem* b) {
    return a->get_counter() < b->get_counter();
}
bool compare_A(QVector<ImageItem*> a, QVector<ImageItem*> b) {
    std::sort(a.begin(), a.end(), compare);
    std::sort(b.begin(), b.end(), compare);
    for (int i = a.size()-1; i >= 0; --i){
        if (a[i]->get_counter() < b[i]->get_counter()){
            return true;
        }
        if (a[i]->get_counter() > b[i]->get_counter()){
            return false;
        }
    }
    for (int i = 0; i<a.size(); ++i){
        if (a[i]->get_number() < b[i]->get_number()){
            return true;
        }
        if (a[i]->get_number() > b[i]->get_number()){
            return false;
        }
    }
    return a[a.size()-1]->get_number() < b[b.size()-1]->get_number();
}
bool compare_by_number(const ImageItem* a, const ImageItem* b) {
    return a->get_number() < b->get_number();
}
// Funkcja do wybierania n obiektów z najmniejszą liczbą wystąpień
QVector<ImageItem*> MainWindow::select_least_counted_objects(QVector<QVector<ImageItem*>>& A) {
    if (A.size() == 0){
        QVector<ImageItem*> empty;
        return empty;
    }
    auto A_copy = A;
    // Sortowanie kopii zbioru A
    std::sort(A_copy.begin(), A_copy.end(), compare_A);
    //Wybór pierwszego pasującego zbioru
    A.removeAll(A_copy[0]);
    return A_copy[0];

}
