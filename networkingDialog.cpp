#include <networkingDialog.h>
#include <QDebug>

NetworkingDialog::NetworkingDialog(QWidget* parent) :
    QDialog(parent)
{
    setWindowTitle("Networking");

    QVBoxLayout* mainLayout = new QVBoxLayout();

    table = new QTableWidget(0, 4, this);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Address"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("State"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("Connection"));

    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    table->setMinimumWidth(getTableWidgetWidth(table));
    table->setMaximumWidth(table->minimumWidth());

    mainLayout->addWidget(table);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    QHBoxLayout* modifyLayout = new QHBoxLayout();

    QLabel* label = new QLabel("Server: ");
    modifyLayout->addWidget(label);

    address = new QLineEdit(this);
    modifyLayout->addWidget(address);

    addButton = new QPushButton("+", this);
    addButton->setMaximumWidth(25);
    addButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(addButton, SIGNAL(released()), this, SLOT(addDevice()));
    modifyLayout->addWidget(addButton);

    removeButton = new QPushButton("-");
    removeButton->setMaximumWidth(25);
    removeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(removeButton, SIGNAL(released()), this, SLOT(removeDevice()));
    modifyLayout->addWidget(removeButton);

    mainLayout->addLayout(modifyLayout);

    setLayout(mainLayout);
}

void NetworkingDialog::addDevice(void)
{
    QString value = address->text();
    QHostAddress ip;
    if(validIP(value)){
        //add
    }
    else{
        QMessageBox mess;
        mess.setText("Invalid IP address");
        mess.exec();
    }
}

bool NetworkingDialog::validIP(QString ip)
{
    QStringList values = ip.split(".");
    if(values.length() != 4)
        return false;
    for(int i = 0; i < 4; i++){
        QString part = values[i];
        if(part.isEmpty())
            return false;
        bool ok = true;
        int num = part.toInt(&ok);
        if(!ok)
            return false;
        if(num < 0 || num > 255)
            return false;
    }
    return true;
}

void NetworkingDialog::removeDevice(void)
{

}

int NetworkingDialog::getTableWidgetWidth(QTableWidget* table)
{
    int size = table->verticalHeader()->width() + 4;
    for(int i = 0; i < table->colorCount(); i++)
        size += table->columnWidth(i);
    size += table->verticalScrollBar()->sizeHint().width();
    return size;
}
