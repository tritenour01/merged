#ifndef JOBWIDGET_H_INCLUDED
#define JOBWIDGET_H_INCLUDED

#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QGroupBox>
#include <QSignalMapper>
#include <QToolButton>
#include <QMessageBox>

#include <vector>

#include <jobManager.h>

struct tableEntry
{
    int entryID;
    QTableWidgetItem* nameItem;
    QTableWidgetItem* statusID;
    QTableWidgetItem* progressID;
    QToolButton* viewButton;
    QToolButton* deleteButton;
};

class JobWidget : public QWidget
{
    Q_OBJECT

    public:

        JobWidget(JobManager*);

        void addEntry(RenderJob*);

        void updateStatus(int, RenderJob::STATUS);
        void imageViewable(int);

    private slots:

        void viewButtonClick(int);
        void deleteButtonClick(int);

    private:

        vector<tableEntry*> entries;

        JobManager* manager;

        QSignalMapper* viewMapper;
        QSignalMapper* deleteMapper;

        QTableWidget* table;
};

#endif // JOBWIDGET_H_INCLUDED
