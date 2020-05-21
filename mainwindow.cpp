#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QRCode/QrCodeGeneratorDemo.h"
#include <iostream>
#include <fstream>
#include <QGraphicsSvgItem>
#include <Barcode/functii.h>
#include <QFileDialog>
#include "quirc/tests/inspect.h"
#include "scan_image.h"

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

void MainWindow::EAN13(QString productname,QString country,QString ean){ //barcode

//    std::string code13 = EAN13::appendChecksum("123", "123456789"); //countrycode 3 letters,European Article Number 9 digits no spaces
//    std::string svg = EAN13::createSvg("productName test", code13);

    std::string code13 = EAN13::appendChecksum(country.toLatin1(), ean.toLatin1()); //countrycode 3 letters,European Article Number 9 digits no spaces
    std::string svg = EAN13::createSvg( productname.toStdString(), code13);

    ofstream write;
    std::string   filename = "tmp.svg";
    write.open(filename.c_str(), ios::out | ios::binary);
    write << svg.c_str();

    QImage *img_object = new QImage();
    img_object->load("./tmp.svg");
    QPixmap image = QPixmap::fromImage(*img_object);
 //   QPixmap scaled_img = image.scaled(this->width(), this->height(), Qt::KeepAspectRatio);
    QPixmap scaled_img = image.scaled(ui->graphicsView->width(), ui->graphicsView->height(), Qt::KeepAspectRatio);
    QGraphicsScene *scene= new QGraphicsScene();
   // scene->addItem(new QGraphicsSvgItem("./tmp.svg"));
    scene->addPixmap(scaled_img);
    scene->setSceneRect(scaled_img.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();

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

    QImage *img_object = new QImage();
    img_object->load("./tmp.svg");
    QPixmap image = QPixmap::fromImage(*img_object);
 //   QPixmap scaled_img = image.scaled(this->width(), this->height(), Qt::KeepAspectRatio);
    QPixmap scaled_img = image.scaled(ui->graphicsView->width(), ui->graphicsView->height(), Qt::KeepAspectRatio);
    QGraphicsScene *scene= new QGraphicsScene();
   // scene->addItem(new QGraphicsSvgItem("./tmp.svg"));
    scene->addPixmap(scaled_img);
    scene->setSceneRect(scaled_img.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();

}

void MainWindow::on_generate_clicked()
{

    QRCode(ui->inputbox->text());

   // system("inkscape -z -e tmp.png -w 1000 -h 1000 tmp.svg");

}

void MainWindow::on_ean13_clicked()
{
    EAN13(ui->eanproductname->text(),ui->ean13ean->text(),ui->ean13country->text());

    // system("inkscape -z -e tmp.png -w 1000 -h 1000 tmp.svg");

}

void MainWindow::on_savetofile_clicked()
{
//    QString fileName = "./file_name.png";
//    QPixmap pixMap = ui->graphicsView->grab(ui->graphicsView->sceneRect().toRect());
//    pixMap.save(fileName);

    QString fileName= QFileDialog::getSaveFileName(this, "Save image", QCoreApplication::applicationDirPath(), "BMP Files (*.bmp);;JPEG (*.JPEG);;PNG (*.png)" );
        if (!fileName.isNull())
        {
            QPixmap pixMap = this->ui->graphicsView->grab();
            pixMap.save(fileName);
        }

}

void MainWindow::on_scan_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open rx/tx"), "./", tr("rx/tx files (*.png *.jpg)"));
    ui->decode->setText(decodeqr(fileName));

}
