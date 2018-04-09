#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QDropEvent>
/**
 * \file
 *
 * \author Valentin Bruder
 *
 * \copyright Copyright (C) 2018 Valentin Bruder
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <QMimeData>
#include <QFutureWatcher>
#include <QProgressBar>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QLabel>
#include <QSettings>
#include <QVector4D>

#include <datagenerator.hpp>
#include <getSetData.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
	/// <summary>
	/// Used for generating random volume data
	/// </summary>
	void randomData();

    void openVolumeFile();

    void addProgress();
    void finishedLoading();

    void loadTff();
    void saveTff();
    void saveRawTff();

    void chooseBackgroundColor();
protected:
    void dragEnterEvent(QDragEnterEvent *ev) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *ev) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    void setVolumeData(const QString &fileName);
    bool readVolumeFile(const QString &fileName);

    void readTff(const QString &fileName);

    void readSettings();
    void writeSettings();

    void setStatusText();

	/// <summary>
	/// Used for reading in configs from .xml oder .json files in MainWindow::openVolumeFile
	/// if a config is read then this method stores it in _cfg
	/// </summary>
	void setConfig();

	/// <summary>
	/// Used for reading in configs from .xml oder .json files in MainWindow::openVolumeFile
	/// if a config is read and stored in _cfg this method updates the values of the UI
	/// </summary>
	void setUI();

    // ----- Members -----
    Ui::MainWindow *ui;

    QSettings *_settings;
    QFutureWatcher<void> *_watcher;
    QProgressBar _progBar;
    QTimer _timer;
    QString _fileName;
    QLabel _statusLabel;

	DataConfig _cfg;
	DataGenerator _dataGenerator;
	GetSetData _getSetData;
};

#endif // MAINWINDOW_H
