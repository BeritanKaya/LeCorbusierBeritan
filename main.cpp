#include "widget.h"
#include <QApplication>
#include <QtSql/QSqlDatabase>
#include <QtDebug>
#include <QtSql/QSqlQuery>
#include <QSqlError>

int main(int argc, char *argv[])
{
    // Création de l'application Qt
    QApplication a(argc, argv);
    Widget w;  // Création de la fenêtre principale de l'application

    // Création de la connexion à la base de données MySQL
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("test");  // Nom de la base de données
    db.setHostName("127.0.0.1");  // Adresse de l'hôte (ici, localhost)
    db.setPort(3306);  // Port de la base de données (port par défaut de MySQL)
    db.setUserName("etudiant");  // Nom d'utilisateur pour se connecter à la base
    db.setPassword("Passciel2");  // Mot de passe pour se connecter à la base

    // Ouvrir la connexion à la base de données
    if (db.open())
    {
        qDebug() << "Vous êtes maintenant connecté à " << db.hostName();  // Affiche un message de succès

        // Créer une requête SQL pour récupérer toutes les données de la table 'jeu'
        QSqlQuery query;

        // Exécuter la requête SQL pour sélectionner toutes les lignes de la table 'jeu'
        query.exec("SELECT * FROM `jeu`");

        // Parcourir les résultats de la requête
        while (query.next()) {
            // Récupérer les valeurs de chaque colonne pour chaque ligne
            QString id = query.value(0).toString();  // ID (colonne 0)
            QString Nom = query.value(1).toString();  // Nom (colonne 1)
            QString club = query.value(2).toString();  // Club (colonne 2)
            QString note = query.value(3).toString();  // Note (colonne 3)

            // Afficher les informations récupérées dans la console de débogage
            qDebug() << Nom << club << note;
        }

        qDebug() << "Requête ok ! :)";  // Affiche un message si la requête est exécutée avec succès
        db.close();  // Ferme la connexion à la base de données
    }
    else
    {
        // Si la connexion échoue, afficher un message d'erreur
        qDebug() << "La connexion a échouée, désolé" << db.lastError().text();
    }

    w.show();  // Afficher la fenêtre principale de l'application
    return a.exec();  // Lancer la boucle d'événements de l'application Qt
}
