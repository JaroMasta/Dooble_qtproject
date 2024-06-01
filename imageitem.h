#ifndef IMAGEITEM_H
#define IMAGEITEM_H
#include <QPixmap>
class ImageItem {

public:
    ImageItem(QPixmap* const pixmap_, int& number_)
        :pixmap(pixmap_), number(number_), counter(0)
    {}
    int get_number() const{
        return number;
    }
    int get_counter() const {
        return counter;
    }
    ImageItem operator++ (int){
        counter++;
        return *this;
    }
    const QPixmap* getPixmap() const { return pixmap; }
protected:
    const QPixmap* pixmap;
    int number;
    int counter;
};
#endif // IMAGEITEM_H
