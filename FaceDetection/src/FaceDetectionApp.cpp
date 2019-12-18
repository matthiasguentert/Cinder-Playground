#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "CinderOpenCV.h"
#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

class FaceDetectionApp : public App {
	
public:
	void setup() override;
	void draw() override;
	std::vector<ci::Rectf> fromOcv(const std::vector<cv::Rect> & cvRects);
	
private:
	cv::CascadeClassifier mFaceClassifier;
	Surface mImage;
	std::vector<Rectf> mFaces;
};

void FaceDetectionApp::setup()
{
	mImage = loadImage(loadAsset("Lenna.png"));

	setWindowSize(mImage.getWidth(), mImage.getHeight());

	static const char * classifer_name = "haarcascade_frontalface_alt.xml";

	mFaceClassifier.load(getAssetPath(classifer_name).string());

	cv::Mat cvImage (toOcv (mImage, CV_8UC1));

	std::vector<cv::Rect> cvFaces;
	mFaceClassifier.detectMultiScale(cvImage, cvFaces);

	mFaces = fromOcv(cvFaces);
}

void FaceDetectionApp::draw()
{
	//gl::clear(Color(1.0, 1.0, 1.0));
	gl::draw(gl::Texture::create(mImage));

	//gl::color(Color(1.0f, 1.0f, 0.0));
	for (const auto& face : mFaces)
	{
		gl::drawStrokedRect(face);
	}
}

std::vector<ci::Rectf> FaceDetectionApp::fromOcv(const std::vector<cv::Rect> & cvRects)
{
	std::vector<ci::Rectf> rects;

	for (const auto& cvRect : cvRects)
	{
		ci::Rectf rect(ci::fromOcv(cvRect));

		rects.push_back(rect);
	}

	return rects;
}

CINDER_APP(FaceDetectionApp, RendererGl)