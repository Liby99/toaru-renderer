#ifndef SHADER_H
#define SHADER_H

#include "utility/math.h"
#include "utility/path.h"
#include <map>
#include <nanogui/nanogui.h>
#include <string>

namespace nash {
  class GLShaderWrapper : public nanogui::GLShader {
  public:
    GLShaderWrapper();
    GLuint getProgramId() const;
  };

  class Shader {
  public:
    static const std::string DEFAULT_VERT_SHADER;
    static const std::string DEFAULT_FRAG_SHADER;

    void init();
    void bind();
    void free();
    GLuint getProgramId();
    void setUniform(const std::string &name, const Matrix4f &mat);
    void setUniform(const std::string &name, const Matrix3f &mat);
    void setUniform(const std::string &name, const Vector4f &vec);
    void setUniform(const std::string &name, const Vector3f &vec);
    void setUniform(const std::string &name, bool value);
    void setUniform(const std::string &name, int value);
    void setUniform(const std::string &name, float value);
    void setUniform(const std::string &name, const std::vector<float> &arr);
    void setUniform(const std::string &name, const float *arr, unsigned int amount);
    void setUniform(const std::string &name, const std::vector<int> &arr);
    void setUniform(const std::string &name, const int *arr, unsigned int amount);
    void uploadAttr(const std::string &name, const MatrixXf &m);
    void uploadAttr(const std::string &name, const MatrixXu &m);
    void freeAttr(const std::string &name);
    bool hasAttr(const std::string &name);
    void shareAttr(const Shader &other, const std::string &name, const std::string &as);
    void uploadIndices(const nanogui::MatrixXu &m);
    void drawArray(int type, uint32_t offset, uint32_t count);
    void drawIndexed(int type, uint32_t offset, uint32_t count);

    static void initAll();
    static void freeAll();
    static Shader &get();
    static Shader &get(const std::string &name);

  private:
    bool simple;
    std::string name;
    std::string path;
    GLShaderWrapper *shader;
    Shader();
    Shader(const std::string &name);
    ~Shader();

    static std::map<std::string, Shader *> store;
  };
}

#endif
