// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * errors.hpp : Errors
 *****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 *
 * Authors: Jean-Baptiste Kempf <jb (at) videolan.org>
 *****************************************************************************/

#ifndef QVLC_ERRORS_DIALOG_H_
#define QVLC_ERRORS_DIALOG_H_ 1

#include "widgets/native/qvlcframe.hpp"
#include "util/singleton.hpp"

class QCheckBox;
class QTextEdit;

class ErrorsDialog : public QVLCDialog, public Singleton<ErrorsDialog>
{
    Q_OBJECT
public:

    void addError( const QString&, const QString& );
    /*void addWarning( QString, QString );*/
private:
    virtual ~ErrorsDialog() {}
    ErrorsDialog( qt_intf_t * );
    void add( bool, const QString&, const QString& );

    QCheckBox *stopShowing;
    QTextEdit *messages;
private slots:
    void close();
    void clear();
    void dontShow();

    friend class    Singleton<ErrorsDialog>;
};

#endif
