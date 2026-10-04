#pragma once

#include <QHeaderView>

class GroupedHeader final : public QHeaderView
{
    Q_OBJECT

public:
    explicit GroupedHeader(Qt::Orientation orientation, QWidget *parent = nullptr, std::vector<QString> deviceNames = {});

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<QString> m_deviceNames;
};
