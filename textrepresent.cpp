#include "textrepresent.h"
#include "ui_textrepresent.h"

TextRepresent::TextRepresent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextRepresent)
{
    ui->setupUi(this);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));    ///< Для рядкових констант
}

void TextRepresent::setText(QString text)
{

    ui->textBrowser->setText(text);
}

void TextRepresent::setHeader(QString header)
{
    ui->labelHead->setText(header);
}

TextRepresent::~TextRepresent()
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));    ///< Для рядкових констант
    delete ui;
}
