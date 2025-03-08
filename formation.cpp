#include "formation.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QMessageBox>

// Constructeurs
formation::formation()
    : id_formation(0), titre(""), centre(""), date_formation(QDate::currentDate()), localisation(""), status("") {}

formation::formation(int id, QString titre, QString centre, QDate date, QString localisation, QString status)
    : id_formation(id), titre(titre), centre(centre), date_formation(date), localisation(localisation), status(status) {}

formation::~formation() {}

// Getters
int formation::getId() const { return id_formation; }
QString formation::getTitre() const { return titre; }
QString formation::getCentre() const { return centre; }
QDate formation::getDateFormation() const { return date_formation; }
QString formation::getLocalisation() const { return localisation; }
QString formation::getStatus() const { return status; } // Getter pour status

// Setters
void formation::setId(int id) { id_formation = id; }
void formation::setTitre(QString titre) { this->titre = titre; }
void formation::setCentre(QString centre) { this->centre = centre; }
void formation::setDateFormation(QDate date) { date_formation = date; }
void formation::setLocalisation(QString localisation) { this->localisation = localisation; }
void formation::setStatus(QString status) { this->status = status; } // Setter pour status

bool formation::ajouter()
{
    // Vérification des champs obligatoires
    if (titre.isEmpty() || centre.isEmpty() || localisation.isEmpty() || status.isEmpty()) {
        QMessageBox::critical(nullptr, "Erreur", "Veuillez remplir tous les champs requis.");
        return false;
    }

    // Validation des longueurs des champs (titre et centre)
    if (titre.length() < 5 || titre.length() > 20) {
        QMessageBox::critical(nullptr, "Erreur", "Le titre doit contenir entre 5 et 20 caractères.");
        return false;
    }

    if (centre.length() < 5 || centre.length() > 20) {
        QMessageBox::critical(nullptr, "Erreur", "Le centre doit contenir entre 5 et 20 caractères.");
        return false;
    }

    // Validation de la date
    if (!date_formation.isValid() || date_formation < QDate::currentDate()) {
        QMessageBox::critical(nullptr, "Erreur", "La date de formation est invalide ou antérieure à aujourd'hui.");
        return false;
    }

    // Validation du statut
    QStringList statutsValides = {"En cours", "Planifiée", "Terminée", "Annulée", "Reportée", "À venir", "En attente"};
    if (!statutsValides.contains(status)) {
        QMessageBox::critical(nullptr, "Erreur", "Le statut doit être l'une des valeurs suivantes : En cours, Planifiée, Terminée, Annulée, Reportée, À venir, En attente.");
        return false;
    }

    // Préparation de la requête SQL
    QSqlQuery query;
    query.prepare("INSERT INTO FORMATION (TITRE, CENTRE, DATE_FORMATION, LOCALISATION, STATUS) "
                  "VALUES (:titre, :centre, :date, :localisation, :status)");
    query.bindValue(":titre", titre);
    query.bindValue(":centre", centre);
    query.bindValue(":date", date_formation);
    query.bindValue(":localisation", localisation);
    query.bindValue(":status", status);

    if (query.exec()) {
        return true; // Ajout réussi
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de l'ajout de la formation : " + query.lastError().text());
        return false; // Ajout échoué
    }
}
// Modifier une formation
bool formation::modifier(int id)
{
    // Vérification des champs obligatoires
    if (titre.isEmpty() || centre.isEmpty() || localisation.isEmpty() || status.isEmpty()) {
        QMessageBox::critical(nullptr, "Erreur", "Veuillez remplir tous les champs requis.");
        return false;
    }

    // Validation des longueurs des champs (titre et centre)
    if (titre.length() < 5 || titre.length() > 20) {
        QMessageBox::critical(nullptr, "Erreur", "Le titre doit contenir entre 5 et 20 caractères.");
        return false;
    }

    if (centre.length() < 5 || centre.length() > 20) {
        QMessageBox::critical(nullptr, "Erreur", "Le centre doit contenir entre 5 et 20 caractères.");
        return false;
    }

    // Préparation de la requête SQL
    QSqlQuery query;
    query.prepare("UPDATE FORMATION SET TITRE=:titre, CENTRE=:centre, DATE_FORMATION=:date, LOCALISATION=:localisation, STATUS=:status "
                  "WHERE ID_FORMATION=:id");
    query.bindValue(":id", id);
    query.bindValue(":titre", titre);
    query.bindValue(":centre", centre);
    query.bindValue(":date", date_formation.toString("yyyy-MM-dd"));
    query.bindValue(":localisation", localisation);
    query.bindValue(":status", status);

    if (query.exec()) {
        return true; // Modification réussie
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la modification de la formation : " + query.lastError().text());
        return false; // Modification échouée
    }
}

// Supprimer une formation
bool formation::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM FORMATION WHERE ID_FORMATION=:id");
    query.bindValue(":id", id);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur suppression formation:" << query.lastError().text();
        return false;
    }
}

// Afficher la liste des formations
QSqlQueryModel *formation::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_FORMATION, TITRE, CENTRE, DATE_FORMATION, LOCALISATION, STATUS FROM FORMATION");
    return model;
}
