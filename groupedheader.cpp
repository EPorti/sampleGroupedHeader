#include "groupedheader.h"

#include <QPainter>
#include <QPaintEvent>
#include <QStyle>
#include <QStyleOptionHeader>

GroupedHeader::GroupedHeader(Qt::Orientation orientation, QWidget *parent, std::vector<QString> deviceNames)
    : QHeaderView(orientation, parent), m_deviceNames(std::move(deviceNames))
{
    setSectionsClickable(false);
    setSectionsMovable(false);
    setMinimumSectionSize(55);
    setDefaultSectionSize(90);
}

QSize GroupedHeader::sizeHint() const
{
    QSize result = QHeaderView::sizeHint();
    if (orientation() == Qt::Horizontal)
        result.setHeight(fontMetrics().height() * 2 + 14);
    return result;
}

void GroupedHeader::paintEvent(QPaintEvent *event)
{
    if (orientation() != Qt::Horizontal) {
        QHeaderView::paintEvent(event);
        return;
    }

    QPainter painter(viewport());
    painter.setClipRegion(event->region());

    const int headerHeight = viewport()->height();
    const int groupHeight = headerHeight / 2;
    const int count = this->count();
    const QPalette colors = palette();
    const QRect dirty = event->rect();

    painter.fillRect(dirty, colors.button());

    // Draw the top-level device labels, each spanning three sections.
    for (int first = 0; first < count; first += 3) {
        const int last = qMin(first + 2, count - 1);
        const int left = sectionViewportPosition(first);
        const int right = sectionViewportPosition(last) + sectionSize(last);
        QRect groupRect(left, 0, right - left, groupHeight);
        if (!groupRect.intersects(dirty))
            continue;

        painter.fillRect(groupRect, colors.button());
        painter.setPen(colors.buttonText().color());
        
        // const QString device = tr("Device %1").arg(first / 3 + 1);
        const QString device = m_deviceNames.at(first / 3); // Use the device name from the vector instead of a generic label

        painter.drawText(groupRect, Qt::AlignCenter, device);
        painter.setPen(colors.mid().color());
        painter.drawLine(groupRect.topLeft(), groupRect.topRight());
        painter.drawLine(groupRect.bottomLeft(), groupRect.bottomRight());
        painter.drawLine(groupRect.topRight(), groupRect.bottomRight());
    }

    // Draw each leaf section in the second header row.
    for (int logical = 0; logical < count; ++logical) {
        const int left = sectionViewportPosition(logical);
        const int width = sectionSize(logical);
        QRect sectionRect(left, groupHeight, width, headerHeight - groupHeight);
        if (!sectionRect.intersects(dirty))
            continue;

        QStyleOptionHeader option;
        initStyleOption(&option);
        option.rect = sectionRect;
        option.section = logical;
        option.text = model()->headerData(logical, orientation(), Qt::DisplayRole).toString();
        option.textAlignment = Qt::AlignCenter;
        option.position = QStyleOptionHeader::Middle;
        if (logical == 0)
            option.position = QStyleOptionHeader::Beginning;
        else if (logical == count - 1)
            option.position = QStyleOptionHeader::End;
        style()->drawControl(QStyle::CE_Header, &option, &painter, this);
    }

    // Separate device groups with a full-height rule.
    for (int boundary = 3; boundary < count; boundary += 3) {
        const int x = sectionViewportPosition(boundary);
        painter.setPen(colors.dark().color());
        painter.drawLine(x, 0, x, headerHeight - 1);
    }
}
