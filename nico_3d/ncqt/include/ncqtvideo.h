#ifndef __NCQTVIDEO_H__
#define __NCQTVIDEO_H__

#include "ncqt.h"
#include "ncqtdock.h"

#include "ncposelist.h"
#include "nccalibrationset.h"
#include "ncmesh.h"

class ncQtVideo : public ncQtDock {

    Q_OBJECT

public:
    ncQtVideo(QString title, ncQtMainWindow* parent , Qt::DockWidgetArea init_area , Qt::DockWidgetAreas allowed_areas );
    ~ncQtVideo();

	void drawPoses(GLenum mode);
	void drawCameras(GLenum mode);

signals:
    void cameraChanged(ncc::v4i VP,ncc::m4x4d KGL);
    void poseChanged(ncc::RTd);
    void startRecord();
    void stopRecord();

public slots:
	void	play();
	void    pause();
	void    togglePlay();
	void	reset();
	void    start();
	void    end();
    void    record();

	void	loadPoseFile();
	void	loadCamFile();
	void    showCamFile();
	void    showSlider();

    void    changePose(int index);
    void    changeCamera(int index);

    void    next();

public:
    bool	loadPoseFile(std::string fname);
    bool	loadCamFile(std::string fname);
    bool    isPlaying();

public:
    static const unsigned int string_cap_length;

  	QAction*            menu_pose_load;
  	QAction*            menu_cam_load;

  	QCheckBox*          static_poses;

  	QSlider*            begin_slider;
  	QSlider*            end_slider;

	QSlider*		    slider_;

	QToolBar*           toolbar;
	QAction*	        play_;
	QAction*	        reset_;
	QAction*	        begin_;
	QAction*	        end_;
	QAction*	        record_;



	QTimer*			    timer_;

	QString             pose_browsepath;
	QLabel*             pose_file_label;
	QPushButton*        pose_file_name;
	QLabel*             pose_label;
	QSpinBox*           pose_spinbox;
	ncPoseList*         poses;

	QString             cam_browsepath;
	QLabel*             cam_file_label;
	QPushButton*        cam_file_name;
	QLabel*             cam_label;
	QSpinBox*           cam_spinbox;
	ncCalibrationSet*   calset;

	ncMesh*				poses_mesh;
	ncPBO*				poses_vert;

	ncMesh*				calset_mesh;
	ncPBO*				calset_vert;
	ncIndexArray*		calset_index;
};

#endif
