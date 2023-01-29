#include "ncqtconfigmanager.h"

#include "nccommon.h"


ncConfigTree::ncConfigTree( QWidget * parent /*= 0*/ ) : QTreeWidget(parent) {
 setAcceptDrops(true);
}

ncConfigTree::~ncConfigTree() {
}

void ncConfigTree::dropEvent ( QDropEvent * event ) {

	QList<QUrl>  urls = event->mimeData()->urls();
	for (int i=0;i<urls.size();++i)
		ncQtConfigManager::instance->addConfig(urls[i].toLocalFile());

}

void ncConfigTree::dragEnterEvent ( QDragEnterEvent * event ) {
    event->acceptProposedAction();
}

void ncConfigTree::dragMoveEvent ( QDragMoveEvent * event ) {
    event->acceptProposedAction();
}



ncQtConfigManager::ncQtConfigManager(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas ) : ncQtDock(title, parent,init_area,allowed_areas) {
	//fprintf(stderr,"ncQtConfigManager::ncQtConfigManager\n");

    instance = this;

     //!GUI

    //! model config

    QHBoxLayout* hbox 	= new QHBoxLayout();
    add_configs      	= new QPushButton(QIcon(":/ncqtconfigmanager/add.png"	),"",this);
    remove_configs   	= new QPushButton(QIcon(":/ncqtconfigmanager/remove.png"),"",this);

    hbox->addWidget(add_configs		);
    hbox->addWidget(remove_configs	);

    grid->addLayout(hbox  ,0,0,1,4);

    configs = new ncConfigTree(this);
    configs->setAlternatingRowColors(true);
    //configs->setStyleSheet("QTreeView { selection-color: blue; selection-background-color: rgb(200, 200, 255); alternate-background-color: rgb(225, 225, 225); background: rgb(255, 255, 255); }");
    configs->setSelectionMode(QAbstractItemView::SingleSelection);
	//configs->setSortingEnabled(true);

	configs->setMinimumSize(256,256);

    configs->setColumnCount(1);
    QStringList headerlabels;
    headerlabels << "Filename";
    configs->setHeaderLabels(headerlabels);

    grid->addWidget(configs ,1,0,1,4);

    save_batch      	= new QPushButton("Save Batch",this);
    save_batch->setEnabled(false);
    grid->addWidget(save_batch ,2,0,1,2);

    batch_process      	= new QPushButton("Process Batch",this);
    batch_process->setEnabled(false);
    grid->addWidget(batch_process ,2,2,1,2);

    action_load_batch	= new QAction ("Load Batch...", this);
    action_load_batch->setIconVisibleInMenu(true);
    connect(action_load_batch   , SIGNAL(triggered())    , this , SLOT(loadBatch()));



    connect(configs         , SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)	), this , SLOT(onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)	));
    connect(add_configs     , SIGNAL(clicked()												), this , SLOT(addConfigs()												));
    connect(remove_configs  , SIGNAL(clicked()												), this , SLOT(removeConfigs()											));
    connect(batch_process   , SIGNAL(clicked()												), this , SLOT(onProcessBatch()											));
    connect(save_batch		, SIGNAL(clicked()												), this , SLOT(onSaveBatch()											));
    connect(this			, SIGNAL(configAdded(QString)									), this , SLOT(onNrConfigsChanged()										));
    connect(this			, SIGNAL(configRemoved(QString)									), this , SLOT(onNrConfigsChanged()										));

    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

}

ncQtConfigManager::~ncQtConfigManager() {
    //fprintf(stderr,"ncQtConfigManager::~ncQtConfigManager\n");
}

void ncQtConfigManager::initialize() {
    //fprintf(stderr,"+ncQtConfigManager::initialize()\n");

	Q_INIT_RESOURCE(icons);

    //fprintf(stderr,"-ncQtConfigManager::initialize()\n");
}

ncQtConfigManager* ncQtConfigManager::instance = 0;

void ncQtConfigManager::addConfigs() {

	//fprintf(stderr,"ncQtConfigManager::+addModels\n");

    QString mstring = "Configs (*.xml)";

    //! init file browser
	QList<QString> fileNames = QFileDialog::getOpenFileNames(0,"Select Config(s)",browsepath, mstring, 0,QFileDialog::ReadOnly);
	qApp->processEvents();

	for (int i=0;i<fileNames.count();++i)
        if (!fileNames[i].isEmpty())
			addConfig(fileNames[i]);
	//fprintf(stderr,"ncQtConfigManager::-addModels\n");
}

void ncQtConfigManager::addConfig(QString filename) {

	//fprintf(stderr,"ncQtConfigManager::+addModels\n");

	QStringList loaded_configs = getConfigs();

    QFileInfo fi = QFileInfo(filename);
    browsepath = fi.path();

    if (!loaded_configs.contains(filename))
	{
		ncConfigItem*   Mitem = new ncConfigItem(filename);
		configs->addTopLevelItem(Mitem);
		configs->setCurrentItem(Mitem);
		configAdded(Mitem->toolTip(Mfilename));
 	}
}

QStringList ncQtConfigManager::getConfigs() {

	QStringList file_list;
	for (int i = 0;i< configs->topLevelItemCount() ; ++i)
        file_list.push_back(configs->topLevelItem(i)->toolTip(Mfilename));
    return file_list;
}

void ncQtConfigManager::removeConfigs() {
	//fprintf(stderr,"+ncQtConfigManager::removeModels\n");

    QList<QTreeWidgetItem *> selection = configs->selectedItems();
    if (selection.count()>0)
    {
        for (int i = 0;i< selection.count() ; ++i)
        {
        	QString tmp = selection[i]->toolTip(Mfilename);
			delete (ncConfigItem*)configs->takeTopLevelItem(configs->indexOfTopLevelItem(selection[i]));
            configRemoved(tmp);
		}
    }

	//fprintf(stderr,"-ncQtConfigManager::removeModels\n");

}

void ncQtConfigManager::clearConfigs() {
	//fprintf(stderr,"+ncQtConfigManager::removeModels\n");

    while (configs->topLevelItemCount()>0)
    {
	    ncConfigItem* item = (ncConfigItem*)configs->takeTopLevelItem(0);
	    QString tmp = item->toolTip(Mfilename);
		delete item;
        configRemoved(tmp);
	}
	//fprintf(stderr,"-ncQtConfigManager::removeModels\n");
}

void ncQtConfigManager::onProcessBatch() {

	QStringList file_list;

	for (int i=0;i<configs->topLevelItemCount();++i)
		file_list.push_back(configs->topLevelItem(i)->toolTip(Mfilename));

    if (!file_list.empty())
    	processBatch(file_list);
}

void ncQtConfigManager::onSaveBatch() {

	QString filename = QFileDialog::getSaveFileName(this, "Save File", QDir::homePath (), tr("Batch Files (*.txt)"));
	if (!filename.isEmpty())
	{
		if(!filename.endsWith(".txt"))
  			filename.append(".txt");

		std::ofstream outfile(filename.toStdString().c_str());
		if (!outfile)
			return;

		for (int i=0;i<configs->topLevelItemCount();++i)
			outfile << configs->topLevelItem(i)->toolTip(Mfilename).toStdString() << std::endl;

		outfile.close();
	}
}

void ncQtConfigManager::onCurrentItemChanged(QTreeWidgetItem* cur,QTreeWidgetItem* prev) {
	if (cur)
		configSelected(cur->toolTip(Mfilename));
	else
		configSelected(QString());
}

int  ncQtConfigManager::getNrConfigs() {
	//fprintf(stderr,"ncQtConfigManager::getNrModels\n");
    return configs->topLevelItemCount();
}

void ncQtConfigManager::onNrConfigsChanged() {

	save_batch->setEnabled(getNrConfigs()>0);
	batch_process->setEnabled(getNrConfigs()>0);
}

void ncQtConfigManager::loadBatch() {

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Batch File"), QDir::homePath (), tr("Batch Files (*.txt)"));
	if (!fileName.isEmpty())
	{
		if (getNrConfigs()>0)
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, "Load Batch", "Config list will be cleared before loading batch file.\nContinue?", QMessageBox::Yes|QMessageBox::No);
			if (reply == QMessageBox::No)
				return;
		}

		clearConfigs();
		loadBatch(fileName);
	}
}

void ncQtConfigManager::loadBatch(QString filename) {

	std::ifstream infile(filename.toStdString().c_str());
    if (!infile)
     	return;

	char str[1024];

	infile >> std::ws;

    while(infile){
        infile.getline(str,1024);
		QString file = QString(str).simplified();
		if (!file.isEmpty())
		{
			if (QFile(file).exists())
				addConfig(file);
			else
			{
				QString file2 = QFileInfo(filename).absoluteDir().absolutePath() + "/" + file;
				if (QFile(file2).exists())
					addConfig(file2);
				else
				QMessageBox::information(0,"Info",QString("Could not find %1").arg(file));
			}
		}
		infile >> std::ws;
    }

    infile.close();
}
