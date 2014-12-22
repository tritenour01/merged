#ifndef NETWORKINGDIALOG_H
#define NETWORKINGDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QScrollBar>
#include <QHeaderView>

class NetworkingDialog : public QDialog
{
    public:

        NetworkingDialog(void);

    private:

        int getTableWidgetWidth(QTableWidget*);
};

#endif // NETWORKINGDIALOG_H
