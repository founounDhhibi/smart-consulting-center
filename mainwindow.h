#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "formation.h"
#include "connection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showEvent(QShowEvent *event) override;

private slots:
    void on_pushButton_ajouter_clicked(); // Slot pour ajouter une formation
    void afficherFormations();
    void on_pushButton_supprimer_clicked();

private:
    Ui::MainWindow *ui;
    formation f; // Instance de formation
};

#endif // MAINWINDOW_H

