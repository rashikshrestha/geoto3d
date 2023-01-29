#ifndef NCQT_SCENIX_H
#define NCQT_SCENIX_H

#include "ncqtglwidget.h"
#include "ncqtdock.h"
#include "ncqtdoubleslidercombo.h"
#include "ncqtsignalhandler.h"

#ifndef WIN32
#include "ncvoxel.h"
#include "ncgvm.h"
#endif

#include "ply.h"

#include "ncmodel.h"

class ncQtScenix : public ncQtDock
{
    Q_OBJECT

signals:
    void valuesChanged();
    void boundingSphereChanged(ncc::v3d center , double radius);

public slots:
    void onItemChanged( QTreeWidgetItem * item, int column );

    void draw(ncc::m4x4d KGL, ncc::RTd MV , ncc::v4i VP);
    void addModels();
    void removeModels();
    void showModels();
    void hideModels();
    void setPointSize(double ps);
  	int  getNrModels();
  	void sendValuesChanged();
    void resetCameraToVisible();
    void resetCameraToSelected();

public:
    ncQtScenix(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas );
    ~ncQtScenix();
    void initialize();
    static ncQtScenix* instance;

private:
    ncQtDoubleSliderCombo*      slider;
    QTreeWidget*                models;
    QPushButton*                add_models;
    QPushButton*                remove_models;
    QPushButton*                show_models;
    QPushButton*                hide_models;

    QPushButton*                camera_visible;
    QPushButton*                camera_selected;

    QString                     browsepath;

    float                       pointsize;
    bool                        pointsmooth;

    ncQtSignalHandler           values_changed;

};

enum { Mmodel = Qt::UserRole+0 };
enum { Mvisible = 0 , Mfilename };

template <class T> class VPtr {
public:
    static T* asPtr(QVariant v) {
        return  (T *) v.value<void *>();
    }

    static QVariant asQVariant(T* ptr) {
        return qVariantFromValue((void *) ptr);
    }
};

class ncModelItem : public QTreeWidgetItem {

public:
    ncModelItem(ncModel* model_, QString filename_) : QTreeWidgetItem()
    {
        setText(Mfilename,filename_);
		setToolTip(Mfilename,filename_ );
        setData(0,Mmodel      ,VPtr<ncModel>::asQVariant(model_));
        setCheckState(Mvisible,Qt::Checked);
    }

    ~ncModelItem() {
        ncModel* tmp = model();
        delete tmp;
        //GroupWriteLock(ncQtScenix::instance->scene_root)->removeChild(SwitchWeakPtr(tmp));
	};

	ncModel* model() {
        return VPtr<ncModel>::asPtr(data(0,Mmodel));
	}
};

#endif // NCQT_SCENIX_H
