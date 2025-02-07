//
//  Node.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/31.
//

#include "Node.hpp"

Node::Node()
: _position(vec3(0.0f))
, _rotation(quat(1, 0, 0, 0))
, _scale(1.0f)
, _parent(nullptr)
, _children()
{
}

void Node::SetParent(Node* parent)
{
    if (parent)
    {
        auto& siblings = parent->_children;
        siblings.erase(std::remove_if(siblings.begin(), siblings.end(), [this](Node* node) {
                           return node == this;
                       }),
                       siblings.end());
    }
    _parent = parent;
    if (_parent)
    {
        _parent->_children.push_back(this);
    }
}

void Node::AddChild(std::shared_ptr<Node> child)
{
    child->SetParent(this);
}

mat4 Node::GetWorldTransform()
{
    if (_parent)
    {
        return _parent->GetWorldTransform() * GetLocalTransform();
    }
    return GetLocalTransform();
}

mat4 Node::GetLocalTransform()
{
    mat4 transform(1.0f);
    transform = translate(transform, _position);
    transform = transform * toMat4(_rotation);
    transform = scale(transform, _scale);
    return transform;
    
}

vec3 Node::GetWorldPosition()
{
    if (_parent)
    {
        return _parent->GetWorldTransform() * vec4(_position, 1.0f);
    }
    return _position;
}

vec3 Node::ConvertToWorldSpace(vec3 localPoint)
{
    mat4 worldTransform = GetWorldTransform();
    vec4 homogeneousPoint = worldTransform * vec4(localPoint, 1.0f);
    return vec3(homogeneousPoint);
}

vec3 Node::ConvertToNodeSpace(vec3 worldPoint)
{
    if (_parent)
    {
        mat4 inverseWorldTransform = inverse(GetWorldTransform());
        vec4 localPoint = inverseWorldTransform * vec4(worldPoint, 1.0f);
        return vec3(localPoint);
    }
    else
    {
        return worldPoint;
    }
}

void Node::Move(vec3 dir, float dt)
{
    _position += dir*dt;
}
