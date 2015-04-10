// Tutorial_01.cpp : Defines the entry point for the console application.
//


///////////////////////////////////////////
// Step 1: Include the necessary OSG header files
// OpenSceneGraph Includes
#include <osg/Group>
#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/Matrix>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgDB/ReaderWriter>

int main(int argc, char* argv[])
{
	
    
    ///////////////////////////////////////////////////////
	// Step 2a: Create the scene graph by loading a model
	// Create a root node
	osg::Group* root = new osg::Group();

	// The additional root node scene is just a helper.
	osg::Group* scene = new osg::Group();

	// Add a transformation
	osg::MatrixTransform*	model_transform = new osg::MatrixTransform();
	
	// create a matrix that contains the transformation
	osg::Matrixf transformation;
	// and apply a transformation
	transformation.translate(10.0, 0.0, 0.0);

	// Pass the matrix to the model_transform node
	model_transform->setMatrix(transformation);

	// Load a model
	// The object type convert works only if the data types are inherited.
	osg::Group* loaded_model = (osg::Group*)osgDB::readNodeFile("jabba_scaled.3ds");

	// Assemble the scene graph
	root->addChild(scene);
	scene->addChild(model_transform);
	model_transform->addChild(loaded_model);


	///////////////////////////////////////////////////////
	// Step 2a: Create the scene graph by adding primitives
	// Create a sphere; this object creates only the geometry of a sphere.
	osg::Sphere* sphere = new osg::Sphere(osg::Vec3(0, 0, 0), 1.0f);

	// Create a drawable; a object that merge the object to be rendered. 
	// It contains all addition information like the color, transparancy etc. 
	osg::ShapeDrawable* sphere_drawable = new osg::ShapeDrawable(sphere);
	sphere_drawable->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));

	// The Geode is a leaf node. It can be attached to a scene graph.
	// It contains all object information about a model to render
	osg::Geode* sphere_geode = new osg::Geode();
	sphere_geode->addDrawable(sphere_drawable);

	// Create a transformation node;
	osg::MatrixTransform*	sphere_transform = new osg::MatrixTransform();

	// apply the transformation immediatelly
	sphere_transform->setMatrix(osg::Matrix::translate(0, 10, 0));

	// Assemble the scene graph
	scene->addChild(sphere_transform);
	sphere_transform->addChild(sphere_geode);

	///////////////////////////////////////////////////////
	// Step 3: Create a viewer
	osgViewer::Viewer*	viewer = new osgViewer::Viewer();
	viewer->setUpViewOnSingleScreen();

	// Pass the scene graph to th viewer
	viewer->setSceneData(root);

	///////////////////////////////////////////////////////
	// Step 4: Create a manipulator
	osgGA::TrackballManipulator* manipulator = new osgGA::TrackballManipulator();
	
	// and pass the manipulator to the viewer
	viewer->setCameraManipulator(manipulator);


	///////////////////////////////////////////////////////
	// Step 5: Fire
	viewer->run();


	return 0;
}

