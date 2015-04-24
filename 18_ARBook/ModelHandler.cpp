//
//  ModelHandler.cpp
//  HCI571X-ARPaddle
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/3/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include "ModelHandler.h"

extern int IntersectTraversalMask;

typedef std::map<std::string, MoveableObject*> ObjectList;
typedef std::map<std::string, ARTagNode*> TrackingPatternList;


int g_copy_id = 0;

ObjectList g_ListOfObjects;
TrackingPatternList g_trackingpattern;

int g_world_pattern = -1;
int g_paddle_pattern = -1;

// vectors for all detected markers
extern std::vector<Marker> detectedMarkers;


/*
 Returns the model reference
 */
//static
MoveableObject* ModelHandler::getModel(std::string model_identifier)
{
    if(model_identifier.length() == 0) return NULL;

    return g_ListOfObjects[model_identifier];
}

/*
 Adds a model to the static model list;
 */
//static
bool ModelHandler::addModel(std::string model_identifier, MoveableObject* model, ARTagNode* pattern)
{
    if(model_identifier.length() == 0) return false;
    
    MoveableObject* obj = getModel(model_identifier);
    
    if(obj == NULL)
    {
        if(model != NULL && pattern != NULL)
        {
            g_ListOfObjects[model_identifier] = model;
            g_trackingpattern[ model_identifier] = pattern;
            return true;
        }
    }
    return false;
    
}


//static
bool ModelHandler::setObjectToWorld(std::string model_identifier, osg::Vec3 location)
{
    if(model_identifier.length() == 0) return false;
    
    MoveableObject* obj = getModel(model_identifier);
    
    if(obj != NULL) // Object exists
    {
        obj->setEnableIntersection(true);
        obj->setMatrix(osg::Matrix::translate(location));
		g_trackingpattern[model_identifier]->setMarkerID(g_world_pattern);
        
        return true;
    }
    return false;
}
 

/*
bool ModelHandler::setObjectToWorld(std::string model_identifier, osg::Vec3 location)
{
    if(model_identifier.length() == 0) return false;
    
    // Locate the object
    MoveableObject* obj = getModel(model_identifier);
    
    if(obj != NULL) // Object exists
    {
        // This enables the ray intersection test
        obj->setEnableIntersection(true);
        
        // Set the object to the new Paddle location
        obj->setMatrix(osg::Matrix::translate(location));
        
        // Assign the world reference coordinate system pattern to the ARToolkit node
        g_trackingpattern[model_identifier]->setTrackingData(ARToolkitOSG::getObjectForPatt(1));
        
        // remove the object from the paddle node
        osg::Group* p = obj->getParent(0);
        if(p != NULL)
        {
            // Activate thread safe ref counting
            obj->setThreadSafeRefUnref(true);
            obj->ref(); // Prevent object from getting deleted. 
            p->removeChild(obj);
            
            // colider group node; the node is identified by name
            _colider_group->addChild(obj);
            return true;
        }
    }
    return false;
}

*/


//static
bool ModelHandler::setObjectToPaddle(std::string model_identifier)
{
    if(model_identifier.length() == 0) return false;
    
    MoveableObject* obj = getModel(model_identifier);
    
    if(obj != NULL)
    {
        obj->setEnableIntersection(false);
        obj->setMatrix(osg::Matrix::translate(0.0, 0.0, 0.0));
		g_trackingpattern[model_identifier]->setMarkerID(g_paddle_pattern);
        
        return true;
    }

}

//static
std::string ModelHandler::copyObjectToPaddle(std::string model_identifier)
{
    // get the object from the object map.
    MoveableObject* obj = getModel(model_identifier);
    
    if(obj != NULL)
    {
        MoveableObject* mynode = new MoveableObject(obj->getFilename(), obj->getIntersectionGroupPtr());
        mynode->setEnableIntersection(false);
        
        std::string label = model_identifier;
        std::strstream conv;
        std::string out;
        conv << g_copy_id++;
        conv >> out;
        label.append("_");
        label.append(out);
        
        osg::Group* localNode = new osg::Group();
        
		ARTagNode* artNode = new ARTagNode(g_paddle_pattern, detectedMarkers, osg::Matrix::identity());
        mynode->setMatrix(osg::Matrix::translate(0, 0, 0));
        mynode->setName(label);
            
        localNode->addChild(artNode);
        artNode->addChild(mynode);
            
        mynode->setNodeMask(IntersectTraversalMask);
        artNode->setNodeMask(IntersectTraversalMask);
            
            
        int i = mynode->getNumChildren();
        if(i != 0)
        {
            for(int j=0; j<i; j++)
            {
                   mynode->getChild(j)->setName(label);
            }
        }
            
        ModelHandler::addModel(label, mynode, artNode);
   
        osg::Group* node = obj;
        while(node->getNumParents() > 0)
        {
            node = node->getParent(0);
            if(node->getName().compare("ARBook") == 0)
            {
                node = node->getParent(0);
                break;
            }
        }
        for(int i=0; i<node->getNumChildren(); i++)
        {
            osg::Group* c = (osg::Group*)node->getChild(i);
            if(c->getName().compare("ColliderGroup") == 0)
            {
                c->addChild(localNode);
            }
        }
        
        
        return label;
    }
    
    return "";
}


//static
bool ModelHandler::selectObject(std::string model_identifier)
{
    MoveableObject* obj = getModel(model_identifier);
    
    if(obj != NULL)
    {
        obj->selectObject(true);
		return true;
    }
	return false;
}


bool ModelHandler::moveObject(std::string model_identifier, osg::Vec3 direction, double distance)
{
    if(model_identifier.length() == 0) return false;
    
    MoveableObject* obj = getModel(model_identifier);
    
    if(obj != NULL)
    {
        direction.normalize();
        osg::Matrixd location = obj->getMatrix();
        osg::Matrixd newLocation = location * osg::Matrix::translate(direction.x() * distance, direction.y() * distance, direction.z() * distance  );
        
        obj->setMatrix(newLocation);
        
        return true;
    }
    
    return false;

}

/*!
Set the id of the world reference marker
*/
//static 
void ModelHandler::setWorldRefMarkerPtr(int pattern)
{
	g_world_pattern = pattern;
}

	/*!
Set the id of the paddle marker
*/
//static 
void ModelHandler::setPaddleRefMarkerPtr(int pattern)
{
	g_paddle_pattern = pattern;
}