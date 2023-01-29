#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {

	QApplication a(argc, argv);
    Q_INIT_RESOURCE(geotoview);

	MainWindow mainw;

    QFile file(":/style/style.qss");
	file.open(QFile::ReadOnly);
	QString StyleSheet = QLatin1String(file.readAll());
	mainw.setStyleSheet(StyleSheet);

	mainw.show();

	return a.exec();
}
