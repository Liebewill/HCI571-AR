/*
 *  ARToolKitMatrix.h
 *  ArtisZee_App
 *
 *  Created by Dr.-Ing. Rafael Radkowski on 19.02.11.
 *  Copyright 2011 Heinz Nixdorf Institute. All rights reserved.
 *
 */
#pragma once

#include <osg/MatrixTransform>
#include <osg/NodeCallback>
#include <osg/Switch>

#include "ARToolkitTypes.h"

class ARToolKitNode : public osg::MatrixTransform
{
private:

    //-----------------------------------------------------------
    // A Ptr that refers to  tracking data
    ObjectInfo*         _trackingData;
    
    //-----------------------------------------------------------
    // The matrix and the confidence value.
    // We get both from ARToolkti
	osg::Matrixd*       _matrix;
	double*             _cf;
    
    //-----------------------------------------------------------
    // The switch of this object
	osg::Switch*        _switch;
	
    
    
	//////////////////////////////////////////////////////////////
	// Update Callback
	class ARToolKitNodeCB : public osg::NodeCallback
	{
	private:
		
		//----------------------------------------------------------
		// Die Einstellmatrix zum Updaten
		ARToolKitNode*	_parent;

		int             _currentframe;

		int				_renderframe;
        
	public:
		/**!
		 Konstruktor
		 */
		ARToolKitNodeCB( ARToolKitNode*	parent)
		{
            _currentframe = -1;
			_parent = parent;
			_renderframe = 0;
            
            // Switch off all children
            this->_parent->_switch->setAllChildrenOff();
		}
		
		/*
		 Der Update Callback des Modell. Hier erfolgt die Simulation des Verhaltens
		 bzw. der Update der Grafik
		 */
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
		{
       
            if((*this->_parent->_cf) > 0)
            {
                // Copy the matrix
                this->_parent->setMatrix(*this->_parent->_matrix);
			
                // Activate all children
                this->_parent->_switch->setAllChildrenOn();

            }
			else
			{
                // Switch off all children
                this->_parent->_switch->setAllChildrenOff();
		
			}
            nv->traverse(*node);
		}
	};
	
	//----------------------------------------------------------
	// Callback
	ARToolKitNodeCB*	_cb;

public:
    
    /*
     Constructor: 
     @param trackingData - A ptr to the trackingData of a particular pattern, 
            that should drive this object
     */
	ARToolKitNode(ObjectInfo* trackingData);
	
    
    /*
     This functions override the "standard" addChild method
     in order to ensure that the added object will be attached
     to the switch-Node of the class and not to the MatrixTransform-Node. 
     */
    bool addChild(osg::Node* node);
    bool addChild(osg::Group* node);
    bool addChild(osg::Switch* node);
    bool addChild(osg::MatrixTransform* node);

	   /*!
     Removes a Node from the Scenegraph
     */
    bool removeChild(osg::Node* node);
    bool removeChild(osg::Group* node);
    bool removeChild(osg::Switch* node);
    bool removeChild(osg::MatrixTransform* node);

    /*!
     Returns the number of Children on this patt
     */
    int getNumChildrenOnPatt(void);
};