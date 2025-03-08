// formation.h
#ifndef FORMATION_H
#define FORMATION_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class formation
{
public:
    formation();
    formation(int id, QString titre, QString centre, QDate date, QString localisation, QString status); // Ajout de status
    ~formation();

    // Getters
    int getId() const;
    QString getTitre() const;
    QString getCentre() const;
    QDate getDateFormation() const;
    QString getLocalisation() const; // Getter pour localisation
    QString getStatus() const; // Getter pour status

    // Setters
    void setId(int id);
    void setTitre(QString titre);
    void setCentre(QString centre);
    void setDateFormation(QDate date);
    void setLocalisation(QString localisation); // Setter pour localisation
    void setStatus(QString status); // Setter pour status

    // Fonctions de gestion des formations
    bool ajouter();
    bool modifier(int id);
    bool supprimer(int id);
    QSqlQueryModel *afficher();

private:
    int id_formation;
    QString titre;
    QString centre;
    QDate date_formation;
    QString localisation; // Ajout de localisation
    QString status; // Ajout de status
};

#endif // FORMATION_H
