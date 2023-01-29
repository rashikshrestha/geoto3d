#include "ncqtdock.h"


ncQtDock::ncQtDock(const QString & title, ncQtMainWindow* mainw , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas) : QDockWidget(title, mainw) {

	grid        = new QGridLayout();
	grid->setContentsMargins( 0, 0, 0, 0 );

	groupwidget = new QGroupBox(this);
	groupwidget->setContentsMargins( 0, 0, 0, 0 );
	groupwidget->setLayout(grid);

    dock_grid	= new QGridLayout();
	dock_grid->setContentsMargins( 0, 0, 0, 0 );
	dock_grid->addWidget(groupwidget,0,0);
	dockwidget  = new QWidget(this);
	dockwidget->setContentsMargins( 0, 0, 0, 0 );
	dockwidget->setLayout(dock_grid);

	setWidget(dockwidget);

	setVisible(true);
	setContextMenuPolicy(Qt::NoContextMenu);
	setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

	setAllowedAreas(allowed_areas);
	mainw->addDockWidget(init_area ,this);
	mainw->windows_menu->addAction(toggleViewAction ());

}

ncQtDock::~ncQtDock() {

}

