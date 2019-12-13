#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/params/Params.h"	// required for InterfaceGl

using namespace ci;
using namespace ci::app;
using namespace std;

class BrigthnessContrastApp : public App {

public:
	void setup() override;
	void draw() override;
	void update() override;
	void keyDown(KeyEvent event) override;

private:
	bool mShowOriginal;
	Surface32f mImageInput;
	Surface32f mImageOutput;
	float mBrightness;
	float mBrightnessPrev;
	float mContrast;
	float mContrastPrev;
	
	params::InterfaceGl mParams;

	float adjustBrigthnessContrast(float value)
	{
		// New Value = (Old Value - 0.5) * Contrast + 0.5 + Brightness
		return (value - 0.5f) * mContrast + 0.5f + mBrightness;
	}
};

static bool equal(float x, float y, float tolerance = 0.001f)
{
	if (fabs(x - y) < tolerance)
		return true;

	return false;
}

void BrigthnessContrastApp::keyDown(KeyEvent event)
{
	if (event.getChar() == KeyEvent::KEY_SPACE)
	{
		mShowOriginal = !mShowOriginal;
	}
}


void BrigthnessContrastApp::setup()
{
	mImageInput = loadImage(loadAsset("Lenna.png"));

	mImageOutput = Surface32f(mImageInput.getWidth(), mImageInput.getHeight(), false);

	mBrightness = 0.0f;
	mBrightnessPrev = -0.5f;
	mContrast = 1.0f;
	mContrastPrev = 0.0f;

	mShowOriginal = false;

	setWindowSize(mImageInput.getWidth(), mImageInput.getHeight());
	
	mParams = params::InterfaceGl("Adjustment Params", vec2(200, 80));

	mParams.addParam("Brightness", &mBrightness, "min=-0.5 max=0.5 step=0.01 keyDecr=d keyIncr=f");
	mParams.addParam("Contrast", &mContrast, "min=0.0 max=2.0 step=0.01 keyDecr=c keyIncr=v");
	
}

void BrigthnessContrastApp::update()
{
	if (equal(mBrightness, mBrightnessPrev) && equal(mContrast, mContrastPrev))
	{
		return;
	}

	Surface32f::Iter pixelInIter = mImageInput.getIter();
	Surface32f::Iter pixelOutIter = mImageOutput.getIter();

	while(pixelInIter.line())
	{
		pixelOutIter.line();

		while(pixelInIter.pixel())
		{
			pixelOutIter.pixel();

			pixelOutIter.r() = adjustBrigthnessContrast(pixelInIter.r());
			pixelOutIter.g() = adjustBrigthnessContrast(pixelInIter.g());
			pixelOutIter.b() = adjustBrigthnessContrast(pixelInIter.b());
		}
	}

	mBrightnessPrev = mBrightness;
	mContrastPrev = mContrast;
}


void BrigthnessContrastApp::draw()
{
	if (mShowOriginal)
	{
		gl::draw(gl::Texture::create(mImageInput), getWindowBounds());
	}
	else
	{
		gl::draw(gl::Texture::create(mImageOutput), getWindowBounds());

		mParams.draw();
	}
}

CINDER_APP(BrigthnessContrastApp, RendererGl)

