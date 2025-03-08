#include "formationui.h"
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

    // Activer l'édition des cellules lors d'un double-clic
    connect(ui->table_formation, &QTableWidget::doubleClicked, this, [=](const QModelIndex &index) {
        int row = index.row(); // Récupérer l'index de la ligne double-cliquée
        this->modifierFormation(row); // Appeler la fonction modifierFormation
    });
}
void MainWindow::afficherFormations()
{
    QSqlQuery query;
    query.prepare("SELECT ID_FORMATION, TITRE, CENTRE, DATE_FORMATION, LOCALISATION, STATUS FROM FORMATION");

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'affichage des formations:" << query.lastError().text();
        return;
    }

    // Effacer les anciennes données du tableau
    ui->table_formation->clear();
    ui->table_formation->setRowCount(0);
    ui->table_formation->setColumnCount(6); // Supprimer la colonne 6 (bouton Modifier)

    // Définir les en-têtes
    QStringList headers = {"ID", "Titre", "Centre", "Date", "Localisation", "Status"};
    ui->table_formation->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (query.next()) {
        ui->table_formation->insertRow(row);
        for (int col = 0; col < 6; col++) {
            ui->table_formation->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}
void MainWindow::modifierFormation(int row)
{
    int id = ui->table_formation->item(row, 0)->text().toInt();

    // Activer l'édition des cellules
    for (int col = 1; col < 6; col++) { // On ne touche pas à l'ID
        ui->table_formation->item(row, col)->setFlags(ui->table_formation->item(row, col)->flags() | Qt::ItemIsEditable);
    }

    // Récupérer le bouton "Modifier" depuis l'interface utilisateur
    QPushButton *btnModifier = ui->modifier; // Utilisez le nom d'objet du bouton

    if (btnModifier) {
        // Déconnecter les anciens signaux (si nécessaire)
        disconnect(btnModifier, &QPushButton::clicked, nullptr, nullptr);

        // Connecter le bouton à la fonction de sauvegarde
        connect(btnModifier, &QPushButton::clicked, this, [=]() {
            QString titre = ui->table_formation->item(row, 1)->text();
            QString centre = ui->table_formation->item(row, 2)->text();
            QDate date = QDate::fromString(ui->table_formation->item(row, 3)->text(), "yyyy-MM-dd");
            QString localisation = ui->table_formation->item(row, 4)->text();
            QString status = ui->table_formation->item(row, 5)->text();

            // Créer un objet formation et modifier la base de données
            formation f(id, titre, centre, date, localisation, status);
            if (f.modifier(id)) {
                QMessageBox::information(this, "Modification", "Formation mise à jour avec succès.");
            } else {
                QMessageBox::critical(this, "Erreur", "Échec de la mise à jour.");
            }

            // Rafraîchir l'affichage
            afficherFormations();
        });
    } else {
        qDebug() << "Erreur : Bouton Modifier non trouvé.";
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_ajouter_clicked()
{
    // Récupérer les valeurs des champs de l'interface utilisateur
    QString titre = ui->titre->text();
    QString centre = ui->centre->text();
    QDate date = ui->date->date();
    QString localisation = ui->localisation->text();
    QString status = ui->status->currentText();

    // Créer un objet formation
    formation f(0, titre, centre, date, localisation, status);

    // Appeler la méthode ajouter() de la classe formation
    if (f.ajouter()) {
        // Réinitialiser les champs de l'interface utilisateur après un ajout réussi
        ui->titre->clear();          // Effacer le champ titre
        ui->centre->clear();         // Effacer le champ centre
        ui->localisation->clear();   // Effacer le champ localisation
        ui->status->setCurrentIndex(0); // Réinitialiser le statut à la première option
        ui->date->setDate(QDate::currentDate()); // Réinitialiser la date à la date actuelle

        // Afficher un message de succès
        QMessageBox::information(this, "Ajout", "Formation ajoutée avec succès.");

        // Rafraîchir l'affichage des formations dans le tableau
        afficherFormations();
    } else {
        // Afficher un message d'erreur en cas d'échec
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


