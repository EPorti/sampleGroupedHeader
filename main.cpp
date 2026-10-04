#include "groupedheader.h" 

#include <QApplication>
#include <QStandardItemModel>
#include <QTableView>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto *model = new QStandardItemModel(5, 6);
    model->setHorizontalHeaderLabels({"val1", "val2", "val3", "val1", "val2", "val3", "val1", "val2"});
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int column = 0; column < model->columnCount(); ++column)
            model->setItem(row, column, new QStandardItem(QString("%1").arg((row + 1) * (column + 1))));
    }

    QTableView table;
    table.setWindowTitle("Grouped device headers");
    table.setModel(model);
    std::vector<QString> deviceNames = {"Device A", "Device B", "Device C"};
    table.setHorizontalHeader(new GroupedHeader(Qt::Horizontal, &table, deviceNames));
    table.horizontalHeader()->setStretchLastSection(true);
    table.verticalHeader()->setVisible(false);
    table.resize(650, 300);
    table.show();

    return app.exec();
}
