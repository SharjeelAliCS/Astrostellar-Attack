#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;//https://github.com/nlohmann/json
namespace game {

    // Possible resource types
    typedef enum Type { Material, PointSet, Mesh, Texture,Save } ResourceType;

    // Class that holds one resource
    class Resource {

        private:
            ResourceType type_; // Type of resource
            std::string name_; // Reference name
			std::string file_path_;
            union {
                struct {
                    GLuint resource_; // OpenGL handle for resource
                };
                struct {
                    GLuint array_buffer_; // Buffers for geometry
                    GLuint element_array_buffer_;
                };
				json data_;
            };
            GLsizei size_; // Number of primitives in geometry

        public:
			Resource(ResourceType type, std::string name, std::string file_path,json data);
            Resource(ResourceType type, std::string name,GLuint resource, GLsizei size);
            Resource(ResourceType type, std::string name,GLuint array_buffer, GLuint element_array_buffer, GLsizei size);
            ~Resource();

			void SetJSON(json data);
            ResourceType GetType(void) const;
            const std::string GetName(void) const;
            GLuint GetResource(void) const;
            GLuint GetArrayBuffer(void) const;
            GLuint GetElementArrayBuffer(void) const;
            GLsizei GetSize(void) const;
			json GetJSON(void) const;
			std::string GetFilePath(void) const;

    }; // class Resource

} // namespace game

#endif // RESOURCE_H_
