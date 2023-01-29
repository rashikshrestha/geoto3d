#include "ncqtconfig3D.h"

ncQtConfig3D::ncQtConfig3D(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas) : ncQtConfig(title,parent,init_area,allowed_areas)
{
    //fprintf(stderr,"+ncQtConfig3D\n");

	//! flow

    flow_grid    = new QGridLayout();
	flow_group   = new QGroupBox("Flow",this);

    imrange_label   = new QLabel("Range",this);
    imrange_spinbox = new QSpinBox(this);
    imrange_spinbox->setAlignment(Qt::AlignRight);
    imrange_spinbox->setRange(0,16);
    flow_grid->addWidget(imrange_label	    ,0,0,1,1);
    flow_grid->addWidget(imrange_spinbox	,0,1,1,1);

    winlod_label   = new QLabel("winLod",this);
    winlod_spinbox = new QSpinBox(this);
    winlod_spinbox->setAlignment(Qt::AlignRight);
    winlod_spinbox->setRange(0,16);
    flow_grid->addWidget(winlod_label	    ,0,2,1,1);
    flow_grid->addWidget(winlod_spinbox	    ,0,3,1,1);

    imlod_label   = new QLabel("imLod",this);
    imlod_spinbox = new QSpinBox(this);
    imlod_spinbox->setAlignment(Qt::AlignRight);
    imlod_spinbox->setRange(0,16);
    flow_grid->addWidget(imlod_label	    ,0,4,1,1);
    flow_grid->addWidget(imlod_spinbox	    ,0,5,1,1);

    wind_label   = new QLabel("winD",this);
    wind_spinbox = new QSpinBox(this);
    wind_spinbox->setAlignment(Qt::AlignRight);
    wind_spinbox->setRange(1,4096);
    flow_grid->addWidget(wind_label	        ,1,0,1,1);
    flow_grid->addWidget(wind_spinbox	    ,1,1,1,1);

    linp_label     = new QLabel("linP",this);
	linp_spinbox   = new QDoubleSpinBox(this);
	linp_spinbox->setAlignment(Qt::AlignRight);
	linp_spinbox->setRange(0.0,1.0);
	linp_spinbox->setSingleStep(0.01);
	linp_spinbox->setDecimals(2);
	linp_spinbox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    flow_grid->addWidget(linp_label	        ,1,2,1,1);
    flow_grid->addWidget(linp_spinbox       ,1,3,1,1);

    gauss_label     = new QLabel("gauss",this);
	gauss_spinbox   = new QDoubleSpinBox(this);
	gauss_spinbox->setAlignment(Qt::AlignRight);
	gauss_spinbox->setRange(0.0,100.0);
	gauss_spinbox->setSingleStep(0.01);
	gauss_spinbox->setDecimals(2);
	gauss_spinbox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    flow_grid->addWidget(gauss_label	    ,1,4,1,1);
    flow_grid->addWidget(gauss_spinbox	    ,1,5,1,1);

    flow_button     = new QPushButton("Flow",this);
    flow_grid->addWidget(flow_button     ,2,0,1,6);

    //!
	flow_group->setLayout(flow_grid);
	grid->addWidget(flow_group	    ,1,0,1,1);



	//! progress
    progress_grid    = new QGridLayout();

    std::vector<barSpec> progress_cam_bspecs;
    progress_cam_bspecs.push_back(barSpec("Camera %1",0.0,QColor(128,128,255,255)));
    progress_cam = new ncQtMultiBar(progress_cam_bspecs, 0, MAX_CAM, true , this);
	progress_grid->addWidget(progress_cam	    ,0,0,1,3);

    std::vector<barSpec> progress_pose_bspecs;
    progress_pose_bspecs.push_back(barSpec("Pose %1",0.0,QColor(128,255,128,255) ));
    progress_pose = new ncQtMultiBar(progress_pose_bspecs, 0, 0, true , this);
	progress_grid->addWidget(progress_pose	    ,0,3,1,3);

    //!
	grid->addLayout(progress_grid	    ,2,0,1,1);

	//! vox
	vox_grid    = new QGridLayout();
	vox_group   = new QGroupBox("Vox",this);

	max_carve_label   = new QLabel("Threshold",this);
    max_carve_spinbox = new QSpinBox(this);
    max_carve_spinbox->setAlignment(Qt::AlignRight);
    max_carve_spinbox->setRange(0,1024);
    vox_grid->addWidget(max_carve_label	    ,0,0,1,1);
    vox_grid->addWidget(max_carve_spinbox	,0,1,1,1);

	min_level_label   = new QLabel("Level",this);
    min_level_spinbox = new QSpinBox(this);
    min_level_spinbox->setAlignment(Qt::AlignRight);
    min_level_spinbox->setRange(0,16);
    vox_grid->addWidget(min_level_label	    ,1,0,1,1);
    vox_grid->addWidget(min_level_spinbox	,1,1,1,1);

	upscale_label   = new QLabel("Upscale",this);
    upscale_spinbox = new QSpinBox(this);
    upscale_spinbox->setAlignment(Qt::AlignRight);
    upscale_spinbox->setRange(0,4);
    vox_grid->addWidget(upscale_label	    ,1,2,1,1);
    vox_grid->addWidget(upscale_spinbox	    ,1,3,1,1);

    bbox_label   = new QLabel("BBox",this);
    bbox_combobox = new QComboBox(this);
    bbox_combobox->insertItem(NC_BBOX_AUTO  ,"Auto");
    bbox_combobox->insertItem(NC_BBOX_MANUAL,"Manual");
    vox_grid->addWidget(bbox_label	    ,0,2,1,1);
    vox_grid->addWidget(bbox_combobox	,0,3,1,1);

    std::vector<barSpec> vox_bspecs;
    vox_bspecs.push_back(barSpec("Used: %1 MB",0.0,Qt::red  ));
    vox_bspecs.push_back(barSpec("Pool: %1 MB",0.0,Qt::green));
    vox_bspecs.push_back(barSpec("Free: %1 MB",1.0 ,Qt::gray ));
	vox_mem = new ncQtMultiBar(vox_bspecs, 0, 1<<12, false , this);
	vox_grid->addWidget(vox_mem	    ,2,0,1,4);


    carve_button    = new QPushButton("Carve",this);
    recarve_button  = new QPushButton("ReCarve",this);
    remesh_button   = new QPushButton("ReMesh",this);
	vox_grid->addWidget(carve_button    ,3,0,1,1);
	vox_grid->addWidget(recarve_button  ,3,1,1,1);
	vox_grid->addWidget(remesh_button   ,3,2,1,1);


    vox_group->setLayout(vox_grid);
	grid->addWidget(vox_group	    ,3,0,1,1);



	connect(imrange_spinbox     ,SIGNAL(valueChanged(int))      ,this,SLOT(setImRange(int))     );
	connect(winlod_spinbox      ,SIGNAL(valueChanged(int))      ,this,SLOT(setWinLod(int))      );
	connect(imlod_spinbox       ,SIGNAL(valueChanged(int))      ,this,SLOT(setImLod(int))       );
	connect(wind_spinbox        ,SIGNAL(valueChanged(int))      ,this,SLOT(setWinD(int))        );
	connect(linp_spinbox        ,SIGNAL(valueChanged(double))   ,this,SLOT(setLinP(double))     );
	connect(gauss_spinbox       ,SIGNAL(valueChanged(double))   ,this,SLOT(setGauss(double))    );
	connect(max_carve_spinbox   ,SIGNAL(valueChanged(int))      ,this,SLOT(setMaxCarve(int))    );
	connect(min_level_spinbox   ,SIGNAL(valueChanged(int))      ,this,SLOT(setMinLevel(int))    );
	connect(upscale_spinbox     ,SIGNAL(valueChanged(int))      ,this,SLOT(setUpscale(int))     );

    //fprintf(stderr,"-ncQtConfig3D\n");
}

ncQtConfig3D::~ncQtConfig3D()
{
    //dtor
}

bool ncQtConfig3D::load(std::string fname)
{
    this->blockSignals(true);

    if (!ncQtConfig::load(fname))
    {
	    this->blockSignals(false);
    	return false;
	}

    //!flow
    imrange_spinbox->blockSignals(true);
    imrange_spinbox->setValue(flow.imrange);
    imrange_spinbox->blockSignals(false);
    winlod_spinbox->blockSignals(true);
    winlod_spinbox->setValue(flow.winlod);
    winlod_spinbox->blockSignals(false);
    imlod_spinbox->blockSignals(true);
    imlod_spinbox->setValue(flow.imlod);
    imlod_spinbox->blockSignals(false);
    wind_spinbox->blockSignals(true);
    wind_spinbox->setValue(flow.winD);
    wind_spinbox->blockSignals(false);
    linp_spinbox->blockSignals(true);
    linp_spinbox->setValue(flow.linP);
    linp_spinbox->blockSignals(false);
    gauss_spinbox->blockSignals(true);
    gauss_spinbox->setValue(flow.gauss);
    gauss_spinbox->blockSignals(false);

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

    //!vox
    max_carve_spinbox->blockSignals(true);
    max_carve_spinbox->setValue(vox.max_carve);
    max_carve_spinbox->blockSignals(false);
    min_level_spinbox->blockSignals(true);
    min_level_spinbox->setValue(vox.min_level);
    min_level_spinbox->blockSignals(false);
    upscale_spinbox->blockSignals(true);
    upscale_spinbox->setValue(vox.upscale);
    upscale_spinbox->blockSignals(false);

    bbox_combobox->setCurrentIndex(vox.bb_mode);

    this->blockSignals(false);
    valuesChanged();
    return true;
}

void ncQtConfig3D::initProgress()
{
    progress_cam->setRange(0,MAX_CAM);
    progress_cam->setValue(0,0);
    progress_cam->update();
    progress_pose->setRange(project.poses.start_pose, project.poses.end_pose);
    progress_pose->setValue(0,0);
    progress_pose->update();
}

void ncQtConfig3D::setProgress(int cid, int vid)
{
    progress_cam->setValue(0,cid/double(MAX_CAM));
    progress_cam->update();
    progress_pose->setValue(0,double(vid-project.poses.start_pose)/double(project.poses.end_pose-project.poses.start_pose));
    progress_pose->update();
}

void ncQtConfig3D::exitProgress()
{
    progress_cam->setValue(0,1);
    progress_cam->update();
    progress_pose->setValue(0,1);
    progress_pose->update();
}

void ncQtConfig3D::setImRange(int imrange)
{
    flow.imrange = imrange;
    valuesChanged();
}

void ncQtConfig3D::setWinLod(int winlod)
{
    flow.winlod = winlod;
    valuesChanged();
}

void ncQtConfig3D::setImLod(int imlod)
{
    flow.imlod = imlod;
    valuesChanged();
}

void ncQtConfig3D::setWinD(int wind)
{
    flow.winD = wind;
    valuesChanged();
}

void ncQtConfig3D::setLinP(double linp)
{
    flow.linP = linp;
    valuesChanged();
}

void ncQtConfig3D::setGauss(double gauss)
{
    flow.gauss = gauss;
    valuesChanged();
}

void ncQtConfig3D::setMaxCarve(int max_carve)
{
    vox.max_carve = max_carve;
    valuesChanged();
}

void ncQtConfig3D::setMinLevel(int min_level)
{
    vox.min_level = min_level;
    valuesChanged();
}

void ncQtConfig3D::setUpscale(int upscale)
{
    vox.upscale = upscale;
    valuesChanged();
}
