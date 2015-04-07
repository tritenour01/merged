#include <jobWidget.h>

JobWidget::JobWidget(JobManager* m)
{
    manager = m;
    manager->setWidget(this);

    viewMapper = new QSignalMapper(this);
    connect(viewMapper, SIGNAL(mapped(int)), this, SLOT(viewButtonClick(int)));

    deleteMapper = new QSignalMapper(this);
    connect(deleteMapper, SIGNAL(mapped(int)), this, SLOT(deleteButtonClick(int)));

    QVBoxLayout* layout = new QVBoxLayout;

    table = new QTableWidget(0, 5, this);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Status"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("Progress"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem(""));
    table->setHorizontalHeaderItem(4, new QTableWidgetItem(""));

    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    layout->addWidget(table, 1);

    setLayout(layout);
}

void JobWidget::addEntry(RenderJob* job)
{
    TableEntry* newEntry = new TableEntry(job);

    viewMapper->setMapping(newEntry->viewButton, newEntry->getID());
    connect(newEntry->viewButton, SIGNAL(clicked()), viewMapper, SLOT(map()));

    deleteMapper->setMapping(newEntry->deleteButton, newEntry->getID());
    connect(newEntry->deleteButton, SIGNAL(clicked()), deleteMapper, SLOT(map()));

    int row = table->rowCount();
    table->insertRow(row);

    table->setItem(row, 0, newEntry->nameItem);
    table->setItem(row, 1, newEntry->statusItem);
    table->setItem(row, 2, newEntry->progressItem);
    table->setCellWidget(row, 3, newEntry->viewButton);
    table->setCellWidget(row, 4, newEntry->deleteButton);

    entries.push_back(newEntry);
}

void JobWidget::imageViewable(int id)
{
    TableEntry* entry = NULL;
    for(int i = 0; i < entries.size(); i++){
        if(entries[i]->getID() == id){
            entry = entries[i];
            break;
        }
    }

    entry->viewButton->setEnabled(true);
}

void JobWidget::viewButtonClick(int id)
{
    manager->showViewer(id);
}

void JobWidget::deleteButtonClick(int id)
{
    manager->killJob(id);
}

TableEntry::TableEntry(RenderJob* job)
{
    entryID = job->getID();
    if(job->getNum() == 0)
        nameItem = new QTableWidgetItem(job->getName());
    else
        nameItem = new QTableWidgetItem(job->getName() + "-" + QString::number(job->getNum()));
    progressItem = new QTableWidgetItem(QString::number(job->getProgress()) + "%");
    statusItem = new QTableWidgetItem(RenderJob::statusToString(job->getStatus()));

    viewButton = new QToolButton(this);
    QIcon* viewIcon = new QIcon("icons/view.png");
    viewButton->setText("View");
    viewButton->setEnabled(false);

    deleteButton = new QToolButton(this);
    QIcon* deleteIcon = new QIcon("icons/delete.png");
    deleteButton->setIcon(*deleteIcon);

    connect(job, SIGNAL(statusChanged(RenderJob::STATUS)), this, SLOT(updateStatus(RenderJob::STATUS)));
    connect(job, SIGNAL(progressChanged(int)), this, SLOT(updateProgress(int)));
}

int TableEntry::getID(void)
{
    return entryID;
}

void TableEntry::updateStatus(RenderJob::STATUS s)
{
    statusItem->setData(0, RenderJob::statusToString(s));
    if(s == RenderJob::STATUS::Complete)
        deleteButton->setEnabled(false);
}

void TableEntry::updateProgress(int p)
{
    progressItem->setData(0, QString::number(p) + "%");
}
