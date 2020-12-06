#include <exception>

#include "resource.h"

namespace game {

Resource::Resource(ResourceType type, std::string name, GLuint resource, GLsizei size){
    type_ = type;
    name_ = name;
    resource_ = resource;
    size_ = size;
}

Resource::Resource(ResourceType type, std::string name, std::string file_path,json data) {
	type_ = type;
	name_ = name;
	data_ = data;
	file_path_ = file_path;
}

Resource::Resource(ResourceType type, std::string name, GLuint array_buffer, GLuint element_array_buffer, GLsizei size){
    type_ = type;
    name_ = name;
    array_buffer_ = array_buffer;
    element_array_buffer_ = element_array_buffer;
    size_ = size;
}


Resource::~Resource(){

}

void Resource::SetJSON(json data) {
	data_ = data;
}
ResourceType Resource::GetType(void) const {

    return type_;
}


const std::string Resource::GetName(void) const {

    return name_;
}


GLuint Resource::GetResource(void) const {

    return resource_;
}


GLuint Resource::GetArrayBuffer(void) const {

    return array_buffer_;
}


GLuint Resource::GetElementArrayBuffer(void) const {

    return element_array_buffer_;
}


GLsizei Resource::GetSize(void) const {

    return size_;
}

json Resource::GetJSON(void) const {
	return data_;
}
std::string Resource::GetFilePath(void) const {
	return file_path_;
}

NodeResources::NodeResources() : geom(NULL), mat(NULL), tex(NULL), norm(NULL) {}
} // namespace game
