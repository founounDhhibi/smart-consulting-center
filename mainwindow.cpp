#include "mainwindow.h"
#include "ui_formation.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connexion à la base de données
    Connection c;
    if (c.createconnect()) {
        QMessageBox::information(this, "Connexion", "Connexion à la base de données réussie.");
    } else {
        QMessageBox::critical(this, "Connexion", "Échec de connexion à la base de données.");
    }

    // Connexion du bouton à la fonction ajouter
    connect(ui->ajouter, &QPushButton::clicked, this, &MainWindow::on_pushButton_ajouter_clicked);
    connect(ui->afficher, &QPushButton::clicked, this, &MainWindow::afficherFormations);
    connect(ui->supprimer, &QPushButton::clicked, this, &MainWindow::on_pushButton_supprimer_clicked);
    ui->status->addItem("En cours ");
    ui->status->addItem("Planifiée");
    ui->status->addItem("Terminée ");
    ui->status->addItem("Annulée ");
    ui->status->addItem("Reportée ");
    ui->status->addItem("À venir");
    ui->status->addItem("En attente");



}
void MainWindow::afficherFormations()
{
    QSqlQuery query;
    query.prepare("SELECT ID_FORMATION, TITRE, CENTRE, DATE_FORMATION, LOCALISATION FROM FORMATION");

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'affichage des formations:" << query.lastError().text();
        return;
    }

    // Effacer les anciennes données du tableau
    ui->table_formation->clear();
    ui->table_formation->setRowCount(0);
    ui->table_formation->setColumnCount(5);

    // Définir les en-têtes
    QStringList headers = {"ID", "Titre", "Centre", "Date", "Localisation"};
    ui->table_formation->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (query.next()) {
        ui->table_formation->insertRow(row);
        for (int col = 0; col < 5; col++) {
            ui->table_formation->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}




MainWindow::~MainWindow()
{
    delete ui;
}

// Fonction pour ajouter une formation
void MainWindow::on_pushButton_ajouter_clicked()
{

    QString titre = ui->titre->text();
    QString centre = ui->centre->text();
    QDate date = ui->date->date();
    QString localisation = ui->localisation->text();
    QString status = ui->status->currentText();

    formation f(0, titre, centre, date, localisation,status);

    if (f.ajouter()) {
        QMessageBox::information(this, "Ajout", "Formation ajoutée avec succès.");
    } else {
        QMessageBox::critical(this, "Ajout", "Échec de l'ajout de la formation.");
    }
}
void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    afficherFormations(); // Appel direct à votre fonction qui remplit le tableau
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    int row = ui->table_formation->currentRow(); // Récupérer la ligne sélectionnée
    if (row == -1) {
        QMessageBox::warning(this, "Suppression", "Veuillez sélectionner une formation à supprimer.");
        return;
    }

    int id = ui->table_formation->item(row, 0)->text().toInt(); // Récupérer l'ID de la formation

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer cette formation ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (f.supprimer(id)) {
            QMessageBox::information(this, "Suppression", "Formation supprimée avec succès.");
            afficherFormations(); // Rafraîchir l'affichage
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
        }
    }
}


