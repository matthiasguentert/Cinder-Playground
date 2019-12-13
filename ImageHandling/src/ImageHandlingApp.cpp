#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImageHandlingApp : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;
private:
	gl::Texture2dRef texture;
};

void ImageHandlingApp::setup()
{
	DataSourceRef asset = loadAsset("Lenna.png");
	ImageSourceRef image = loadImage(asset);

	Surface surface = Surface(image);

	this->texture = gl::Texture::create(surface);
}

void ImageHandlingApp::mouseDown(MouseEvent event)
{
}

void ImageHandlingApp::update()
{
}

void ImageHandlingApp::draw()
{
	gl::clear(Color(0, 0, 0));

	gl::draw(this->texture, getWindowBounds());
}

CINDER_APP(ImageHandlingApp, RendererGl)
