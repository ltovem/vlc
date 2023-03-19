// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * convert.hpp : GotoTime dialogs
 *****************************************************************************
 * Copyright (C) 2007 the VideoLAN team
 *
 * Authors: Jean-Baptiste Kempf <jb (at) videolan.org>
 *****************************************************************************/

#ifndef QVLC_CONVERT_DIALOG_H_
#define QVLC_CONVERT_DIALOG_H_ 1

#include <QUrl>

#include "widgets/native/qvlcframe.hpp"

class QLineEdit;
class QCheckBox;
class QRadioButton;
class QPushButton;
class VLCProfileSelector;

class ConvertDialog : public QVLCDialog
{
    Q_OBJECT
public:
    ConvertDialog(QWindow *, qt_intf_t *, const QStringList& );
    virtual ~ConvertDialog(){}

    QStringList getMrls() {return mrls;}

private:
    QLineEdit *fileLine;

    QCheckBox *displayBox, *deinterBox, *appendBox;
    QRadioButton *dumpRadio;
    QPushButton *okButton;
    VLCProfileSelector *profile;

    const QStringList *incomingMRLs;
    const bool singleFileSelected;
    QUrl outgoingMRL;
    QStringList mrls;

private slots:
    void close() override;
    void cancel() override;
    void fileBrowse();
    void setDestinationFileExtension();
    void validate();
};

#endif
