#ifndef __NCQTCAMERA_H__
#define __NCQTCAMERA_H__

//#include "ncqt.h"
#include "ncteximage.h"
#include "ncmesh.h"
#include "ncqtprogram.h"
#include "ncqtdock.h"
#include "ncqtsignalhandler.h"
#include "ncqtdoubleslidercombo.h"
/*
#ifdef _MSC_VER
#include <float.h>  // for _isnan() on VC++
#define isnan(x) _isnan(x)  // VC++ uses _isnan() instead of isnan()
#else
#include <math.h>  // for isnan() everywhere else
#endif
*/

class ncSkyDome : public QComboBox {

    Q_OBJECT

public:
    ncSkyDome();
    ~ncSkyDome();

    void clear();
    void draw(ncc::m4x4d KGL , ncc::RTd MV);

public slots:
    void setIndex(int i);

signals:
    void indexChanged();

private:
	ncTexImageCube*		    cube_map;
	ncMesh*                 cube_mesh;
	ncPBO*                  cube_vert;
    ncProgram*	            cube_prog;
    QString                 base_dir;
    bool                    valid;

};


class ncQtCamera : public ncQtDock {

    Q_OBJECT

public:
    ncQtCamera(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas);
    ~ncQtCamera();
    void		drawBackGround();

signals:
    void		valuesChanged();

public slots:
   	void		boundingSphereChanged(ncc::v3d center , double radius);

    void		sendValuesChanged();
    void		processEvent(QMouseEvent *event);

	void		setColor();
	void 		setKGL(ncc::m4x4d KGL__);
	void		setFOV(double fov_);
	void		setZnear(double znear_);
	void		setZfar(double zfar_);
	void		setMouseSens(double msens_);
    void		setRoll(double r);
	void		setPitch(double p);
	void		setYaw(double y);
	void		setX(double x);
	void		setY(double y);
	void		setZ(double z);
	void		setColor(ncc::v4ub col);
	void		setSize(ncc::v2i size);
    void		setMV(ncc::RTd mat);

public:
    ncc::v4ub	getColor();
    double		getRoll();
	double		getPitch();
	double		getYaw();
	double		getFOV();
	double		getAspectRatio();
	double		getZnear();
	double		getZfar();
	double		getMouseSens();
	ncc::v2i	getSize();
	QPoint		getMousePos();
    ncc::v3d	getRPY();
    ncc::v3d	getPos();

    void		setMV(ncc::v3d rpy , ncc::v3d pos);

	ncc::RTd	MV();
	ncc::m4x4d	KGL();

private:

	void		strafe(double x, double y, double z);
	void		rotate(ncc::v4i in);
	void		setRPY(ncc::v3d rpy_);
	void		setPos(ncc::v3d pos_);

	void		setupSignals();
    ncc::RTd	compose(ncc::v3d rpy , ncc::v3d pos);
    void		decompose(ncc::RTd rot, ncc::v3d & rpy , ncc::v3d & pos);



	ncQtSignalHandler		values_changed;

	ncQtDoubleSliderCombo*  camFOV;
	//ncQtDoubleSliderCombo*  camAR;
	ncQtDoubleSliderCombo*  camZnear;
	ncQtDoubleSliderCombo*  camZfar;
	ncQtDoubleSliderCombo*  Msens;

	QCheckBox*				roll_checkbox;
	ncQtDoubleSpinBox*		roll_spinbox;
	QCheckBox*				pitch_checkbox;
	ncQtDoubleSpinBox*		pitch_spinbox;
	QCheckBox*				yaw_checkbox;
    ncQtDoubleSpinBox*		yaw_spinbox;;

	QLabel*					x_label;
	ncQtDoubleSpinBox*		x_spinbox;
	QLabel*					y_label;
	ncQtDoubleSpinBox*		y_spinbox;
	QLabel*					z_label;
	ncQtDoubleSpinBox*		z_spinbox;;

	QPoint					mousePos;

	bool					lmb;
	bool					mmb;
	bool					rmb;

	int						width;
	int						height;

    QPushButton*			bg_select;
	ncc::v4ub				bgcolor;

	ncSkyDome*              skydome;
	int                     bgmode;

	QRadioButton*           bgcolor_radio;
	QRadioButton*           skydome_radio;
};
#endif
