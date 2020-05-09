#include "PolygonCollider.h"

namespace engine {

    PolygonColliderPtr PolygonCollider::create(GameObject* _gameObject, const Vec2f& _center, const std::vector<Vec2f>& _vertices, bool _isGhost) {
        return std::make_shared<PolygonCollider>(_gameObject, _center, _vertices, _isGhost);
    }

    /// ----------------------------------------------------------------------------------------------------------------

    PolygonCollider::PolygonCollider(GameObject* _gameObject, const Vec2f& _center, const std::vector<Vec2f>& _vertices, bool _isGhost) : 
    Collider(_gameObject, ComponentType::POLYGON_COLLIDER, _isGhost) {
        Collider::vertices          = _vertices;
        Collider::numberOfVertices  = (int)_vertices.size();
        Collider::center            = _center;

        for(int _v = 0; _v < _vertices.size(); _v++) {
            Collider::vertices[_v].x = Collider::center.x + Collider::vertices[_v].x;
            Collider::vertices[_v].y = Collider::center.y + Collider::vertices[_v].y;
        }
    }

    PolygonCollider::PolygonCollider(GameObject* _gameObject, const std::vector<Vec2f>& _vertices, bool _isGhost) : 
    Collider(_gameObject, ComponentType::POLYGON_COLLIDER, _isGhost) {
        Collider::vertices          = _vertices;
        Collider::numberOfVertices  = (int)_vertices.size();
        Collider::center            = Collider::getCentroid();
    }

    Mtv PolygonCollider::isColliding(Collider& _collider, float _push) {
        Mtv _mtv;

        if (_collider.componentType == ComponentType::CIRCLE_COLLIDER)
            _mtv = this->collidingWithCircle(_collider, _push);
        else
            _mtv = Collider::collidingWithSat(_collider, _push);

        return _mtv;
    }

    Mtv PolygonCollider::collidingWithCircle(Collider& _collider, float _push) {
        std::vector<Edge> _edges;
        for (int _v = 0; _v < Collider::numberOfVertices; _v++)
            _edges.emplace_back(Edge(Collider::vertices.at(_v), Collider::vertices.at((_v + 1) % Collider::numberOfVertices)));

        Vec2f _closestAxis(-BIG_NUM, BIG_NUM);
        float _closestDistance = BIG_NUM;
        for (auto& _edge : _edges) {
            Vec2f _vec = _collider.center - _edge.v0;
            float _t = _vec.dotProduct(_edge.vector) * _edge.ilenthsquared;
            if (_t < 0) _t = 0; else if (_t > 1) _t = 1;
            Vec2f _axis = _collider.center - (_edge.v0 + (_edge.vector * _t));
            float _axisLengthSquared = _axis.dotProduct(_axis);

            if ((_closestAxis.x == -BIG_NUM && _closestAxis.y == BIG_NUM) || _axisLengthSquared < _closestDistance) {
                _closestAxis = _axis;
                _closestDistance = _axisLengthSquared;
            }
        }
        
        _closestAxis = Vec2f::normalize(_closestAxis);
        
        float _circleProjection = _closestAxis.dotProduct(_collider.center);
        float _polygonProjection = -std::numeric_limits<float>::infinity();

        for(int _v = 0; _v < Collider::numberOfVertices; _v++) {
            float _projection = Collider::vertices.at(_v).dotProduct(_closestAxis)  + _collider.size.width;
            if (_projection > _polygonProjection) _polygonProjection = _projection;
        }
        
        Mtv _mtv;

        float _overlap = _polygonProjection - _circleProjection;
        if(_overlap < 0)
            return _mtv;
        else {
            _mtv.collision = true;
            _mtv.translation = _closestAxis * -_overlap;
        }
        
        return _mtv;
    }


}