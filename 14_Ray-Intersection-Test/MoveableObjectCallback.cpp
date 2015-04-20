//
//  MoveableObjectCallback.cpp
//  HCI571X-CollisionDetectionBasic
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/1/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include "MoveableObjectCallback.h"

extern int IntersectTraversalMask;



MoveableObjectCallback::MoveableObjectCallback(osg::Group* intersection_group, osg::MatrixTransform* moveable_object):
    _intersection_group(intersection_group), _moveable_object(moveable_object)
{
    _current_Position = _moveable_object->getMatrix().getTrans();
    _new_Position = osg::Vec3(0.0, 0.0, 0.0);
    _final_Position = osg::Vec3(0.0, 0.0, 0.0);
    _final_Up = osg::Vec3(0.0, 0.0, 1.0);
    
    _moveable_object_matrix.identity();
    
    _enabled = true;
    
    _boundingSphere = moveable_object->getBound();

}



bool MoveableObjectCallback::intersect(const osg::Vec3d& start,
                                  const osg::Vec3d& end,
                                  osg::Vec3d& intersection,
                                  osg::Vec3d& normal) const
{
    // This creates a line segment intersector
    osg::ref_ptr<osgUtil::LineSegmentIntersector> lsi = new osgUtil::LineSegmentIntersector(start,end);
    
    // The traversal mask visitor. It defines which objects should be traversed
    osgUtil::IntersectionVisitor iv(lsi.get());
    iv.setTraversalMode(osgUtil::IntersectionVisitor::TRAVERSE_ALL_CHILDREN);
    iv.setTraversalMask(IntersectTraversalMask);

    // This starts the traversl / ray intersection test. 
    _intersection_group->accept(iv); 
    
    // this returns the first contact. 
    if (lsi->containsIntersections())
    {
        intersection = lsi->getIntersections().begin()->getWorldIntersectPoint();
        normal = lsi->getIntersections().begin()->getWorldIntersectNormal();
        return true;
    }
    return false;
}


void MoveableObjectCallback::operator()(osg::Node* node, osg::NodeVisitor* nv) 
{
    if(_enabled)
    {
        computeIntersections();
    }
       
    nv->traverse(*node);
}


/*
 This function calculates the next step
 */
bool MoveableObjectCallback::computeIntersections(void)
{
    _collision_down = false;
    
    // Fetch the current position of the 3D object
    _current_Position = _moveable_object->getMatrix().getTrans();

    //////////////////////////////////////////////////////////
    // At this time, the current position is also the new position.
    // We want to change the z-coordinate.
    // Later, after we introduce movement, the new position is the
    // location the user intends to have. 
    _new_Position = _current_Position;
    
    
	// Test the collision to the underground
    osg::Vec3d start = _new_Position + osg::Vec3(0.0, 0.0, -0.05);
	osg::Vec3d end = _new_Position + osg::Vec3(0.0, 0.0, -5000.0);
	
    osg::Vec3d ip, np;
	if(intersect(start, end, ip, np))
	{
		//std::cout << "intersect:" << ip.x() << " : " << ip.y() << " : " << ip.z()  << std::endl;
		
        _final_Position = _new_Position;
        
        // Update the position and the orientation
		_final_Position[2] =  ip.z();
        
        
        /////////////////////////////////////////////////////////////
        // This makes the fall slower
        osg::Vec3d trans = _moveable_object->getMatrix().getTrans();
        _final_Position[2] = trans.z() - 4.0;
		_final_Up = np;
        /////////////////////////////////////////////////////////////
    
        _collision_down = true;
	}

    if(_collision_down)
    {
        // set the translation; orientation is skipped at this movement
        _moveable_object_matrix = osg::Matrix::translate(_final_Position);
    
        // Pass matrix to object
        _moveable_object->setMatrix(_moveable_object_matrix);
    }
    return true;
    
}

/*
 Traverse the colider scene graph and check for bounding sphere intersections
 */
bool MoveableObjectCallback::computeCollisions(osg::Group* node)
{
    
    osg::BoundingSphere bs = node->getBound();
    
    // Check for collisions
    if(_boundingSphere.intersects(bs) == true)
    {
        
        std::cout << "Bounding Sphere Intersection found: " << node->getName()   << std::endl;
        
        return true;
    }

    // Countinue traversing
    int num = node->getNumChildren();
    for(int i=0; i< num; i++)
    {
        return computeCollisions((osg::Group*)node->getChild(i));
    }
    return false;
    
}

/*
 Enables or disables the callback function for intersection
 */
void MoveableObjectCallback::setEnableIntersection(bool value)
{
    this->_enabled = value;
}

