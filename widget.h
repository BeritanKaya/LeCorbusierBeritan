#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTableWidget>  // Inclure pour utiliser QTableWidget, un widget permettant d'afficher des tableaux de données

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT  // Macro nécessaire pour utiliser le système de signal et slot de Qt

public:
    // Constructeur de la classe Widget
    explicit Widget(QWidget *parent = nullptr);
    // Destructeur de la classe Widget
    ~Widget();

private:
    Ui::Widget *ui;  // Pointeur vers l'interface utilisateur générée par Qt Designer

    // Déclaration du QTableWidget pour afficher les données sous forme de tableau
    QTableWidget *tableWidget;

private slots:
    // Slot appelé lorsque l'utilisateur modifie une cellule du QTableWidget
    void updateNoteInDatabase(QTableWidgetItem* item);
};

#endif // WIDGET_H
