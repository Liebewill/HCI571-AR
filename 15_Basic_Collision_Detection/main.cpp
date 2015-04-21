//
//  main.cpp
//  HCI571X-ObjectManagement
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/8/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include <iostream>
#include <osgViewer/Viewer>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgText/Text>



/*!
 Class GetWorldCoordOfNodeVisitor
 Traverses the node path and returns the coordinates of an object in global coordinate frame
 */
class GetWorldCoordOfNodeVisitor : public osg::NodeVisitor
{
public:
    // Constructor
    GetWorldCoordOfNodeVisitor():
        osg::NodeVisitor(NodeVisitor::TRAVERSE_PARENTS), done(false)
    {
        wcMatrix= new osg::Matrixd() ;
    }
    
    /*!
     Traverses the scene graph and calculates the node path after the 
     traversal is completed. 
     */
    virtual void apply(osg::Node &node)
    {
        if (!done)
        {
             if ( 0 == node.getNumParents() ) // no parents
             {
                wcMatrix->set(osg::computeLocalToWorld(this->getNodePath()) ) ;
                done = true ;
             }
             traverse(node) ;
        }
    }

    /*!
     Returns a matrix in world coordinate frame
     */
    osg::Matrixd* getMatrix()
    {
          return wcMatrix ;
    }
    
private:

    bool done ;

    osg::Matrix* wcMatrix ;

} ;


/*!
 Global functin that returns the world coordinates of a given node 
 @param node - node for which the world coordinates are requested
 */
osg::Matrixd* getWorldCoords( osg::Node* node)
{
    GetWorldCoordOfNodeVisitor* ncv = new GetWorldCoordOfNodeVisitor();

    if (node && ncv)
    {
       node->accept(*ncv) ;
       return ncv->getMatrix() ;
    }
    else
    {
       return NULL ;
    }
}




std::string textStr = "";
osg::ref_ptr<osgText::Text> text;

std::string textStr2 = "";
osg::ref_ptr<osgText::Text> text2;

std::string textStr3 = "";
osg::ref_ptr<osgText::Text> text3;

osg::Group* createText(void)
{
    osg::Group* localNode = new osg::Group();
    
    text = new osgText::Text();
    text->setText(textStr);
    text->setAxisAlignment(osgText::Text::SCREEN);
    text->setPosition(osg::Vec3(0.0, 0.0, 150.0));
    text->setCharacterSize(15.0);
    text->setFont("../data_art/arial.ttf");
    text->setColor(osg::Vec4(1.0, 1.0, 0.0, 0.8));
    
    
    
    osg::ref_ptr<osg::Group>    textGroup = new osg::Group();
    osg::ref_ptr<osg::Geode>    textGeode = new osg::Geode();
    textGeode->addDrawable(text);
    textGeode->addDrawable(text2);
    textGeode->addDrawable(text3);
    textGroup->addChild(textGeode);
	
    localNode->addChild(textGroup);
    
    return localNode;
    
}




int main(int argc, const char * argv[])
{
    
    osg::Group* first_node = new osg::Group();
    osg::Group* second_node = new osg::Group();
    
    first_node->ref();
    first_node->unref();
    
   // second_node->unref();
   // second_node->unref();
    
    int ref_count0 = first_node->referenceCount();
    int ref_count1 = second_node->referenceCount();
    
    osg::ref_ptr<osg::Group> root = new osg::Group();
    root->addChild(second_node);
    root->addChild(second_node);
    
    int ref_count2 = root->referenceCount();
    
    // Two transform nodes are created
    osg::ref_ptr<osg::MatrixTransform> trans0 = new osg::MatrixTransform();
    trans0->setMatrix(osg::Matrix::translate(0.0, 100.0, 0.0));
    
    osg::ref_ptr<osg::MatrixTransform> trans1 = new osg::MatrixTransform();
    trans1->setMatrix(osg::Matrix::translate(10.0, -100.0, 0.0));
    
    // One object is loaded
    osg::ref_ptr<osg::Group> model0 = (osg::Group*)osgDB::readNodeFile("../data_art/teapot.3ds" );
    osg::ref_ptr<osg::Group> model1 = (osg::Group*)osgDB::readNodeFile("../data_art/teapot.3ds" );
    
    root->addChild(trans0);
    root->addChild(trans1);
    trans0->addChild(model0);
    trans1->addChild(model1);
    
    
    
    root->addChild(createText());
    
    // The viewer
    osgViewer::Viewer viewer;
    viewer.setSceneData(root);
    viewer.setCameraManipulator(new osgGA::TrackballManipulator);
    viewer.setUpViewOnSingleScreen(0);
    
    

    
    
   
    
    while(!viewer.done())
    {
        // This example code calculates the global coordinates of model 1 and model 0 and
        // determines whether or not both objects intersect with each other.
        osg::Matrixd* wc0 = getWorldCoords( model0);
        osg::Matrixd* wc1 = getWorldCoords( model1);
        
        osg::BoundingSphere bs0 = trans0->getBound();
        osg::BoundingSphere bs1 = trans1->getBound();
        
        bs0.center() = wc0->getTrans();
        bs1.center() = wc1->getTrans();
        
        // This line checks whether or not both objects intersect
        if(bs0.intersects(bs1))
        {
            std::cout << "Found intersection "<< std::endl;
            textStr = "Found collision";
            text->setText(textStr);
        }
        else
        {
            textStr = "No collision";
            text->setText(textStr);
        }
        
#ifdef _WIN32        
        trans1->setMatrix(osg::Matrix::translate(0.0, 100.0 * sin(0.4*(double)clock()/CLOCKS_PER_SEC), 0.0));
#else
		trans1->setMatrix(osg::Matrix::translate(0.0, 100.0 * sin(40.0*(double)clock()/CLOCKS_PER_SEC), 0.0));
#endif
        viewer.frame();
    }

    return 0;
}

