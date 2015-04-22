//
//  ARTagNode.cpp
//  HCI571_MarkerTracking_OSG
//
//  Created by Rafael Radkowski on 3/26/15.
//
//

#include "ARTagNode.h"




ARTagNode::ARTagNode(int marker_id,  std::vector<Marker>& detectedMarkers, osg::Matrix offset)
{ 

    _transform = new osg::MatrixTransform();
    this->osg::Switch::addChild(_transform);
    this->setAllChildrenOn();
    
    
    // Create the callback
    _cb = new ARTagNodeCallback(_transform, marker_id, detectedMarkers, offset);
    
    this->setUpdateCallback(_cb);

}


ARTagNode::~ARTagNode()
{


}


bool ARTagNode::addChild(osg::Node* child )
{
    child->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
    return _transform->osg::MatrixTransform::addChild(child);
}

bool ARTagNode::addChild(osg::Group* child )
{
    child->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
    return _transform->osg::MatrixTransform::addChild(child);
}

bool ARTagNode::addChild(osg::Switch* child )
{
    child->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
    return _transform->osg::MatrixTransform::addChild(child);
}

bool ARTagNode::addChild(osg::MatrixTransform* child )
{
    child->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
    return  _transform->osg::MatrixTransform::addChild(child);
}




ARTagNode::ARTagNodeCallback::ARTagNodeCallback(osg::MatrixTransform* transform, int marker_id,  std::vector<Marker>& detectedMarkers, osg::Matrix& offset):
    _transform(transform), _marker_id(marker_id), _detectedMarkers(detectedMarkers), _offset(offset)
{
	_matrix = osg::Matrix::identity();
    _transform->setMatrix(offset * _matrix);
}



/*
 This is the update callback that copies the matrix information into the
 node matrix.
 */
//virtual
void ARTagNode::ARTagNodeCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{

	bool found_marker = false;

    for (int i = 0; i<_detectedMarkers.size(); i++) {
        
        if(_detectedMarkers[i].id == _marker_id)
        {
			osg::Matrix transform;
            static osg::Matrix scale = osg::Matrix::scale(10.0,10.0,10.0);
            
            Matrix44 glMatrix = _detectedMarkers[i].transformation.getInverted().getMat44();
            _matrix.identity();
            _matrix(0,0) =  glMatrix.mat[0][0];
            _matrix(0,1) =  glMatrix.mat[1][0];
            _matrix(0,2) =  glMatrix.mat[2][0];
            _matrix(1,0) =  glMatrix.mat[0][1];
            _matrix(1,1) =  glMatrix.mat[1][1];
            _matrix(1,2) =  glMatrix.mat[2][1];
            _matrix(2,0) =  glMatrix.mat[0][2];
            _matrix(2,1) =  glMatrix.mat[1][2];
            _matrix(2,2) =  glMatrix.mat[2][2];
            
            _matrix(3,0) =  1.0*glMatrix.mat[3][0];
            _matrix(3,1) =  1.0*glMatrix.mat[3][2];
            _matrix(3,2) =  -1.0*glMatrix.mat[3][1];
            
            _matrix(3,3) =  1.0;
        
  
			// copy the transformation matrix
            _transform->setMatrix( _offset * _matrix * scale  );
            found_marker = true;


			// switch on the 3D model 
			osg::Switch* s = reinterpret_cast<osg::Switch*>(node);
			if(s)s->setAllChildrenOn();
        }

        
    }
    if(!found_marker)
	{
		// if no marker has been found, switch off the 3d model 
		osg::Switch* s = reinterpret_cast<osg::Switch*>(node);
		if(s)s->setAllChildrenOff();
	}
    
    nv->traverse(*node);
}