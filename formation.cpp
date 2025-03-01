#include "formation.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

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

// Ajouter une formation
bool formation::ajouter()
{

    QSqlQuery query;
    QString res = QString::number(id_formation);
    query.prepare("INSERT INTO FORMATION (ID_FORMATION, TITRE, CENTRE, DATE_FORMATION, LOCALISATION, STATUS) "
                  "VALUES (:id, :titre, :centre, :date, :localisation, :status)");
    query.bindValue(":id", res);
    query.bindValue(":titre", titre);
    query.bindValue(":centre", centre);
    query.bindValue(":date", date_formation);
    query.bindValue(":localisation", localisation);
    query.bindValue(":status", status); // Ajouter le statut à la requête

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur ajout formation:" << query.lastError().text();
        return false;
    }
}

// Modifier une formation
bool formation::modifier(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE FORMATION SET TITRE=:titre, CENTRE=:centre, DATE_FORMATION=:date, LOCALISATION=:localisation, STATUS=:status "
                  "WHERE ID_FORMATION=:id");
    query.bindValue(":id", id);
    query.bindValue(":titre", titre);
    query.bindValue(":centre", centre);
    query.bindValue(":date", date_formation);
    query.bindValue(":localisation", localisation);
    query.bindValue(":status", status); // Ajouter le statut à la requête

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur modification formation:" << query.lastError().text();
        return false;
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
