#include <networkingDialog.h>

NetworkingDialog::NetworkingDialog(void)
{
    setWindowTitle("Networking");

    QVBoxLayout* layout = new QVBoxLayout();

    QTableWidget* widget = new QTableWidget(10, 4, this);
    widget->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
    widget->setHorizontalHeaderItem(1, new QTableWidgetItem("Address"));
    widget->setHorizontalHeaderItem(2, new QTableWidgetItem("State"));
    widget->setHorizontalHeaderItem(3, new QTableWidgetItem("Connection"));

    //widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    widget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    widget->setSelectionBehavior(QAbstractItemView::SelectRows);

    //widget->adjustSize();

    widget->setMinimumWidth(getTableWidgetWidth(widget));
    widget->setMaximumWidth(widget->minimumWidth());

    layout->addWidget(widget);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(layout);
}

int NetworkingDialog::getTableWidgetWidth(QTableWidget* table)
{
    int size = table->verticalHeader()->width() + 4;
    for(int i = 0; i < table->colorCount(); i++)
        size += table->columnWidth(i);
    size += table->verticalScrollBar()->sizeHint().width();
    return size;
}
