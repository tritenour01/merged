#ifndef NETWORKINGDIALOG_H
#define NETWORKINGDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QScrollBar>
#include <QHeaderView>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHostAddress>
#include <QMessageBox>

class NetworkingDialog : public QDialog
{
    Q_OBJECT

    public:

        NetworkingDialog(QWidget*);

    private slots:
        void addDevice(void);
        void removeDevice(void);

    private:

        QTableWidget* table;

        QLineEdit* address;

        QPushButton* addButton;
        QPushButton* removeButton;

        int getTableWidgetWidth(QTableWidget*);
        bool validIP(QString);
};

#endif // NETWORKINGDIALOG_H
