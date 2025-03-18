//
//  SphereRenderer.cpp
//  ParticleSystem
//
//  Created by evanbfeng on 2025/3/17.
//

#include "SphereRenderer.hpp"

SphereRenderer::SphereRenderer(float radius, int sectors, int stacks)
{
    GenerateSphereData(radius, sectors, stacks);
    SetupOpenGLResources();

    // 初始化着色器
    _shader = std::make_shared<Shader>("./shader_file/sphere_shader.vs", "./shader_file/sphere_shader.fs");
}

void SphereRenderer::GenerateSphereData(float radius, int sectors, int stacks)
{
    float sectorStep = 2 * M_PI / sectors;
    float stackStep = M_PI / stacks;

    // 生成顶点数据
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = M_PI / 2 - i * stackStep;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * sectorStep;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;

            float u = (float)j / sectors;
            float v = (float)i / stacks;

            _vertices.push_back({x, y, z, nx, ny, nz, u, v});
        }
    }

    // 生成索引数据（用于 GL_TRIANGLE_STRIP）
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j <= sectors; ++j) {
            _indices.push_back(i * (sectors + 1) + j);
            _indices.push_back((i + 1) * (sectors + 1) + j);
        }
        // 添加退化三角形以连接不同堆栈
        if (i < stacks - 1) {
            _indices.push_back((i + 1) * (sectors + 1) + sectors);
            _indices.push_back((i + 1) * (sectors + 1));
        }
    }
}

void SphereRenderer::SetupOpenGLResources()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // 绑定顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

    // 绑定索引数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void SphereRenderer::SetWorldTransform(const mat4& worldTransform)
{
    _worldTransform = worldTransform;
}

void SphereRenderer::SetCamera(CameraPtr camera)
{
    _camera = camera;
}

void SphereRenderer::Render()
{
    if (!_shader || !_camera) return;
    
    _shader->use();

    // 设置变换矩阵
    _shader->setMat4("model", _worldTransform);
    _shader->setMat4("view", _camera->GetViewTransform());
    _shader->setMat4("projection", _camera->GetProjTransform());

    // 渲染球体
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertices.size());
    glBindVertexArray(0);
}
