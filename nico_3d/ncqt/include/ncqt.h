#ifndef __NCQT_H__
#define __NCQT_H__

#include "ncgl.h"

#include <QAbstractSpinBox>
#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDate>
#include <QDesktopWidget>
#include <QDir>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QEvent>
#include <QFile>
#include <QFileDialog>
#include <QFrame>
#include <QGLWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QProgressDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QSplitter>
#include <QStatusBar>
#include <QStyle>
#include <QTableWidget>
#include <QTcpSocket>
#include <QTextBrowser>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "nccommon.h"

class ncqt {

public:

	static bool	readImage(QString fname, ncImageBuffer<ncc::v3ub> & imbuf);
	static bool	readImage(QString fname, ncImageBuffer<ncc::v4ub> & imbuf);

	static int		frame_style;
	static QString	browsepath;
};

#endif
