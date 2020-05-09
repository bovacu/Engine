#include "BoxCollider.h"

namespace engine {

    BoxColliderPtr BoxCollider::create(GameObject* _gameObject, const Vec2f& _center, const Size& _size, bool _isGhost) {
        return std::make_shared<BoxCollider>(_gameObject, _center, _size, _isGhost);
    }

    /// ----------------------------------------------------------------------------------------------------------------

    BoxCollider::BoxCollider(GameObject* _gameObject, const Vec2f& _center, const Size& _size, bool _isGhost) : 
    Collider(_gameObject, ComponentType::BOX_COLLIDER, _isGhost) {
        Collider::vertices.emplace_back(Vec2f(_center.x - _size.width / 2, _center.y - _size.height / 2));
        Collider::vertices.emplace_back(Vec2f(_center.x - _size.width / 2, _center.y + _size.height / 2));
        Collider::vertices.emplace_back(Vec2f(_center.x + _size.width / 2, _center.y + _size.height / 2));
        Collider::vertices.emplace_back(Vec2f(_center.x + _size.width / 2, _center.y - _size.height / 2));

        Collider::size              = _size;
        Collider::numberOfVertices  = 4;
        Collider::center            = _center; 
    }

    Mtv BoxCollider::isColliding(Collider& _collider, float _push) {
        Mtv _mtv;

        if(_collider.componentType == Collider::componentType)
            _mtv = this->collidingWithAABB(_collider, _push);
        else if (_collider.componentType == ComponentType::POLYGON_COLLIDER)
            _mtv = Collider::collidingWithSat(_collider, _push);
        else if (_collider.componentType == ComponentType::CIRCLE_COLLIDER)
            _mtv = this->collidingWithCircle(_collider, _push);
        return _mtv;
    }

    Mtv BoxCollider::collidingWithCircle(Collider& _collider, float _push) {
        Mtv _mtv;
        Vec2f _n = Collider::center - _collider.center;
        Vec2f _closest = _n;
        
        float _x_extent = Collider::size.width / 2;
        float _y_extent = Collider::size.height / 2;
    
        _closest.x = functions::clamp( _closest.x, -_x_extent, _x_extent );
        _closest.y = functions::clamp( _closest.y, -_y_extent, _y_extent );
        
        bool _inside = false;
        
        if(_n == _closest) {
            _inside = true;
        
            if(std::abs( _n.x ) > std::abs( _n.y )) {
                if(_closest.x > 0)
                    _closest.x = _x_extent;
                else
                    _closest.x = -_x_extent;
            } else { 
                if(_closest.y > 0)
                    _closest.y = _y_extent;
                else
                    _closest.y = -_y_extent;
            }
        }
        
        Vec2f _normal = _n - _closest;
        
        float _d = _normal.x * _normal.x + _normal.y * _normal.y;
        float _r = _collider.size.width;
        
        if(_d > _r * _r && !_inside)
            return _mtv;
        
        _d = std::sqrt(_d);
        
        float penetration = _r - _d;
        _normal = Vec2f::normalize(_normal);
        _mtv.collision = true; 
        _mtv.translation = _normal * penetration * (_inside ? -1.0f : 1.0f);
        
        return _mtv;
    }

    Mtv BoxCollider::collidingWithAABB(Collider& _collider, float _push) {
        Mtv _mtv;
        
        Vec2f _n = Collider::center - _collider.center;
        
        float a_extent = Collider::size.width / 2;
        float b_extent = _collider.size.width / 2;
        
        float x_overlap = a_extent + b_extent - std::abs( _n.x );
        
        if(x_overlap >= 0) {
            float a_extent = Collider::size.height / 2;
            float b_extent = _collider.size.height / 2;
        
            float y_overlap = a_extent + b_extent - std::abs( _n.y );
        
            if(y_overlap >= 0) {
                if(x_overlap <= y_overlap) {
                    
                    if(_n.x <= 0) { // Down Collision
                        _mtv.translation.x = -x_overlap;
                        _mtv.collision = true;
                        return _mtv;
                    } else {        // Up Collision
                        _mtv.translation.x = x_overlap;
                        _mtv.collision = true;
                        return _mtv;
                    }
                } else {
                    if(_n.y <= 0) { // Left Collision
                        _mtv.translation.y = -y_overlap;
                        _mtv.collision = true;
                        return _mtv;
                    } else {        // Right Collision
                        _mtv.translation.y = y_overlap;
                        _mtv.collision = true;
                        return _mtv;
                    }
                }
            }
        }

        return _mtv; 
    }
}