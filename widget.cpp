#include "widget.h"
#include "ui_widget.h"
#include <QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QTableWidget>
#include <QtDebug>
#include <QSqlError>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // Initialiser le QTableWidget (tableau pour afficher les données)
    tableWidget = new QTableWidget(this);
    tableWidget->setRowCount(0);  // Commencer avec 0 ligne dans le tableau
    tableWidget->setColumnCount(3);  // Trois colonnes : Nom, Club, Note
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Nom" << "Club" << "Note");

    // Activer l'édition des cellules (permet de modifier les données)
    tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    // Connexion à la base de données MySQL
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("test");  // Nom de la base de données
    db.setUserName("etudiant");  // Nom d'utilisateur pour la connexion
    db.setPassword("Passciel2");  // Mot de passe pour la connexion

    // Vérifier si la connexion à la base de données a réussi
    if (db.open())
    {
        qDebug() << "Connecté à la base de données.";

        // Créer une requête SQL pour récupérer les données de la table 'jeu'
        QSqlQuery query("SELECT * FROM `jeu`");

        // Remplir le QTableWidget avec les données récupérées
        int row = 0;  // Compteur pour les lignes du tableau
        while (query.next()) {
            QString nom = query.value(1).toString();  // Récupérer le nom
            QString club = query.value(2).toString();  // Récupérer le club
            int note = query.value(3).toInt();  // Récupérer la note

            // Insérer une nouvelle ligne dans le tableau
            tableWidget->insertRow(row);
            // Ajouter les données de chaque colonne dans la ligne correspondante
            tableWidget->setItem(row, 0, new QTableWidgetItem(nom));  // Nom
            tableWidget->setItem(row, 1, new QTableWidgetItem(club));  // Club
            tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(note)));  // Note
            row++;  // Passer à la ligne suivante
        }

        // Redimensionner et positionner le tableau dans la fenêtre
        tableWidget->resize(400, 300);
        tableWidget->move(50, 50);

        // Connecter le signal 'itemChanged' au slot 'updateNoteInDatabase'
        connect(tableWidget, &QTableWidget::itemChanged, this, &Widget::updateNoteInDatabase);

        db.close();  // Fermer la connexion à la base de données
    }
    else
    {
        qDebug() << "Échec de la connexion à la base de données:" << db.lastError().text();
    }
}

void Widget::updateNoteInDatabase(QTableWidgetItem* item)
{
    // Vérifier si la colonne modifiée est la colonne "Note" (colonne 2)
    if (item->column() != 2) return;  // Si ce n'est pas la colonne 2, ignorer

    // Récupérer la ligne et la nouvelle valeur de la note
    int row = item->row();
    bool ok;
    int newNote = item->text().toInt(&ok);  // Essayer de convertir la note en entier

    // Si la note n'est pas un entier valide, afficher une erreur et arrêter
    if (!ok) {
        qDebug() << "La note n'est pas un entier valide";
        return;
    }

    // Récupérer le nom du joueur (colonne 0)
    QString nom = tableWidget->item(row, 0)->text();

    // Créer une connexion à la base de données
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("test");
    db.setUserName("etudiant");
    db.setPassword("Passciel2");

    // Vérifier si la connexion à la base de données a réussi
    if (db.open()) {
        // Préparer la requête SQL pour mettre à jour la note du joueur
        QSqlQuery query;
        query.prepare("UPDATE `jeu` SET `Note` = :note WHERE `Nom` = :nom");
        query.bindValue(":note", newNote);  // Lier la nouvelle note
        query.bindValue(":nom", nom);  // Lier le nom du joueur

        // Exécuter la requête pour mettre à jour la note dans la base de données
        if (query.exec()) {
            qDebug() << "Nouvelle note pour" << nom << "qui passe à:" << newNote;
        } else {
            qDebug() << "Erreur lors de la mise à jour de la note:" << query.lastError().text();
        }

        db.close();  // Fermer la connexion après la mise à jour
    } else {
        qDebug() << "Échec de la connexion pour la mise à jour de la note.";
    }
}

Widget::~Widget()
{
    delete ui;  // Libérer la mémoire allouée pour l'interface utilisateur
}
