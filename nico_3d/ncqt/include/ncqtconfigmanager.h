#ifndef NCQT_CONFIGMANAGER_H
#define NCQT_CONFIGMANAGER_H

#include "ncqtdock.h"
#include "ncqtsignalhandler.h"

enum { Mfilename = 0 };

class ncConfigItem : public QTreeWidgetItem {

public:
    ncConfigItem(QString filename_) : QTreeWidgetItem()
    {
        setText(Mfilename,QFileInfo(filename_).fileName());
		setToolTip(Mfilename,filename_ );
    }

    ~ncConfigItem() {
	}

};

class ncConfigTree : public QTreeWidget {

public:
    ncConfigTree( QWidget * parent = 0 );
    ~ncConfigTree();

	void dragEnterEvent ( QDragEnterEvent  * event );
	void dragMoveEvent ( QDragMoveEvent  * event );
	void dropEvent ( QDropEvent * event );

};


class ncQtConfigManager : public ncQtDock
{
    Q_OBJECT

signals:
    void configAdded(QString filename);
    void configRemoved(QString filename);
    void configSelected(QString filename);
    void processBatch(QStringList file_list);

public slots:
    void onNrConfigsChanged();
    void onCurrentItemChanged(QTreeWidgetItem* cur,QTreeWidgetItem* prev);
    void onProcessBatch();
    void onSaveBatch();
	void loadBatch();

    void addConfigs();
    void removeConfigs();
    void clearConfigs();
  	int  getNrConfigs();

public:
    ncQtConfigManager(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas );
    ~ncQtConfigManager();

    void 		initialize();
    QStringList getConfigs();
    void 		addConfig(QString filename);
	void 		loadBatch(QString filename);


    static ncQtConfigManager* instance;

    QAction*            		action_load_batch;


private:
    ncConfigTree*               configs;
    QPushButton*                add_configs;
    QPushButton*                remove_configs;
    QPushButton*                batch_process;
    QPushButton*                save_batch;

    QString                     browsepath;
};

#endif // NCQT_CONFIGMANAGER_H
