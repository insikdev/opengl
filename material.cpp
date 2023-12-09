#include "pch.h"
#include "material.h"
#include "gl_program.h"
#include "gl_texture.h"

void Material::SetToProgram(const Wrapper::Program* program) const
{
    int textureCount = 0;
    if (diffuse) {
        glActiveTexture(GL_TEXTURE0 + textureCount);
        program->SetUniform("material.diffuse", textureCount);
        diffuse->Bind();
        textureCount++;
    }
    if (specular) {
        glActiveTexture(GL_TEXTURE0 + textureCount);
        program->SetUniform("material.specular", textureCount);
        specular->Bind();
        textureCount++;
    }
    glActiveTexture(GL_TEXTURE0 + textureCount);
    program->SetUniform("material.shininess", shininess);
}
