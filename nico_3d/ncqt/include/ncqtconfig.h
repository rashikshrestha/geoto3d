#ifndef NCQTCONFIG_H
#define NCQTCONFIG_H

#include "ncconfig.h"
#include "ncqtdock.h"
#include "ncqtdoublespinbox.h"

class cam_cfg : public QRadioButton {

    Q_OBJECT

public:
    cam_cfg(int cam_index_,QWidget* parent);

signals:
    void edit_signal(int);
    void toggle_signal(int,bool);

public slots:
    void edit_slot();
    void toggle_slot(bool);

private:
    int    cam_index;
};

class ncQtConfig : public ncQtDock , public ncConfig {

    Q_OBJECT

public:
    ncQtConfig(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas);
    ~ncQtConfig();

    bool load(std::string fname);
    void save(std::string fname);

    virtual bool vload(std::string fname);

signals:
    void valuesChanged();
    void configLoaded(QString filename);
    void configCopied(QString filename);

public slots:
    void newConfig();
    void loadConfig();
    void editCamera(int);
    void toggleCamera(int,bool);
    void setZnear(double znear);
    void setZfar(double zfar);
    void setVignet(double vignet);
    void setGamma(double gamma);
    void setScale(double scale);
    void setWB();
    void setStartPose(int start_pose);
    void setEndPose(int end_pose);
    void setStepPose(double step_pose);
    void setCameraFile(QString cam_file);
    void setPoseFile(QString pose_file);
    void updateActiveCamIDs();
    void copyConfigFile();

private:
    void updateToolTip(int cam_id);

public:
    QPushButton*        clobber_button;
    QPushButton*        copy_button;
    QAction*            action_new;
    QAction*            action_load;

protected:

    //! project
    QGroupBox*          project_group;
    QGridLayout*        project_grid;

	QString             cam_browsepath;
	QLabel*             cam_label;
	QToolButton*        cam_file;

    cam_cfg*            cameras[MAX_CAM];
    QGridLayout*        cam_grid;

	QString             pose_browsepath;
	QLabel*             pose_label;
	QToolButton*        pose_file;

    QLabel*		        id_label;
	QSpinBox*		    id_spinbox;
    QLabel*		        znear_label;
	QDoubleSpinBox*		znear_spinbox;
    QLabel*             zfar_label;
    QDoubleSpinBox*     zfar_spinbox;
    QLabel*		        vignet_label;
	QDoubleSpinBox*		vignet_spinbox;
    QLabel*             gamma_label;
    QDoubleSpinBox*     gamma_spinbox;
    QLabel*             scale_label;
    QDoubleSpinBox*     scale_spinbox;
    QLabel*             wbr_label;
    QDoubleSpinBox*     wbr_spinbox;
    QLabel*             wbg_label;
    QDoubleSpinBox*     wbg_spinbox;
    QLabel*             wbb_label;
    QDoubleSpinBox*     wbb_spinbox;
    int                 current_cam;

    QLabel*             start_pose_label;
    QSpinBox*           start_pose_spinbox;

    QLabel*             end_pose_label;
    QSpinBox*           end_pose_spinbox;

    QLabel*             step_pose_label;
    QDoubleSpinBox*     step_pose_spinbox;

};

#endif // NCQTCONFIG_H
