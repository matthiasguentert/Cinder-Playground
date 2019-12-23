#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include <cmath>

using namespace ci;
using namespace ci::app;
using namespace std;

class AnimationApp : public App {
public:
	void prepareSettings(Settings* settings);
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;
	
	void updateSinus();
	void updateClickableCircle();
	void updateRandomCircle();
	vec2 GenerateRandomVectorInsideWindow();
	
private:
	vec2 mPosition;
	vec2 mCurrentPosClickable, mTargetPosClickable;
	vec2 mCurrentPosRandom, mTargetPosRandom;
};

void AnimationApp::prepareSettings(Settings* settings)
{
	settings->setWindowSize(800, 600);
	settings->setFrameRate(60);
}

void AnimationApp::setup()
{
	Rand::randomize();
	
	// Starting position sinus circle
	mPosition = vec2(0.0f, getWindowHeight() / 2.0f);

	// Starting position clickable circle
	mCurrentPosClickable = vec2(getWindowWidth() / 2.0f, getWindowHeight() / 2.0f);
	mTargetPosClickable = mCurrentPosClickable;
}

void AnimationApp::mouseDown(MouseEvent event)
{
	mTargetPosClickable = event.getPos();
}

void AnimationApp::updateSinus()
{
	if (mPosition.x <= getWindowWidth())
	{
		mPosition.x++;
	}
	else
	{
		mPosition.x = 0.0f;
	}

	const float y0 = getWindowHeight() / 2.0f;	// vertical center
	const float a = getWindowHeight() / 3.0f;
	const float omega = 2.0f * M_PI / getWindowWidth();

	mPosition.y = y0 - a * sin(omega * (mPosition.x));
}

void AnimationApp::updateClickableCircle()
{
	vec2 deltaPos = mTargetPosClickable - mCurrentPosClickable;

	deltaPos *= 0.9f;
	mCurrentPosClickable = mTargetPosClickable - deltaPos;
}

void AnimationApp::updateRandomCircle()
{
	vec2 deltaPos = mTargetPosRandom - mCurrentPosRandom;

	deltaPos *= 0.97f;

	mCurrentPosRandom = mTargetPosRandom - deltaPos;

	if (distance(mCurrentPosRandom, mTargetPosRandom) < 1.0f)
	{
		mTargetPosRandom = GenerateRandomVectorInsideWindow();
	}
}


vec2 AnimationApp::GenerateRandomVectorInsideWindow()
{
	const float maxX = static_cast<float>(getWindowWidth());
	const float maxY = static_cast<float>(getWindowHeight());

	return vec2(Rand::randFloat(0.0f, maxX), Rand::randFloat(0.0f, maxY));
}

void AnimationApp::update()
{
	updateClickableCircle();
	updateSinus();
	updateRandomCircle();
}

void AnimationApp::draw()
{
	gl::clear(Color(0, 0, 0));

	gl::color(Color(1, 1, 0));
	gl::drawSolidCircle(mPosition, 10);

	gl::color(Color(1, 0, 1));
	gl::drawSolidCircle(mCurrentPosClickable, 10);

	gl::color(Color(0, 0, 1));
	gl::drawSolidCircle(mCurrentPosRandom, 10);
}

CINDER_APP(AnimationApp, RendererGl)
