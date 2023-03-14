/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VLC_QT_SELECTABLE_LIST_MODEL_HPP_
#define VLC_QT_SELECTABLE_LIST_MODEL_HPP_

#include <QAbstractListModel>

namespace vlc {

class SelectableListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int selectedCount READ getSelectedCount NOTIFY selectedCountChanged FINAL)
public:
    SelectableListModel(QObject *parent = nullptr) :
        QAbstractListModel(parent) {}

    virtual ~SelectableListModel() = default;

    Q_INVOKABLE bool isSelected(int index) const;
    Q_INVOKABLE void setSelected(int index, bool selected);
    Q_INVOKABLE void toggleSelected(int index);
    Q_INVOKABLE void setSelection(const QList<int> &sortedIndexes);
    Q_INVOKABLE QList<int> getSelection() const;
    Q_INVOKABLE void setRangeSelected(int first, int count, bool selected);
    Q_INVOKABLE void selectAll();
    Q_INVOKABLE void deselectAll();

    virtual int getSelectedCount() const;

signals:
    void selectedCountChanged();

protected:
    virtual bool isRowSelected(int row) const = 0;
    virtual void setRowSelected(int row, bool selected) = 0;

    /* return the role to notify when selection changes */
    virtual int getSelectedRole() const = 0;
};

} // namespace vlc

#endif
