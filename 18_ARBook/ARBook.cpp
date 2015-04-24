//
//  ARBook.cpp
//  HCI571X-ARPaddle
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/8/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include "ARBook.h"
#include "ModelHandler.h"

extern int IntersectTraversalMask;


// vectors for all detected markers
extern std::vector<Marker> detectedMarkers;


ARBook::ARBook(osg::Group* worldRefObject):
    _worldRefObject(worldRefObject)
{
    
    this->setName("ARBook");
}

void ARBook::init(void)
{

    
}

osg::Group* ARBook::addARBookPattern(int pattID, std::string filename, std::string label, double x, double y, double z)
{
    osg::Group* localNode = new osg::Group();
    
    MoveableObject* mynode = new MoveableObject(filename, _worldRefObject );
    
    if(mynode != NULL)
    {
		ARTagNode* artNode = new ARTagNode(pattID, detectedMarkers, osg::Matrix::identity());
        mynode->setMatrix(osg::Matrix::translate(x, y, z));
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
    }
    
    addChild(localNode);
    
    return localNode;
    
}


