/*****************************************************************************
 * preferences_widgets.hpp : Widgets for preferences panels
 ****************************************************************************
 * Copyright (C) 2006-2011 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Antoine Cellerier <dionoea@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef VLC_QT_PREFERENCES_WIDGETS_HPP_
#define VLC_QT_PREFERENCES_WIDGETS_HPP_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qt.hpp"

#include <QLabel>

class QWidget;
class QGroupBox;
class QGridLayout;
class QBoxLayout;
class QDoubleSpinBox;
class QCheckBox;
class QComboBox;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QFontComboBox;
class QSlider;
class QAbstractButton;

/*******************************************************
 * Simple widgets
 *******************************************************/

class InterfacePreviewWidget : public QLabel
{
    Q_OBJECT
public:
    InterfacePreviewWidget( QWidget * );
    enum enum_style {
                 COMPLETE, // aka MPC
                 MINIMAL,  // aka WMP12 minimal
                 SKINS };
public slots:
    void setPreview( enum_style );
    void setNormalPreview( bool b_minimal );
};

/*******************************************************
 * Variable controls
 *******************************************************/

class ConfigControl : public QObject
{
    Q_OBJECT
public:
    int getType() const { return p_item->i_type; }
    const char * getName() const { return  p_item->psz_name; }
    void hide() { changeVisibility( false ); }
    void show() { changeVisibility( true ); }
    /* ConfigControl factory */
    static ConfigControl * createControl( module_config_t*, QWidget* );
    static ConfigControl * createControl( module_config_t*, QWidget*,
                                          QGridLayout *, int line = 0 );
    static ConfigControl * createControl( module_config_t*, QWidget*,
                                          QBoxLayout *, int line = 0 );
    /** Inserts control into an existing grid layout */
    virtual void insertInto( QGridLayout*, int row = 0 ) { Q_UNUSED( row ); }
    /** Inserts control into an existing box layout */
    virtual void insertInto( QBoxLayout*, int index = 0 ) { Q_UNUSED( index ); }
    virtual void doApply() = 0;
    virtual void storeValue() = 0;
protected:
    ConfigControl( module_config_t *_p_conf ) : p_item( _p_conf ) {}
    virtual void changeVisibility( bool ) { }
    module_config_t *p_item;
signals:
    void changed();
};

/*******************************************************
 * Integer-based controls
 *******************************************************/
class VIntConfigControl : public ConfigControl
{
    Q_OBJECT
public:
    virtual int getValue() const = 0;
    virtual void doApply() Q_DECL_OVERRIDE;
    virtual void storeValue() Q_DECL_OVERRIDE;
protected:
    VIntConfigControl( module_config_t *i ) : ConfigControl(i) {}
};

class IntegerConfigControl : public VIntConfigControl
{
    Q_OBJECT
public:
    IntegerConfigControl( module_config_t *, QWidget * );
    IntegerConfigControl( module_config_t *, QLabel*, QSpinBox* );
    void insertInto( QGridLayout*, int row = 0 ) Q_DECL_OVERRIDE;
    void insertInto( QBoxLayout*, int index = 0 ) Q_DECL_OVERRIDE;
    int getValue() const Q_DECL_OVERRIDE;
protected:
    QSpinBox *spin;
    void changeVisibility( bool ) Q_DECL_OVERRIDE;
private:
    QLabel *label;
    void finish();
};

class IntegerRangeConfigControl : public IntegerConfigControl
{
    Q_OBJECT
public:
    IntegerRangeConfigControl( module_config_t *, QWidget * );
    IntegerRangeConfigControl( module_config_t *, QLabel*, QSpinBox* );
    IntegerRangeConfigControl( module_config_t *, QLabel*, QSlider* );
private:
    void finish();
};

class IntegerRangeSliderConfigControl : public VIntConfigControl
{
    Q_OBJECT
public:
    IntegerRangeSliderConfigControl( module_config_t *, QLabel *, QSlider * );
    int getValue() const Q_DECL_OVERRIDE;
protected:
    QSlider *slider;
    void changeVisibility( bool ) Q_DECL_OVERRIDE;
private:
    QLabel *label;
    void finish();
};

class IntegerListConfigControl : public VIntConfigControl
{
Q_OBJECT
public:
    IntegerListConfigControl( module_config_t *, QWidget * );
    IntegerListConfigControl( module_config_t *, QLabel *, QComboBox* );
    void insertInto( QGridLayout*, int row = 0 ) Q_DECL_OVERRIDE;
    void insertInto( QBoxLayout*, int index = 0 ) Q_DECL_OVERRIDE;
    int getValue() const Q_DECL_OVERRIDE;
protected:
    void changeVisibility( bool ) Q_DECL_OVERRIDE;
private:
    void finish(module_config_t * );
    QLabel *label;
    QComboBox *combo;
};

class BoolConfigControl : public VIntConfigControl
{
    Q_OBJECT
public:
    BoolConfigControl( module_config_t *, QWidget * );
    BoolConfigControl( module_config_t *, QLabel *, QAbstractButton* );
    void insertInto( QGridLayout*, int row = 0 ) Q_DECL_OVERRIDE;
    void insertInto( QBoxLayout*, int index = 0 ) Q_DECL_OVERRIDE;
    int getValue() const Q_DECL_OVERRIDE;
protected:
    void changeVisibility( bool ) Q_DECL_OVERRIDE;
private:
    QAbstractButton *checkbox;
    void finish();
};

class ColorConfigControl : public VIntConfigControl
{
    Q_OBJECT
public:
    ColorConfigControl( module_config_t *, QWidget * );
    ColorConfigControl( module_config_t *, QLabel *, QAbstractButton* );
    virtual ~ColorConfigControl() { delete color_px; }
    void insertInto( QGridLayout*, int row = 0 ) Q_DECL_OVERRIDE;
    void insertInto( QBoxLayout*, int index = 0 ) Q_DECL_OVERRIDE;
    int getValue() const Q_DECL_OVERRIDE;
protected:
    void changeVisibility( bool ) Q_DECL_OVERRIDE;
private:
    QLabel *label;
    QAbstractButton *color_but;
    QPixmap *color_px;
    int i_color;
    void finish();
private slots:
    void selectColor();
};

/*******************************************************
 * Float-based controls
 *******************************************************/
class VFloatConfigControl : public ConfigControl
{
    Q_OBJECT
public:
    virtual float getValue() const = 0;
    void doApply() Q_DECL_OVERRIDE;
    void storeValue() Q_DECL_OVERRIDE;
protected:
    VFloatConfigControl( module_config_t *i ) : ConfigControl(i) {}
};

class FloatConfigControl : public VFloatConfigControl
{
    Q_OBJECT
public:
    FloatConfigControl( module_config_t *, QWidget * );
    FloatConfigControl( module_config_t *, QLabel*, QDoubleSpinBox* );
    void insertInto( QGridLayout*, int row = 0 ) Q_DECL_OVERRIDE;
    void insertInto( QBoxLayout*, int index = 0 ) Q_DECL_OVERRIDE;
    float getValue() const Q_DECL_OVERRIDE;

protected:
    void changeVisibility( bool ) Q_DECL_OVERRIDE;
    QDoubleSpinBox *spin;

private:
    QLabel *label;
    void finish();
};

class FloatRangeConfigControl : public FloatConfigControl
{
    Q_OBJECT
public:
    FloatRangeConfigControl( module_config_t *, QWidget * );
    FloatRangeConfigControl( module_config_t *, QLabel*, QDoubleSpinBox* );
private:
    void finish();
};

/*******************************************************
 * String-based controls
 *******************************************************/
class VStringConfigControl : public ConfigControl
{
    Q_OBJECT
public:
    virtual QString getValue() const = 0;
    void doApply() Q_DECL_OVERRIDE;
    void storeValue() Q_DECL_OVERRIDE;
protected:
    VStringConfigControl( module_config_t *i ) : ConfigControl(i) {}
};

class StringConfigControl : public VStringConfigControl
{
    Q_OBJECT
public:
    StringConfigControl( module_config_t *, QWidget * );
    StringConfigControl( module_config_t *, QLabel *, QLineEdit* );
    void insertInto( QGridLayout*, int row = 0 ) Q_DECL_OVERRIDE;
    void insertInto( QBoxLayout*, int index = 0 ) Q_DECL_OVERRIDE;
    QString getValue() const Q_DECL_OVERRIDE;
protected:
    void changeVisibility( bool ) Q_DECL_OVERRIDE;
    QLineEdit *text;
private:
    void finish();
    QLabel *label;
};

class PasswordConfigControl : public StringConfigControl
{
    Q_OBJECT
public:
    PasswordConfigControl( module_config_t *, QWidget * );
    PasswordConfigControl( module_config_t *, QLabel *, QLineEdit* );
private:
    void finish();
};

class FileConfigControl : public VStringConfigControl
{
    Q_OBJECT
public:
    FileConfigControl( module_config_t *, QWidget * );
    FileConfigControl( module_config_t *, QLabel *, QLineEdit *, QPushButton * );
    void insertInto( QGridLayout*, int row = 0 ) Q_DECL_OVERRIDE;
    void insertInto( QBoxLayout*, int index = 0 ) Q_DECL_OVERRIDE;
    QString getValue() const Q_DECL_OVERRIDE;
public slots:
    virtual void updateField();
protected:
    void changeVisibility( bool ) Q_DECL_OVERRIDE;
    void finish();
    QLineEdit *text;
    QLabel *label;
    QPushButton *browse;
};

class DirectoryConfigControl : public FileConfigControl
{
    Q_OBJECT
public:
    DirectoryConfigControl( module_config_t *, QWidget * );
    DirectoryConfigControl( module_config_t *, QLabel *, QLineEdit *, QPushButton * );
public slots:
    void updateField() Q_DECL_OVERRIDE;
};

class FontConfigControl : public VStringConfigControl
{
    Q_OBJECT
public:
    FontConfigControl( module_config_t *, QWidget * );
    FontConfigControl( module_config_t *, QLabel *, QFontComboBox *);
    void insertInto( QGridLayout*, int row = 0 ) Q_DECL_OVERRIDE;
    void insertInto( QBoxLayout*, int index = 0 ) Q_DECL_OVERRIDE;
    QString getValue() const Q_DECL_OVERRIDE;
protected:
    void changeVisibility( bool ) Q_DECL_OVERRIDE;
    QLabel *label;
    QFontComboBox *font;
};

class ModuleConfigControl : public VStringConfigControl
{
    Q_OBJECT
public:
    ModuleConfigControl( module_config_t *, QWidget * );
    ModuleConfigControl( module_config_t *, QLabel *, QComboBox* );
    void insertInto( QGridLayout*, int row = 0 ) Q_DECL_OVERRIDE;
    void insertInto( QBoxLayout*, int index = 0 ) Q_DECL_OVERRIDE;
    QString getValue() const Q_DECL_OVERRIDE;
protected:
    void changeVisibility( bool ) Q_DECL_OVERRIDE;
private:
    void finish( );
    QLabel *label;
    QComboBox *combo;
};

struct checkBoxListItem {
    QCheckBox *checkBox;
    char *psz_module;
};

class ModuleListConfigControl : public VStringConfigControl
{
    Q_OBJECT
    friend class ConfigControl;
public:
    ModuleListConfigControl( module_config_t *, QWidget *, bool );
    virtual ~ModuleListConfigControl();
    void insertInto( QGridLayout*, int row = 0 ) Q_DECL_OVERRIDE;
    void insertInto( QBoxLayout*, int index = 0 ) Q_DECL_OVERRIDE;
    QString getValue() const Q_DECL_OVERRIDE;
public slots:
    void onUpdate();
protected:
    void changeVisibility( bool ) Q_DECL_OVERRIDE;
private:
    void finish( bool );
    void checkbox_lists(module_t*);
    void checkbox_lists( QString, QString, const char* );
    QList<checkBoxListItem*> modules;
    QGroupBox *groupBox;
    QLineEdit *text;
};

class StringListConfigControl : public VStringConfigControl
{
    Q_OBJECT
public:
    StringListConfigControl( module_config_t *, QWidget * );
    StringListConfigControl( module_config_t *, QLabel *, QComboBox* );
    void insertInto( QGridLayout*, int row = 0 ) Q_DECL_OVERRIDE;
    void insertInto( QBoxLayout*, int index = 0 ) Q_DECL_OVERRIDE;
    QString getValue() const Q_DECL_OVERRIDE;
protected:
    void changeVisibility( bool ) Q_DECL_OVERRIDE;
    QComboBox *combo;
private:
    void finish(module_config_t * );
    QLabel *label;
private slots:
    void comboIndexChanged( int );
};

void setfillVLCConfigCombo(const char *configname, QComboBox *combo );
#endif
