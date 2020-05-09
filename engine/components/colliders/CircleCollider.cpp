#include "CircleCollider.h"

namespace engine {

    CircleColliderPtr CircleCollider::create(GameObject* _gameObject, const Vec2f& _center, float _radius, bool _isGhost) {
        return std::make_shared<CircleCollider>(_gameObject, _center, _radius, _isGhost);
    }

    /// ----------------------------------------------------------------------------------------------------------------

    CircleCollider::CircleCollider(GameObject* _gameObject, const Vec2f& _center, float _radious, bool _isGhost) : 
    Collider(_gameObject, ComponentType::CIRCLE_COLLIDER, _isGhost) { 
        Collider::size.width        = _radious;
        Collider::numberOfVertices  = 0;
        Collider::center            = _center;
    }
    
    Mtv CircleCollider::isColliding(Collider& _collider, float _push) {
        Mtv _mtv;

        if(_collider.componentType == Collider::componentType)
            _mtv = this->collidingWithCircle(_collider, _push);
        else if(_collider.componentType == ComponentType::BOX_COLLIDER)
            _mtv = this->collidingWithAABB(_collider, _push);
        else {
            _mtv = this->collidingWithPolygon(_collider, _push);
        }

        return _mtv;
    }

    Mtv CircleCollider::collidingWithCircle(Collider& _collider, float _push) {
        Mtv _mtv;
        Vec2f _n = _collider.center - Collider::center;
        
        float _r = Collider::size.width + _collider.size.width;
        _r *= _r;
        
        if((_n.x * _n.x + _n.y * _n.y) > _r)
            return _mtv;
        
        float _d = _n.magnitude();
        float _penetration;
        _mtv.collision = true;
        if(_d != 0) {
            _penetration = std::sqrt(_r) - _d;
            _n = Vec2f::normalize(_n);
            _mtv.translation = _n * -_penetration;
            return _mtv;
        } else {
            _penetration = Collider::size.width;
            _mtv.translation = Vec2f(1, 0) * _penetration;
            return _mtv;
        }
    }

    Mtv CircleCollider::collidingWithAABB(Collider& _collider, float _push) {
        Mtv _mtv;
        Vec2f _n = Collider::center - _collider.center;
        Vec2f _closest = _n;
        
        float _x_extent = _collider.size.width / 2;
        float _y_extent = _collider.size.height / 2;
    
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
        float _r = Collider::size.width;
        
        if(_d > _r * _r && !_inside)
            return _mtv;
        
        _d = std::sqrt(_d);
        
        float penetration = _r - _d;
        _normal = Vec2f::normalize(_normal);
        _mtv.collision = true; 
        _mtv.translation = _normal * penetration * (_inside ? -1.0f : 1.0f);
        
        return _mtv;
    }

    Mtv CircleCollider::collidingWithPolygon(Collider& _collider, float _float) {
        std::vector<Edge> _edges;
        for (int _v = 0; _v < _collider.numberOfVertices; _v++)
            _edges.emplace_back(Edge(_collider.vertices.at(_v), _collider.vertices.at((_v + 1) % _collider.numberOfVertices)));

        Vec2f _closestAxis(-BIG_NUM, BIG_NUM);
        float _closestDistance = BIG_NUM;
        for (auto& _edge : _edges) {
            Vec2f _vec = Collider::center - _edge.v0;
            float _t = _vec.dotProduct(_edge.vector) * _edge.ilenthsquared;
            if (_t < 0) _t = 0; else if (_t > 1) _t = 1;
            Vec2f _axis = Collider::center - (_edge.v0 + (_edge.vector * _t));
            float _axisLengthSquared = _axis.dotProduct(_axis);

            if ((_closestAxis.x == -BIG_NUM && _closestAxis.y == BIG_NUM) || _axisLengthSquared < _closestDistance) {
                _closestAxis = _axis;
                _closestDistance = _axisLengthSquared;
            }
        }
        
        _closestAxis = Vec2f::normalize(_closestAxis);
        
        float _circleProjection = _closestAxis.dotProduct(Collider::center);
        float _polygonProjection = -std::numeric_limits<float>::infinity();

        for(int _v = 0; _v < _collider.numberOfVertices; _v++) {
            float _projection = _collider.vertices.at(_v).dotProduct(_closestAxis)  + Collider::size.width;
            if (_projection > _polygonProjection) _polygonProjection = _projection;
        }

        Mtv _mtv;

        float _overlap = _polygonProjection - _circleProjection;
        if(_overlap < 0)
            return _mtv;
        else {
            _mtv.collision = true;
            _mtv.translation = _closestAxis * _overlap;
        }
        
        return _mtv;
    }
}