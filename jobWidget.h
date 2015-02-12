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

class TableEntry : public QWidget
{
    Q_OBJECT

    public:

        TableEntry(RenderJob*);

        int getID(void);

        QTableWidgetItem* nameItem;
        QTableWidgetItem* statusItem;
        QTableWidgetItem* progressItem;
        QToolButton* viewButton;
        QToolButton* deleteButton;

    private slots:

        void updateStatus(RenderJob::STATUS);
        void updateProgress(int);

    private:

        int entryID;
};

class JobWidget : public QWidget
{
    Q_OBJECT

    public:

        JobWidget(JobManager*);

        void addEntry(RenderJob*);

        void imageViewable(int);

    private slots:

        void viewButtonClick(int);
        void deleteButtonClick(int);

    private:

        vector<TableEntry*> entries;

        JobManager* manager;

        QSignalMapper* viewMapper;
        QSignalMapper* deleteMapper;

        QTableWidget* table;
};

#endif // JOBWIDGET_H_INCLUDED
