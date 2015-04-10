// This code is part of HCI 571X - Augmented Reality
// and demonstrates the usage of a shader program with OpenSceneGraph
// The code loads a 3D teapot and changes the color using a shader program.

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
#include <osg/Uniform>


// A uniform variable that allows to pass a parameter to the shader program.
osg::Uniform* myUniform = NULL;


// IGNORE THIS CLASS RIGHT NOW. IT HELPS TO CHANGE SOME VALUES
//***************************************************************************
// This class handles keyboard events
// This event handler allows to use the arrow keys (left, right)
// to change a uniform variable which changes the color or the 3D model.
// 
class MyKeyboardEventHandler : public  osgGA::GUIEventHandler
{
private:
    
    float   m_value;
    
public:
    
	MyKeyboardEventHandler()
	{
        m_value = 0.0;
        
	};

	   
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
    {
        
        switch(ea.getEventType())
		{
			case(osgGA::GUIEventAdapter::KEYDOWN):
			{
				int key = ea.getKey();
   
				switch(key)
				{
                    case osgGA::GUIEventAdapter::KEY_Left:
                        if(myUniform != NULL)
                        {
                            m_value -= 5.0;
                            if (m_value < 0.0) m_value = 0.0;
                            myUniform->set(m_value);
                        }
                        break;
                    case osgGA::GUIEventAdapter::KEY_Right:
                        if(myUniform != NULL)
                        {
                            m_value += 5.0;
                            if (m_value > 100.0) m_value = 100.0;
                            myUniform->set(m_value);
                        }
                        break;
                        
                    
                }
                // If this event handler handles the incomimg message,
                // the return value should be "true" in order to stop the message.
                // If the return is false, the message will be passed to the next receiver
                // in an event handler daisy chain.
                return true;

            }
        }
        
        // If this event handler did not handle the incomimg message,
        // the return value must be "false" in order to pass the message
        // to the next receivers in an event handler daisy chaing.
        return false;
    }

};


//***************************************************************************
// The main function
int main(int argc, char* argv[])
{
	
    
    ///////////////////////////////////////////////////////
	// Step 2: Create the scene graph by loading a model
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
	osg::Group* loaded_model = (osg::Group*)osgDB::readNodeFile("../teapot.3ds");

    
    if(loaded_model == NULL)
    {
        std::cout << "Could not load the 3D model. Check your working directory!" << std::endl;
        system("pause");
        exit(1);
    }
    
	// Assemble the scene graph
	root->addChild(scene);
	scene->addChild(model_transform);
	model_transform->addChild(loaded_model);
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Step 3: This line loads the shader and applies it on
    // on the model node loaded_model


	//----------------------------------------
	// Creates a shader program object
	osg::Program* _shaderProgram = new osg::Program();
	// Add a name to that object
	_shaderProgram->setName(  "basic_shader" );

	//----------------------------------------
	// Creates a vertex and a fragment shader operation
	osg::Shader* _vertObj = new osg::Shader( osg::Shader::VERTEX );
	osg::Shader* _fragObj = new osg::Shader( osg::Shader::FRAGMENT );

	// These lines load the shader. The first make sure that the file is available
	std::string fqFileName = osgDB::findDataFile("../shader/basic.vs");
    if( fqFileName.length() != 0 ) _vertObj->loadShaderSourceFromFile( fqFileName.c_str() );
    else  osg::notify(osg::WARN) << "File " << "../shader/basic.vs" << "not found." << std::endl;
    

	fqFileName = osgDB::findDataFile("../shader/basic.fs");
    if( fqFileName.length() != 0 ) _fragObj->loadShaderSourceFromFile( fqFileName.c_str() );
    else  osg::notify(osg::WARN) << "File " << "../shader/basic.fs" << " not found." << std::endl;


	// Add this shader to the program
	_shaderProgram->addShader( _fragObj );
	_shaderProgram->addShader( _vertObj );


	//----------------------------------------
	// The stateset, that should be affected by this shader
	osg::StateSet*	_stateSet = loaded_model->getOrCreateStateSet();
	
	// Apply the shader program on a stateset
	_stateSet->setAttributeAndModes(_shaderProgram, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);


    ///////////////////////////////////////////////////////
	// Step 4: This adds a uniform variable with name colorvalue
    // to the shader program.
    
    // The first parameter is the name. The name in the shader program must be the same.
    // The second parameter is the start value. ATTENTION: the value's type must meet the parameter type
    // i.e., float -> 5.0f / int -> 5
    // GLSL has not automatic type conversion like C/C++ has.
    myUniform = new osg::Uniform("colorvalue",0.0f);
    loaded_model->getOrCreateStateSet()->addUniform(myUniform);
    myUniform->set(0.0f);
    

	///////////////////////////////////////////////////////
	// Step 5: Create a viewer
	osgViewer::Viewer*	viewer = new osgViewer::Viewer();
	viewer->setUpViewOnSingleScreen();
    // This line adds the keyboard event handler.
    viewer->addEventHandler(new MyKeyboardEventHandler());

	// Pass the scene graph to th viewer
	viewer->setSceneData(root);

	///////////////////////////////////////////////////////
	// Step 6: Create a manipulator
	osgGA::TrackballManipulator* manipulator = new osgGA::TrackballManipulator();
	
	// and pass the manipulator to the viewer
	viewer->setCameraManipulator(manipulator);


	///////////////////////////////////////////////////////
	// Step 7: Fire
	viewer->run();


	return 0;
}

