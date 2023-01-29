#include "ncqtcamera.h"

ncSkyDome::ncSkyDome() {

    Q_INIT_RESOURCE(ncqt_shaders);

    cube_prog   = 0;
    cube_vert   = 0;
    cube_mesh   = 0;
    cube_map    = 0;
    valid       = false;

    base_dir = QDir::currentPath() + "/skydomes";
    QDir dir1(base_dir);
    dir1.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    dir1.setSorting(QDir::Name);

    QFileInfoList dirlist = dir1.entryInfoList();
    if (dirlist.size() > 0)
    {
        for (int i = 0; i < dirlist.size(); ++i) {
            QFileInfo dirInfo = dirlist.at(i);
            addItem(dirInfo.fileName());
            std::cerr << qPrintable(QString("Found skydome %1").arg(dirInfo.absoluteFilePath())) << std::endl;
        }
    }

    connect(this , SIGNAL(currentIndexChanged(int)) , this , SLOT(setIndex(int)));
}

ncSkyDome::~ncSkyDome() {

    //fprintf(stderr,"~ncSkyDome()\n");
    clear();
}

void ncSkyDome::clear() {
    //fprintf(stderr,"clear()\n");

    if(cube_prog)
    {
        delete cube_prog;
        cube_prog = 0;
    }
    if(cube_vert)
    {
        delete cube_vert;
        cube_vert = 0;
    }
    if(cube_mesh)
    {
        delete cube_mesh;
        cube_mesh = 0;
    }
    if(cube_map)
    {
        delete cube_map;
        cube_map = 0;
    }
    valid = false;
}

void ncSkyDome::setIndex(int i) {

    if (!valid)
    {
        cube_prog = new ncQtProgram(":/shaders/cube.vert"		, ":/shaders/cube.geom"		, ":/shaders/cube.frag"		);
        ncc::v4f origin = ncc::v4f(0.0f,0.0f,0.0f,1.0f);
        cube_vert = new ncPBO;
        cube_vert->setup(1,&origin);
        cube_mesh = new ncMesh;
        cube_mesh->bindAttribF(0,cube_vert,4,GL_FLOAT,GL_FALSE	, sizeof(ncc::v4f ),0);
        cube_map = new ncTexImageCube(GL_RGBA8,64);
        valid = true;
    }

    //fprintf(stderr,"setIndex(%d) [%d]\n",i,count());
    if ((i>=0) && (i<count()))
    {
        ncImageBuffer<ncc::v4ub> texdat;

        QString mydir = base_dir + "/" + itemText(i);

        ncc::readImage((mydir + "/posx.jpg").toStdString().c_str(),texdat);
        //cube_map->resize(tw);
        delete cube_map;
        cube_map = new ncTexImageCube(GL_RGBA8,texdat.width);
        cube_map->setFilters(GL_LINEAR,GL_LINEAR);
        cube_map->setData(0,texdat.data);

        ncc::readImage((mydir + "/negx.jpg").toStdString().c_str(),texdat);
        cube_map->setData(1,texdat.data);

        ncc::readImage((mydir + "/posy.jpg").toStdString().c_str(),texdat);
        cube_map->setData(2,texdat.data);

        ncc::readImage((mydir + "/negy.jpg").toStdString().c_str(),texdat);
        cube_map->setData(3,texdat.data);

        ncc::readImage((mydir + "/posz.jpg").toStdString().c_str(),texdat);
        cube_map->setData(4,texdat.data);

        ncc::readImage((mydir + "/negz.jpg").toStdString().c_str(),texdat);
        cube_map->setData(5,texdat.data);

        indexChanged();
    }
}

void ncSkyDome::draw(ncc::m4x4d KGL , ncc::RTd MV) {

    if (!valid)
        setIndex(currentIndex());

    ncc::RTd cMVi = MV.inverse();
    cMVi[3][0] = cMVi[3][1] = cMVi[3][2] = 0.0;

    glClearDepth(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_CLAMP);
    glDepthMask(GL_FALSE);
    cube_prog->Activate();
    cube_prog->setUniform("mRadius",1.0f);
    cube_prog->setUniform("matP",KGL*cMVi);
    cube_prog->setSampler("cube_tex",cube_map,0);
    cube_mesh->draw(GL_POINTS,1);
    cube_prog->Deactivate();
    glDepthMask(GL_TRUE);
    glDisable(GL_DEPTH_CLAMP);

}

ncQtCamera::ncQtCamera(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas) : ncQtDock(title,parent,init_area,allowed_areas) {

    skydome = new ncSkyDome;
    bgmode  = 0;

	//! parameters

	setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Minimum);

	camFOV			= new ncQtDoubleSliderCombo("Field Of View"	, 1.0	    , 179.0		, 0.1	, this);
	camFOV->setValue(60.0);
	grid->addWidget(camFOV->label   ,0,0);
	grid->addWidget(camFOV->slider  ,0,1);
	grid->addWidget(camFOV->spinbox ,0,2);

	/*
	camAR			= new ncQtDoubleSliderCombo("Aspect Ratio"	, 0.1	    , 1.9		, 0.1	, this);
	camAR->setValue(1.0);
	grid->addWidget(camAR->label   ,1,0);
	grid->addWidget(camAR->slider  ,1,1);
	grid->addWidget(camAR->spinbox ,1,2);
	*/

	camZnear		= new ncQtDoubleSliderCombo("Z near       "	, 0.1	    , 1000.0    , 0.1	, this);
	camZnear->setValue(0.1);
	grid->addWidget(camZnear->label   ,2,0);
	grid->addWidget(camZnear->slider  ,2,1);
	grid->addWidget(camZnear->spinbox ,2,2);

	camZfar			= new ncQtDoubleSliderCombo("Z far        "	, 0.1	    , 10000.0    , 0.1	, this);
	camZfar->setValue(10000.0);
	grid->addWidget(camZfar->label   ,3,0);
	grid->addWidget(camZfar->slider  ,3,1);
	grid->addWidget(camZfar->spinbox ,3,2);

	Msens			= new ncQtDoubleSliderCombo("Sensitivity  "	, 0.0	    , 10.0		, 0.1	, this);
	Msens->setValue(1.0);
	grid->addWidget(Msens->label   ,4,0);
	grid->addWidget(Msens->slider  ,4,1);
	grid->addWidget(Msens->spinbox ,4,2);


	QGridLayout* rpy_grid = new QGridLayout();

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
	pitch_spinbox   = new ncQtDoubleSpinBox(this);
	pitch_spinbox->setRange(-M_PI,M_PI);
	pitch_spinbox->setSingleStep(0.01);
	yaw_spinbox     = new ncQtDoubleSpinBox(this);
    yaw_spinbox->setRange(-M_PI,M_PI);
	yaw_spinbox->setSingleStep(0.01);

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
	y_spinbox   = new ncQtDoubleSpinBox(this);
	y_spinbox->setSingleStep(0.01);
	z_spinbox     = new ncQtDoubleSpinBox(this);
	z_spinbox->setSingleStep(0.01);

	rpy_grid->addWidget(x_label ,2,0,1,1,Qt::AlignHCenter);
    rpy_grid->addWidget(y_label ,2,1,1,1,Qt::AlignHCenter);
    rpy_grid->addWidget(z_label ,2,2,1,1,Qt::AlignHCenter);

    rpy_grid->addWidget(x_spinbox ,3,0,1,1,Qt::AlignHCenter);
    rpy_grid->addWidget(y_spinbox ,3,1,1,1,Qt::AlignHCenter);
    rpy_grid->addWidget(z_spinbox ,3,2,1,1,Qt::AlignHCenter);

	grid->addLayout(rpy_grid , 5, 0, 1, 3);

	bgcolor     = ncc::v4ub(0,0,0,0);
    bg_select	= new QPushButton("Select Background Color",this);


    bgcolor_radio = new QRadioButton("Fixed Color", this);
    skydome_radio = new QRadioButton("Sky Dome"   , this);

    if (skydome->count()==0)
    {
        //grid->addWidget(bgcolor_radio   ,5,0,1,1);
        grid->addWidget(bg_select	    ,6,0,1,3);
        //grid->addWidget(skydome_radio	,6,0,1,1);
        //grid->addWidget(skydome	        ,6,1,1,2);

        skydome_radio->hide();
        skydome->hide();
        bgcolor_radio->click();
        bgcolor_radio->hide();
    }
    else
    {
        grid->addWidget(bgcolor_radio   ,6,0,1,1);
        grid->addWidget(bg_select	    ,6,1,1,2);
        grid->addWidget(skydome_radio	,7,0,1,1);
        grid->addWidget(skydome	        ,7,1,1,2);

        skydome_radio->click();
    }
	setColor(bgcolor);

	grid->setRowStretch(7,10);

	setupSignals();

    lmb = false;
    mmb = false;
    rmb = false;

    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

}

ncQtCamera::~ncQtCamera() {
}

void ncQtCamera::drawBackGround() {

    //fprintf(stderr,"drawing background...\n");
    if (bgcolor_radio->isChecked())
    {
        //fprintf(stderr,"drawing bgcolor...\n");
        ncProgram::Deactivate();
        float sc = 1.0f/255.0f;
        glClearDepth(1.0f);
        glClearColor(sc*bgcolor.x,sc*bgcolor.y,sc*bgcolor.z,sc*bgcolor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else
    {
        //fprintf(stderr,"drawing skydome...\n");
        skydome->draw(KGL(),MV());
    }
}

void ncQtCamera::setupSignals() {

	connect(roll_spinbox    , SIGNAL(valueChanged(double)	)   , &values_changed	, SLOT(input())				);
	connect(pitch_spinbox   , SIGNAL(valueChanged(double)	)   , &values_changed	, SLOT(input())				);
	connect(yaw_spinbox     , SIGNAL(valueChanged(double)	)   , &values_changed	, SLOT(input())				);
	connect(x_spinbox       , SIGNAL(valueChanged(double)	)   , &values_changed	, SLOT(input())				);
	connect(y_spinbox       , SIGNAL(valueChanged(double)	)   , &values_changed	, SLOT(input())				);
	connect(z_spinbox       , SIGNAL(valueChanged(double)	)   , &values_changed	, SLOT(input())				);
	connect(camFOV		    , SIGNAL(valueChanged(double)	)   , &values_changed	, SLOT(input())				);
	//connect(camAR		    , SIGNAL(valueChanged(double)	)   , &values_changed	, SLOT(input())				);
	connect(camZnear	    , SIGNAL(valueChanged(double)	)   , this				, SLOT(setZnear(double))	);
	connect(camZfar		    , SIGNAL(valueChanged(double)	)   , this				, SLOT(setZfar(double))	);
	connect(bg_select		, SIGNAL(clicked()				)	, this				, SLOT(setColor())			);
	connect(bgcolor_radio   , SIGNAL(clicked()              )   , &values_changed	, SLOT(input())				);
	connect(skydome_radio   , SIGNAL(clicked()              )   , &values_changed	, SLOT(input())				);
	connect(skydome         , SIGNAL(indexChanged()         )   , &values_changed	, SLOT(input())				);

	connect(&values_changed	, SIGNAL(output()				)	, this				, SLOT(sendValuesChanged())	);

}

void ncQtCamera::setColor() {

	QColor color = QColorDialog::getColor();
	setColor(ncc::v4ub(color.red(),color.green(),color.blue(),color.alpha()));
}

void ncQtCamera::setColor(ncc::v4ub col) {
	values_changed++;
	bgcolor = col;
    values_changed--;
}

ncc::v4ub ncQtCamera::getColor() {
	return bgcolor;
}

void ncQtCamera::setRoll(double r) {
	roll_spinbox->setValue(r);
}

void ncQtCamera::setPitch(double p) {
	pitch_spinbox->setValue(p);
}

void ncQtCamera::setYaw(double y) {
	yaw_spinbox->setValue(y);
}

void ncQtCamera::setX(double x) {
	x_spinbox->setValue(x);
}

void ncQtCamera::setY(double y) {
	y_spinbox->setValue(y);
}

void ncQtCamera::setZ(double z) {
	z_spinbox->setValue(z);
}

ncc::RTd  ncQtCamera::MV() {
	return compose(getRPY(),getPos());
}

ncc::m4x4d  ncQtCamera::KGL() {

	double znear = getZnear();
	double zfar = getZfar();
	double fovyInDegrees = getFOV();

	double ymax = znear * tan(fovyInDegrees * M_PI / 360.0);
    double xmax = ymax * getAspectRatio();

    ncc::m4x4d KGL_;
	KGL_[0][0] = znear / xmax;
    KGL_[0][1] = 0.0;
    KGL_[0][2] = 0.0;
    KGL_[0][3] = 0.0;
    KGL_[1][0] = 0.0;
    KGL_[1][1] = -znear / ymax;
    KGL_[1][2] = 0.0;
    KGL_[1][3] = 0.0;
    KGL_[2][0] = 0.0;
    KGL_[2][1] = 0.0;
    KGL_[2][2] = (zfar + znear) / (zfar - znear);
    KGL_[2][3] = 1.0;
    KGL_[3][0] = 0.0;
    KGL_[3][1] = 0.0;
    KGL_[3][2] = (-2.0 * znear * zfar) / (zfar - znear);
    KGL_[3][3] = 0.0;

	return KGL_;
}

void ncQtCamera::setKGL(ncc::m4x4d KGL_) {
	KGL_.print("KGL");

}

void ncQtCamera::sendValuesChanged() {
    valuesChanged();
}

void ncQtCamera::processEvent(QMouseEvent *event) {


	int dx = event->x() - mousePos.x();
    int dy = event->y() - mousePos.y();

    mousePos = event->pos();

    lmb = event->buttons() & Qt::LeftButton;
    mmb = event->buttons() & Qt::MidButton;
    rmb = event->buttons() & Qt::RightButton;

    if (event->type() == QEvent::MouseMove)
    {
		if (lmb || mmb || rmb)
		{
			values_changed++;
			if (lmb && !mmb && !rmb)
				rotate(ncc::v4i(mousePos.x()-dx,mousePos.y()-dy,mousePos.x(),mousePos.y()));
			if (!lmb && mmb && !rmb)
				strafe(dx,dy,0.0);
			if (!lmb && !mmb && rmb)
				strafe(0,0,dy);
			values_changed--;
		}
    }

}

QPoint  ncQtCamera::getMousePos() {
    return mousePos;
}

void ncQtCamera::strafe(double x, double y, double z) {

	double msens = getMouseSens();
	double fx = -x*0.1*msens;
	double fy = -y*0.1*msens;
	double fz = -z*0.1*msens;
	ncc::v4d tmp = MV()*ncc::v4d(fx,fy,fz,0.0);
	setPos(getPos()+tmp.xyz());
}

void ncQtCamera::rotate(ncc::v4i in) {

    double xold =           (in.x+0.5);
    double yold = height-1- (in.y+0.5);
    double xnew =           (in.z+0.5);
    double ynew = height-1- (in.w+0.5);

    ncc::m4x4d R = MV();
    R[3][0] = 0.0;
    R[3][1] = 0.0;
    R[3][2] = 0.0;

    ncc::m4x4d Ki = KGL().inverse();

    ncc::v4d Pnew;
    Pnew.x = 2.0*(xnew/width ) -1.0;
    Pnew.y = 2.0*(ynew/height) -1.0;
    Pnew.z = -1.0;
    Pnew.w = 1.0;

    ncc::v3d Rnew = (Ki*Pnew).xyz().normalize();

    ncc::v4d Pold;
    Pold.x = 2.0*(xold/width ) -1.0;
    Pold.y = 2.0*(yold/height) -1.0;
    Pold.z = -1.0;
    Pold.w = 1.0;

    ncc::v3d Rold = (R*Ki*Pold).xyz().normalize();

    double a,b,c,alpha;
    double roll = getRoll();
    double pitch;
    double yaw;
    double tmp,tmp1,tmp2,tmp3,tmp4;

	ncc::v3d rpy = getRPY();


    //!pitch

    a = -Rnew.z;
    b = sin(roll)*Rnew.x - cos(roll)*Rnew.y;
    c = Rold.z;

    alpha   = atan2(b,a);
    tmp     = asin(c/sqrt(a*a+b*b));

#ifdef WIN32
	if (_isnan(tmp))
#else
	if (isnan(tmp))
#endif
	{
        return;
    }
    else
    {
        tmp1 =         tmp  - alpha;
        tmp2 = (M_PI - tmp) - alpha;

        tmp3 = (cos(rpy.y)*cos(tmp1) +sin(rpy.y)*sin(tmp1));
        tmp4 = (cos(rpy.y)*cos(tmp2) +sin(rpy.y)*sin(tmp2));

        if ( tmp3 > tmp4 )
            pitch = atan2(sin(tmp1),cos(tmp1));
        else
            pitch = atan2(sin(tmp2),cos(tmp2));
    }


    //! yaw

    a = cos(pitch)*Rold.x;
    b = cos(pitch)*Rold.y;
    c = Rnew.z + sin(pitch)*Rold.z;

    alpha   = atan2(b,a);
    tmp     = asin(c/sqrt(a*a+b*b));

#ifdef WIN32
	if (_isnan(tmp))
#else
	if (isnan(tmp))
#endif
    {
        return;
    }
    else
    {
        tmp1 =         tmp  - alpha;
        tmp2 = (M_PI - tmp) - alpha;

        tmp3 = Rold.x*cos(tmp1) - Rold.y*sin(tmp1) - (cos(roll)*Rnew.x + sin(roll)*Rnew.y);
        tmp4 = Rold.x*cos(tmp2) - Rold.y*sin(tmp2) - (cos(roll)*Rnew.x + sin(roll)*Rnew.y);

        if (fabs(tmp3)<fabs(tmp4))
            yaw = atan2(sin(tmp1),cos(tmp1));
        else
            yaw = atan2(sin(tmp2),cos(tmp2));
    }

    setRPY(ncc::v3d(roll,pitch,yaw));
}

void ncQtCamera::setPos(ncc::v3d pos_) {

	values_changed++;
	setX(pos_.x);
	setY(pos_.y);
	setZ(pos_.z);
	values_changed--;
}

void ncQtCamera::setRPY(ncc::v3d rpy_) {
	values_changed++;
	if (!roll_checkbox->isChecked())
        setRoll(rpy_.x);
    if (!pitch_checkbox->isChecked())
        setPitch(rpy_.y);
    if (!yaw_checkbox->isChecked())
        setYaw(rpy_.z);
	values_changed--;

}

void ncQtCamera::setMV(ncc::v3d rpy_ , ncc::v3d pos_) {

    values_changed++;
	setRPY(rpy_);
    setPos(pos_);
    values_changed--;
}

void ncQtCamera::setMV(ncc::RTd mat) {
    ncc::v3d rpy_,pos_;
    decompose(mat,rpy_,pos_);
    setMV(rpy_,pos_);
}

void ncQtCamera::setFOV(double fov_) {
	camFOV->setValue(fov_);
}

void ncQtCamera::setZnear(double znear_) {

    znear_=(std::min)(znear_,camZfar->maximum()-0.0001);

	values_changed++;
	camZnear->blockSignals(true);
	if (camZfar->value() <= znear_)
    {
    	camZfar->blockSignals(true);
		camZfar->setValue(znear_+0.0001);
        camZfar->blockSignals(false);
    }
	camZnear->setValue(znear_);
	camZnear->blockSignals(false);
    values_changed--;
}

void ncQtCamera::setZfar(double zfar_) {
    zfar_=(std::max)(zfar_,camZnear->minimum()+0.0001);

	values_changed++;
	camZfar->blockSignals(true);
	if (camZnear->value() >= zfar_)
    {
    	camZnear->blockSignals(true);
		camZnear->setValue(zfar_-0.0001);
        camZnear->blockSignals(false);
    }
	camZfar->setValue(zfar_);
	camZfar->blockSignals(false);
    values_changed--;
}

void ncQtCamera::setMouseSens(double msens_) {
	Msens->setValue(msens_);
}

void ncQtCamera::setSize(ncc::v2i size) {
    values_changed++;
    width  = size.x;
    height = size.y;
    values_changed--;
}

double ncQtCamera::getFOV() {
	return camFOV->value();
}

double ncQtCamera::getZnear() {
	return camZnear->value();
}

double ncQtCamera::getZfar() {
	return camZfar->value();
}

double ncQtCamera::getMouseSens() {
	return Msens->value();
}

double ncQtCamera::getAspectRatio() {
	return (double)width/(double)height;
}

double ncQtCamera::getRoll() {
    return roll_spinbox->value();
}

double ncQtCamera::getPitch() {
    return pitch_spinbox->value();
}

double ncQtCamera::getYaw() {
    return yaw_spinbox->value();
}

ncc::RTd ncQtCamera::compose(ncc::v3d rpy , ncc::v3d pos) {

    double cr = cos(rpy.x);
    double sr = sin(rpy.x);

    double cp = cos(rpy.y);
    double sp = sin(rpy.y);

    double cy = cos(rpy.z);
    double sy = sin(rpy.z);

    ncc::RTd out;
    out.setIdentity();

    out[0][0] =  cr*cy+sr*sp*sy;
    out[0][1] = -cr*sy+sr*sp*cy;
    out[0][2] =  sr*cp;

    out[1][0] =  sr*cy-cr*sp*sy;
    out[1][1] = -sr*sy-cr*sp*cy;
    out[1][2] = -cr*cp;

    out[2][0] =  cp*sy;
    out[2][1] =  cp*cy;
    out[2][2] = -sp;

    out[3][0] = pos.x;
    out[3][1] = pos.y;
    out[3][2] = pos.z;

    return out;

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

void ncQtCamera::boundingSphereChanged(ncc::v3d sphere_center , double sphere_radius) {

	double		distance = sphere_radius/sin(0.5*getFOV()*M_PI/180.0);
	ncc::v3d	newpos = ncc::v3d(sphere_center.x,sphere_center.y-distance,sphere_center.z);

	blockSignals(true);
	if (getZnear() > (distance-sphere_radius))
		setZnear(distance-sphere_radius);
	if (getZfar() < (distance+sphere_radius))
		setZfar(distance+sphere_radius);
	blockSignals(false);
    setMV(ncc::v3d(0.0,0.0,0.0),newpos);
}

ncc::v3d ncQtCamera::getPos() {
    return ncc::v3d(x_spinbox->value(),y_spinbox->value(),z_spinbox->value());
}

ncc::v3d ncQtCamera::getRPY() {
    return ncc::v3d(roll_spinbox->value(),pitch_spinbox->value(),yaw_spinbox->value());
}
