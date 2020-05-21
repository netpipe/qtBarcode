#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QRCode/QrCodeGeneratorDemo.h"
#include <iostream>
#include <fstream>
#include <QGraphicsSvgItem>
#include <Barcode/functii.h>

using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::EAN13(QString country,QString ean){ //barcode

//    std::string code13 = EAN13::appendChecksum("123", "123456789"); //countrycode 3 letters,European Article Number 9 digits no spaces
//    std::string svg = EAN13::createSvg("productName test", code13);

    std::string code13 = EAN13::appendChecksum("123", "123456789"); //countrycode 3 letters,European Article Number 9 digits no spaces
    std::string svg = EAN13::createSvg("productName test", code13);

    ofstream write;
    std::string   filename = "tmp.svg";
    write.open(filename.c_str(), ios::out | ios::binary);
    write << svg.c_str();


}

void MainWindow::QRCode(QString text2) {

std::wstring text ( text2.toStdWString() );

//char *text2 = text.c_str();
const wchar_t* wstr = text.c_str() ;
    char mbstr[4000];
    std::wcstombs(mbstr, wstr, 4000);

    const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;  // Error correction level

    const QrCode qr = QrCode::encodeText( mbstr , errCorLvl);

    ofstream write;

    std::string   filename = "tmp.svg";
    write.open(filename.c_str(), ios::out | ios::binary);
    write << qr.toSvgString(4);

}

void MainWindow::on_generate_clicked()
{

    QRCode(ui->inputbox->text());

   // system("inkscape -z -e tmp.png -w 1000 -h 1000 tmp.svg");

    QGraphicsScene *scene= new QGraphicsScene();
    scene->addItem(new QGraphicsSvgItem("./tmp.svg"));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();

}

void MainWindow::on_ean13_clicked()
{
    EAN13(ui->ean13ean->text(),ui->ean13country->text());

    // system("inkscape -z -e tmp.png -w 1000 -h 1000 tmp.svg");

    QGraphicsScene *scene= new QGraphicsScene();
    scene->addItem(new QGraphicsSvgItem("./tmp.svg"));
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
}
