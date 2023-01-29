#include "ncqtcamera.h"

ncQtCamera::ncQtCamera(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas) : ncQtDock(title,parent,init_area,allowed_areas) {

    manip = new osgGA::TrackballManipulator;
    setAllowEventFocus( false );
	setClearMask(0);
    setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);


    //! parameters

	setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);

	camFOV			= new ncQtDoubleSliderCombo("Field Of View"	, 1.0	    , 179.0		, 0.1	, this);
	camFOV->setValue(60.0);
	grid->addWidget(camFOV->label   ,0,0);
	grid->addWidget(camFOV->slider  ,0,1);
	grid->addWidget(camFOV->spinbox ,0,2);

	camZnear		= new ncQtDoubleSliderCombo("Z near       "	, 0.1	    , 1000.0    , 0.1	, this);
	camZnear->setValue(0.1);
	grid->addWidget(camZnear->label   ,1,0);
	grid->addWidget(camZnear->slider  ,1,1);
	grid->addWidget(camZnear->spinbox ,1,2);

	camZfar			= new ncQtDoubleSliderCombo("Z far        "	, 0.1	    , 1000.0    , 0.1	, this);
	camZfar->setValue(1000.0);
	grid->addWidget(camZfar->label   ,2,0);
	grid->addWidget(camZfar->slider  ,2,1);
	grid->addWidget(camZfar->spinbox ,2,2);

	QGridLayout* rpy_grid = new QGridLayout();

    int precision = 5;

	//! rpy

	roll_checkbox  = new QCheckBox("Roll",this);
	roll_checkbox->setChecked(true);
	pitch_checkbox  = new QCheckBox("Pitch",this);
	pitch_checkbox->setChecked(false);
	yaw_checkbox  = new QCheckBox("Yaw",this);
	yaw_checkbox->setChecked(false);

	roll_spinbox    = new ncQtDoubleSpinBox(this);
	roll_spinbox->setRange(-M_PI,M_PI);
	roll_spinbox->setSingleStep(0.01);
	roll_spinbox->setDecimals(precision);
	pitch_spinbox   = new ncQtDoubleSpinBox(this);
	pitch_spinbox->setRange(-M_PI,M_PI);
	pitch_spinbox->setSingleStep(0.01);
	pitch_spinbox->setDecimals(precision);
	yaw_spinbox     = new ncQtDoubleSpinBox(this);
    yaw_spinbox->setRange(-M_PI,M_PI);
	yaw_spinbox->setSingleStep(0.01);
	yaw_spinbox->setDecimals(precision);

    rpy_grid->addWidget(roll_checkbox  ,0,0,1,1,Qt::AlignHCenter);
    rpy_grid->addWidget(pitch_checkbox ,0,1,1,1,Qt::AlignHCenter);
    rpy_grid->addWidget(yaw_checkbox   ,0,2,1,1,Qt::AlignHCenter);

    rpy_grid->addWidget(roll_spinbox  ,1,0,1,1,Qt::AlignHCenter);
    rpy_grid->addWidget(pitch_spinbox ,1,1,1,1,Qt::AlignHCenter);
    rpy_grid->addWidget(yaw_spinbox   ,1,2,1,1,Qt::AlignHCenter);


    //! pos

	x_label = new QLabel("X",this);
	y_label = new QLabel("Y",this);
	z_label = new QLabel("Z",this);

	x_spinbox    = new ncQtDoubleSpinBox(this);
	x_spinbox->setSingleStep(0.01);
	x_spinbox->setDecimals(precision);
	y_spinbox   = new ncQtDoubleSpinBox(this);
	y_spinbox->setSingleStep(0.01);
	y_spinbox->setDecimals(precision);
	z_spinbox     = new ncQtDoubleSpinBox(this);
	z_spinbox->setSingleStep(0.01);
	z_spinbox->setDecimals(precision);

	rpy_grid->addWidget(x_label ,2,0,1,1,Qt::AlignHCenter);
    rpy_grid->addWidget(y_label ,2,1,1,1,Qt::AlignHCenter);
    rpy_grid->addWidget(z_label ,2,2,1,1,Qt::AlignHCenter);

    rpy_grid->addWidget(x_spinbox ,3,0,1,1,Qt::AlignHCenter);
    rpy_grid->addWidget(y_spinbox ,3,1,1,1,Qt::AlignHCenter);
    rpy_grid->addWidget(z_spinbox ,3,2,1,1,Qt::AlignHCenter);

	grid->addLayout(rpy_grid , 4, 0, 1, 3);

	bgcolor     = ncc::v4ub(0,0,0,0);
    bg_select	= new QPushButton("Select Background Color",this);

    grid->addWidget(bg_select	,5,0,1,3);
	grid->setRowStretch(6,10);

    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    setRoll(0.0);
	setPitch(0.0);
	setYaw(0.0);
	setX(0.0);
	setY(0.0);
	setZ(0.0);

	connect(camFOV          , SIGNAL(valueChanged(double)) , this , SLOT(setFOV(double))    );
    connect(camZnear        , SIGNAL(valueChanged(double)) , this , SLOT(setZnear(double))  );
    connect(camZfar         , SIGNAL(valueChanged(double)) , this , SLOT(setZfar(double))   );
    connect(roll_spinbox    , SIGNAL(valueChanged(double)) , this , SLOT(setRoll(double))   );
    connect(pitch_spinbox   , SIGNAL(valueChanged(double)) , this , SLOT(setPitch(double))  );
    connect(yaw_spinbox     , SIGNAL(valueChanged(double)) , this , SLOT(setYaw(double))    );
    connect(x_spinbox       , SIGNAL(valueChanged(double)) , this , SLOT(setX(double))      );
    connect(y_spinbox       , SIGNAL(valueChanged(double)) , this , SLOT(setY(double))      );
    connect(z_spinbox       , SIGNAL(valueChanged(double)) , this , SLOT(setZ(double))      );

  	connect(&values_changed	, SIGNAL(output())			   , this , SLOT(updateSlot()) );

}

ncQtCamera::~ncQtCamera() {
}

void ncQtCamera::updateGui() {

    double fovy, aspectRatio, zNear, zFar;
    osg::Vec3d eye, center, up;

    getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
    getViewMatrixAsLookAt(eye, center, up, 1.0);

    ncc::v3d rpy,pos;
    decompose(MV(),rpy,pos);

    this->blockSignals(true);
    //fprintf(stderr,"blocked++\n");
    values_changed++;
    camFOV->setValue(fovy);
    camZnear->setValue(zNear);
    camZfar->setValue(zFar);
    roll_spinbox->setValue(rpy[0]);
	pitch_spinbox->setValue(rpy[1]);
	yaw_spinbox->setValue(rpy[2]);
    x_spinbox->setValue(eye[0]);
	y_spinbox->setValue(eye[1]);
	z_spinbox->setValue(eye[2]);
    values_changed--;
    this->blockSignals(false);
    //fprintf(stderr,"blocked--\n");

}

void ncQtCamera::setFOV(double fov_) {
    //fprintf(stderr,"setting fov to %f\n",fov_);
    values_changed++;
    double fovy, aspectRatio, zNear, zFar;
    getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
    fovy = fov_;
    setProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
    values_changed--;
}

void ncQtCamera::setZnear(double znear_) {

    //fprintf(stderr,"setting Znear to %f\n",znear_);
    values_changed++;
    double fovy, aspectRatio, zNear, zFar;
    getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
    zNear = znear_;
    setProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
    values_changed--;
}

void ncQtCamera::setZfar(double zfar_) {

	//fprintf(stderr,"setting Zfar to %f\n",zfar_);
    values_changed++;
    double fovy, aspectRatio, zNear, zFar;
    getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
    zFar = zfar_;
    setProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
    values_changed--;
}

void ncQtCamera::setRoll(double roll_) {

    values_changed++;
    ncc::v3d rpy,pos;
    ncc::RTd tmp = MV();
    decompose(tmp,rpy,pos);
    rpy.x = roll_;
    compose(tmp,rpy,pos);
    setMV(tmp);
    values_changed--;
}

void ncQtCamera::setPitch(double pitch_) {
    values_changed++;
    ncc::v3d rpy,pos;
    ncc::RTd tmp = MV();
    decompose(tmp,rpy,pos);
    rpy.y = pitch_;
    compose(tmp,rpy,pos);
    setMV(tmp);
    values_changed--;
}

void ncQtCamera::setYaw(double yaw_) {
    values_changed++;
    ncc::v3d rpy,pos;
    ncc::RTd tmp = MV();
    decompose(tmp,rpy,pos);
    rpy.z = yaw_;
    compose(tmp,rpy,pos);
    setMV(tmp);
    values_changed--;
}

void ncQtCamera::setX(double x_) {

    values_changed++;
    ncc::v3d rpy,pos;
    ncc::RTd tmp = MV();
    decompose(tmp,rpy,pos);
    pos.x = x_;
    compose(tmp,rpy,pos);
    setMV(tmp);
    values_changed--;
}

void ncQtCamera::setY(double y_) {

    values_changed++;
    ncc::v3d rpy,pos;
    ncc::RTd tmp = MV();
    decompose(tmp,rpy,pos);
    pos.y = y_;
    compose(tmp,rpy,pos);
    setMV(tmp);
    values_changed--;
}

void ncQtCamera::setZ(double z_) {

    values_changed++;
    ncc::v3d rpy,pos;
    ncc::RTd tmp = MV();
    decompose(tmp,rpy,pos);
    pos.z = z_;
    compose(tmp,rpy,pos);
    setMV(tmp);
    values_changed--;
}

ncc::RTd  ncQtCamera::MV() {
	return osg2ncc(getInverseViewMatrix())*ncc::RTd::scale(1.0,-1.0,-1.0);
}

ncc::m4x4d  ncQtCamera::KGL() {
	return osg2ncc(getProjectionMatrix())*ncc::RTd::scale(1.0,-1.0,-1.0);
}

void ncQtCamera::setMV(ncc::RTd mat) {

    //mat.print("pre-test");
    values_changed++;
    ncc::RTd MVV = ncc::RTd::scale(1.0,-1.0,-1.0)*mat.inverse();
    manip->setByInverseMatrix(ncc2osg(MVV));
    setViewMatrix (ncc2osg(MVV));
    values_changed--;
    //qApp->processEvents();
    //MV().print("post-test");
}

void ncQtCamera::compose(ncc::RTd & mat, ncc::v3d rpy , ncc::v3d pos) {

    double cr = cos(rpy.x);
    double sr = sin(rpy.x);

    double cp = cos(rpy.y);
    double sp = sin(rpy.y);

    double cy = cos(rpy.z);
    double sy = sin(rpy.z);

    mat.setIdentity();

    mat[0][0] =  cr*cy+sr*sp*sy;
    mat[0][1] = -cr*sy+sr*sp*cy;
    mat[0][2] =  sr*cp;

    mat[1][0] =  sr*cy-cr*sp*sy;
    mat[1][1] = -sr*sy-cr*sp*cy;
    mat[1][2] = -cr*cp;

    mat[2][0] =  cp*sy;
    mat[2][1] =  cp*cy;
    mat[2][2] = -sp;

    mat[3][0] = pos.x;
    mat[3][1] = pos.y;
    mat[3][2] = pos.z;

}

void ncQtCamera::decompose(ncc::RTd mat, ncc::v3d & rpy , ncc::v3d & pos) {

    double roll,pitch,yaw;
    double tmp;

    roll = atan2(mat[0][2] , -mat[1][2]);
    tmp   = cos(roll);

    if (tmp>0.0)
        pitch= atan2(-tmp*mat[2][2] , -mat[1][2]);
    else
        pitch= atan2( tmp*mat[2][2] ,  mat[1][2]);

    tmp = cos(pitch);

    if (tmp>0.0)
    {
        roll = atan2( mat[0][2] , -mat[1][2]);
        yaw  = atan2( mat[2][0] ,  mat[2][1]);
    }
    else
    {
        roll = atan2(-mat[0][2] ,  mat[1][2]);
        yaw  = atan2(-mat[2][0] , -mat[2][1]);
    }

    rpy.x = roll;
    rpy.y = pitch;
    rpy.z = yaw;

    pos.x = mat[3][0];
    pos.y = mat[3][1];
    pos.z = mat[3][2];

}

void ncQtCamera::updateSlot() {
    //fprintf(stderr,"sending ncqtCamera::valuesChanged()\n");
    valuesChanged();
}
