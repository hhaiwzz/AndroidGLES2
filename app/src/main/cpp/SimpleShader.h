//
// Created by hhai on 3/18/18.
//

#ifndef GLES2_SIMPLESHADER_H
#define GLES2_SIMPLESHADER_H

#include <string>
#include <GLES2/gl2.h>

using std::string;

class SimpleShader
{
private:
    const char* SIMPLE_SHADER_TAG = "SimpleShader";

    // next time
    char* defaultVertexShader = nullptr;
    char* defaultFragmentShader = nullptr;

    GLuint vertexShaderId = 0;
    char* vertexShader = nullptr;
    GLuint fragmentShaderId = 0;
    char* fragmentShader = nullptr;

    GLint programId = -1;

    char* ReadFile(const string& shaderFile);
    // Create shader in opengl and return shader id
    GLuint CreateShader(const char* source, GLenum type);
    bool CompileShader(GLuint shader);
    bool CreateProgram();
    bool Link();

    inline void DeleteVertexShader();
    inline void DeleteFragmentShader();

public:
    SimpleShader();
    ~SimpleShader();

    bool LoadShaderFromFile(const string &vertexShaderFile, const string &fragmentShaderFile);
    bool LoadShaderFromString(char* vertexShader, char* fragmentShader);
};

#endif //GLES2_SIMPLESHADER_H
