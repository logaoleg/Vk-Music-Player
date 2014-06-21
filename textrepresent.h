/**
  Class for represent showing track's lyrics
  **/

#ifndef TEXTREPRESENT_H
#define TEXTREPRESENT_H

#include <QDialog>
#include <QString>
#include<QTextCodec>

namespace Ui {
class TextRepresent;
}

class TextRepresent : public QDialog
{
    Q_OBJECT
    
public:
    explicit TextRepresent(QWidget *parent = 0);
    void setText(QString text); //setLyrics
    void setHeader(QString header); // set name of track in header
    ~TextRepresent();
    
private:
    Ui::TextRepresent *ui;
};

#endif // TEXTREPRESENT_H
