#pragma once
#pragma comment(lib, "opencv_world310d.lib")
#include "stdafx.h"
//#include <opencv2/core.hpp>
//#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
//#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

enum Pattern { CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
using namespace cv;
using namespace std;
class cv_cali
{
public:
	struct cv_stereo_pair_res {
		Mat R, T, E, F;
		Mat cameraMatrix[2], distCoeffs[2];
		double rms;
		double err = 0;
		int npoints = 0;
		Mat R1, R2, P1, P2, Q;
	};
	struct cv_stereo_pair_res *stereo_res;
public:
	cv_cali();
	virtual ~cv_cali();
	void loadCameraParams(CString FilePath, Mat &cameraMatrix, Mat &distCoeffs);
	double computeReprojectionErrors(
		const vector<vector<Point3f> >& objectPoints,
		const vector<vector<Point2f> >& imagePoints,
		const vector<Mat>& rvecs, const vector<Mat>& tvecs,
		const Mat& cameraMatrix, const Mat& distCoeffs,
		vector<float>& perViewErrors);
	bool do_cali(vector<vector<Point2f> > imagePoints,
		Size imageSize, Size boardSize, Pattern patternType,
		float squareSize, float aspectRatio,
		int flags, Mat& cameraMatrix, Mat& distCoeffs,
		vector<Mat>& rvecs, vector<Mat>& tvecs,
		vector<float>& reprojErrs,
		double& totalAvgErr);
	void calcChessboardCorners(Size boardSize, float squareSize, vector<Point3f>& corners, Pattern patternType);
	void saveCameraParams(CString filename,
		Size imageSize, Size boardSize,
		float squareSize, float aspectRatio, //int flags,
		const Mat& cameraMatrix, const Mat& distCoeffs,
		const vector<Mat>& rvecs, const vector<Mat>& tvecs,
		const vector<float>& reprojErrs,
		const vector<vector<Point2f> >& imagePoints,
		double totalAvgErr);
	bool runAndSave(CString outputFilename,
		const vector<vector<Point2f> >& imagePoints,
		Size imageSize, Size boardSize, Pattern patternType, float squareSize,
		float aspectRatio, int flags, Mat& cameraMatrix,
		Mat& distCoeffs, bool writeExtrinsics, bool writePoints);
	void StereoCalib(const vector<string>& imagelist, Size boardSize, bool displayCorners = false, bool useCalibrated = true, bool showRectified = true);
	vector<Point3f> conv_cornors3f(vector<CPoint> points);
	vector<Point2f> conv_cornors2f(vector<CPoint> points);
	Size boardSize, imageSize;
	float squareSize=1.0f, aspectRatio=1.0f;
	Mat cameraMatrix, distCoeffs;
	vector<vector<Point2f> > imagePoints;
	int flags = 0;
	void read_board_size_from_conf(string conf_path, Size &bo_size);
	void save_stereo_conf(string conf_path, struct cv_stereo_pair_res *stereo_res);
};
//only one calibration class
extern cv_cali *cali;