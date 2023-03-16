// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 the VideoLAN team
 *****************************************************************************/

#ifndef COLORSCHEMEMODEL_HPP
#define COLORSCHEMEMODEL_HPP

#include <QAbstractListModel>

#include <memory>

class ColorSchemeModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString current READ currentText NOTIFY currentChanged FINAL)
    Q_PROPERTY(ColorScheme scheme READ currentScheme NOTIFY currentChanged FINAL)

public:
    enum ColorScheme
    {
        System,
        Day,
        Night
    };

    struct Item
    {
        QString text;
        ColorScheme scheme;
    };

    Q_ENUM(ColorScheme);

    explicit ColorSchemeModel(QObject* parent = nullptr);

    virtual int rowCount(const QModelIndex& parent) const override;
    virtual Qt::ItemFlags flags (const QModelIndex& index) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    int currentIndex() const;
    void setCurrentIndex(int newIndex);

    QString currentText() const;
    QVector<Item> getSchemes() const;

    ColorScheme currentScheme() const;

signals:
    void currentChanged();

private:
    class SchemeList
    {
    public:
        virtual ~SchemeList() = default;
        virtual ColorScheme scheme(int i) const = 0;
        virtual QString text(int i) const = 0;
        virtual int size() const = 0;
    };

    class DefaultSchemeList;

    static std::unique_ptr<SchemeList> createList(ColorSchemeModel *parent);

    // \internal used by SchemeList to notify scheme changed
    void indexChanged(int i);

    const QVector<ColorSchemeModel::Item> m_list;
    int m_currentIndex;
};

#endif // COLORSCHEMEMODEL_HPP
