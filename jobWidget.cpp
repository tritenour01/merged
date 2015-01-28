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

    //test->setData(0, "2");

    layout->addWidget(table, 1);

    setLayout(layout);
}

void JobWidget::addEntry(RenderJob* job)
{
    tableEntry* newEntry = new tableEntry();

    newEntry->entryID = job->getID();
    if(job->getNum() == 0)
        newEntry->nameItem = new QTableWidgetItem(job->getName());
    else
        newEntry->nameItem = new QTableWidgetItem(job->getName() + "-" + QString::number(job->getNum()));
    newEntry->progressID = new QTableWidgetItem(QString::number(job->getProgress()) + "%");
    newEntry->statusID = new QTableWidgetItem(RenderJob::statusToString(job->getStatus()));

    newEntry->viewButton = new QToolButton(this);
    QIcon* viewIcon = new QIcon("icons/view.png");
    //newEntry->viewButton->setIcon(*viewIcon);
    newEntry->viewButton->setText("View");
    newEntry->viewButton->setEnabled(false);
    viewMapper->setMapping(newEntry->viewButton, newEntry->entryID);
    connect(newEntry->viewButton, SIGNAL(clicked()), viewMapper, SLOT(map()));

    newEntry->deleteButton = new QToolButton(this);
    QIcon* deleteIcon = new QIcon("icons/delete.png");
    newEntry->deleteButton->setIcon(*deleteIcon);
    deleteMapper->setMapping(newEntry->deleteButton, newEntry->entryID);
    connect(newEntry->deleteButton, SIGNAL(clicked()), deleteMapper, SLOT(map()));

    int row = table->rowCount();
    table->insertRow(row);

    table->setItem(row, 0, newEntry->nameItem);
    table->setItem(row, 1, newEntry->statusID);
    table->setItem(row, 2, newEntry->progressID);
    table->setCellWidget(row, 3, newEntry->viewButton);
    table->setCellWidget(row, 4, newEntry->deleteButton);

    entries.push_back(newEntry);
}

void JobWidget::updateStatus(int id, RenderJob::STATUS s)
{
    tableEntry* entry = NULL;
    for(int i = 0; i < entries.size(); i++){
        if(entries[i]->entryID == id){
            entry = entries[i];
            break;
        }
    }

    entry->statusID->setData(0, RenderJob::statusToString(s));
}

void JobWidget::imageViewable(int id)
{
    tableEntry* entry = NULL;
    for(int i = 0; i < entries.size(); i++){
        if(entries[i]->entryID == id){
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

}
