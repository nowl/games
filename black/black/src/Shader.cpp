#include "Black.h"

using namespace std;

static void show_info_log(GLuint object,
    PFNGLGETSHADERIVPROC glGet__iv,
    PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
    GLint log_length;
    char *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = (char*)malloc(log_length);
    glGet__InfoLog(object, log_length, NULL, log);
    printf("%s", log);
    free(log);
}

Shader::Shader(const char *vert_filename, const char *frag_filename)
{
    GrowableString vert_string;
    int vert_string_len;
    readFileContents(vert_string, vert_filename, vert_string_len);

    GrowableString frag_string;
    int frag_string_len;
    readFileContents(frag_string, frag_filename, frag_string_len);

    vertex_shader = make_shader(GL_VERTEX_SHADER, vert_string.Val, vert_string_len);
    if (vertex_shader == 0)
        throw runtime_error("problem compiling vertex shader");

    fragment_shader = make_shader(GL_FRAGMENT_SHADER, frag_string.Val, frag_string_len);
    if (fragment_shader == 0)
        throw runtime_error("problem compiling fragment shader");

    program = make_program();
    if (program == 0)
        throw runtime_error("problem linking shaders");
}

Shader::~Shader()
{
    // TODO: destroy shaders, programs, etc.
}

GLuint Shader::make_shader(GLenum type, char *sourceCode, int length)
{
    GLchar *source = sourceCode;
    GLuint shader;
    GLint shader_ok;

    if (!source)
        return 0;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&source, &length);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        printf("Failed to compile\n");
        show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
    }
    //printf("successful compile\n");
    return shader;
}

GLuint Shader::make_program()
{
    GLint program_ok;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if (!program_ok) {
        printf("Failed to link shader program:\n");
        show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    }
    //printf("successful link\n");

    return program;
}

void Shader::bind()
{
    glUseProgram(program);
}

void Shader::unbind()
{
    glUseProgram(0);
}

void Shader::setUniformInt(const char *name, int value)
{
    int uniform_location = glGetUniformLocation(program, name);
    glUniform1i(uniform_location, value);
}
