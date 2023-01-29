#ifndef NCQTCONFIG3D_H
#define NCQTCONFIG3D_H

#include "ncqtconfig.h"
#include "ncqtdock.h"
#include "ncqtdoublespinbox.h"
#include "ncqtmultibar.h"

class ncQtConfig3D : public ncQtConfig {

    Q_OBJECT

public:
    ncQtConfig3D(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas);
    ~ncQtConfig3D();

    void initProgress();
    void setProgress(int cid, int vid);
    void exitProgress();
    virtual bool load(std::string fname);

public slots:
    void setImRange(int imrange);
    void setWinLod(int winlod);
    void setImLod(int imlod);
    void setWinD(int wind);
    void setLinP(double linp);
    void setGauss(double gauss);
    void setMaxCarve(int max_carve);
    void setMinLevel(int min_level);
    void setUpscale(int upscale);

public:
    QPushButton*        flow_button;
    QPushButton*        carve_button;
    QPushButton*        recarve_button;
    QPushButton*        remesh_button;

    ncQtMultiBar*       progress_cam;
    ncQtMultiBar*       progress_pose;
    ncQtMultiBar*       vox_mem;

private:
    //! flow
    //QTabWidget*             flow_tab;
    //std::vector<QWidget* >  flow_pass;

    QGroupBox*          flow_group;
    QGridLayout*        flow_grid;

    QLabel*             imrange_label;
    QSpinBox*           imrange_spinbox;

    QLabel*             winlod_label;
    QSpinBox*           winlod_spinbox;

    QLabel*             imlod_label;
    QSpinBox*           imlod_spinbox;

    QLabel*             wind_label;
    QSpinBox*           wind_spinbox;

    QLabel*             linp_label;
    QDoubleSpinBox*     linp_spinbox;

    QLabel*             gauss_label;
    QDoubleSpinBox*     gauss_spinbox;

    //! progress
    QGridLayout*        progress_grid;

    //! vox
    QGroupBox*          vox_group;
    QGridLayout*        vox_grid;

    QLabel*             max_carve_label;
    QSpinBox*           max_carve_spinbox;

    QLabel*             min_level_label;
    QSpinBox*           min_level_spinbox;

    QLabel*             upscale_label;
    QSpinBox*           upscale_spinbox;

    QLabel*             bbox_label;
    QComboBox*          bbox_combobox;

};

#endif // NCQTCONFIG3D_H
