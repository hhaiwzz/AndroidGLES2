//
// Created by hhai on 3/18/18.
//

#include "SimpleShader.h"
#include "JniHelper.h"
#include <android/log.h>
#include <fstream>

SimpleShader::SimpleShader()
{

}

SimpleShader::~SimpleShader()
{
    delete[] vertexShader;
    delete[] fragmentShader;
}

bool SimpleShader::LoadShaderFromFile(const string &vertexShaderFile, const string &fragmentShaderFile)
{
    if (vertexShaderFile.empty() || fragmentShaderFile.empty())
    {
        __android_log_print(ANDROID_LOG_ERROR, SIMPLE_SHADER_TAG, "Shader's path is empty");
        return false;
    }

    // extract file from asset
    if (!JniHelper::jniHelper->ExtractFile(vertexShaderFile))
        return false;
    if (!JniHelper::jniHelper->ExtractFile(fragmentShaderFile))
        return false;

    // read and create vertex shader
    string vsPath = JniHelper::jniHelper->GetFullPath(vertexShaderFile);
    __android_log_print(ANDROID_LOG_DEBUG, SIMPLE_SHADER_TAG, "Reading vertex shader (%s)", vsPath.c_str());

    char* vertexShader = ReadFile(vsPath);
    if (vertexShader == nullptr)
        return false;

    // read and create fragment shader
    string fsPath = JniHelper::jniHelper->GetFullPath(fragmentShaderFile);
    __android_log_print(ANDROID_LOG_DEBUG, SIMPLE_SHADER_TAG, "Reading fragment shader (%s)", fsPath.c_str());

    char* fragmentShader = ReadFile(fsPath);
    if (fragmentShader == nullptr)
    {
        delete[] vertexShader;
        return false;
    }

    if (!LoadShaderFromString(vertexShader, fragmentShader))
    {
        return false;
    }

    return true;
}

bool SimpleShader::LoadShaderFromString(char* vertexShader, char* fragmentShader)
{
    if (vertexShader == nullptr || fragmentShader == nullptr)
        return false;

    // remove last shader
    if (this->vertexShader != nullptr)
        DeleteVertexShader();
    if (this->fragmentShader != nullptr)
        DeleteFragmentShader();

    // vertex shader
    this->vertexShader = vertexShader;
    __android_log_print(ANDROID_LOG_DEBUG, SIMPLE_SHADER_TAG, "Vertex shader: %s", vertexShader);

    vertexShaderId = CreateShader(vertexShader, GL_VERTEX_SHADER);
    if (vertexShaderId == 0)
    {
        DeleteVertexShader();
        return false;
    }

    if (!CompileShader(vertexShaderId))
    {
        DeleteVertexShader();
        return false;
    }

    // fragment shader
    this->fragmentShader = fragmentShader;
    __android_log_print(ANDROID_LOG_DEBUG, SIMPLE_SHADER_TAG, "Fragment shader: %s", fragmentShader);

    fragmentShaderId = CreateShader(fragmentShader, GL_FRAGMENT_SHADER);
    if (fragmentShaderId == 0)
    {
        DeleteVertexShader();
        DeleteFragmentShader();
        return false;
    }

    if (!CompileShader(fragmentShaderId))
    {
        DeleteVertexShader();
        DeleteFragmentShader();
        return false;
    }

    CreateProgram();

    return true;
}

char* SimpleShader::ReadFile(const string &shaderFile)
{
    std::ifstream file(shaderFile, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        __android_log_print(ANDROID_LOG_ERROR, SIMPLE_SHADER_TAG, "Cannot open file %s", shaderFile.c_str());
        return nullptr;
    }

    std::streampos size = file.tellg();
    if (size <= 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, SIMPLE_SHADER_TAG, "File %s is empty", shaderFile.c_str());
        return nullptr;
    }

    char* data = new char[size];
    file.seekg(std::ios::beg);
    file.read(data, size);
    file.close();

    __android_log_print(ANDROID_LOG_DEBUG, SIMPLE_SHADER_TAG, "Shader is loaded");

    return data;
}

GLuint SimpleShader::CreateShader(const char *source, GLenum type)
{
    GLuint shaderId = glCreateShader(type);
    if (shaderId == 0)
        __android_log_print(ANDROID_LOG_ERROR, SIMPLE_SHADER_TAG, "Cannot create %s shader",
                            type == GL_VERTEX_SHADER ? "vertex" : "fragment");
    else
        glShaderSource(shaderId, 1, &source, nullptr);

    return shaderId;
}

bool SimpleShader::CompileShader(GLuint shader)
{
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (compiled != GL_TRUE)
    {
        GLint infoLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

        if (infoLength > 0)
        {
            char* log = new char[infoLength];
            glGetShaderInfoLog(shader, infoLength, nullptr, log);
            __android_log_print(ANDROID_LOG_ERROR, SIMPLE_SHADER_TAG, "Error compile shader: %s", log);

            delete[] log;
            glDeleteShader(shader);

            return false;
        }
    }

    return true;
}

bool SimpleShader::CreateProgram()
{
    programId = glCreateProgram();
    if (programId == 0)
    {
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
        __android_log_print(ANDROID_LOG_DEBUG, SIMPLE_SHADER_TAG, "Cannot create program");
        return false;
    }

    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);

    GLint linked;
    glGetProgramiv(programId, GL_COMPILE_STATUS, &linked);

    if (linked != GL_TRUE)
    {
        GLint infoLength = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength > 0)
        {
            char* log = new char[infoLength];
            glGetProgramInfoLog(programId, infoLength, nullptr, log);
            __android_log_print(ANDROID_LOG_ERROR, SIMPLE_SHADER_TAG, "Error linking program: %s", log);

            delete[] log;
            glDeleteProgram(programId);

            return false;
        }
    }

    return true;
}

bool SimpleShader::Link() {
    return false;
}

void SimpleShader::DeleteVertexShader() {
    delete[] vertexShader;
    vertexShader = nullptr;
}

void SimpleShader::DeleteFragmentShader() {
    delete[] fragmentShader;
    fragmentShader = nullptr;
}
