#ifndef __SHADER_H__
#define __SHADER_H__

class Shader
{
public:
    Shader(const char *vert_filename, const char *frag_filename);
    ~Shader();

    void bind();
    void unbind();

    void setUniformInt(const char *name, int value);

private:
    GLuint vertex_shader, fragment_shader, program;

    GLuint make_shader(GLenum type, char *sourceCode, int length);
    GLuint make_program();
};

#endif // __SHADER_H__
