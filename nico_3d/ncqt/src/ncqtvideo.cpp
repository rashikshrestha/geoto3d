#include "ncqtvideo.h"

#include <fstream>

const unsigned int ncQtVideo::string_cap_length = 32;

ncQtVideo::ncQtVideo(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas ) : ncQtDock(title, parent,init_area,allowed_areas) {

    poses                   = 0;
	poses_mesh				= 0;
	poses_vert				= 0;

    calset                  = 0;
	calset_mesh				= 0;
	calset_vert				= 0;
	calset_index			= 0;

	pose_browsepath         = QDir::homePath();
    cam_browsepath          = QDir::homePath();

    menu_pose_load = new QAction ("Load &Pose File", this);
    menu_pose_load->setIconVisibleInMenu(true);
    connect(menu_pose_load   , SIGNAL(triggered())           , this , SLOT(loadPoseFile()));

    menu_cam_load = new QAction ("Load &Camera File", this);
    menu_cam_load->setIconVisibleInMenu(true);
    connect(menu_pose_load   , SIGNAL(triggered())           , this , SLOT(loadCamFile()));

    timer_		= new QTimer(this);

	pose_file_label  = new QLabel("Poses file:",this);
	pose_file_name   = new QPushButton("Load Poses File",this);
	pose_label   = new QLabel("PID:",this);
	pose_spinbox = new QSpinBox(this);
    pose_spinbox->setEnabled(false);
    pose_spinbox->blockSignals(true);

    static_poses = new QCheckBox("include static poses",this);
    static_poses->setEnabled(false);
    static_poses->blockSignals(true);

	cam_file_label  = new QLabel("Camera file:",this);
	cam_file_name   = new QPushButton("Load Camera File",this);
	cam_file_name->setEnabled(false);
	cam_label  = new QLabel("CID:",this);
	cam_spinbox= new QSpinBox(this);
    cam_spinbox->setEnabled(false);
    cam_spinbox->blockSignals(true);

    begin_slider    = new QSlider(this);
	begin_slider->setOrientation(Qt::Horizontal);
	begin_slider->setEnabled(false);
    end_slider	    = new QSlider(this);
	end_slider->setOrientation(Qt::Horizontal);
	end_slider->setEnabled(false);
	slider_	    = new QSlider(this);
	slider_->setOrientation(Qt::Horizontal);
	slider_->setEnabled(false);
    slider_->blockSignals(true);

	Q_INIT_RESOURCE(icons);

	grid->addWidget(cam_file_label	,0,0);
	grid->addWidget(cam_file_name   ,0,1);
	grid->addWidget(cam_label       ,0,2);
	grid->addWidget(cam_spinbox     ,0,3);

	grid->addWidget(pose_file_label	,1,0);
	grid->addWidget(pose_file_name  ,1,1);
	grid->addWidget(pose_label      ,1,2);
	grid->addWidget(pose_spinbox    ,1,3);

	grid->setColumnStretch(1,1.0);


    grid->addWidget(static_poses    ,2,0,1,4);

	grid->addWidget(begin_slider	,3,0,1,4);
	grid->addWidget(end_slider      ,3,0,1,4);
	grid->addWidget(slider_	        ,3,0,1,4);

	toolbar = new QToolBar(this);
    toolbar->setIconSize(QSize(48,32));
	toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	toolbar->setEnabled(false);
	play_   = toolbar->addAction(QIcon(":ncqtvideo/play.png"),"Play");
	play_->setToolTip("");
	reset_  = toolbar->addAction(QIcon(":ncqtvideo/reset.png"),"Reset");
	reset_->setToolTip("");
	toolbar->addSeparator();
	begin_  = toolbar->addAction(QIcon(":ncqtvideo/start.png"),"");
	begin_->setToolTip("Sets start position of loop sequence to current value");
	end_    = toolbar->addAction(QIcon(":ncqtvideo/end.png"),"");
	end_->setToolTip("Sets stop position of loop sequence to current value");
	toolbar->addSeparator();
	record_ = toolbar->addAction(QIcon(":ncqtvideo/record.png"),"Record");
	record_->setToolTip("");

	grid->addWidget(toolbar,5,0,1,4);



    connect(pose_file_name   , SIGNAL(clicked())             , this          , SLOT(loadPoseFile())  );
    connect(cam_file_name    , SIGNAL(clicked())             , this          , SLOT(loadCamFile())   );

    connect(play_       , SIGNAL(triggered())			, this			, SLOT(togglePlay())    );
    connect(reset_      , SIGNAL(triggered())			, this			, SLOT(reset())		    );
    connect(begin_      , SIGNAL(triggered())			, this			, SLOT(start())		    );
    connect(end_        , SIGNAL(triggered())			, this			, SLOT(end())		    );
    connect(record_     , SIGNAL(triggered())			, this			, SLOT(record())		);

    connect(slider_     , SIGNAL(valueChanged(int))     , this          , SLOT(changePose(int)) );
    connect(pose_spinbox, SIGNAL(valueChanged(int))     , this          , SLOT(changePose(int)) );
    connect(cam_spinbox , SIGNAL(valueChanged(int))     , this          , SLOT(changeCamera(int)));

    connect(timer_      , SIGNAL(timeout())             , this          , SLOT(next())          );

    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
}

ncQtVideo::~ncQtVideo() {
	if (poses)
	{
		delete poses;
		poses = 0;
	}
	if (poses_vert)
	{
		delete poses_vert;
		poses_vert = 0;
	}
	if (poses_mesh)
	{
		delete poses_mesh;
		poses_mesh = 0;
	}
	if (calset)
	{
		delete calset;
		calset = 0;
	}
	if (calset_mesh)
	{
		delete calset_mesh;
		calset_mesh = 0;
	}
	if (calset_vert)
	{
		delete calset_vert;
		calset_vert = 0;
	}
	if (calset_index)
	{
		delete calset_index;
		calset_index = 0;
	}
}

void ncQtVideo::showCamFile() {
	cam_file_name->setEnabled(true);
}

void ncQtVideo::showSlider() {

    pose_spinbox->setRange(poses->getMinVid(), poses->getMaxVid());
    pose_spinbox->setEnabled(true);

    static_poses->setEnabled(true);

    slider_->setRange(poses->getMinVid(), poses->getMaxVid());
    slider_->setValue(poses->getMinVid());
    slider_->setEnabled(true);

    begin_slider->setRange(poses->getMinVid(), poses->getMaxVid());
    begin_slider->setValue(poses->getMinVid());
	begin_->setText(QString::number(poses->getMinVid()));
    begin_slider->setEnabled(true);

    end_slider->setRange(poses->getMinVid(), poses->getMaxVid());
    end_slider->setValue(poses->getMaxVid());
	end_->setText(QString::number(poses->getMaxVid()));
    end_slider->setEnabled(true);

    pose_spinbox->blockSignals(false);
    slider_->blockSignals(false);
    static_poses->blockSignals(false);

	toolbar->setEnabled(true);

}

void ncQtVideo::play() {
    if (!isPlaying())
    {
        play_->setText("Pause");
        play_->setIcon(QIcon(":ncqtvideo/pause.png"));
        double fps = 25.0;
        timer_->start(1000.0/fps);
    }
}

void ncQtVideo::pause() {
    if (isPlaying())
    {
        play_->setText("Play");
        play_->setIcon(QIcon(":ncqtvideo/play.png"));
       	timer_->stop();
    }
}

void ncQtVideo::togglePlay() {

    if (isPlaying())
        pause();
    else
        play();
}

void ncQtVideo::reset() {
	pause();
	slider_->setValue(poses->getMinVid());
	begin_slider->setValue(poses->getMinVid());
	end_slider->setValue(poses->getMaxVid());
}

void ncQtVideo::start() {

    if (slider_->value() < end_slider->value())
	{
        begin_slider->setValue(slider_->value());
		begin_->setText(QString::number(slider_->value()));
	}
	else
        QMessageBox::information(0,"Oops!", "Start value must be smaller than end value\n");
}

void ncQtVideo::end() {
    if (slider_->value() > begin_slider->value())
    {
		end_slider->setValue(slider_->value());
		end_->setText(QString::number(slider_->value()));
	}
	else
        QMessageBox::information(0,"Oops!", "End value must be larger than start value\n");

}

void ncQtVideo::record() {
    pause();
    startRecord();
}

void ncQtVideo::loadPoseFile() {

    QString full_file = QFileDialog::getOpenFileName(0,tr("Select Pose File\n"),pose_browsepath, "Pose files *", 0,QFileDialog::ReadOnly);
	qApp->processEvents();
    loadPoseFile(full_file.toStdString());

    ncc::RTd MVpose = (*poses)[pose_spinbox->value()];
    if (calset)
        poseChanged(MVpose*(*calset)[cam_spinbox->value()].MV());
    else
        poseChanged(MVpose);
}

void ncQtVideo::loadCamFile() {

    QString full_file = QFileDialog::getOpenFileName(0,tr("Select Camera File\n"),cam_browsepath, "Camera files *.txt", 0,QFileDialog::ReadOnly);
	qApp->processEvents();
    loadCamFile(full_file.toStdString());

    const ncCalibration* cal = &(*calset)[cam_spinbox->value()];
	cameraChanged(cal->VP(0),cal->KGL(0));

    ncc::RTd MVpose = (*poses)[pose_spinbox->value()];
    if (calset)
        poseChanged(MVpose*cal->MV());
    else
        poseChanged(MVpose);
}

bool ncQtVideo::loadPoseFile(std::string filename) {

    if (filename.empty())
        return false;

    if (poses)
	{
		delete poses;
		poses = 0;
	}
	if (poses_vert)
	{
		delete poses_vert;
		poses_vert = 0;
	}
	if (poses_mesh)
	{
		delete poses_mesh;
		poses_mesh = 0;
	}

	poses = new ncPoseList;
    bool ok = poses->readFile(filename);

    if (!ok)
    {
        delete poses;
        poses = 0;
        QMessageBox::critical(0,"Load error","Not a valid pose file");
        return false;
    }

    int nrvalid = 0;
    for (unsigned int i=poses->getMinVid();i<=poses->getMaxVid();++i)
        if (poses->isValid(i))
        {
            nrvalid++;
            if (nrvalid>=2)
                break;
        }

    /*
    if (nrvalid<2)
    {
        delete poses;
        poses = 0;
        QMessageBox::critical(0,"Load error","At least 2 valid poses are required");
        return false;
    }
    */


	//! transfer to GPU
	int maxv_	= poses->getMaxVid();
	int minv_	= poses->getMinVid();
	int nrposes = maxv_-minv_+1;
	ncc::v4f* pose_points	= new ncc::v4f[nrposes];
	ncc::RTd  transform		= poses->getTransform();
	ncc::RTd itransform		= transform.inverse();
	for (int i=0;i<nrposes;++i)
		pose_points[i] = itransform*((*poses)[minv_+i][3]);

	poses_mesh = new ncMesh;
	poses_vert = new ncPBO;
	poses_vert->setup(nrposes,&pose_points[0]);
	poses_mesh->bindAttribF(0,poses_vert,4,GL_FLOAT,GL_FALSE,sizeof(ncc::v4f ),0);

	delete[] pose_points;

    QString cap_file = filename.c_str();
    if (cap_file.length()>string_cap_length)
        cap_file = cap_file.right(string_cap_length-3).prepend("...");

    QFileInfo pathInfo(filename.c_str());
    pose_file_name->setText(cap_file);
    pose_browsepath = pathInfo.path();
    cam_browsepath = pathInfo.path();
    showSlider();
    showCamFile();

    return true;
}

bool ncQtVideo::loadCamFile(std::string filename) {

    if (filename.empty())
        return false;

    if (calset)
	{
		delete calset;
		calset = 0;
	}
	if (calset_mesh)
	{
		delete calset_mesh;
		calset_mesh = 0;
	}
	if (calset_vert)
	{
		delete calset_vert;
		calset_vert = 0;
	}
	if (calset_index)
	{
		delete calset_index;
		calset_index = 0;
	}

	calset = new ncCalibrationSet;
    calset->readFile(filename);

	//! transfer to GPU
	int nrcams = calset->getNrCameras();
	ncc::v4f* cam_points	= new ncc::v4f[nrcams+1];
	for (int i=0;i<nrcams;++i)
		cam_points[i] = (*calset)[i].MV()[3];
	cam_points[nrcams] = ncc::v4f(0.0f,0.0f,0.0f,1.0f);
	calset_mesh = new ncMesh;
	calset_vert = new ncPBO;
	calset_vert->setup(nrcams+1,&cam_points[0]);
	calset_mesh->bindAttribF(0,calset_vert,4,GL_FLOAT,GL_FALSE,sizeof(ncc::v4f ),0);

	calset_index = new ncIndexArray;
	unsigned int* cam_indices = new unsigned int[nrcams*2];

	for (int i=0;i<nrcams;++i)
	{
		cam_indices[2*i+0] = nrcams;
		cam_indices[2*i+1] = i;
	}

	calset_index->setup(nrcams*2,&cam_indices[0]);

	delete[] cam_indices;
	delete[] cam_points;


    QString cap_file = filename.c_str();
    if (cap_file.length()>string_cap_length)
        cap_file = cap_file.right(string_cap_length-3).prepend("...");

    QFileInfo pathInfo(filename.c_str());
    cam_file_name->setText(cap_file);
    cam_browsepath = pathInfo.path();

    cam_spinbox->setRange(0,calset->getNrCameras()-1);
    cam_spinbox->setEnabled(true);
    cam_spinbox->blockSignals(false);

    return true;
}

void  ncQtVideo::drawPoses(GLenum mode) {
	poses_mesh->draw(mode,poses_vert->nrelements);
}

void  ncQtVideo::drawCameras(GLenum mode) {
	if (mode == GL_POINTS)
		calset_mesh->draw(mode,calset->getNrCameras());
	if (mode == GL_LINES)
		calset_mesh->draw(mode,calset_index);
}

void ncQtVideo::changeCamera(int index) {
	const ncCalibration* cal = &(*calset)[index];
    cameraChanged(cal->VP(0),cal->KGL(0));
    ncc::RTd MVpose = (*poses)[slider_->value()];
    poseChanged(MVpose*cal->MV());
}

void ncQtVideo::changePose(int index) {

    if (!static_poses->checkState() && !poses->isValid(index))
    {
        while ( (index < (int)poses->getMaxVid()) && (!poses->isValid(index)))
            index++;
    }

    slider_->blockSignals(true);
    pose_spinbox->blockSignals(true);
    slider_->setValue(index);
    pose_spinbox->setValue(index);
    slider_->blockSignals(false);
    pose_spinbox->blockSignals(false);

    ncc::RTd MVpose = (*poses)[index];
    if (calset)
        poseChanged(MVpose*(*calset)[cam_spinbox->value()].MV());
    else
        poseChanged(MVpose);
}

void ncQtVideo::next() {

    if ((slider_->value() < begin_slider->value()) || (slider_->value() >= end_slider->value()))
        slider_->setValue(begin_slider->value());
    else
        slider_->setValue(slider_->value()+1);
}

bool ncQtVideo::isPlaying() {
    return (play_->text() != "Play");
}

