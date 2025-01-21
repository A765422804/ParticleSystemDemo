//
//  Node.hpp
//  ParticleSystem
//
//  Created by evanbfeng on 2024/12/31.
//

#pragma once

#include "../Head.h"

class Node
{
public:
    Node();
    ~Node() = default;
    
public:
    void SetParent(Node* parent);
    void AddChild(std::shared_ptr<Node> child);
    
public:
    Node* GetParent();
    std::vector<std::shared_ptr<Node>> GetChildren();

public:
    mat4 GetWorldTransform();
    mat4 GetLocalTransform();
    
public:
    vec2 GetPosition2D();
    vec3 GetPosition3D();
    
    void SetPosition3D(vec3);
    
public:
    vec3 GetWorldPosition();
    
public:
    vec3 ConvertToWorldSpace(vec3 localPoint);
    vec3 ConvertToNodeSpace(vec3 worldPoint);
    
public:
    void Move(vec3 dir, float dt);
    
protected:
    vec3 _position;
    quat _rotation;
    vec3 _scale;
    Node* _parent;
    std::vector<std::shared_ptr<Node>> _children;
};

using NodePtr = std::shared_ptr<Node>;

// TODO: scale暂时不知道如何使用

#pragma mark -

inline Node* Node::GetParent()
{
    return _parent;
}

inline std::vector<std::shared_ptr<Node>> Node::GetChildren()
{
    return _children;
}

inline vec2 Node::GetPosition2D()
{
    return vec2(_position);
}

inline vec3 Node::GetPosition3D()
{
    return _position;
}

inline void Node::SetPosition3D(vec3 position)
{
    _position = position;
}
