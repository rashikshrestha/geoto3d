#include "ncqtscenix.h"

#include "ncqtcamera.h"
#include "nccommon.h"

ncQtScenix::ncQtScenix(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas ) : ncQtDock(title, parent,init_area,allowed_areas) {
	//fprintf(stderr,"ncQtScenix::ncQtScenix\n");

    //setenv("NVSG_DAE_WORKAROUNDS","ONE_MINUS_TRANSPARENCY=TRUE",1);

    instance = this;

     //!GUI
    pointsize   = 1.0;

    //! model control

    QHBoxLayout* hbox = new QHBoxLayout();
    add_models      = new QPushButton(QIcon(":/ncqtmodel/add.png"),"",this);
    remove_models   = new QPushButton(QIcon(":/ncqtmodel/remove.png"),"",this);
    show_models     = new QPushButton(QIcon(":/ncqtmodel/show.png"),"",this);
    hide_models     = new QPushButton(QIcon(":/ncqtmodel/hide.png"),"",this);

    hbox->addWidget(add_models);
    hbox->addWidget(remove_models);
    hbox->addWidget(show_models);
    hbox->addWidget(hide_models);

    grid->addLayout(hbox  ,0,0,1,4);

    models = new QTreeWidget(this);
    models->setSelectionMode(QAbstractItemView::ExtendedSelection);
	models->setSortingEnabled(true);

	models->setMinimumHeight(128);

    models->setColumnCount(2);
    QStringList headerlabels;
    headerlabels << "Visible" << "Filename";
    models->setHeaderLabels(headerlabels);

    grid->addWidget(models ,1,0,1,4);

    camera_visible  = new QPushButton("Reset camera to visible",this);
    camera_selected = new QPushButton("Reset camera to selected",this);

    grid->addWidget(camera_visible   ,2,0,1,2);
    grid->addWidget(camera_selected  ,2,2,1,2);

	slider = new ncQtDoubleSliderCombo("Point Size", 1.0, 16.0, 0.1, this);
    grid->addWidget(slider->label    ,3,0);
    grid->addWidget(slider->slider   ,3,1,1,2);
    grid->addWidget(slider->spinbox  ,3,3);
    slider->setValue(pointsize);



    connect(models          , SIGNAL(itemChanged( QTreeWidgetItem * , int )), this              , SLOT(onItemChanged( QTreeWidgetItem * , int )));
    connect(add_models      , SIGNAL(clicked())                             , this	            , SLOT(addModels())                             );
    connect(remove_models   , SIGNAL(clicked())                             , this	            , SLOT(removeModels())                          );
    connect(show_models     , SIGNAL(clicked())                             , this	            , SLOT(showModels())                            );
    connect(hide_models     , SIGNAL(clicked())                             , this	            , SLOT(hideModels())                            );
    connect(slider          , SIGNAL(valueChanged(double))                  , this	            , SLOT(setPointSize(double))                    );
    connect(camera_visible  , SIGNAL(clicked())                             , this	            , SLOT(resetCameraToVisible())                  );
    connect(camera_selected , SIGNAL(clicked())                             , this	            , SLOT(resetCameraToSelected())                 );
    connect(&values_changed , SIGNAL(output())                              , this              , SLOT(sendValuesChanged())                     );

    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

}

ncQtScenix::~ncQtScenix() {
    //fprintf(stderr,"ncQtScenix::~ncQtScenix\n");
    //nvsgTerminate();
}

void ncQtScenix::initialize() {
    //fprintf(stderr,"+ncQtScenix::initialize()\n");

	Q_INIT_RESOURCE(icons);

    //fprintf(stderr,"-ncQtScenix::initialize()\n");
}

ncQtScenix* ncQtScenix::instance = 0;

void ncQtScenix::draw(ncc::m4x4d KGL, ncc::RTd MV , ncc::v4i VP) {
	//fprintf(stderr,"+ncQtScenix::draw\n");

	glPointSize(pointsize);

	glViewport(VP[0],VP[1],VP[2],VP[3]);

    for (int i=0;i< models->topLevelItemCount();++i)
    {
        ncModelItem* model_item = (ncModelItem*)models->topLevelItem(i);

        if (model_item->checkState(Mvisible))
            model_item->model()->draw(KGL,MV.inverse());
    }


    //ncgl::checkErrors();
	//fprintf(stderr,"-ncQtScenix::draw\n");
}

void ncQtScenix::addModels() {

	//fprintf(stderr,"ncQtScenix::+addModels\n");

    QString mstring = "Models (*.gvm *.mdl *.ply *.dae)";

    //! init file browser
	QList<QString> fileNames = QFileDialog::getOpenFileNames(0,tr("Select Model\n"),browsepath, mstring, 0,QFileDialog::ReadOnly);
	qApp->processEvents();

	bool updateBB = (getNrModels()==0);

	for (int i=0;i<fileNames.count();++i)
	{
        if (!fileNames[i].isEmpty()) {

            QFileInfo fi = QFileInfo(fileNames[i]);
            browsepath = fi.path();

            char model_filename[1024];
            sprintf(model_filename      ,"%s"   ,fileNames[i].toStdString().c_str());

            ncModel*        model = ncModel::loadFile(model_filename);
            ncModelItem*    Mitem = new ncModelItem(model,fileNames[i]);
            models->addTopLevelItem(Mitem);
        }
	}

	if (updateBB)
	{
	    /*
	    nvmath::Sphere3f    sph  = nvsg::getBoundingSphere(nvsg_scene_root);
        nvmath::Vec3f       sphc = sph.getCenter();
        float               sphr = sph.getRadius();
        ncc::v3d center(sphc[0],sphc[1],sphc[2]);
        //fprintf(stderr,"boundingSphereChanged(%f %f %f , %f)\n",sphc[0],sphc[1],sphc[2],sphr);
        boundingSphereChanged(center , sphr);
        */
	}
	else
	    values_changed();

	//fprintf(stderr,"ncQtScenix::-addModels\n");
}

void ncQtScenix::removeModels() {
	//fprintf(stderr,"+ncQtScenix::removeModels\n");

    QList<QTreeWidgetItem *> selection = models->selectedItems();
    if (selection.count()>0)
    {
        values_changed++;
        for (int i = 0;i< selection.count() ; ++i)
			delete models->takeTopLevelItem(models->indexOfTopLevelItem(selection[i]));
		values_changed--;
    }

	//fprintf(stderr,"-ncQtScenix::removeModels\n");

}

void ncQtScenix::showModels() {
	//fprintf(stderr,"ncQtScenix::showModels\n");

    QList<QTreeWidgetItem *> selection = models->selectedItems();
    if (selection.count()>0)
    {
        values_changed++;
        for (int i = 0;i< selection.count() ; ++i)
            selection[i]->setCheckState(Mvisible,Qt::Checked);
        values_changed--;
    }
}

void ncQtScenix::hideModels() {
	//fprintf(stderr,"ncQtScenix::hideModels\n");

    QList<QTreeWidgetItem *> selection = models->selectedItems();
    if (selection.count()>0)
    {
        values_changed++;
        for (int i = 0;i< selection.count() ; ++i)
            selection[i]->setCheckState(Mvisible,Qt::Unchecked);
        values_changed--;
    }
}

void ncQtScenix::setPointSize(double ps) {
	//fprintf(stderr,"ncQtScenix::setPointSize\n");
    values_changed++;
    pointsize = ps;
    values_changed--;
}

void ncQtScenix::onItemChanged( QTreeWidgetItem * item, int column ) {
	//fprintf(stderr,"ncQtScenix::onItemChanged\n");
    values_changed++;
    /*
    nvsg::SwitchHandle* tmp = VPtr<nvsg::SwitchHandle>::asPtr(item->data(0,Mmodel));
    item->checkState(Mvisible) ? SwitchWriteLock(tmp)->setActive(0) : SwitchWriteLock(tmp)->setInactive(0);
    */
    values_changed--;
}

int  ncQtScenix::getNrModels() {
	//fprintf(stderr,"ncQtScenix::getNrModels\n");
    return models->topLevelItemCount();
}

void ncQtScenix::sendValuesChanged() {
    //fprintf(stderr,"ncQtScenix::sendValuesChanged\n");
    valuesChanged();
}

void ncQtScenix::resetCameraToVisible() {
#if 0
    int mp = 0;
    while ((mp<models->topLevelItemCount()) && !(models->topLevelItem(mp)->checkState(Mvisible)))
        mp++;
    if (mp<models->topLevelItemCount())
    {
        nvsg::GroupSharedPtr group = nvsg::Group::create();

        while (mp<models->topLevelItemCount())
        {
            if (models->topLevelItem(mp)->checkState(Mvisible))
                 GroupWriteLock(group)->addChild(VPtr<nvsg::SwitchHandle>::asPtr( models->topLevelItem(mp)->data(0,Mmodel)));
            mp++;
        }

        nvmath::Sphere3f    sph  = nvsg::getBoundingSphere(group);
        nvmath::Vec3f       sphc = sph.getCenter();
        float               sphr = sph.getRadius();
        ncc::v3d center(sphc[0],sphc[1],sphc[2]);
        boundingSphereChanged(center , sphr);
    }

#endif
}

void ncQtScenix::resetCameraToSelected() {

#if 0
    QList<QTreeWidgetItem *> selection = models->selectedItems();

    if (selection.count()>0)
    {
        nvsg::GroupSharedPtr group = nvsg::Group::create();

        for (int i = 0;i< selection.count() ; ++i)
            GroupWriteLock(group)->addChild(VPtr<nvsg::SwitchHandle>::asPtr( selection[i]->data(0,Mmodel)));

        nvmath::Sphere3f    sph  = nvsg::getBoundingSphere(group);
        nvmath::Vec3f       sphc = sph.getCenter();
        float               sphr = sph.getRadius();
        ncc::v3d center(sphc[0],sphc[1],sphc[2]);
        boundingSphereChanged(center , sphr);
    }
#endif
}

