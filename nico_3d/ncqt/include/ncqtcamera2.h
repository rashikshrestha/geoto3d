#ifndef __NCQTCAMERA_H__
#define __NCQTCAMERA_H__

#include "ncqt.h"
#include "nccalibration.h"
#include "ncqtdock.h"
#include "ncqtdoubleslidercombo.h"
#include "ncqtsignalhandler.h"

#include <osg/Camera>
#include <osgGA/TrackballManipulator>



class ncQtCamera : public ncQtDock , public osg::Camera {

    Q_OBJECT

signals:
    void valuesChanged();

public slots:
    void    setFOV(double fov_);
    void    setZnear(double znear_);
    void    setZfar(double zfar_);
    void    setRoll(double roll_);
    void    setPitch(double pitch_);
    void    setYaw(double yaw_);
    void    setX(double x_);
    void    setY(double y_);
    void    setZ(double z_);
    void    setMV(ncc::RTd mat);
    void    updateSlot();

public:
    ncQtCamera(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas);
    ~ncQtCamera();

    ncc::RTd    MV();
    ncc::m4x4d  KGL();
    void        updateGui();

    osgGA::TrackballManipulator* manip;

private:

    void    compose(ncc::RTd & mat, ncc::v3d rpy , ncc::v3d pos);
    void    decompose(ncc::RTd mat, ncc::v3d & rpy , ncc::v3d & pos);

	ncQtDoubleSliderCombo*  camFOV;
	ncQtDoubleSliderCombo*  camZnear;
	ncQtDoubleSliderCombo*  camZfar;

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

    QPushButton*			bg_select;
	ncc::v4ub				bgcolor;

    ncQtSignalHandler	    values_changed;


};

#endif
