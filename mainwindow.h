#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void QRCode(QString text);
   void EAN13(QString productname,QString country,QString ean);
QString decodeqr(QString image);
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_generate_clicked();

    void on_ean13_clicked();

    void on_savetofile_clicked();

    void on_scan_clicked();

    void on_scan2_clicked();

    void on_XZINGGenerate_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
