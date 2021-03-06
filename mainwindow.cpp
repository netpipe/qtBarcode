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
#include "generate_image.h"

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


    QString maxqrstr;
    for (int i=0 ;i < 2000 ; i++){ //4296 / 8 per ascii  537
        maxqrstr.append("i");
    }

 //   qDebug () << maxqrstr.toUtf8().size();
 //       qDebug () << maxqrstr.toLatin1().size();
         //   qDebug () << maxqrstr.toWCharArray().size();

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

void MainWindow::on_scan2_clicked()
{
#ifdef ZXING
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open rx/tx"), "./", tr("rx/tx files (*.png *.jpg)"));
    ui->decode->setText(decodeqr(fileName));

  //   char *argv1[]={"appname","-format","EAN13","test.png","test"};
    char *argv1[]={"appname","-format",ui->XZINGformat->currentText().toLocal8Bit().data(),fileName.toLocal8Bit().data(),"null"};
  // QString str1 = "appname","-format"","+ui->XZINGformat->currentText()+","+fileName.toLatin1()+"," "null";
  //  QByteArray ba = {"appname","-format",ui->XZINGformat->currentText(),fileName.toLatin1(),"null"}; //str1.toLocal8Bit();
 //   const char *argv1 = ba.data();

     int argc1 = sizeof(argv1) / sizeof(char*) - 1;
   ui->xzingdecode->setText( readbarcode(argc1, argv1 ).toLatin1());
#endif
}

void MainWindow::on_XZINGGenerate_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open rx/tx"), "./", tr("rx/tx files (*.png *.jpg)"));


//    QString maxqrstr;
//    for (int i=0 ;i < 2000 ; i++){ //4296 / 8 per ascii  537
//        maxqrstr.append("i");
//    }
//ui->xzingencode->setText(maxqrstr.toLatin1());
        char *argv1[]={"appname",ui->xzingformat2->currentText().toLocal8Bit().data(),ui->xzingencode->text().toLocal8Bit().data(),fileName.toLocal8Bit().data(),"null"};
  int argc1 = sizeof(argv1) / sizeof(char*) - 1;
       int result= xzingencode(argc1, argv1);
    if (result != -1){
        QImage *img_object = new QImage();
        img_object->load(fileName);
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

}
