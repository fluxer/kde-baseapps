/*
    Copyright 2007-2008 by Robert Knight <robertknight@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

#ifndef COLORSCHEMEEDITOR_H
#define COLORSCHEMEEDITOR_H

// Qt
#include <QWidget>
#include <QTableWidgetItem>

// KDE
#include <KDialog>

// Konsole
#include "Profile.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ColorSchemeEditor; }
QT_END_NAMESPACE

namespace Konsole
{
class ColorScheme;

/**
 * A dialog for editing color schemes.
 *
 * After creation, the dialog can be initialized with the settings
 * of a color scheme using the setup() method.
 *
 * The dialog creates a copy of the supplied color scheme to which
 * any changes made are applied.  The modified color scheme
 * can be retrieved using the colorScheme() method.
 *
 * When changes are made the colorsChanged() signal is emitted.
 */
class KONSOLEPRIVATE_EXPORT ColorSchemeEditor : public KDialog
{
    Q_OBJECT

public:
    /** Constructs a new color scheme editor with the specified parent. */
    explicit ColorSchemeEditor(QWidget* parent = 0);
    virtual ~ColorSchemeEditor();

    /** Initializes the dialog with the properties of the specified color scheme. */
    void setup(const ColorScheme* scheme, bool isNewScheme);
    /** Returns the modified color scheme. */
    ColorScheme& colorScheme() const;
    bool isNewScheme() const;

signals:
    /** Emitted when the colors in the color scheme change. */
    void colorsChanged(ColorScheme* scheme);
    /** Used to send back colorscheme changes into the profile edited */
    void colorSchemeSaveRequested(const ColorScheme& scheme,bool isNewScheme);

public slots:
    /** Sets the text displayed in the description edit field. */
    void setDescription(const QString& description);

private slots:
    void setTransparencyPercentLabel(int percent);
    void setRandomizedBackgroundColor(bool randomized);
    void editColorItem(QTableWidgetItem* item);
    void wallpaperPathChanged(const QString& path);
    void wallpaperTiledChanged(const bool tiled);
    void selectWallpaper();
    /** Triggered by apply/ok buttons */
    void saveColorScheme();

private:
    void setupColorTable(const ColorScheme* table);

    bool _isNewScheme;
    Ui::ColorSchemeEditor* _ui;
    ColorScheme* _colors;
};
}

#endif // COLORSCHEMEEDITOR_H
