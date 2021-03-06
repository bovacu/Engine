#include "pch.h"

#include "OpenGLShader.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace engine {

    /// Handful method to transform the string type to OpenGL shader type.
    static GLenum ShaderTypeFromString(const std::string& _type) {
        if (_type == "vertex")
            return GL_VERTEX_SHADER;
        if (_type == "fragment" || _type == "pixel")
            return GL_FRAGMENT_SHADER;
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& _filepath) {

        std::string _source = this->readFile(_filepath);
        auto _shaderSources = this->preProcess(_source);
        this->compile(_shaderSources);

        // Extract name from filepath
        auto _lastSlash = _filepath.find_last_of("/\\");
        _lastSlash = _lastSlash == std::string::npos ? 0 : _lastSlash + 1;
        auto lastDot = _filepath.rfind('.');
        auto count = lastDot == std::string::npos ? _filepath.size() - _lastSlash : lastDot - _lastSlash;
        this->name = _filepath.substr(_lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const std::string& _name, const std::string& _vertexSrc, const std::string& _fragmentSrc) : name(_name) {
        std::unordered_map<GLenum, std::string> _sources;
        _sources[GL_VERTEX_SHADER] = _vertexSrc;
        _sources[GL_FRAGMENT_SHADER] = _fragmentSrc;
        this->compile(_sources);
    }

    OpenGLShader::~OpenGLShader() {
        glDeleteProgram(this->renderID);
    }

    std::string OpenGLShader::readFile(const std::string& _filepath) {
        /// This method is not mine, as I found this one really useful I didn't make it.
        /// It reads the content of the file and returns it as a string.
        std::string _result;
        std::ifstream _in(_filepath, std::ios::in | std::ios::binary);
        if (_in) {
            _in.seekg(0, std::ios::end);
            size_t size = _in.tellg();
            if (size != -1) {
                _result.resize(size);
                _in.seekg(0, std::ios::beg);
                _in.read(&_result[0], size);
                _in.close();
            }
            else {
                LOG_CRITICAL_CORE("Could not read from file to read the shader");
            }
        }
        else {
            LOG_CRITICAL_CORE("Could not open file of the shader");
        }

        return _result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& _source) {
        /// This method is not mine, as I found this one really useful I didn't make it.
        /// It checks if there are syntax error on the shader and if there is an specification of a wrong shader.
        /// It returns a map of type [typeOfShader, codeOfTheShader].
        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = _source.find(typeToken, 0); //Start of shader type declaration line
        while (pos != std::string::npos) {
            size_t eol = _source.find_first_of("\r\n", pos); //End of shader type declaration line
            ENGINE_CORE_ASSERT(eol != std::string::npos, "Syntax error")
            size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
            std::string type = _source.substr(begin, eol - begin);
            ENGINE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified")

            size_t nextLinePos = _source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
            ENGINE_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error")
            pos = _source.find(typeToken, nextLinePos); //Start of next shader type declaration line

            shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? _source.substr(nextLinePos) : _source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& _shaderSources) {
        /// First, we need to store memory for our shader (program).
        /// We are going to follow similar steps as to create buffers and fill them with data, but OpenGL's API is not
        /// consistent on this.
        GLuint _program = glCreateProgram();

        /// This means we still don't use Tessellation or Geometry shaders, just Vertex and Fragment.
        ENGINE_CORE_ASSERT(_shaderSources.size() <= 2, "We only support 2 shaders for now")

        /// We are storing the 2 shaders IDs, the fragment and vertex.
        std::array<GLenum, 2> _glShaderIDs {  };


        int _glShaderIDIndex = 0;
        for (auto& _kv : _shaderSources) {
            /// First we get the type and code of each shader.
            GLenum _type = _kv.first;
            const std::string& _source = _kv.second;

            /// Then, the memory is reserved for the specific type of shader.
            GLuint _shader = glCreateShader(_type);

            /// The source code is set to the desired shader buffer memory.
            const GLchar* _sourceCStr = _source.c_str();
            glShaderSource(_shader, 1, &_sourceCStr, nullptr);

            /// The shader is compiled.
            glCompileShader(_shader);

            /// It is checked whether or not it is compiled.
            GLint _isCompiled = 0;
            glGetShaderiv(_shader, GL_COMPILE_STATUS, &_isCompiled);
            if (_isCompiled == GL_FALSE) {
                /// In case is not, try to output as much information as possible.
                GLint _maxLength = 0;
                glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &_maxLength);

                std::vector<GLchar> _infoLog(_maxLength);
                glGetShaderInfoLog(_shader, _maxLength, &_maxLength, &_infoLog[0]);

                glDeleteShader(_shader);

                LOG_ERROR_CORE("{0}", _infoLog.data());
                ENGINE_CORE_ASSERT(false, "Shader compilation failure!")
                break;
            }

            /// With the shader compiled correctly, we can attach it to the program.
            glAttachShader(_program, _shader);
            _glShaderIDs[_glShaderIDIndex++] = _shader;
        }

        this->renderID = _program;

        /// Linking creates the executable of the shader for the GPU.
        glLinkProgram(_program);

        /// Note the different functions here: glGetProgram* instead of glGetShader*.
        /// Checking if it can be linked or not.
        GLint isLinked = 0;
        glGetProgramiv(_program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE) {
            /// In case is not, try to output as much information as possible.
            GLint _maxLength = 0;
            glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &_maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> _infoLog(_maxLength);
            glGetProgramInfoLog(_program, _maxLength, &_maxLength, &_infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(_program);

            for (auto _id : _glShaderIDs)
                glDeleteShader(_id);

            LOG_ERROR_CORE("{0}", _infoLog.data());
            ENGINE_CORE_ASSERT(false, "Shader link failure!")
            return;
        }

        /// Removing from memory.
        for (auto _id : _glShaderIDs) {
            glDetachShader(_program, _id);
            glDeleteShader(_id);
        }
    }

    void OpenGLShader::bind() const {
        /// Bind our program, see the inconsistency of the API.
        glUseProgram(this->renderID);
    }

    void OpenGLShader::unbind() const {
        /// Unbind our program, see the inconsistency of the API.
        glUseProgram(0);
    }

    void OpenGLShader::setInt(const std::string& _name, int _value) {
        this->uploadUniformInt(_name, _value);
    }

    void OpenGLShader::setIntArray(const std::string& _name, int* _values, uint32_t _count) {
        this->uploadUniformIntArray(_name, _values, _count);
    }

    void OpenGLShader::setFloat(const std::string& _name, float _value) {
        this->uploadUniformFloat(_name, _value);
    }

    void OpenGLShader::setFloat2(const std::string& _name, const glm::vec2& _value) {
        this->uploadUniformFloat2(_name, _value);
    }

    void OpenGLShader::setFloat3(const std::string& _name, const glm::vec3& _value) {
        this->uploadUniformFloat3(_name, _value);
    }

    void OpenGLShader::setFloat4(const std::string& _name, const glm::vec4& _value) {
        this->uploadUniformFloat4(_name, _value);
    }

    void OpenGLShader::setMat4(const std::string& _name, const glm::mat4& _value) {
        this->uploadUniformMat4(_name, _value);
    }

    void OpenGLShader::uploadUniformInt(const std::string& _name, int _value) const {
        /// First, we need the location of a specific variable on the shader code to work with it, and we get
        /// it through the shader program ID and the name of the variable.
        GLint _location = glGetUniformLocation(this->renderID, _name.c_str());

        /// Once we have te location, we can set the value.
        glUniform1i(_location, _value);
    }

    void OpenGLShader::uploadUniformIntArray(const std::string& _name, int* _values, uint32_t _count) const {
        /// First, we need the location of a specific variable on the shader code to work with it, and we get
        /// it through the shader program ID and the name of the variable.
        GLint _location = glGetUniformLocation(this->renderID, _name.c_str());

        /// Once we have te location, we can set the value.
        glUniform1iv(_location, _count, _values);
    }

    void OpenGLShader::uploadUniformFloat(const std::string& _name, float _value) const {
        /// First, we need the location of a specific variable on the shader code to work with it, and we get
        /// it through the shader program ID and the name of the variable.
        GLint _location = glGetUniformLocation(this->renderID, _name.c_str());

        /// Once we have te location, we can set the value.
        glUniform1f(_location, _value);
    }

    void OpenGLShader::uploadUniformFloat2(const std::string& _name, const glm::vec2& _value) const {
        /// First, we need the location of a specific variable on the shader code to work with it, and we get
        /// it through the shader program ID and the name of the variable.
        GLint _location = glGetUniformLocation(this->renderID, _name.c_str());

        /// Once we have te location, we can set the value.
        glUniform2f(_location, _value.x, _value.y);
    }

    void OpenGLShader::uploadUniformFloat3(const std::string& _name, const glm::vec3& _value) const {
        /// First, we need the location of a specific variable on the shader code to work with it, and we get
        /// it through the shader program ID and the name of the variable.
        GLint _location = glGetUniformLocation(this->renderID, _name.c_str());

        /// Once we have te location, we can set the value.
        glUniform3f(_location, _value.x, _value.y, _value.z);
    }

    void OpenGLShader::uploadUniformFloat4(const std::string& _name, const glm::vec4& _value) const {
        /// First, we need the location of a specific variable on the shader code to work with it, and we get
        /// it through the shader program ID and the name of the variable.
        GLint _location = glGetUniformLocation(this->renderID, _name.c_str());

        /// Once we have te location, we can set the value.
        glUniform4f(_location, _value.x, _value.y, _value.z, _value.w);
    }

    void OpenGLShader::uploadUniformMat3(const std::string& _name, const glm::mat3& _matrix) const {
        /// First, we need the location of a specific variable on the shader code to work with it, and we get
        /// it through the shader program ID and the name of the variable.
        GLint _location = glGetUniformLocation(this->renderID, _name.c_str());

        /// Once we have te location, we can set the value.
        glUniformMatrix3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(glm::value_ptr(_matrix)));
    }

    void OpenGLShader::uploadUniformMat4(const std::string& _name, const glm::mat4& _matrix) const {
        /// First, we need the location of a specific variable on the shader code to work with it, and we get
        /// it through the shader program ID and the name of the variable.
        GLint _location = glGetUniformLocation(this->renderID, _name.c_str());

        /// Once we have te location, we can set the value.
        glUniformMatrix4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(glm::value_ptr(_matrix)));
    }

}