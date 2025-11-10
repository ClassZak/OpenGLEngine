# OpenGL Engine

The OpenGL Engine is framework or library for take more abstract code for OpenGL projects using 2D or 3D rendering.

This repository can be used to build the same OpenGL Engine or its examples.

The Engine directory is used to build the same graphic framework.

The Airplane directory is used to show you example how to use the OpenGL Engine making your project.

The Jelly directory is used to show you example how to create new shapes for your needs.

## 🛠 Dependencies

|Library        |
|---------------|
|OpenGL			|
|glfw3			|
|glew			|
|glm			|
|tinyobjloader	|
|stb_image.h	|

## Base classes

### OpenGLClasses

- AOpenGLClass - base abstract class for using other classes
- IndexBufferObject - class for indecies storing and declaring in OpenGL
- Mesh - the base class for 3D objects
- Shader - the class for shader storing and declaring in OpenGL
- Texture - the class for texture storing and declaring in OpenGL. This class also has static methods for texture generation
- UniformInfo and Uniform for taking arguments for enable the shader with correspoding arguments
- VertexArrayObject - class for using OpenGL VAO
- VertexBufferElement and VertexBufferLayout is used to bind attributes of vertices for VAO
- VertexBufferObject is used to store vertices, bind and draw them

### Engine core

- AssetsManager - static class for assets loading (textures, meshes, shaders)
- ObjLoader - class for meshs loading
- Renderes - class for shapes rendering
