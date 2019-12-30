#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <Box2D/Box2D.h>
#include "CinderOpenCV.h"
#include <algorithm>
#include <vector>
#include <windows.h>
#include <ObjIdl.h>
#include <minmax.h>
#include <gdiplus.h>

using namespace Gdiplus;
using namespace ci;
using namespace ci::app;

#pragma comment (lib, "Gdiplus.lib")

class GlyphRainApp : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

	b2PolygonShape create_shape(const WCHAR* letters);
	Path2d create_path(const WCHAR* letters);
	
private:
	b2World* world;
	std::vector<b2Body*> glyphs;
};

void GlyphRainApp::setup()
{
	// Create Box2d world
	b2Vec2 gravity(0.0f, 10.0f);
	world = new b2World(gravity);

	// Create ground definition
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, getWindowHeight());
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox(getWindowWidth(), 10.0f);

	// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 0.0f);
}

void GlyphRainApp::mouseDown(MouseEvent event)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(event.getX(), event.getY());

	b2Body* body = world->CreateBody(&bodyDef);

	b2PolygonShape shape = create_shape(L"A");

	b2FixtureDef fixture_def;
	fixture_def.shape = &shape;
	fixture_def.density = 1.0f;
	fixture_def.friction = 0.3f;
	fixture_def.restitution = 0.5f;

	body->CreateFixture(&fixture_def);
	glyphs.push_back(body);
}

void GlyphRainApp::update()
{
	float32 timeStep = 1 / 30.0f;    //the length of time passed to simulate (seconds)
	int32 velocityIterations = 10;   //how strongly to correct velocity
	int32 positionIterations = 10;   //how strongly to correct position

	for (int i = 0; i < 10; ++i)
	{
		world->Step(timeStep, velocityIterations, positionIterations);
	}
}

void GlyphRainApp::draw()
{
	gl::clear();

	// Draw solid ground
	gl::color(1, 1, 1);
	gl::drawSolidRect(Rectf(0, getWindowHeight() - 10.0f, getWindowWidth(), getWindowHeight()));

	gl::color(1, 0.5f, 0.25f);
	for (const auto& glyph : glyphs)
	{
		gl::pushModelMatrix();
		gl::translate(glyph->GetPosition().x, glyph->GetPosition().y);
		gl::rotate(glyph->GetAngle());
		gl::draw(create_path(L"A"));
		//gl::drawString("A", vec2(0), ColorA(1, 0.5f, 0.25f), ci::Font("Arial", 48));
		gl::popModelMatrix();
	}
}

b2PolygonShape GlyphRainApp::create_shape(const WCHAR* letters)
{
	b2PolygonShape shape;
	GraphicsPath graphics_path;
	FontFamily font_family(L"Arial");

	// Add string to path 
	if (graphics_path.AddString(letters, -1, &font_family, FontStyleRegular, 48, PointF(0.0f, 0.0f), nullptr) != Status::Ok)
	{
		std::cout << "Error while adding points" << std::endl;
	}
			
	// Retrieve points from path
	const auto pointCount = graphics_path.GetPointCount();
	const auto points = new PointF[pointCount];
	if (graphics_path.GetPathPoints(points, pointCount) != Status::Ok)
	{
		std::cout << "Error while getting points" << std::endl;
	}

	// Copy into vector
	std::vector<cv::Point2f> input;
	for (auto i = 0; i < pointCount; i++)
	{
		cv::Point2f point(points[i].X, points[i].Y);
		input.push_back(point);
	}

	// Create convex hull from points
	std::vector<cv::Point2f> hull;
	cv::convexHull(input, hull);

	// Created vertices that can be used to create shape
	auto const vertices = new b2Vec2[hull.size()];
	for (auto i = 0; i < hull.size(); i++)
	{
		vertices[i].Set(hull[i].x, hull[i].y);
	}

	shape.Set(vertices, hull.size());
	
	return shape;
}

Path2d GlyphRainApp::create_path(const WCHAR* letters)
{
	Path2d path;
	GraphicsPath graphics_path;
	FontFamily font_family(L"Arial");

	// Add string to path 
	if (graphics_path.AddString(letters, -1, &font_family, FontStyleRegular, 48, PointF(0.0f, 0.0f), nullptr) != Status::Ok)
	{
		std::cout << "Error while adding points" << std::endl;
	}
			
	// Retrieve points from path
	const auto pointCount = graphics_path.GetPointCount();
	const auto points = new PointF[pointCount];
	if (graphics_path.GetPathPoints(points, pointCount) != Status::Ok)
	{
		std::cout << "Error while getting points" << std::endl;
	}

	// Create OpenGL path
	path.moveTo(points[0].X, points[0].Y);
	for (auto i = 1; i < pointCount; i++)
	{
		path.lineTo(points[i].X, points[i].Y);
	}

	return path;
}

CINDER_APP(GlyphRainApp, RendererGl)
