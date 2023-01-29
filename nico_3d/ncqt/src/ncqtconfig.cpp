#include "ncqtconfig.h"

cam_cfg::cam_cfg(int cam_index_ , QWidget* parent) : QRadioButton(QString::number(cam_index_).rightJustified(2,'0'),parent)
{
    //fprintf(stderr,"+cam_cfg\n");

    cam_index = cam_index_;

    setAutoExclusive(false);
    //setToolTip(cam_index);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this , SIGNAL(customContextMenuRequested(const QPoint & )	) , this , SLOT(edit_slot()			));
    connect(this , SIGNAL(toggled(bool)									) , this , SLOT(toggle_slot(bool)	));

    //fprintf(stderr,"-cam_cfg\n");
}

void cam_cfg::edit_slot()
{
    edit_signal(cam_index);
}

void cam_cfg::toggle_slot(bool checked)
{
    toggle_signal(cam_index,checked);
}

ncQtConfig::ncQtConfig(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas) : ncQtDock(title,parent,init_area,allowed_areas)
{
    //fprintf(stderr,"+ncQtConfig\n");

    action_new         = new QAction ("New...", this);
    action_new->setIconVisibleInMenu(true);
    connect(action_new   , SIGNAL(triggered()) , this , SLOT(newConfig()));

    action_load        = new QAction ("Open...", this);
    action_load->setIconVisibleInMenu(true);
    connect(action_load   , SIGNAL(triggered())    , this , SLOT(loadConfig()));

    //!project

    project_grid    = new QGridLayout();
	project_group   = new QGroupBox("Project",this);

    //! cameras
    cam_browsepath  = QDir::homePath();
	cam_label       = new QLabel("Camera file:",this);
	cam_file        = new QToolButton(this);
	cam_file->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

	project_grid->addWidget(cam_label	    ,0,0,1,3);
	project_grid->addWidget(cam_file	    ,0,3,1,3);

	 //! poses
  	pose_browsepath = QDir::homePath();
   	pose_label      = new QLabel("Poses file:",this);
	pose_file       = new QToolButton(this);
	pose_file->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

	project_grid->addWidget(pose_label	    ,1,0,1,3);
	project_grid->addWidget(pose_file	    ,1,3,1,3);

	start_pose_label   = new QLabel("First",this);
    start_pose_spinbox = new QSpinBox(this);
    start_pose_spinbox->setAlignment(Qt::AlignRight);
    start_pose_spinbox->setRange(0,0);
    project_grid->addWidget(start_pose_label	,3,0,1,1);
    project_grid->addWidget(start_pose_spinbox	,3,1,1,1);

    end_pose_label   = new QLabel("Last",this);
    end_pose_spinbox = new QSpinBox(this);
    end_pose_spinbox->setAlignment(Qt::AlignRight);
    end_pose_spinbox->setRange(0,0);
    project_grid->addWidget(end_pose_label	    ,3,2,1,1);
    project_grid->addWidget(end_pose_spinbox	,3,3,1,1);

    step_pose_label   = new QLabel("Step",this);
    step_pose_spinbox = new QDoubleSpinBox(this);
    step_pose_spinbox->setAlignment(Qt::AlignRight);
    step_pose_spinbox->setRange(0.0,1000.0);
    project_grid->addWidget(step_pose_label	    ,3,4,1,1);
    project_grid->addWidget(step_pose_spinbox	,3,5,1,1);

    id_label     = new QLabel("ID",this);
	id_spinbox   = new QSpinBox(this);
	id_spinbox->setAlignment(Qt::AlignRight);
	id_spinbox->setRange(0,MAX_CAM-1);
	znear_label     = new QLabel("Zn",this);
	znear_spinbox   = new QDoubleSpinBox(this);
	znear_spinbox->setAlignment(Qt::AlignRight);
	znear_spinbox->setRange(0.0,1000.0);
	znear_spinbox->setSingleStep(0.1);
	znear_spinbox->setDecimals(1);
	znear_spinbox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	zfar_label     = new QLabel("Zf",this);
	zfar_spinbox   = new QDoubleSpinBox(this);
	zfar_spinbox->setAlignment(Qt::AlignRight);
	zfar_spinbox->setRange(0.0,1000.0);
	zfar_spinbox->setSingleStep(0.1);
	zfar_spinbox->setDecimals(1);
	zfar_spinbox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	vignet_label     = new QLabel("Vignet",this);
	vignet_spinbox   = new QDoubleSpinBox(this);
	vignet_spinbox->setAlignment(Qt::AlignRight);
	vignet_spinbox->setRange(0.0,100.0);
	vignet_spinbox->setSingleStep(0.1);
	vignet_spinbox->setDecimals(1);
	vignet_spinbox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	gamma_label     = new QLabel("Gamma",this);
	gamma_spinbox   = new QDoubleSpinBox(this);
	gamma_spinbox->setAlignment(Qt::AlignRight);
	gamma_spinbox->setRange(0.0,10.0);
	gamma_spinbox->setSingleStep(0.1);
	gamma_spinbox->setDecimals(1);
	gamma_spinbox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	scale_label     = new QLabel("Scale",this);
	scale_spinbox   = new QDoubleSpinBox(this);
	scale_spinbox->setAlignment(Qt::AlignRight);
	scale_spinbox->setRange(0.0,100.0);
	scale_spinbox->setSingleStep(0.1);
	scale_spinbox->setDecimals(1);
	scale_spinbox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	wbr_label     	= new QLabel("Red",this);
	wbr_spinbox   	= new QDoubleSpinBox(this);
	wbr_spinbox->setAlignment(Qt::AlignRight);
	wbr_spinbox->setRange(0.0,2.0);
	wbr_spinbox->setSingleStep(0.1);
	wbr_spinbox->setDecimals(1);
	wbr_spinbox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	wbg_label     = new QLabel("Green",this);
	wbg_spinbox   = new QDoubleSpinBox(this);
	wbg_spinbox->setAlignment(Qt::AlignRight);
	wbg_spinbox->setRange(0.0,2.0);
	wbg_spinbox->setSingleStep(0.1);
	wbg_spinbox->setDecimals(1);
	wbg_spinbox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	wbb_label     = new QLabel("Blue",this);
	wbb_spinbox   = new QDoubleSpinBox(this);
	wbb_spinbox->setAlignment(Qt::AlignRight);
	wbb_spinbox->setRange(0.0,2.0);
	wbb_spinbox->setSingleStep(0.1);
	wbb_spinbox->setDecimals(1);
	wbb_spinbox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	project_grid->addWidget(id_label     	,4,0,1,1,Qt::AlignHCenter);
	project_grid->addWidget(id_spinbox   	,4,1,1,1,Qt::AlignHCenter);
	project_grid->addWidget(znear_label  	,4,2,1,1,Qt::AlignHCenter);
	project_grid->addWidget(znear_spinbox	,4,3,1,1,Qt::AlignHCenter);
	project_grid->addWidget(zfar_label   	,4,4,1,1,Qt::AlignHCenter);
	project_grid->addWidget(zfar_spinbox 	,4,5,1,1,Qt::AlignHCenter);
	project_grid->addWidget(vignet_label	,5,0,1,1,Qt::AlignHCenter);
	project_grid->addWidget(vignet_spinbox	,5,1,1,1,Qt::AlignHCenter);
	project_grid->addWidget(gamma_label   	,5,2,1,1,Qt::AlignHCenter);
	project_grid->addWidget(gamma_spinbox 	,5,3,1,1,Qt::AlignHCenter);
	project_grid->addWidget(scale_label   	,5,4,1,1,Qt::AlignHCenter);
	project_grid->addWidget(scale_spinbox 	,5,5,1,1,Qt::AlignHCenter);
	project_grid->addWidget(wbr_label   	,6,0,1,1,Qt::AlignHCenter);
	project_grid->addWidget(wbr_spinbox 	,6,1,1,1,Qt::AlignHCenter);
	project_grid->addWidget(wbg_label   	,6,2,1,1,Qt::AlignHCenter);
	project_grid->addWidget(wbg_spinbox 	,6,3,1,1,Qt::AlignHCenter);
	project_grid->addWidget(wbb_label   	,6,4,1,1,Qt::AlignHCenter);
	project_grid->addWidget(wbb_spinbox 	,6,5,1,1,Qt::AlignHCenter);
    cam_grid = new QGridLayout();
    cam_grid->setSpacing(0);
    cam_grid->setContentsMargins(0,0,0,0);
    for (unsigned int i=0;i<MAX_CAM;++i)
    {
        cameras[i] = new cam_cfg(i,this);
        cameras[i]->setContentsMargins(0,0,0,0);
        cam_grid->addWidget(cameras[i]  ,i%5,i/5,1,1,Qt::AlignHCenter);
        connect(cameras[i],SIGNAL(edit_signal(int)			), this, SLOT(editCamera(int)));
        connect(cameras[i],SIGNAL(toggle_signal(int,bool)	), this, SLOT(toggleCamera(int,bool)));
    }
    project_grid->addLayout(cam_grid , 7, 0, 1, 6);

    clobber_button  = new QPushButton("Clobber",this);
	copy_button     = new QPushButton("Copy Config",this);
	project_grid->addWidget(clobber_button  ,8,0,1,3);
	project_grid->addWidget(copy_button	    ,8,3,1,3);

    //!
	project_group->setLayout(project_grid);
	grid->addWidget(project_group	    ,0,0,1,1);


	//! //!
	grid->setRowStretch(100,1);

	connect(id_spinbox      ,SIGNAL(valueChanged(int))          ,this,SLOT(editCamera(int))     );
	connect(znear_spinbox   ,SIGNAL(valueChanged(double))       ,this,SLOT(setZnear(double))    );
	connect(zfar_spinbox    ,SIGNAL(valueChanged(double))       ,this,SLOT(setZfar(double))     );
	connect(vignet_spinbox  ,SIGNAL(valueChanged(double))       ,this,SLOT(setVignet(double))   );
	connect(gamma_spinbox   ,SIGNAL(valueChanged(double))       ,this,SLOT(setGamma(double))    );
	connect(scale_spinbox   ,SIGNAL(valueChanged(double))       ,this,SLOT(setScale(double))    );
	connect(wbr_spinbox   	,SIGNAL(valueChanged(double))       ,this,SLOT(setWB())    			);
	connect(wbg_spinbox   	,SIGNAL(valueChanged(double))       ,this,SLOT(setWB())    			);
	connect(wbb_spinbox   	,SIGNAL(valueChanged(double))       ,this,SLOT(setWB())    			);

	connect(start_pose_spinbox  ,SIGNAL(valueChanged(int))      ,this,SLOT(setStartPose(int))   );
	connect(end_pose_spinbox    ,SIGNAL(valueChanged(int))      ,this,SLOT(setEndPose(int))     );
	connect(step_pose_spinbox   ,SIGNAL(valueChanged(double))   ,this,SLOT(setStepPose(double)) );
	connect(copy_button         ,SIGNAL(clicked())              ,this,SLOT(copyConfigFile())    );

	for (unsigned int i=0;i<MAX_CAM; ++i )
        connect(cameras[i]    ,SIGNAL(toggled(bool))   ,this,SLOT(updateActiveCamIDs()) );

    //fprintf(stderr,"-ncQtConfig\n");

}

ncQtConfig::~ncQtConfig()
{
    //dtor
}

void ncQtConfig::newConfig()
{

	QString fileName = QFileDialog::getSaveFileName(this, "Save File", QDir::homePath(), tr("Config Files (*.xml)"));
	if (!fileName.isEmpty())
	{
	    if (QFileInfo(fileName).suffix() != "xml")
            fileName.append(".xml");

        QString im_dir      = QFileDialog::getExistingDirectory (this, tr("Select Image Directory"  ), QFileInfo(fileName ).path(), QFileDialog::ShowDirsOnly         );
        QString cam_file    = QFileDialog::getOpenFileName      (this, tr("Select Camera File"      ), QFileInfo(im_dir   ).path(), tr("Camera Set (camera_set.txt)") );
        QString pose_file   = QFileDialog::getOpenFileName      (this, tr("Select Pose File"        ), QFileInfo(cam_file ).path(), tr("Pose File (*poses)")          );
        QString track_dir   = QFileDialog::getExistingDirectory (this, tr("Select Track Directory"  ), QFileInfo(pose_file).path(), QFileDialog::ShowDirsOnly         );
        {
            ncConfig tmpcfg;
            if ((!cam_file.isEmpty()) && (!pose_file.isEmpty()) && (!im_dir.isEmpty()))
            {
				tmpcfg.project.cameras.directory = ncc::getDirName(cam_file.toStdString());
                tmpcfg.project.cameras.filename  = ncc::getFileName(cam_file.toStdString());

                tmpcfg.project.poses.directory = ncc::getDirName(pose_file.toStdString());
                tmpcfg.project.poses.filename  = ncc::getFileName(pose_file.toStdString());

                ncPoseList* plist = new ncPoseList(tmpcfg.project.poses.getPath());
                tmpcfg.project.poses.start_pose = plist->getMinVid(true);
                tmpcfg.project.poses.end_pose = plist->getMaxVid(true);

                ncCalibrationSet* cset = new ncCalibrationSet(tmpcfg.project.cameras.getPath());
                tmpcfg.project.images.im_info.setup(im_dir.toStdString(),*cset,*plist);

    	        tmpcfg.project.tracks.fromDirName(track_dir.toStdString());

                tmpcfg.project.cameras.calset = cset;
	            tmpcfg.save(fileName.toStdString().c_str());
                tmpcfg.project.cameras.calset = 0;

                delete cset;
                delete plist;

		        load(fileName.toStdString().c_str());
            }
        }
    }
}

void ncQtConfig::loadConfig()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath (), tr("Config Files (*.xml)"));
	if (!fileName.isEmpty())
        load(fileName.toStdString().c_str());
}

void ncQtConfig::editCamera(int cid)
{
    //fprintf(stderr,"editing camera %d\n",cid);

    id_spinbox->blockSignals(true);
    id_spinbox->setValue(cid);
    id_spinbox->blockSignals(false);

    znear_spinbox->blockSignals(true);
    znear_spinbox->setValue(project.cameras.znear[cid]);
    znear_spinbox->blockSignals(false);

    zfar_spinbox->blockSignals(true);
    zfar_spinbox->setValue(project.cameras.zfar[cid]);
    zfar_spinbox->blockSignals(false);

    vignet_spinbox->blockSignals(true);
    vignet_spinbox->setValue((*project.cameras.calset)[cid].getVignet());
    vignet_spinbox->blockSignals(false);

    gamma_spinbox->blockSignals(true);
    gamma_spinbox->setValue((*project.cameras.calset)[cid].getGamma());
    gamma_spinbox->blockSignals(false);

    wbr_spinbox->blockSignals(true);
    wbr_spinbox->setValue((*project.cameras.calset)[cid].getWB()[0]);
    wbr_spinbox->blockSignals(false);

    wbg_spinbox->blockSignals(true);
    wbg_spinbox->setValue((*project.cameras.calset)[cid].getWB()[1]);
    wbg_spinbox->blockSignals(false);

    wbb_spinbox->blockSignals(true);
    wbb_spinbox->setValue((*project.cameras.calset)[cid].getWB()[2]);
    wbb_spinbox->blockSignals(false);

    scale_spinbox->blockSignals(true);
    scale_spinbox->setValue((*project.cameras.calset)[cid].getScale());
    scale_spinbox->blockSignals(false);

    valuesChanged();
}

void ncQtConfig::updateToolTip(int cam_id)
{
	const ncCalibration& cam = (*project.cameras.calset)[cam_id];
    cameras[cam_id]->setToolTip(QString("Zn: %1 Zf: %2 Vignet: %3 Gamma: %4 Scale: %5").arg(project.cameras.znear[cam_id]).arg(project.cameras.zfar[cam_id]).arg(cam.getVignet()).arg(cam.getGamma()).arg(cam.getScale()));
}

void ncQtConfig::setZnear(double zn)
{
    project.cameras.znear[id_spinbox->value()] = zn;
    (*project.cameras.calset)(id_spinbox->value()).setDepthRange(zn);
    updateToolTip(id_spinbox->value());
    valuesChanged();
}

void ncQtConfig::setZfar(double zfar)
{
    project.cameras.zfar[id_spinbox->value()] = zfar;
    updateToolTip(id_spinbox->value());
    valuesChanged();
}

void ncQtConfig::setVignet(double vignet)
{
    (*project.cameras.calset)(id_spinbox->value()).setVignet( vignet );
    updateToolTip(id_spinbox->value());
    valuesChanged();
}

void ncQtConfig::setGamma(double gamma)
{
    (*project.cameras.calset)(id_spinbox->value()).setGamma( gamma );
    updateToolTip(id_spinbox->value());
    valuesChanged();
}

void ncQtConfig::setWB()
{
	float r = wbr_spinbox->value();
	float g = wbg_spinbox->value();
	float b = wbb_spinbox->value();

	fprintf(stderr,"setWB %f %f %f\n",r,g,b);

    (*project.cameras.calset)(id_spinbox->value()).setWB( ncc::v3f(r,g,b) );
    updateToolTip(id_spinbox->value());
    valuesChanged();
}

void ncQtConfig::setScale(double scale)
{
    (*project.cameras.calset)(id_spinbox->value()).setScale( scale );
    updateToolTip(id_spinbox->value());
    valuesChanged();
}

void ncQtConfig::setStartPose(int start_pose)
{
    project.poses.start_pose = start_pose;
    valuesChanged();
}

void ncQtConfig::setEndPose(int end_pose)
{
    project.poses.end_pose = end_pose;
    valuesChanged();
}

void ncQtConfig::setStepPose(double step_pose)
{
    project.poses.step_pose = step_pose;
    valuesChanged();
}

void ncQtConfig::setCameraFile(QString cam_file)
{
    project.cameras.directory = ncc::getDirName(cam_file.toStdString());
    project.cameras.filename  = ncc::getFileName(cam_file.toStdString());
    valuesChanged();
}

void ncQtConfig::setPoseFile(QString pose_file)
{
    project.poses.directory = ncc::getDirName(pose_file.toStdString());
    project.poses.filename  = ncc::getFileName(pose_file.toStdString());
    valuesChanged();
}

void ncQtConfig::updateActiveCamIDs()
{
    for (unsigned int i=0;i<MAX_CAM;++i)
        project.cameras.active[i] = cameras[i]->isChecked();
    valuesChanged();
}

void ncQtConfig::copyConfigFile()
{
	apply();
	QString fileName = QFileDialog::getSaveFileName(this, "Save File", QString(file_name.c_str()), tr("Config Files (*.xml)"));
	if (!fileName.isEmpty())
	{
	    setWindowTitle(QFileInfo(fileName).fileName());
        save(fileName.toStdString().c_str());
		configCopied(fileName);
	}
    valuesChanged();
}

bool ncQtConfig::load(std::string fname)
{
	bool res = vload(fname);
	if (res)
	{
		valuesChanged();
    	configLoaded(fname.c_str());
	}
	return res;
}

bool ncQtConfig::vload(std::string fname)
{
    if (!ncConfig::load(fname))
    	return false;

    //!project

    //!cameras
    unsigned int strsize = 18;
    QString cfile = QString(project.cameras.getPath().c_str());
    if (cfile.length()>strsize)
        cfile = cfile.right(strsize-3).prepend("...");
    cam_file->blockSignals(true);
    cam_file->setText(cfile);
    cam_file->setToolTip(project.cameras.getPath().c_str());
    cam_file->blockSignals(false);

    for (unsigned int i=0;i<project.cameras.calset->getNrCameras(); ++i )
        updateToolTip(i);

    for (unsigned int i=0;i<MAX_CAM; ++i )
    {
        cameras[i]->blockSignals(true);
        cameras[i]->setChecked(false);
    }

    for (unsigned int i=0;i<MAX_CAM; ++i )
        cameras[i]->setChecked(false);

	std::vector<int> acids = getActiveCids();
    for (unsigned int i=0;i<acids.size(); ++i )
        cameras[acids[i]]->setChecked(true);

    for (unsigned int i=0;i<MAX_CAM; ++i )
        cameras[i]->blockSignals(false);

    //!poses
    QString pfile = QString(project.poses.getPath().c_str());
    if (pfile.length()>strsize)
        pfile = pfile.right(strsize-3).prepend("...");
    pose_file->blockSignals(true);
    pose_file->setText(pfile);
    pose_file->setToolTip(project.poses.getPath().c_str());
    pose_file->blockSignals(false);

    start_pose_label->setToolTip(QString("min pose : %1\n").arg(project.poses.poselist->getMinVid(true)));
    start_pose_spinbox->blockSignals(true);
    start_pose_spinbox->setRange(project.poses.poselist->getMinVid(true),project.poses.poselist->getMaxVid(true));
    start_pose_spinbox->setValue(project.poses.start_pose);
    start_pose_spinbox->blockSignals(false);
    end_pose_label->setToolTip(QString("max pose : %1\n").arg(project.poses.poselist->getMaxVid(true)));
    end_pose_spinbox->blockSignals(true);
    end_pose_spinbox->setRange(project.poses.poselist->getMinVid(true),project.poses.poselist->getMaxVid(true));
    end_pose_spinbox->setValue(project.poses.end_pose);
    end_pose_spinbox->blockSignals(false);
    step_pose_spinbox->blockSignals(true);
    step_pose_spinbox->setValue(project.poses.step_pose);
    step_pose_spinbox->blockSignals(false);

    editCamera(0);

    setWindowTitle(QFileInfo(fname.c_str()).fileName());

    return true;
}

void ncQtConfig::save(std::string fname)
{
	ncConfig::save(fname);
}

void ncQtConfig::toggleCamera(int id, bool checked)
{
	if (checked)
		editCamera(id);

}

