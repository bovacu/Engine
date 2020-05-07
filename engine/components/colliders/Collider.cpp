#include "Collider.h"
#include "../../gameObject/GameObject.h"

namespace engine {

    Collider::Collider(GameObject* _gameObject, const ComponentType& _componentType, bool _isGhost) : 
    Component(_gameObject, _componentType),
    isGhost(_isGhost), collisionEntered(false) {  }

    Mtv Collider::collidingWithSat(Collider& _collider, float _push) {
        Vec2f _smallestAxis;
        float _minimumDistance = BIG_NUM;

        // Checking A over B
        for(int _i = 0; _i < Collider::numberOfVertices; _i++) {
            Vec2f _currentVertex = Collider::vertices.at(_i);
            Vec2f _nextVertex = Collider::vertices.at((_i + 1) % Collider::numberOfVertices);
            Vec2f _vector = _nextVertex - _currentVertex;
            Vec2f _axis(-_vector.y, _vector.x);
            _axis = Vec2f::normalize(_axis);

            float _maximumProjectionOfBodyA = -std::numeric_limits<float>::infinity();
            float _minimumProjectionOfBodyA = std::numeric_limits<float>::infinity();
            float _maximumProjectionOfBodyB = -std::numeric_limits<float>::infinity();
            float _minimumProjectionOfBodyB = std::numeric_limits<float>::infinity();

            for(int _v = 0; _v < Collider::numberOfVertices; _v++) {
                float _projection = Collider::vertices.at(_v).dotProduct(_axis);
                if(_projection < _minimumProjectionOfBodyA) _minimumProjectionOfBodyA = _projection;
                if(_projection > _maximumProjectionOfBodyA) _maximumProjectionOfBodyA = _projection;
            }

            for(int _v = 0; _v < _collider.numberOfVertices; _v++) {
                float _projection = _collider.vertices.at(_v).dotProduct(_axis);
                if(_projection < _minimumProjectionOfBodyB) _minimumProjectionOfBodyB = _projection;
                if(_projection > _maximumProjectionOfBodyB) _maximumProjectionOfBodyB = _projection;
            }
            
            if(_maximumProjectionOfBodyA < _minimumProjectionOfBodyB || _maximumProjectionOfBodyB < _minimumProjectionOfBodyA)
                return Mtv();
            else {
                double _distance = std::min(_maximumProjectionOfBodyB - _minimumProjectionOfBodyA,
                                            _maximumProjectionOfBodyA - _minimumProjectionOfBodyB);

                if(_distance < _minimumDistance) {
                    _minimumDistance = _distance;
                    _smallestAxis = _axis;
                }  
            }
	    }

        // Checking B over A
        for(int _i = 0; _i < _collider.numberOfVertices; _i++) {
            Vec2f _currentVertex = _collider.vertices.at(_i);
            Vec2f _nextVertex = _collider.vertices.at((_i + 1) % _collider.numberOfVertices);
            Vec2f _vector = _nextVertex - _currentVertex;
            Vec2f _axis(-_vector.y, _vector.x);
            _axis = Vec2f::normalize(_axis);

            float _maximumProjectionOfBodyA = -std::numeric_limits<float>::infinity();
            float _minimumProjectionOfBodyA = std::numeric_limits<float>::infinity();
            float _maximumProjectionOfBodyB = -std::numeric_limits<float>::infinity();
            float _minimumProjectionOfBodyB = std::numeric_limits<float>::infinity();

            for(int _v = 0; _v < Collider::numberOfVertices; _v++) {
                float _projection = Collider::vertices.at(_v).dotProduct(_axis);
                if(_projection < _minimumProjectionOfBodyA) _minimumProjectionOfBodyA = _projection;
                if(_projection > _maximumProjectionOfBodyA) _maximumProjectionOfBodyA = _projection;
            }

            for(int _v = 0; _v < _collider.numberOfVertices; _v++) {
                float _projection = _collider.vertices.at(_v).dotProduct(_axis);
                if(_projection < _minimumProjectionOfBodyB) _minimumProjectionOfBodyB = _projection;
                if(_projection > _maximumProjectionOfBodyB) _maximumProjectionOfBodyB = _projection;
            }
            
            if(_maximumProjectionOfBodyA < _minimumProjectionOfBodyB || _maximumProjectionOfBodyB < _minimumProjectionOfBodyA)
                return Mtv();
            else {
                double _distance = std::min(_maximumProjectionOfBodyB - _minimumProjectionOfBodyA,
                                            _maximumProjectionOfBodyA - _minimumProjectionOfBodyB);

                if(_distance < _minimumDistance) {
                    _minimumDistance = _distance;
                    _smallestAxis = _axis;
                }  
            }
	    }

        Mtv _mtv;
        _mtv.collision = true;

        float _distanceBetweenOrthogonalVectorSquared = _minimumDistance / _smallestAxis.dotProduct(_smallestAxis);
        _mtv.translation = _smallestAxis * _distanceBetweenOrthogonalVectorSquared;
        Vec2f _d = _collider.center - Collider::center;
        if (_d.dotProduct(_mtv.translation) > 0)
            _mtv.translation = _mtv.translation * -1;

        return _mtv;
    }

    void Collider::update(float _dt) {

    }

    void Collider::fixUpdate(float _fixDt) {

    }

    void Collider::postUpdate(float _postDt) {
        this->setPosition(this->gameObject->transform.position);
        this->rotate(this->gameObject->transform.rotation);
    }

    void Collider::update(Transform2D& _transform) {
        std::cerr << "UPDATE WITH TRANSFORM NOT YET IMPLEMENTED" << std::endl;
    }

    void Collider::update(const Vec2f& _position, float _rotation, const Vec2f& _scale){
        this->setPosition(_position);
        this->rotate(_rotation);
    }

    Mtv Collider::collides(Collider& _collider, float _push) {
        Mtv _mtv = this->isColliding(_collider, _push);

        if(_mtv.collision && !this->collisionEntered) {
            this->collisionEntered = true;
            if(this->onEnter != nullptr)
                this->onEnter(_collider);
        }

        if(_mtv.collision)
            if(this->onCollision != nullptr)
                this->onCollision(_collider);

        if(!_mtv.collision && this->collisionEntered) {
            this->collisionEntered = false;
            if(this->onExit != nullptr)
                this->onExit(_collider);
        }

        return _mtv;
    }

    void Collider::rotate(float _alpha) {
        this->rotateRadians(TO_RAD(_alpha));
    }

    void Collider::rotateRadians(float _alpha) {
        if(_alpha > 0.f) {
            for(int _i = 0; _i < this->numberOfVertices; _i++) {
                float _translatedX = this->vertices.at(_i).x - this->center.x;
                float _translatedY = this->vertices.at(_i).y - this->center.y;

                float _rotatedX = (cos(_alpha) * _translatedX - sin(_alpha) * _translatedY);
                float _rotatedY = (sin(_alpha) * _translatedX + cos(_alpha) * _translatedY);

                this->vertices.at(_i).x = _rotatedX + this->center.x;
                this->vertices.at(_i).y = _rotatedY + this->center.y;
            }
        }
    }

    void Collider::setPosition(float _x, float _y) {
        float _distanceX = _x - this->center.x;
        float _distanceY = _y - this->center.y;

        for(int _v = 0; _v < this->numberOfVertices; _v++) {
            this->vertices.at(_v).x += _distanceX;
            this->vertices.at(_v).y += _distanceY;
        }

        this->center.x = _x;
        this->center.y = _y;
    }

    void Collider::setPosition(const Vec2f& _position) {
        float _distanceX = _position.x - this->center.x;
        float _distanceY = _position.y - this->center.y;

        for(int _v = 0; _v < this->numberOfVertices; _v++) {
            this->vertices.at(_v).x += _distanceX;
            this->vertices.at(_v).y += _distanceY;
        }

        this->center.x = _position.x;
        this->center.y = _position.y;
    }

    Vec2f Collider::getCentroid() {
        float _centroidX = 0, _centroidY = 0;
        float _det = 0, _tempDet = 0;
        unsigned int _j = 0;
        unsigned int _nVertices = (unsigned int)Collider::vertices.size();

        for (unsigned int _i = 0; _i < _nVertices; _i++) {
            // closed polygon
            if (_i + 1 == _nVertices)
                _j = 0;
            else
                _j = _i + 1;

            // compute the _determinant
            _tempDet = Collider::vertices[_i].x * Collider::vertices[_j].y - Collider::vertices[_j].x * Collider::vertices[_i].y;
            _det += _tempDet;

            _centroidX += (Collider::vertices[_i].x + Collider::vertices[_j].x) * _tempDet;
            _centroidY += (Collider::vertices[_i].y + Collider::vertices[_j].y) * _tempDet;
        }

        // divide by the total mass of the polygon
        _centroidX /= 3 * _det;
        _centroidY /= 3 * _det;

        return Vec2f(_centroidX, _centroidY);
    }

    Vec2f Collider::getClosestVertexToCircleCenter(std::vector<Vec2f>& _verteces, const Vec2f& _center) {
        float _closest = std::sqrt((_verteces.at(0).x - _center.x) * (_verteces.at(0).x - _center.x) + 
                                   (_verteces.at(0).y - _center.y) * (_verteces.at(0).y - _center.y));
        int _vertex = 0;
        for(int _v = 0; _v < _verteces.size(); _v++) {
            float _distance = std::sqrt((_verteces.at(_v).x - _center.x) * (_verteces.at(_v).x - _center.x) + 
                                        (_verteces.at(_v).y - _center.y) * (_verteces.at(_v).y - _center.y));
            if(_distance < _closest) {
                _closest = _distance;
                _vertex = _v;
            }
        }

        return _verteces.at(_vertex);
    }
}

